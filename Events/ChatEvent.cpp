#include "Events.h"

#include <iostream>
#include <string>
using namespace std;

ChatEvent::ChatEvent(){

}

void ChatEvent::ClientChatEvent(ChatEventData* data){
    Client* client = data->client;
    string message = data->message;
    cout << client->getNickname() << endl;
    cout << message << endl;


    Server* server = Server::getInstance();
    
    string titled_message = "["+client->getNickname()+"] "+ message;
    PlayOutChat chat = PlayOutChat(titled_message);
    Packet<PlayOutChat> packet = Packet<PlayOutChat>(chat); 

    server->broadcast(packet,client);
}