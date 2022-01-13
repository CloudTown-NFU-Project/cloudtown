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

/**
 * @brief 
 * 
 * @return BasePacket* 
 */
BasePacket* socketHelper::recvPacket(){
    int retVal;
    char packetLength[11];
    char packetID[3];

    if (safeRecv(packetLength,10)) return nullptr;
    int length = atoi(packetLength);


    if(safeRecv(packetID,2)) return nullptr;


    char packetData[length-2+1];
    if(safeRecv(packetData,length-2)) return nullptr;


    char msg[1000];
    sprintf(msg,"from port:%d,Length:%d,PacketID:%s,Data:%s",PORT,length,packetID,packetData);
    logger::print(string(msg),DEBUG);
    if (strcmp(packetID,"01") == 0){
        char msg[1000];
        //sprintf(msg,"Length:%d,PacketID:%s,Data:%s",length,packetID,packetData);

        PlayOutChat chat = PlayOutChat::decode(packetData);
        Packet<PlayOutChat> *packet = new Packet<PlayOutChat>(chat);
        return packet;
    }else if (strcmp(packetID,"02") == 0){
        return nullptr;
    }else if (strcmp(packetID,"03") == 0){
        PlayOutPlayerMove moveData = PlayOutPlayerMove::decode(packetData);
        Packet<PlayOutPlayerMove> *packet = new Packet<PlayOutPlayerMove>(moveData);
        return packet;
    }else if (strcmp(packetID,"04") == 0){
        PlayOutPlayerStop stopData = PlayOutPlayerStop::decode(packetData);
        Packet<PlayOutPlayerStop> *packet = new Packet<PlayOutPlayerStop>(stopData);
        return packet;
    }else if (strcmp(packetID,"05") == 0){
        PlayInPlayerLogin data = PlayInPlayerLogin::decode(packetData);
        Packet<PlayInPlayerLogin> *packet = new Packet<PlayInPlayerLogin>(data);
        return packet;
    }else if (strcmp(packetID,"06") == 0){
        PlayOutPlayerSpawn data = PlayOutPlayerSpawn::decode(packetData);
        Packet<PlayOutPlayerSpawn> *packet = new Packet<PlayOutPlayerSpawn>(data);
        return packet;
    }else if (strcmp(packetID,"07") == 0){
        PlayOutPlayerDespawn data = PlayOutPlayerDespawn::decode(packetData);
        Packet<PlayOutPlayerDespawn> *packet = new Packet<PlayOutPlayerDespawn>(data);
        return packet;
    }else if (strcmp(packetID,"08") == 0){
        PlayOutPlayerLogin data = PlayOutPlayerLogin::decode(packetData);
        Packet<PlayOutPlayerLogin> *packet = new Packet<PlayOutPlayerLogin>(data);
        return packet;
    }else if (strcmp(packetID,"09") == 0){
        PlayInLoginSuccess data = PlayInLoginSuccess::decode(packetData);
        Packet<PlayInLoginSuccess> *packet = new Packet<PlayInLoginSuccess>(data);
        return packet;
    }else return nullptr;



    
}


/**
 * @brief send packet action 
 * 
 */
void socketHelper::sendPacket(){
    std::string message = message_buffer.front();
    message_buffer.pop_front();
    int retVal = send(sock_id,message.c_str(),message.size(),0);
    if (retVal == -1){
        logger::print("packet sent error",ERROR);
        return;
    }
}

/**
 * @brief add packet to send queue
 * 
 * @param packet 
 */
void socketHelper::PreSendPacket(BasePacket &packet){
    if (Packet<PlayOutChat>::instanceof(packet)){
        Packet<PlayOutChat> p = Packet<PlayOutChat>(packet);
        PreSendPacket(p.data->encode());
    }
}

/**
 * @brief add packet with high priority (may be sent first)
 *
 * @param encoded 
 */
void socketHelper::PropSendEncode(std::string encoded){
    message_buffer.push_front(encoded);
}


void socketHelper::PreSendEncode(std::string encoded){
    message_buffer.push_back(encoded);
}

void socketHelper::PreSendPacket(std::string message){
    message_buffer.push_back(message);
}

/**
 * @brief to check if have packet in packet queue
 * 
 * @return true 
 * @return false 
 */
bool socketHelper::hasPacketToSent(){
    return message_buffer.size() != 0;
}


/**
 * @brief a safe receive make sure will get [ buffer_size ] chars
 * 
 * @param buffer char*
 * @param buffer_size receive length
 * @return true - error happened
 * @return false 
 */
bool socketHelper::safeRecv(char* buffer,int buffer_size){
    int left_to_recv = buffer_size;
    int bytes_read = 0;
    char contentBuffer[buffer_size];
    while(left_to_recv > 0){
        memset(contentBuffer,'\0',sizeof(contentBuffer));
        int retVal = recv(sock_id,contentBuffer,left_to_recv,0);
        if (retVal <= 0){
            logger::print("recv error, client may be disconnected ",ERROR);
            return true;

        }
        for (int i = 0 ; i < retVal ; i++){
            buffer[bytes_read++] = contentBuffer[i];
        }
        left_to_recv = left_to_recv - retVal;
    }
    buffer[bytes_read] = '\0';
    return false;
}