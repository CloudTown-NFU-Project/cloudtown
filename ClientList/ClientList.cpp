#include "ClientList.h"
#include "Iterator.h"


ClientNode *ClientList::av = nullptr;//available list
#include <iostream>

ClientList::ClientList(){
		
	sentinel = new ClientNode(); // sentinel 
	sentinel->next = sentinel;
	head = sentinel->next;
	head->next = sentinel;
	tail = sentinel->next;
	tail->next = sentinel;
}


ClientList::ClientList(const ClientList & b){
	sentinel = new ClientNode(); // sentinel 
	sentinel->next = sentinel;
	ClientNode *prev = sentinel,*current,*b_current = b.head;
	if (b_current == b.sentinel){// circular list b is empty
		head = sentinel->next;
		head->next = sentinel;
		tail= sentinel->next;
		tail->next = sentinel;
	}else{
		current = new ClientNode(b_current->data);
		head = current;
		prev->next = current; 
		prev = current;
		b_current = b_current->next;
		while(b_current != b.sentinel){
			current = new ClientNode(b_current->data);
			prev->next = current;
			prev = current;
			b_current = b_current->next;
		}
		tail = current;
		tail->next = sentinel;
	}
}


void ClientList::InsertFront(const Client& e){
	ClientNode *newNode = new ClientNode(e);
	InsertFront(newNode);
} 


void ClientList::InsertFront(ClientNode *newNode){
	if (head != sentinel){ 	
		newNode->next = sentinel->next;
		sentinel->next = newNode;
		head = newNode;
	}else{
		//empty list
		head = newNode;
		tail = newNode;
		newNode->next = sentinel;
		sentinel->next = newNode;
	}
} 


void ClientList::InsertTail(const Client& e){ // insert last
	ClientNode *newNode = new ClientNode(e);
	InsertTail(newNode);
} 

void ClientList::InsertTail(ClientNode *newNode){ // insert last
	if (tail != sentinel){	
		tail->next = newNode;
		tail = newNode;
		newNode->next = sentinel;
	}else{
		//empty list
		head = newNode;
		tail = newNode;
		sentinel->next = newNode;
		sentinel->next = newNode;
		tail->next = sentinel;
		head->next = sentinel;
	}
} 

void ClientList::print(){
	ClientNode *current = head;
	if (head == nullptr || sentinel == nullptr) return;
	while(current != sentinel){
		std::cout << current->data << " ";
		current = current->next; 
	}
	std::cout << std::endl; 
}

#include <iostream>
using namespace std;

void ClientList::Delete(const Client& client){
	int index = 0;
	if (head == nullptr || sentinel == nullptr) return;
	ClientNode *current = head;
	while(current != sentinel && current->data != client){
		index++;
		current = current->next;
	}
	if (current == sentinel){
		return;
	}
	//cout << index<< endl;
	current = head;
	if (head->next == sentinel && index == 0){
		//only have 1 node (sentinel)
		if (head != nullptr) delete head;
		head = sentinel;
		tail = sentinel;
		sentinel->next = sentinel;
		return;
	}else if (index == 0){
		//remove head
		ClientNode *deleteNode = head;
		head = head->next;
		sentinel->next = head;
		delete deleteNode;
	}
	else{
		for (int i = 0 ; i < index-1 ; i++){
			if (current->next == sentinel){
				throw "out of range";
			}
			current = current->next;
			
		}
		if (current->next == sentinel) throw "out of range";
		ClientNode *deleteNode = current->next;
		if (current->next == tail) {
			tail = current;
		}
		current->next = current->next->next;
		delete deleteNode;
	}
}



void ClientList::Delete(int index){
	ClientNode *current = head;
	if (head->next == sentinel && index == 0){
		//only have 1 node (sentinel)
		delete head;
		head = sentinel;
		tail = sentinel;
		sentinel->next = sentinel;
	}else if (index == 0){
		//remove head
		ClientNode *deleteNode = head;
		head = head->next;
		sentinel->next = head;
		delete deleteNode;
	}
	else{
		for (int i = 0 ; i < index-1 ; i++){
			if (current->next == sentinel){
				throw "out of range";
			}
			current = current->next;
			
		}
		if (current->next == sentinel) throw "out of range";
		ClientNode *deleteNode = current->next;
		current->next = current->next->next;
		delete deleteNode;
	}
	if (current->next == tail) tail = current;
}



ClientList::Iterator ClientList::begin(){
	return ClientList::Iterator(head);
} 


ClientList::Iterator ClientList::end(){
	return ClientList::Iterator(sentinel);
} 



ClientNode* ClientList::GetSentinel(){
	return sentinel;
}


int ClientList::Length(){
	ClientNode *current = head;
	int size = 0;
	if (head == sentinel || sentinel == nullptr) return 0;
	while(current != sentinel){
		size++;
		current = current->next; 
	}
	return size;
}



ClientNode* ClientList::GetNode(){
	ClientNode *x;
	if(av){
		x = av;
		av = av->next;
		x->next = nullptr;// remove next link
	} else x = nullptr ;// return empty 
	return x;
}



ClientNode* ClientList::GetAVlist(){
	ClientNode *x;
	if(av){
		x = av;
		av = nullptr;
	} else x = nullptr ;// return empty 
	return x;
}


/*
將一個節點放回  鏈結串列裡面 
*/
void ClientList::RetNode(ClientNode*& x){
	x->next = av;
	av = x;
	x = 0;
}


/*
當 LL 是以 CircularList 方式建構時 ， 解構 可以將 整串鏈結串列 鏈結到 av 之前  
*/
ClientList::~ClientList(){
	if (head != sentinel){
		tail->next = av;
		av = head;
		tail = 0;
		head = 0;
	}
}


bool ClientList::comp(Client a, Client b) {
	return true;
}

void ClientList::InsertSort(ClientNode *newNode, bool(*c)(Client,Client) ) {
	ClientNode* prev = sentinel, * current = head;
	int index = 0;
	if (c == nullptr) {
		while (current != sentinel && comp(newNode->data, current->data)) {
			prev = current;
			current = current->next;
			index++;
		}
	}
	else {
		while (current != sentinel && (*c)(newNode->data, current->data)) {
			prev = current;
			current = current->next;
			index++;
		}
	}
	if (index == 0) {
		//add front
		// change head
		InsertFront(newNode);
	}
	else if (current == sentinel) {
		//add last
		// change tail
		InsertTail(newNode);
	}
	else {
		// just add
		newNode->next = prev->next;
		prev->next = newNode;
	}
}