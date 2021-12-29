//used for while loop the client selction (in the Running() to check if client sending message or not )
//used circular list (data structure)
#include "ClientNode.h" 
#ifndef ClientList_H
#define ClientList_H //debug 


class ClientList{
	private:
		ClientNode *head;
		ClientNode *tail;
		ClientNode *sentinel;
	public:
		static ClientNode *av;//available list
		
		ClientList();
		~ClientList();
		ClientList(const ClientList &); // copy constructor
		void InsertFront(const Client&);
		void InsertTail(const Client&);
		void InsertFront(Client*);
		void InsertTail(Client*);
		void InsertFront(ClientNode*);
		void InsertTail(ClientNode*);
		void InsertSort(ClientNode*, bool(*c)(Client,Client) = nullptr);
		static ClientNode* GetNode();
		static ClientNode* GetAVlist();
		static void RetNode(ClientNode*&);
		void print();// print all list element
		void Delete(int);
		void Delete(const Client&);
		
		class Iterator;
		Iterator begin();
		Iterator end();
		
		bool comp(Client,Client);
		
		ClientNode* GetSentinel();
		int Length();
};


#endif