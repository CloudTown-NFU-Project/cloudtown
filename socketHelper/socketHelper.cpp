#include "socketHelper.h"

socketHelper::socketHelper(std::string socketName,int BUF_SIZE){ // new and get a socket 
    this->BUF_SIZE = BUF_SIZE;
    this->socketName = socketName;
}

/**
 * @brief setup the server ip , if this is a server socket the the third param 
 * init = true , will create a speical socket for server socket 
 * @param IP ip
 * @param PORT port
 * @param init if is server socket or not
 * @return true setup failed
 * @return false setup successful
 */
bool socketHelper::setup(std::string IP,int PORT,bool init){
    this->IP = IP;
    this->PORT = PORT;
    if (init){
        //create server socket
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = htonl(INADDR_ANY);
        addr.sin_port = htons((short)7117);

        sock_id = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        return sock_id == -1;
    }
    return true;
}


/**
 * @brief bind server port
 * 
 * @return true bind failed
 * @return false bind successful
 */
bool socketHelper::helper_bind(){
    int result = bind(sock_id, (struct sockaddr*) &addr, sizeof(struct sockaddr_in));
    return result == -1;
}


/**
 * @brief listen 
 * 
 * @param size 
 * @return true listen failed
 * @return false listen successful
 */
bool socketHelper::helper_listen(int size){
    int result = listen(sock_id, 10);
    return result == -1;
}

#include<arpa/inet.h>


/**
 * @brief accept new connection
 * 
 * @param clientSocket this have to be a server socket
 * @return true accept failed , aka accept() = -1
 * @return false accecpt successful
 */
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
#include "../utils/debugMessage.h"
socketHelper::~socketHelper(){
    logger::print("socket destructor " + to_string(sock_id));
    if (sock_id == -1) return;
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

void socketHelper::unboundSocket(){
    if (sock_id == -1) return;
    close(sock_id);
    sock_id = -1;
}

#include "../Packet/Packet.h"
#include "../Packet/PacketData.h"
void socketHelper::sendPacket(BasePacket &packet){
    if (Packet<PlayOutChat>::instanceof(packet)) {
        Packet<PlayOutChat> p = Packet<PlayOutChat>(packet);
        sendEncode(p.data->encode());
    }
}

BasePacket* socketHelper::recvPacket(){
    int retVal;
    char packetLength[11];
    char packetID[3];

    retVal = recv(sock_id,packetLength,10,0);
    packetLength[retVal] = '\0';
    int length = atoi(packetLength);


    retVal = recv(sock_id,packetID,2,0);
    packetID[retVal] = '\0';

    char packetData[length-2+1];
    retVal = recv(sock_id,packetData, length-2,0);
    packetData[retVal] = '\0';
    char msg[1000];
    sprintf(msg,"Length:%d,PacketID:%s,Data:%s",length,packetID,packetData);
    logger::print(string(msg),DEBUG);
    PlayOutChat chat = PlayOutChat(string(packetData));
    Packet<PlayOutChat> *packet = new Packet<PlayOutChat>(chat);
    return packet;
}


void socketHelper::sendEncode(std::string encoded){
    int retVal = send(sock_id,encoded.c_str(),encoded.size(),0);
}