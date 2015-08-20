#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/sysinfo.h>
#include <errno.h>
#include <fcntl.h>
#include <map>
#include <curl/curl.h>
#include "logger.h"
using namespace std;

string env_HOME = getenv("HOME");

map<string,string> Conf;
string LOG_FILE = env_HOME+"/thewatchman.log";
Logger logger(LOG_FILE);

bool getConfiguration(string &file){
    ifstream ci(file.c_str());
    if(!ci){
        cout << "Archivo de configuración no encontrado" << endl;
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
    cout << Conf["MID"]
         << Conf["URL"]
         << Conf["GET_NAME"]
         << Conf["INTERVAL"]
         << endl;
    return true;
}
string getCPU();
string getRAM();
void postJSON(string json);

int main(int argc, char* argv[]){
    string conffile;
    if(argc == 1){
        cout << "Configuration file has't be provided." <<
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
            cout << "cargado archivo de configuracion"
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
        logger.write("Error generando SID");
        exit(EXIT_FAILURE);
    }
    if((chdir("/"))<0)exit(EXIT_FAILURE);

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    while(true){
        logger.pull();
        string json = "{'"+Conf["MID"]+"':{" + getCPU()+"," + getRAM() + "}}";
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
    struct sysinfo info;
    if (sysinfo(&info) != 0)
        logger.write("sysinfo: error reading system statistics when opening to get CPU usage.");
    return "'cpu:'"+to_string(info.loads[0]/1000)+"'";
}

string getRAM(){
    struct sysinfo info;
    if (sysinfo(&info) != 0)
        logger.write("sysinfo: error reading system statistics when opening to get RAM usage.");
    return "'totalram':'"+to_string(info.totalram/1024/1024)+"','freeram':'"+to_string(info.freeram/1024/1024)+"'";
}






void postJSON(string json){
    CURL* curl;
    long response;
    string url = Conf["URL"] + "?"+ Conf["GET_NAME"] +"="+json;

    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_strerror(CURLE_HTTP_RETURNED_ERROR);
    curl_easy_perform(curl);
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response);

    if(response != 200)logger.write("HTTP Error");
    if(response == 404)logger.write("HTTP 404, service not found");

    curl_easy_cleanup(curl);
    curl_global_cleanup();
}
