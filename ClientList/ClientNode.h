#ifndef ClientNode_H
#define ClientNode_H
#include "../Client/Client.h"

class ClientNode{
	public:
		Client *data;
		ClientNode *next = nullptr;
		ClientNode(Client *data,ClientNode *next):data(data),next(next){}
		ClientNode(Client *data):data(data),next(nullptr){}
		ClientNode():data(new Client("",nullptr)),next(nullptr){}
		
};


#endif