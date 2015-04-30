#pragma once

#include "File.h"

#include <map>

namespace AirDB {

using namespace std;

class FileRBTree {
public:
	typedef bool(*Comparator)(void*, void*);
	class Iterator;
private:
	class Metadata;
	class FileMetadata;
	class Node;
	class FileNode;

	Metadata* meta;
	File* file;
	map<long long, Node*>* nodes;
	Comparator comparatorLT;

	void createOrOpenFile(const char* fileName);
	Node* loadNode(long long nodeAddress);
	void loadNode(long long nodeAddress, Node* node);
	Node* getUncle(Node* node);
	void rotateLeft(Node* node);
	void rotateRight(Node* node);
	void validateInsert12(Node* current);
	void validateInsert3(Node* current);
	void validateInsert4(Node* current);
	void validateInsert5(Node* current);
	void insertWithoutValidation(Node* newNode);
	long long appendNode(FileNode* node);
	void flush();

public:
	FileRBTree(const char* fileName, Comparator comparatorLT);
	~FileRBTree();
	void add(long long indexData);
	bool first(Iterator* iterator);
	bool validate();
	long long getNodeCount();
	void clearBuffer();

public:
	class Iterator {
	private:
		friend class FileRBTree;
		Node* node;
	public:
		Iterator();
		~Iterator();
		bool isEmpty();
		bool next();
		long long getIndexData();
	};

private:
	#pragma pack(push, 1)
	class FileMetadata {
	private:
		friend class Metadata;
		long long size;
		short indexDataSize;
		long long addressRoot;
		long long addressFirst;
		long long addressLast;
	public:
		FileMetadata() { memset(this, 0, sizeof(*this)); }
		FileMetadata(Metadata* src);
	};
	
	class FileNode {
	private:
		friend class Node;
		long long addressParent;
		long long addressNext;
		long long addressPrev;
		long long addressLeft;
		long long addressRight;
		long long addressData;
		long long indexData;
		bool red;
	public:
		FileNode() : FileNode(0, false) {};
		FileNode(long long indexData, bool red);
		FileNode(Node* node);
	};
	#pragma pack(pop)

	class Metadata {
	private:
		FileRBTree* tree;
		long long size;
		short indexDataSize;
		Node* _root;
		Node* _first;
		Node* _last;
		long long addressRoot;
		long long addressFirst;
		long long addressLast;
	public:
		Metadata(FileRBTree* tree, FileMetadata* fileMeta);

		short getIndexDataSize() { return indexDataSize; }
		long long getSize() { return size; }

		void setSize(long long size) { this->size = size; }

		Node* root() {
			if ((_root == 0) && (addressRoot != 0)) {
				_root = tree->loadNode(addressRoot);
			}
			return _root;
		}

		void setRoot(Node* node) {
			addressRoot = node == 0 ? 0 : node->addressSelf;
			_root = node;
		}

		Node* first() {
			if ((_first == 0) && (addressFirst != 0)) {
				_first = tree->loadNode(addressFirst);
			}
			return _first;
		}

		void setFirst(Node* node) {
			addressFirst = node == 0 ? 0 : node->addressSelf;
			_first = node;
		}

		Node* last() {
			if ((_last == 0) && (addressLast != 0)) {
				_last = tree->loadNode(addressLast);
			}
			return _last;
		}

		void setLast(Node* node) {
			addressLast = node == 0 ? 0 : node->addressSelf;
			_last = node;
		}
	};

	class Node {
	private:
		friend class Metadata;
		FileRBTree* tree;
		Node* _parent;
		Node* _next;
		Node* _prev;
		Node* _left;
		Node* _right;
		long long addressSelf;
		long long addressParent;
		long long addressNext;
		long long addressPrev;
		long long addressLeft;
		long long addressRight;
		long long addressData;
		long long indexData;
		bool red;
		bool modified;
	public:
		Node(FileRBTree* tree, long long nodeAddress, FileNode* fileNode);

		long long getAddressSelf() { return addressSelf; }
		long long getAddressParent() { return addressParent; }
		long long getAddressNext() { return addressNext; }
		long long getAddressPrev() { return addressPrev; }
		long long getAddressLeft() { return addressLeft; }
		long long getAddressRight() { return addressRight; }
		void* getIndexData() { return &indexData; }
		bool isRed() { return red; }
		bool isModified() { return modified; }
		void setNotModified() { this->modified = false; }

		void setRed(bool value) {
			red = value;
			modified = true;
		}

		void setAddressNext(long long addressNext) {
			this->addressNext = addressNext;
			this->_next = 0;
			this->modified = true;
		}

		Node* parent() {
			if ((_parent == 0) && (addressParent != 0)) {
				_parent = tree->loadNode(addressParent);
			}
			return _parent;
		}

		void setParent(Node* node) {
			addressParent = node == 0 ? 0 : node->addressSelf;
			_parent = node;
			modified = true;
		}

		Node* next() {
			if ((_next == 0) && (addressNext != 0)) {
				_next = tree->loadNode(addressNext);
			}
			return _next;
		}

		bool next(Node* dest) {
			if (addressNext != 0) {
				tree->loadNode(addressNext, dest);
				return true;
			}
			return false;
		}

		void setNext(Node* node) {
			addressNext = node == 0 ? 0 : node->addressSelf;
			_next = node;
			modified = true;
		}

		Node* prev() {
			if ((_prev == 0) && (addressPrev != 0)) {
				_prev = tree->loadNode(addressPrev);
			}
			return _prev;
		}

		void setPrev(Node* node) {
			addressPrev = node == 0 ? 0 : node->addressSelf;
			_prev = node;
			modified = true;
		}

		Node* left() {
			if ((_left == 0) && (addressLeft != 0)) {
				_left = tree->loadNode(addressLeft);
			}
			return _left;
		}

		void setLeft(Node* node) {
			addressLeft = node == 0 ? 0 : node->addressSelf;
			_left = node;
			modified = true;
		}

		Node* right() {
			if ((_right == 0) && (addressRight != 0)) {
				_right = tree->loadNode(addressRight);
			}
			return _right;
		}

		void setRight(Node* node) {
			addressRight = node == 0 ? 0 : node->addressSelf;
			_right = node;
			modified = true;
		}
	};
};

}