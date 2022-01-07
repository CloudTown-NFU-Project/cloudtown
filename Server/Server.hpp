#ifndef Server_HPP
#define Server_HPP

#include "../Events/Events.h"
#include "../Client/Client.h"
#include "../ClientList/ClientList.cpp"
#include "../ClientList/Iterator.cpp"
#include "../socketHelper/socketHelper.h"
#include "../utils/debugMessage.h"
#include <sys/select.h>// select
#include <sys/ioctl.h>
#include <iostream>
#include <stdio.h>
#include <map>
using namespace std;


class Server{
    private:
    	static Server *instance;
        socketHelper *serverSocket = nullptr;
        //fd_set file descriptor sets for use with FD_ macros
        fd_set masterfds;
        fd_set sentfds;
        fd_set tempfds;
        //unsigned integer to keep track of maximum fd value, required for select()
        uint16_t maxfd;


        int mastersocket_fd; //master socket which receives new connections


        Server(){
        	connectionEvent = new ConnectionEvent();//register event
            chatEvent = new ChatEvent();            //register event
            playerEvent = new PlayerEvent();        //reigster event
        }
    public:
        static ClientList onlineClient;
        map<int,Client*> sock_to_client;
        ConnectionEvent *connectionEvent = nullptr;
        ChatEvent *chatEvent = nullptr;
        PlayerEvent *playerEvent = nullptr;
        static Server* getInstance(){
            if (instance == nullptr){
                instance = new Server();
            }
            return instance;
        }

        /**
         * @brief setup() the server 
         * 
         */
        void setup(){
            serverSocket = new socketHelper("server");
            if (serverSocket->setup("20.210.250.165",7117,true))
                logger::print("Server IP setup failed",ERROR);
            else
                logger::print("Server IP setup successfuly",INFO);
            
            if (serverSocket->helper_bind())
                logger::print("Server bind failed",ERROR);
            else
                logger::print("Server bind successfuly",INFO);

            if (serverSocket->helper_listen(10))
                logger::print("Server listen failed",ERROR);
            else
                logger::print("Server listen successfuly",INFO);

            int optval = 1;
            int ret = setsockopt(serverSocket->getSockID(), SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
            
            FD_SET(serverSocket->getSockID(), &masterfds); //insert the master socket file-descriptor into the master fd-set
            maxfd = serverSocket->getSockID(); //set the current known maximum file descriptor count
            logger::print("Server is ready",INFO);

        }

        
        /**
         * @brief used for running the server
         * int this function will create a thread that accepting client join the server
         * and the running function will manage (socket select) the huge client (including send/recv action)
         */
        void Running(){
            struct timeval check_send_timeout = {1000,0};
            logger::print("Server is running",INFO);
            pthread_t packetAction;
            pthread_create(&packetAction, NULL,( void* (*)(void*))&(this->threadRunning), nullptr);
            while(1){
                tempfds = masterfds; //copy fd_set for select()
                int sel = select(maxfd + 1, &tempfds, NULL, NULL, NULL); //blocks until activity
                // this will block the io 
                // so we may have two thread to do this
                // aka Running
                // 
                
                

                if (sel < 0) {
                    logger::print("select() is error",ERROR);
                    continue;
                }


                if (FD_ISSET(serverSocket->getSockID(),&tempfds)){
                    logger::print("client is connected",DEBUG);
                    handleNewConnection();
                }


                for (int sock_id = 0; sock_id <= maxfd; sock_id++){
                    if (sock_id == serverSocket->getSockID()) continue;
                    if (FD_ISSET(sock_id,&tempfds)){
                        handleRecvPacket(sock_id);
                        //cout << "client is sending message " << endl;
                    }
            
                }
                //usleep(1*1000);//delay 1 m sec
                continue;
            }
        }
        void bufferRunning(){
            struct timeval check_send_timeout = {10,0};
            while(1){
                
                for (int sock_id = 0; sock_id <= maxfd; sock_id++){
                    if (sock_id == serverSocket->getSockID()) continue;
                    if (FD_ISSET(sock_id,&masterfds)){
                        usleep(4*1000);//delay 1 m sec
                        if (sock_to_client[sock_id] == nullptr || sock_to_client[sock_id] == 0) continue;
                        
                        socketHelper *clientSH = sock_to_client[sock_id]->getSocket();
                        if (clientSH != nullptr &&clientSH->hasPacketToSent()){
                            //client has packet to sent
                            
                            sentfds = masterfds; //copy fd_set for select()
                            //here to check if client is sending or not

                            

                            int isSent = select(maxfd+1,NULL,&sentfds,NULL,NULL);
                            if (!FD_ISSET(sock_id,&sentfds)){
                                //logger::print("client is sending packet right now");
                            }else{
                                //logger::print("client send packet now");
                                clientSH->sendPacket(); // sent packcet now
                            }
                        }
                    }
                }
            }
        }
        static void threadRunning(void *param){
            instance->bufferRunning();
        }

        /**
         * @brief handle new connection
         * 
         * @param serverSocket which is the server socket 
         */
        void handleNewConnection(){
            //handle new connection when client join
            socketHelper *newConnectedClient = new socketHelper("random socket name (client)");
            
            bool accpectError = serverSocket->helper_accept(newConnectedClient);
            if (accpectError){
                delete newConnectedClient;
                return;
            }

            // connect to the server
            FD_SET(newConnectedClient->getSockID(),&masterfds);

            if (newConnectedClient->getSockID() > maxfd){
                maxfd = newConnectedClient->getSockID();
            }
            Client *newClient = new Client("default"+to_string(rand()%99999),newConnectedClient);// there may be "only one" that client create , not anywhere else
            onlineClient.InsertTail(newClient);// in this function InserTail(*pointer)
            sock_to_client[newClient->getSocket()->getSockID()] = newClient;

            //below may need a thread to use | or i can just do a function pointer that call a event (event) into it
            pthread_t acceptConnection;
            pthread_create(&acceptConnection, NULL,( void* (*)(void*))&(this->threadNewConnection), newClient);
        }

        static void threadNewConnection(void *newClient){
            Client *joinClient = (Client*) newClient;
            //logger::print("make sure not create");

            PlayInPlayerLogin loginData = PlayInPlayerLogin(1,joinClient->getSocket()->getSockID());
            joinClient->getSocket()->PropSendEncode(loginData.encode());

            instance->connectionEvent->ClientJoinEvent(joinClient);//because of call this even so use thread 
            
            
            


            //instance->playerEvent->PlayerSpawnEvent(join);// we should do this after the success login?
            //usleep(1000*1000);//delay 1 sec
            //instance->loadOnlineClient(join); // we should do this after the success login
        }

        /**
         * @brief can do like /stop (stop server), /list (list client), /restart (restart the server),etc...
         * 
         * @param control which can controll entire system like stop the Running() ,
         * when stop the Running() also mean to be stop server 
         */
        static void consoleCommandLine(void *control){

        }


        void handleDisConnection(int dis_sock){
            Client *disconnectClient = nullptr,*pointer_disconnectClient = nullptr;
            for (ClientList::Iterator it = onlineClient.begin();it != onlineClient.end();it++){
                if (it->getSocket()->getSockID() == dis_sock){
                    disconnectClient = (*it);
                    break;
                }
            }

            onlineClient.Delete(disconnectClient); // remove client from client list
            FD_CLR(dis_sock,&masterfds); // remove dis_sock from master fds set
            sock_to_client[disconnectClient->getSocket()->getSockID()] = nullptr;

            //below may need a thread to use | or i can just do a function pointer that call a event (event) into it
            
            pthread_t disConnection;
            pthread_create(&disConnection, NULL,( void* (*)(void*))&(this->threadDisConnection), disconnectClient);
        }
        static void threadDisConnection(void *disClient){
            Client *disconnectClient = (Client*) disClient;
            instance->playerEvent->PlayerDespawnEvent(disconnectClient);
            instance->connectionEvent->ClientLeaveEvent(disconnectClient);
            
            disconnectClient->~Client(); // there may be "only one" that remove the client 
        }

        void handleRecvPacket(int recv_id){
            Client *client = sock_to_client[recv_id];
            if (client == nullptr) {
                logger::print("recv action client disconnect?",ERROR);
                return;
            }
            client->recvPacket();
            //pthread_t recv_action;
            //pthread_create(&recv_action, NULL,( void* (*)(void*))&(this->threadRecvPacket), c);
        }

        //consider to make this as a thread (full note***)
        // static void threadRecvPacket(void* param){
        //     Client *c = (Client*) param;
        //     logger::print("start recv");
        //     BasePacket *packet = c->getSocket()->recvPacket();
        //     logger::print("end recv");

        //     if (Packet<PlayOutChat>::instanceof(*packet)){
        //         Packet<PlayOutChat> chatPacket = Packet<PlayOutChat>(*packet);
        //         ChatEventData *data = new ChatEventData;
        //         data->client = c;
        //         data->message = chatPacket.data->getMessage();
        //         instance->chatEvent->ClientChatEvent(data);
        //         //pthread_t disConnection;
        //         //pthread_create(&disConnection, NULL,( void* (*)(void*))&(this->threadClientChatEvent), data);
        //     }else if(Packet<PlayOutPlayerMove>::instanceof(*packet)){
        //         Packet<PlayOutPlayerMove> pack = Packet<PlayOutPlayerMove>(*packet);
        //         c->setX(pack.data->getX());
        //         c->setY(pack.data->getY());
        //         c->setFacing(pack.data->getFace());
        //         instance->playerEvent->PlayerMovementEvent(c); //consider to make this as a thread
        //     }else if(Packet<PlayOutPlayerStop>::instanceof(*packet)){
        //         Packet<PlayOutPlayerStop> pack = Packet<PlayOutPlayerStop>(*packet);
        //         c->setX(pack.data->getX());
        //         c->setY(pack.data->getY());
        //         c->setFacing(pack.data->getFace());
        //         instance->playerEvent->PlayerStopMovementEvent(c); //consider to make this as a thread
        //     }else if (Packet<PlayOutPlayerLogin>::instanceof(*packet)){
        //         Packet<PlayOutPlayerLogin> pack = Packet<PlayOutPlayerLogin>(*packet);
        //         //logger::print("player send an play out player login packet "+to_string(pack.data->getX())+","+to_string(pack.data->getY()),DANGER);
                
        //         int success = 0;
        //         if (pack.data->getPlayerId() == c->getSocket()->getSockID()) success = 1;
        //         PlayInLoginSuccess loginSuccess = PlayInLoginSuccess(success,c->getSocket()->getSockID());
        //         //start send?
        //         //logger::print(loginSuccess.encode()+" "+to_string(success),DANGER);
        //         c->getSocket()->PropSendEncode(loginSuccess.encode());
        //         //usleep(100*1000);//delay 1 m sec
        //         if (success){
        //             instance->PlayerEventJoin(c);
        //             //instance->playerEvent->PlayerSpawnEvent(c);// we should do this after the success login?
        //             //usleep(100*1000);//delay 1 m sec
        //             instance->loadOnlineClient(c); // we should do this after the success login
        //             c->setLoadedSuccess();
        //         } 
        //     }
        //     if (packet == nullptr) {
        //         logger::print("null packet received",DANGER);
        //         return;
        //     }
        //     delete packet;
        // }

        static void threadClientChatEvent(void *eventData){
            ChatEventData * data= (ChatEventData*) eventData;
            instance->chatEvent->ClientChatEvent(data);
            
            delete data;
        }


        int getOnlinePlayers(){
            return onlineClient.Length();
        }

        bool isOnline(int sockfd){
            return sock_to_client[sockfd] != nullptr;
        }

        bool isOnline(Client* client){
            for (ClientList::Iterator it = onlineClient.begin();it != onlineClient.end();it++){
                if (client == *it) return true;
            }
            return false;
        }

        /*void broadcast(BasePacket &packet,Client* without = nullptr){
            for (ClientList::Iterator it = onlineClient.begin();it != onlineClient.end();it++){
                if (without == *it) continue;
                else if ((*it)->isLoadedClient()){
                    (*it)->getSocket()->PreSendPacket(packet);
                }
            }
        }*/
        void movementUpdate(std::string encode,Client* without = nullptr){
            for (ClientList::Iterator it = onlineClient.begin();it != onlineClient.end();it++){
                if (without == *it) continue;
                else if ((*it)->isLoadedClient()){
                    (*it)->getSocket()->PreSendEncode(encode);
                }
            }
        }

        void loadOnlineClient(Client* joinedClient){
            for (ClientList::Iterator it = onlineClient.begin();it != onlineClient.end();it++){
                if (joinedClient == *it) {
                    continue;
                }
                else{
                    PlayOutPlayerSpawn toJoinedClientSideSpawnOtherClient = PlayOutPlayerSpawn((*it)->getX(),(*it)->getY(),(*it)->getFacing(), (*it)->getSocket()->getSockID());
                    (joinedClient)->getSocket()->PropSendEncode(toJoinedClientSideSpawnOtherClient.encode());
                }
            }

        }

        void broadcast(string encode,Client* without = nullptr){
            for (ClientList::Iterator it = onlineClient.begin();it != onlineClient.end();it++){
                if (without == *it) continue;
                else{
                    (*it)->getSocket()->PreSendEncode(encode);
                }
            }
        }

        void messageBroadCast(string encode,Client* without = nullptr){
            for (ClientList::Iterator it = onlineClient.begin();it != onlineClient.end();it++){
                if (without == *it) continue;
                else{
                    (*it)->getSocket()->PropSendEncode(encode);
                }
            }
        }

        void PlayerEventJoin(Client* joinedClient){
            PlayOutPlayerSpawn clientSpawn = PlayOutPlayerSpawn(joinedClient->getX(),joinedClient->getY(),joinedClient->getFacing(),joinedClient->getSocket()->getSockID()); 
            for (ClientList::Iterator it = onlineClient.begin();it != onlineClient.end();it++){
                if (joinedClient == *it) {
                    continue;
                }
                else{
                    Client *c = (*it);
                    c->getSocket()->PropSendEncode(clientSpawn.encode());
                }
            }
            
        }

        
        bool isclosed(int sock) {
            fd_set rfd;
            FD_ZERO(&rfd);
            FD_SET(sock, &rfd);
            timeval tv = { 0 };
            select(sock+1, &rfd, 0, 0, &tv);
            if (!FD_ISSET(sock, &rfd))
                return false;
            int n = 0;
            ioctl(sock, FIONREAD, &n);
            return n == 0;
        }
};
Server *Server::instance = nullptr;
ClientList Server::onlineClient;

#include "../Events/ChatEvent.cpp"
#include "../Events/ConnectionEvent.cpp"
#include "../Events/PlayerEvent.cpp"


/**
 * @brief handleClientRecv redfine at Client.cpp
 *  used to recv client send packet if 
 *  the packet not receive successfully
 *  e.x. we receive 10 char packet length at first
 *  but have some problem during the client->server transmission 
 *  we may receieved 10 char like 00000064 but only receive 2 char  00
 *  then the socketHelper->recvPacket() broken
 *  so we have to make recvPacket like I/O block
 * @param client_ptr 
 */
void handleClientRecv(void *client_ptr){
    Client *client =  (Client*) client_ptr;
    
    //because of the disconnection is a message that send to server
    //so we have to check if disconnect
    if (checkIfDisconnect(client->getSocket()->getSockID())){
        return;
    }


    BasePacket *packet = client->getSocket()->recvPacket(); // recv packet is I/O block
    Server* server = Server::getInstance();
    if (packet == nullptr) {
        logger::print("null packet received , disconnect client",DANGER);
        server->handleDisConnection(client->getSocket()->getSockID());
        return;
    }
    if (Packet<PlayOutChat>::instanceof(*packet)){
        Packet<PlayOutChat> chatPacket = Packet<PlayOutChat>(*packet);
        ChatEventData *data = new ChatEventData;
        data->client = client;
        data->message = chatPacket.data->getMessage();
        server->chatEvent->ClientChatEvent(data);
        

        delete data;// remove data

        //pthread_t disConnection;
        //pthread_create(&disConnection, NULL,( void* (*)(void*))&(this->threadClientChatEvent), data);
    }else if(Packet<PlayOutPlayerMove>::instanceof(*packet)){
        Packet<PlayOutPlayerMove> pack = Packet<PlayOutPlayerMove>(*packet);
        client->setX(pack.data->getX());
        client->setY(pack.data->getY());
        client->setFacing(pack.data->getFace());
        server->playerEvent->PlayerMovementEvent(client); //consider to make this as a thread
    }else if(Packet<PlayOutPlayerStop>::instanceof(*packet)){
        Packet<PlayOutPlayerStop> pack = Packet<PlayOutPlayerStop>(*packet);
        client->setX(pack.data->getX());
        client->setY(pack.data->getY());
        client->setFacing(pack.data->getFace());
        server->playerEvent->PlayerStopMovementEvent(client); //consider to make this as a thread
    }else if (Packet<PlayOutPlayerLogin>::instanceof(*packet)){
        Packet<PlayOutPlayerLogin> pack = Packet<PlayOutPlayerLogin>(*packet);
        //logger::print("player send an play out player login packet "+to_string(pack.data->getX())+","+to_string(pack.data->getY()),DANGER);
                
        int success = 0;
        if (pack.data->getPlayerId() == client->getSocket()->getSockID()) success = 1;
        PlayInLoginSuccess loginSuccess = PlayInLoginSuccess(success,client->getSocket()->getSockID());
        //start send?
        //logger::print(loginSuccess.encode()+" "+to_string(success),DANGER);
        client->getSocket()->PropSendEncode(loginSuccess.encode());
        //usleep(100*1000);//delay 1 m sec
        if (success){
            server->PlayerEventJoin(client);
            //instance->playerEvent->PlayerSpawnEvent(c);// we should do this after the success login?
            //usleep(100*1000);//delay 1 m sec
            server->loadOnlineClient(client); // we should do this after the success login
            client->setLoadedSuccess(); 
        }
    }
    
    delete packet;
    
    
    client->canRecv = true;
    logger::print("finished");
}

bool checkIfDisconnect(int sock_id){
    Server* server = Server::getInstance();
    if (server->isclosed(sock_id)){
        logger::print("client is disconnected",DEBUG);
        server->handleDisConnection(sock_id);
        return true;
    }

    return false;
}

#endif