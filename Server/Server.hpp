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
#include <map>
using namespace std;


class Server{
    private:
    	static Server *instance;
        socketHelper *serverSocket = nullptr;
        //fd_set file descriptor sets for use with FD_ macros
        fd_set masterfds;
        fd_set tempfds;
        //unsigned integer to keep track of maximum fd value, required for select()
        uint16_t maxfd;


        int mastersocket_fd; //master socket which receives new connections


        Server(){
        	connectionEvent = new ConnectionEvent();//register event
            chatEvent = new ChatEvent();            //register event
        }
    public:
        static ClientList onlineClient;
        map<int,Client*> sock_to_client;
        ConnectionEvent *connectionEvent = nullptr;
        ChatEvent *chatEvent = nullptr;
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
            logger::print("Server is running",INFO);
            while(1){
                tempfds = masterfds; //copy fd_set for select()
                int sel = select(maxfd + 1, &tempfds, NULL, NULL, NULL); //blocks until activity


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
                        if (isclosed(sock_id)){
                            logger::print("client is disconnected",DEBUG);
                            handleDisConnection(sock_id);
                            continue;
                        }
                        handleRecvPacket(sock_id);
                        
                        //cout << "client is sending message " << endl;
                    }
                }
                //usleep(1*1000);//delay 1 m sec
                continue;
            }
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
            Client *newClient = new Client("default",newConnectedClient);// there may be "only one" that client create , not anywhere else
            onlineClient.InsertTail(newClient);// in this function InserTail(*pointer)
            sock_to_client[newClient->getSocket()->getSockID()] = newClient;

            //below may need a thread to use | or i can just do a function pointer that call a event (event) into it
            pthread_t acceptConnection;
            pthread_create(&acceptConnection, NULL,( void* (*)(void*))&(this->threadNewConnection), newClient);
        }

        static void threadNewConnection(void *newClient){
            Client *join = (Client*) newClient;
            logger::print("make sure not create");
            instance->connectionEvent->ClientJoinEvent(join);//because of call this even so use thread 
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
            sock_to_client[disconnectClient->getSocket()->getSockID()] = nullptr;
            FD_CLR(dis_sock,&masterfds); // remove dis_sock from master fds set

            //below may need a thread to use | or i can just do a function pointer that call a event (event) into it
            
            pthread_t disConnection;
            pthread_create(&disConnection, NULL,( void* (*)(void*))&(this->threadDisConnection), disconnectClient);
        }
        static void threadDisConnection(void *disClient){
            Client *disconnectClient = (Client*) disClient;
            instance->connectionEvent->ClientLeaveEvent(disconnectClient);
            
            disconnectClient->~Client(); // there may be "only one" that remove the client 
        }

        void handleRecvPacket(int recv_id){
            Client *c = sock_to_client[recv_id];
            if (c == nullptr) {
                logger::print("recv action client disconnect?",ERROR);
                return;
            }
            BasePacket *packet = c->getSocket()->recvPacket();

            if (Packet<PlayOutChat>::instanceof(*packet)){
                Packet<PlayOutChat> chatPacket = Packet<PlayOutChat>(*packet);
                ChatEventData *data = new ChatEventData;
                data->client = c;
                data->message = chatPacket.data->getMessage();
                pthread_t disConnection;
                pthread_create(&disConnection, NULL,( void* (*)(void*))&(this->threadClientChatEvent), data);
            }
            delete packet;
        }

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

        void broadcast(BasePacket &packet,Client* without = nullptr){
            for (ClientList::Iterator it = onlineClient.begin();it != onlineClient.end();it++){
                if (without == *it) continue;
                else{
                    (*it)->getSocket()->sendPacket(packet);
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
#endif