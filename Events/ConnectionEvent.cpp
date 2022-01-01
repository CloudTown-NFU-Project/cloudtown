#include "Events.h"
#include "../Server/Server.hpp"
#include <iostream>
#include "../Packet/Packet.h"
#include "../Packet/PacketData.h"
#include <string>
using namespace std;

ConnectionEvent::ConnectionEvent(){
    
}

void ConnectionEvent::ClientJoinEvent(Client *client){
    Server* server = Server::getInstance();

    PlayOutChat playerJoinMessage = PlayOutChat(client->getNickname() + " joined the server. ["+to_string(server->onlineClient.Length())+"/100]"); 
    Packet<PlayOutChat> packet = Packet<PlayOutChat>(playerJoinMessage); 
    server->broadcast(packet);


    // now can do refresh the map
}

void ConnectionEvent::ClientLeaveEvent(Client *client){
    Server* server = Server::getInstance();
    
    PlayOutChat playerLeftMessage = PlayOutChat(client->getNickname() + " left the server. ["+to_string(server->onlineClient.Length())+"/100]"); 
    Packet<PlayOutChat> packet = Packet<PlayOutChat>(playerLeftMessage); 
    server->broadcast(packet,client);

    // now can do refresh the map

}