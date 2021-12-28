#include "Iterator.h"

ClientList::Iterator::Iterator(ClientNode* startNode){
	current = startNode;
} 


Client& ClientList::Iterator::operator*() const{
	return current->data;
} 



Client* ClientList::Iterator::operator->() const{
	return &current->data;
} 

typename ClientList::Iterator& ClientList::Iterator::operator++(){
	current = current->next;
	return *this;
}

typename ClientList::Iterator& ClientList::Iterator::operator++(int){
	ClientList::Iterator *old = this;
	current = current->next;
	return *old;
}


bool ClientList::Iterator::operator!=(const ClientList::Iterator right) const{
	return current != right.current;
}

bool ClientList::Iterator::operator==(const ClientList::Iterator right) const{
	return current == right.current;
}

ClientList::Iterator::operator bool(){
	return (bool) (current != nullptr);
}