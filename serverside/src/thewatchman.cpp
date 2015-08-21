#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/sysinfo.h>
#include <sys/statvfs.h>
// #include <errno.h>
#include <fcntl.h>
#include <map>
// #include <curl/curl.h>
#include "logger.h"
using namespace std;

string env_HOME = getenv("HOME");

map<string,string> Conf;
string LOG_FILE = env_HOME+"/thewatchman.log";
Logger logger(LOG_FILE);

bool getConfiguration(string &file){
    ifstream ci(file.c_str());
    if(!ci){
        cout << "Configuration file not found." << endl;
        return false;
    }
    string line;
    while(getline(ci, line)){
        string key, value;
        ci >> key >> value;
        Conf[key] = value;
    }
    if(Conf.find("MID") == Conf.end())Conf["MID"] = "Default";
    if(Conf.find("URL") == Conf.end())Conf["URL"] = "localhost";
    if(Conf.find("GET_NAME") == Conf.end())Conf["GET_NAME"] = "json";
    if(Conf.find("INTERVAL") == Conf.end())Conf["INTERVAL"] = "100";
    return true;
}
string getCPU();
string getRAM();
string getHHD();
void postJSON(string json);

int main(int argc, char* argv[]){
    string conffile;
    if(argc == 1){
        cout << "Configuration file has't be provided, you can provide a conf file manually as first parameter 'thewatchman PATH/TO/CONF/FILE'" <<
        endl << "Looking for thewatchman.conf in default folder " + env_HOME + "/.config/" << endl;
        conffile = env_HOME + "/.config/thewatchman.conf";
    }
    if(argc > 2){
        cout << "Just one argument can be passed to TheWatchman"
             << endl;
        exit(1);
    }else{
        if(conffile.length() < 1){
            conffile = argv[1];
        }
        if(!getConfiguration(conffile)){
            exit(1);
        }else{
            cout << "Loading config file"
                 << conffile<< endl;
        };
    }

    // DAEMONIZE
    pid_t pid, sid;

    pid = fork();
    if(pid<0)exit(EXIT_FAILURE);
    if(pid>0)exit(EXIT_SUCCESS);
    umask(0);

    logger.write("Service initialized");
    logger.write("Sending info to "+Conf["URL"]+" every "+Conf["INTERVAL"]+" miliseconds.");

    sid = setsid();
    if(sid<0){
        logger.write("Error getting SID");
        exit(EXIT_FAILURE);
    }
    if((chdir("/"))<0)exit(EXIT_FAILURE);

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    while(true){
        logger.pull();
        string json = "%7B'"+Conf["MID"]+"':%7B" + getCPU()+"," + getRAM() + ","+getHHD()+"%7D%7D";
        postJSON(json);
        usleep(atoi(Conf["INTERVAL"].c_str()));
    }
    exit(EXIT_SUCCESS);
}





//////////////////////////////////////
//                                  //
//       MONITIRIZE FUNCTIONS       //
//                                  //
//////////////////////////////////////
string getCPU(){
    string buffer[7];
    string trash;

    ifstream gis("/proc/stat");
    if(!gis){
        logger.write("sysinfo: error reading system statistics when opening to get CPU usage.");
    } 
    float gstats[7];
    gis >> trash >> buffer[0] >> buffer[1] >> buffer[2]
                >> buffer[3] >> buffer[4] >> buffer[5]
                >> buffer[6];
    gis.close();

    for(int x = 0; x<7; x++){
        gstats[x] = atoi(buffer[x].c_str());
    }

    usleep(1000000);

    ifstream is("/proc/stat");
    if(!is){
        logger.write("sysinfo: error reading system statistics when opening to get CPU usage.");
    }
    float stats[7];
    is >> trash >> buffer[0] >> buffer[1] >> buffer[2]
                >> buffer[3] >> buffer[4] >> buffer[5]
                >> buffer[6];
    is.close();

    for(int x = 0; x<7; x++){
        stats[x] = atoi(buffer[x].c_str());
    }

    float tot_G = 0;
    float tot_p = 0;
    float work_G = 0;
    float work_p = 0;
    for(int x = 0; x<7;x++){
        tot_G += gstats[x];
        tot_p += stats[x];

        if(x<3){
            work_G += gstats[x];
            work_p += stats[x];
        }
    }

    float workop, totalop, perc;
    workop = work_p - work_G;
    totalop = tot_p - tot_G;
    perc = workop/totalop * 100;

    return "'cpu':'"+to_string((int)perc)+"'";
}

string getRAM(){
    struct sysinfo info;
    if (sysinfo(&info) != 0)
        logger.write("sysinfo: error reading system statistics when opening to get RAM usage.");

    float freeram = info.freeram;
    float totalram = info.totalram;
    float percentage = (freeram/totalram)*100;
    return "'ram_total':'"+to_string(info.totalram/1024/1024)+"','ram_free':'"+to_string(info.freeram/1024/1024)+"','ram_usage':'"+to_string(100-(int)percentage)+"'";
}

string getHHD(){
    struct statvfs info;
    if(statvfs("/",&info) != 0)
        logger.write("statvfs: error reading system statistics when opening to get HHD usage.");
    
    unsigned long total = (info.f_blocks * info.f_bsize)/1024/1024;
    unsigned long freed = (info.f_bfree * info.f_bsize)/1024/1024;
    float usage = ((float)freed / (float)total)*100;

    logger.write("BSIZE: "+to_string(usage));
    return "'hd_total':'"+to_string((int)total)+"','hd_free':'"+to_string((int)freed)+"','hd_usage':'"+to_string(100-(int)usage)+"'";
}





void postJSON(string json){
    string url = Conf["URL"] + "?"+ Conf["GET_NAME"] +"="+json;
    string gogo = "curl "+url;
    system(gogo.c_str());
}
