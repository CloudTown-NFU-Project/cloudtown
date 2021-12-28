#ifndef ClientIterator_H
#define ClientIterator_H
#include "ClientList.h"

class ClientList::Iterator{
	private:
		ClientNode *current;
	public:
		Iterator(ClientNode *startNode = 0);
		Client& operator*() const;
		Client* operator->() const;
		ClientList::Iterator& operator++();
		ClientList::Iterator& operator++(int);
		bool operator !=(const Iterator right) const;
		bool operator ==(const Iterator right) const;
		operator bool();
};
#endif //CircularList_Iterator_H