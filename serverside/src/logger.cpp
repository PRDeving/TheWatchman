#include "logger.h"
#include "tools.h"

Logger::Logger(string LOG_FILE){
    _logfile.open(LOG_FILE.c_str(), ios::out);
    if(!_logfile){
        cout << "error opening logfile: " << LOG_FILE.c_str() << endl;
    }
}
Logger::~Logger(){
    this->write("closing log");
    _logfile.close();
}
void Logger::write(string msn){
    string currenttime = getCurrentTime();
    string mess = "[" + currenttime + "]:" + msn;
    _msn_queue.push(mess);
}
void Logger::pull(){
    if(!_msn_queue.empty()){
        _logfile << _msn_queue.front() << endl;
        _msn_queue.pop();
    }
}
