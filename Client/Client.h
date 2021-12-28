//here may be included anywhere so that used .h 
#ifndef Client_H
#define Client_H
#include <iostream>
#include <string>
#include "../socketHelper/socketHelper.h"

class Client{
    private:
        std::string nickname;
        socketHelper *sock;
    public:
        Client(std::string,socketHelper *sock=nullptr);
        Client(const Client* copy);//copy constructor
        friend std::ostream& operator<<(std::ostream&, Client&); // out stream
        friend std::ostream& operator<<(std::ostream&,const Client&); // out stream
        bool operator!=(const Client &b){
            return !(this->nickname == b.nickname && this->sock == b.sock);
        }
        bool operator==(const Client &b){
            return (this->nickname == b.nickname && this->sock == b.sock);
        }
        bool operator==(Client *b){
            return (this->nickname == b->nickname && this->sock == b->sock);
        }
        socketHelper* getSocket();
};


#include "Client.cpp"
#endif