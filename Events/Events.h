#ifndef Events_HPP
#define Events_HPP
#include "../Client/Client.h"
#include <string>


class ConnectionEvent{
    //used for handle connection event
    public:
        ConnectionEvent();

        void ClientJoinEvent(Client *client);

        void ClientLeaveEvent(Client *client);

        
};

typedef struct{
    Client* client;
    std::string message;
}ChatEventData;

class ChatEvent{
    public:
        ChatEvent();

        void ClientChatEvent(ChatEventData* data);
};
#endif