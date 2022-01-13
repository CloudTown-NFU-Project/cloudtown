#include "Events.h"
#include "../Server/Server.hpp"
#include <iostream>
#include <string>
using namespace std;

ChatEvent::ChatEvent(){

}

void ChatEvent::ClientChatEvent(ChatEventData* data){
    Client* client = data->client;
    string message = data->message;
    
    Server* server = Server::getInstance();
    
    string titled_message = "["+client->getNickname()+"] "+ message;
    cout << titled_message << endl;
    PlayOutChat chat = PlayOutChat(message,client->getSocket()->getSockID());

    server->messageBroadCast(chat.encode());
}