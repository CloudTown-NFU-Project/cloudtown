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
        ~Client();//destructor
        friend std::ostream& operator<<(std::ostream&, Client*); // out stream
        bool operator!=(const Client &b){
            return !(this->nickname == b.nickname && this->sock == b.sock);
        }
        bool operator==(const Client &b){
            return (this->nickname == b.nickname && this->sock == b.sock);
        }
        bool operator==(Client *b){
            return (this->nickname == b->nickname && this->sock == b->sock);
        }
        void setNickname(std::string);
        std::string getNickname();
        socketHelper* getSocket();
};


#include "Client.cpp"
#endif