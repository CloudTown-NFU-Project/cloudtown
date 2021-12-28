#include "socketHelper.h"

socketHelper::socketHelper(std::string socketName,int BUF_SIZE){ // new and get a socket 
    this->BUF_SIZE = BUF_SIZE;
    this->socketName = socketName;
}

bool socketHelper::setup(std::string IP,int PORT,bool init){
    this->IP = IP;
    this->PORT = PORT;
    if (init){
        //create server socket
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = htonl(INADDR_ANY);
        addr.sin_port = htons((short)7117);

        sock_id = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        return sock_id != -1;
    }
    return false;
}

bool socketHelper::helper_bind(){
    int result = bind(sock_id, (struct sockaddr*) &addr, sizeof(struct sockaddr_in));
    return result != -1;
}


bool socketHelper::helper_listen(int size){
    int result = listen(sock_id, 10);
    return result != -1;
}

#include<arpa/inet.h>
bool socketHelper::helper_accept(socketHelper *clientSocket){
    int sin_size = sizeof(clientSocket->addr);
    clientSocket->sock_id = accept(sock_id, (struct sockaddr*)&(clientSocket->addr), (socklen_t*)&sin_size);
    std::string clientIP = std::string(inet_ntoa(clientSocket->addr.sin_addr));
    int PORT = ntohs(clientSocket->addr.sin_port);

    clientSocket->setup(clientIP,PORT);// setup the client ip and port

    if (clientSocket->sock_id == -1){
        //accept failed
        return true;
    }else{
        return false;
    }
    return false;
}

#include <unistd.h>
socketHelper::~socketHelper(){
    close(sock_id);
    sock_id = -1;
}

std::string socketHelper::getIP(){
    return IP;
}


int socketHelper::getPORT(){
    return PORT;
}

int socketHelper::getSockID(){
    return sock_id;
}