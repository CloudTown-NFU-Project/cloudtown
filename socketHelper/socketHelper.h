#ifndef socketHelper_H
#define socketHelper_H
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <pthread.h>
#include <unistd.h>
#include "../Packet/Packet.h"
#include "../Client/Client.h"
#include <deque>
//#define  _WINSOCK_DEPRECATED_NO_WARNINGS


class socketHelper{
    friend class Client;
    private:
        std::deque<std::string> message_buffer;
        std::string socketName; 
        int BUF_SIZE;
        int sock_id = -1;//invalid socket
        std::string IP;
        int PORT;
        struct sockaddr_in addr;
    public:
        /**
         * @brief Construct a new socket Helper object
         * 
         * @param socketName unique socket name for debug
         * @param BUF_SIZE send 
         */
        socketHelper(std::string socketName,int BUF_SIZE =1024); // new and get a socket 
        bool setup(std::string IP,int PORT,bool init=false);
        bool helper_bind();//bind
        bool helper_listen(int size);

        /**
         * @brief accept the new connection
         * 
         * @param clientSocket another socket Helper object  
         * 
         */
        bool helper_accept(socketHelper *clientSocket);

        //void send();//send message
        //void recv();//recv message

        
        void sendPacket(); // send packet in running function
        BasePacket* recvPacket();            // recv packet

        std::string getIP();
        int getPORT();
        int getSockID();

        bool safeRecv(char* buffer,int buffer_size);
        // make sure that receive the [buffer_size] length  

        void PropSendEncode(std::string encoded);
        void PreSendEncode(std::string encoded);
        void PreSendPacket(std::string message);
        void PreSendPacket(BasePacket &packet);
        bool hasPacketToSent();

        void unboundSocket();
        ~socketHelper();//destructor
};


#include "socketHelper.cpp"
#endif