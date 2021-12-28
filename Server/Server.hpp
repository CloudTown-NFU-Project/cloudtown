#ifndef Server_HPP
#define Server_HPP

#include "../Events/Events.h"
#include "../Client/Client.h"
#include "../ClientList/ClientList.cpp"
#include "../ClientList/Iterator.cpp"
#include "../socketHelper/socketHelper.h"

#include <sys/select.h>// select
#include <sys/ioctl.h>
#include <iostream>
using namespace std;


class Server{
    private:
    	static Server *instance;
        
        Server(){
        	connectionEvent = new ConnectionEvent();//register event
            chatEvent = new ChatEvent();            //register event
        }
    public:
        static ClientList onlineClient;
        ConnectionEvent *connectionEvent = nullptr;
        ChatEvent *chatEvent = nullptr;
        static Server* getInstance(){
            if (instance == nullptr){
                instance = new Server();
            }
            return instance;
        }
        /**
         * @brief used for running the server
         * int this function will create a thread that accepting client join the server
         * and the running function will manage (socket select) the huge client (including send/recv action)
         */
        void Running(){
            socketHelper *serverSocket = new socketHelper("server");


            //open a thread that accept the client 
            pthread_t acceptConnection;
            pthread_t disConnection;
            //open a command line that can let admin(which from console) can use some command
            pthread_t cmd;
            
            serverSocket->setup("20.210.97.63",7117,true);
            serverSocket->helper_bind();
            serverSocket->helper_listen(10);


            pthread_create(&acceptConnection, NULL,( void* (*)(void*))&(this->handleNewConnection), serverSocket);
            //pthread_create(cmd, NULL, consoleCommandLine, /*control*/);

            while(1){
                //cout << "online player: " << onlineClient.Length() << endl;
                Client *disconnectClient = nullptr;
                for (ClientList::Iterator it = onlineClient.begin();it != onlineClient.end();it++){
                    if (it->getSocket()->getSockID() == -1 || isclosed(it->getSocket()->getSockID())){
                        disconnectClient = &(*it);
                        Client *pointer_disconnectClient = new Client(disconnectClient);
                        pthread_create(&disConnection, NULL,( void* (*)(void*))&(this->handleDisConnection),  pointer_disconnectClient); // disconnection thread
                        break;
                    }
                    //cout << *it << endl;
                }
                if (disconnectClient != nullptr){
                    //cout << "remove client addreess: " << disconnectClient << endl;
                    onlineClient.Delete(*disconnectClient);

                }                
                usleep(100*1000);//delay 1 sec
                //do something
            }
        }

        /**
         * @brief handle new connection
         * 
         * @param serverSocket which is the server socket 
         */
        static void handleNewConnection(void *serverSocket){
            //handle new connection when client join
            socketHelper *sock = (socketHelper*) serverSocket;
            while (true){
                socketHelper *client1 = new socketHelper("random socket name (client)");
                sock->helper_accept(client1);
                Client newClient = Client("random client name",client1);
                onlineClient.InsertTail(newClient);
                instance->connectionEvent->ClientJoinEvent(newClient);
            }
        }

        /**
         * @brief can do like /stop (stop server), /list (list client), /restart (restart the server),etc...
         * 
         * @param control which can controll entire system like stop the Running() ,
         * when stop the Running() also mean to be stop server 
         */
        static void consoleCommandLine(void *control){

        }


        static void handleDisConnection(void *disConnectionClient){
            Client *client = (Client *) disConnectionClient;
            //call the event
            instance->connectionEvent->ClientLeaveEvent(*client);
            client = nullptr;
            delete client;
        }


        int getOnlinePlayers(){
            return onlineClient.Length();
        }

        bool isOnline(Client& client){
            for (ClientList::Iterator it = onlineClient.begin();it != onlineClient.end();it++){
                if (client == *it) return true;
            }
            return false;
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