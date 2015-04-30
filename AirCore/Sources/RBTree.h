#pragma once

#include "ChunkedAllocator.h"

namespace AirDB {

class RBTree {
public:
	typedef bool(*Comparator)(void*, void*);
private:
	class Node;
	ChunkedAllocator* allocator;
	Node* root;
	Node* first;
	Node* last;
	Comparator comparatorLT;
	Comparator comparatorEQ;
	long long size;
	int nodeDataSize;

	int getNodeSize() { return sizeof(Node) + nodeDataSize; }
	Node* getUncle(Node* node);
	Node* getSibling(Node* node);
	void insertWithoutValidation(Node* newNode);
	void validateInsert12(Node* current);
	void validateInsert3(Node* current);
	void validateInsert4(Node* current);
	void validateInsert5(Node* current);
	void rotateLeft(Node* node);
	void rotateRight(Node* node);
	Node* findNode(void* nodeData);
	void removeWithoutValidation(Node* node, Node* child);
	void move(Node* src, Node* dest);
	void validateDelete2(Node* node);
	void validateDelete3(Node* node);
	void validateDelete5(Node* node);
	void validateDelete6(Node* node);
public:
	class Iterator;
	RBTree(Comparator comparatorLT, Comparator comparatorEQ, int nodeDataSize);
	~RBTree();
	void* add(void* nodeData);
	bool remove(void* nodeData);
	void* find(void* nodeData);
	bool begin(Iterator* it);
	long long getSize();
private:
	#pragma pack(push, 1)
	class Node {
		friend class RBTree;
		Node* parent;
		Node* next;
		Node* prev;
		Node* left;
		Node* right;
		bool red;
		void* getData() { return (void*)((char*)this + sizeof(Node)); }
	};
	#pragma pack(pop)
public:
	class Iterator {
		friend class RBTree;
	private:
		Node* node;
		Iterator(Node* node);
	public:
		Iterator() { node = 0; }
		bool next() { node = node->next; return node != 0; }
		void* getData() { return node->getData(); }
	};
};

}