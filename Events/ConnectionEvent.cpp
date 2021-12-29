#include "Events.h"
#include "../Server/Server.hpp"
#include <iostream>
using namespace std;

ConnectionEvent::ConnectionEvent(){
    
}

void ConnectionEvent::ClientJoinEvent(Client & client){
    Server* server = Server::getInstance();
    //some one join the server
    cout << client << " join the server!!" << endl;
    if(server->isOnline(client)){
        cout << "wow" << endl;
    }

    // now can do refresh the map
    usleep(10000*1000);//delay 1 sec
    //client.getSocket()->~socketHelper();
}

void ConnectionEvent::ClientLeaveEvent(Client & client){
    Server* server = Server::getInstance();
    //some one leave the server
    cout << client << " leave the server!!" << endl;
    if(server->isOnline(client)){
        cout << "so sad" << endl;
    }

    // now can do refresh the map

}