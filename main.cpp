#include "Server/Server.hpp"  // include singleton class

#include "socketHelper/socketHelper.h" // such as RPC , just a socket helper (easy to use linux socket function)

#include <unistd.h>
#include <pthread.h> // used for thread in linux



#include <iostream>
#include <string>
using namespace std;



int main(){
    Server *Server = Server::getInstance();
    Server->setup();
    Server->Running();//running
    Server->~Server();
}