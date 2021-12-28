#include "Client.h"

Client::Client(std::string name,socketHelper *socket){
    nickname = name;
    sock = socket;
}


Client::Client(const Client* b){
    nickname = b->nickname;
    sock = b->sock;
}


std::ostream& operator<<(std::ostream& os,Client &client){
	os << client.nickname << " " << client.sock->getIP() << " " << client.sock->getPORT() << " |";
	return os;
}

std::ostream& operator<<(std::ostream& os,const Client &client){
	os << client.nickname << " " << client.sock->getIP() << " " << client.sock->getPORT() << " |";
	return os;
}

socketHelper* Client::getSocket(){
    return sock;
}