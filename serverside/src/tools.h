#ifndef TOOLS_H
#define TOOLS_H
#include <time.h>
#include <string>
#include <sys/stat.h>
using namespace std;

inline bool fileExists (const std::string& name) {
    struct stat buffer;   
    return (stat (name.c_str(), &buffer) == 0); 
}
string getCurrentTime(){
    time_t rawtime;
    struct tm * timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);
    string currenttime = asctime(timeinfo);
    return currenttime;
}

#endif
