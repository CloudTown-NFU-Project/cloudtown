#ifndef debugMessage_H
#define debugMessage_H
#include <iostream>
#include <string>
using namespace std;

enum messageType{
    ERROR,
    DANGER,
    DEBUG,
    INFO,
    TEST
};

bool showdebug = false;

class logger{
    
    public:
        static void print(string message,messageType type=TEST){
            if (type == ERROR){
                cout << "====================================" << endl;
                cout << "[ERROR] " << message << endl;
                cout << "====================================" << endl;
            }else if (type == DANGER){
                cout << "=*==*==*==*==*==*==*==*==*==*==*==*=" << endl;
                cout << "[DANGER] " << message << endl;
                cout << "=*==*==*==*==*==*==*==*==*==*==*==*=" << endl;
            }else if (type == INFO){
                cout << "[INFO] " << message << endl;
            }else if (type == TEST){
                cout << "TEST>- " << message << endl;
            }else if (type == DEBUG && showdebug){
                cout << "[DEBUG] " << message << endl;
            }
        }
};

#endif