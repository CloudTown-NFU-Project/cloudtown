#ifndef Events_HPP
#define Events_HPP
#include "../Client/Client.h"



class ConnectionEvent{
    //used for handle connection event
    public:
        ConnectionEvent();

        void ClientJoinEvent(Client &client);

        void ClientLeaveEvent(Client &client);

        
};

class ChatEvent{
    public:
        ChatEvent();

        void ClientChatEvent();
};
#endif