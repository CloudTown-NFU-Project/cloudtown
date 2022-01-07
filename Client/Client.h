//here may be included anywhere so that used .h 
#ifndef Client_H
#define Client_H
#include <iostream>
#include <string>
#include "../socketHelper/socketHelper.h"

#include <unistd.h>
#include <pthread.h> // used for thread in linux

bool checkIfDisconnect(int);
void handleClientRecv(void*);
class Client{
    private:
        int pos_x;
        int pos_y;
        int facing;
        std::string nickname;
        socketHelper *sock;
        bool isLoadClient = false;
        bool canRecv = true;
        pthread_t packetrecv;

    public:
        friend void handleClientRecv(void*);
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
        void setX(int x);
        void setY(int y);
        void setFacing(int f);
        int getX();
        int getY();
        int getFacing();

        // to check if client load success
        bool isLoadedClient();


        void setLoadedSuccess();

        void recvPacket();//recv Packet

        
};



#include "Client.cpp"
#endif