#include "Client.h"
#include "../utils/debugMessage.h"

Client::Client(std::string name,socketHelper *socket){
    nickname = name;
    sock = socket;
}


Client::Client(const Client* b){
    nickname = b->nickname;
    sock = b->sock;
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