#include "Client.h"
#include "../Packet/Packet.h"
#include "../Packet/PacketData.h"
#include "../utils/debugMessage.h"
#include "../Server/Server.hpp"
#include "../Events/Events.h"

Client::Client(std::string name,socketHelper *socket){
    pos_x = 400;
    pos_y = 400;
    facing = 0;
    nickname = name;
    sock = socket;
    isLoadClient = false;
}


Client::Client(const Client* b){
    pos_x = 400;
    pos_y = 400;
    facing = 0;
    nickname = b->nickname;
    sock = b->sock;
    isLoadClient = false;
}

std::ostream& operator<<(std::ostream& os,Client *client){
	os << "[" << client->sock->getIP() << ":" << client->sock->getPORT() << "] " << client->nickname << "";
	return os;
}
void Client::setNickname(std::string s){
    nickname = s;
}
std::string Client::getNickname(){
    return nickname;
}

socketHelper* Client::getSocket(){
    return sock;
}

Client::~Client(){
    delete sock;
}

void Client::setX(int x){
    pos_x = x;
}
void Client::setY(int y){
    pos_y = y;
}
void Client::setFacing(int f){
    facing = f;
}

int Client::getX(){
    return pos_x;
}
int Client::getY(){
    return pos_y;
}
int Client::getFacing(){
    return facing;
}

void Client::setLoadedSuccess(){
    isLoadClient = true;
}

bool Client::isLoadedClient(){
    return isLoadClient;
}

void Client::recvPacket(){
    if (canRecv){
        canRecv = false;
        pthread_create(&packetrecv, NULL,( void* (*)(void*))&(handleClientRecv), &(*this));
    }
}
