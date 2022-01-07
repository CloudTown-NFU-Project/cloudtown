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

class PlayerEvent{
    public:
        PlayerEvent();

        void PlayerSpawnEvent(Client* client);  // called this after the client join event
        void PlayerDespawnEvent(Client* client);// called this after the client leave event
        void PlayerMovementEvent(Client* client);
        void PlayerStopMovementEvent(Client* client);
};

class MoveEvent{
    public:
        MoveEvent();

        void ClientMoveEvent(){

        }
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