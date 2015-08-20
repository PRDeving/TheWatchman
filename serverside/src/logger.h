#ifndef LOGGER_H
#define LOGGER_H
#include <iostream>
#include <fstream>
#include <queue>
#include <string>
using namespace std;

class Logger{
    public:
        Logger(string LOG_FILE);
        ~Logger();
        void write(string msn);
        void pull();
    private:
        fstream _logfile;
        queue<string> _msn_queue;
};

#endif
