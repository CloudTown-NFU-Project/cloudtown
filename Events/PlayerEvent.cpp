#include "Events.h"
#include "../Server/Server.hpp"
#include "../Packet/PacketData.h"
#include "../Packet/Packet.h"
#include <iostream>
#include <string>
using namespace std;

PlayerEvent::PlayerEvent(){

}


// called this if receive move packet
void PlayerEvent::PlayerMovementEvent(Client* client){
    logger::print("client "+client->getNickname()+" is move" + to_string(client->getX()) + ","+to_string(client->getY()));
    Server* server = Server::getInstance();
    PlayOutPlayerMove clientStop = PlayOutPlayerMove(client->getX(),client->getY(),client->getFacing(),client->getSocket()->getSockID()); 
    server->movementUpdate( clientStop.encode(),client);
    logger::print("sent");
}
// called this if receive stop packet
void PlayerEvent::PlayerStopMovementEvent(Client* client){
    logger::print("client "+client->getNickname()+" is stop");
    Server* server = Server::getInstance();
    PlayOutPlayerStop clientStop = PlayOutPlayerStop(client->getX(),client->getY(),client->getFacing(),client->getSocket()->getSockID()); 
    //logger::print("send packet?",ERROR);
    server->movementUpdate( clientStop.encode(),client);
}


// called this after the client join event
void PlayerEvent::PlayerSpawnEvent(Client* client){
    //logger::print("client "+client->getNickname()+" is spawned");
    Server* server = Server::getInstance();
    
    server->PlayerEventJoin(client);
    


}

// called this after the client leave event
void PlayerEvent::PlayerDespawnEvent(Client* client){
    //logger::print("client "+client->getNickname()+" is despawned");
    Server* server = Server::getInstance();
    PlayOutPlayerDespawn clientDespawn = PlayOutPlayerDespawn(client->getSocket()->getSockID()); 
    //logger::print(clientDespawn.encode()+" test "+to_string(client->getSocket()->getSockID()));
    server->broadcast(clientDespawn.encode(),client);
}