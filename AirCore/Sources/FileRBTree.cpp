#include "FileRBTree.h"

#include <memory>

namespace AirDB {

FileRBTree::FileRBTree(const char* fileName, Comparator comparatorLT) {
	this->meta = 0;
	this->file = 0;
	createOrOpenFile(fileName);
	this->nodes = new map<long long, Node*>();
	this->comparatorLT = comparatorLT;
}

FileRBTree::~FileRBTree() {
	flush();

	for (auto& i : *nodes) {
		delete i.second;
	}
	delete nodes;

	delete file;
	delete meta;
}

void FileRBTree::createOrOpenFile(const char* fileName) {
	FileMetadata fileMeta;
	if (!File::exists(fileName)) {
		unique_ptr<File> newFile (new File(fileName, "a"));
		newFile->write(&fileMeta, sizeof(fileMeta));
		newFile.reset();
	}

	file = new File(fileName);
	file->read(&fileMeta, sizeof(fileMeta));
	meta = new Metadata(this, &fileMeta);
}

bool FileRBTree::first(Iterator* iterator) {
	Node* firstNode = meta->first();
	if (firstNode != 0) {
		*iterator->node = *firstNode;
	}
	return firstNode != 0;
}

bool FileRBTree::validate() {
	Node node(0, 0, 0);
	Node* first = meta->first();
	if (first == 0) {
		return true;
	}
	memcpy(&node, first, sizeof(node));

	for (;;) {
		if (node.getAddressLeft() != 0) {
			Node left(0, 0 ,0);
			loadNode(node.getAddressLeft(), &left);
			bool valid = comparatorLT(left.getIndexData(), node.getIndexData()) && (!node.isRed() || !left.isRed());
			if (!valid) {
				return false;
			}
		}

		if (node.getAddressRight() != 0) {
			Node right(0, 0, 0);
			loadNode(node.getAddressRight(), &right);
			bool valid = comparatorLT(node.getIndexData(), right.getIndexData()) && (!node.isRed() || !right.isRed());
			if (!valid) {
				return false;
			}
		}

		if (!node.next(&node)) {
			break;
		}
	}

	return true;
}

long long FileRBTree::getNodeCount() {
	return nodes->size();
}

void FileRBTree::clearBuffer() {
	flush();

	delete meta;

	for (auto& i : *nodes) {
		delete i.second;
	}
	delete nodes;

	nodes = new map<long long, Node*>();

	FileMetadata fileMeta;
	file->read(0, &fileMeta, sizeof(fileMeta));
	meta = new Metadata(this, &fileMeta);
}

void FileRBTree::add(long long indexData) {
	FileNode fileNode(indexData, true);
	long long nodeAddress = appendNode(&fileNode);
	Node* newNode = new Node(this, nodeAddress, &fileNode);
	(*nodes)[nodeAddress] = newNode;

	if (meta->root() == 0) {
		meta->setRoot(newNode);
		meta->setFirst(newNode);
		meta->setLast(newNode);
	} else {
		insertWithoutValidation(newNode);
	}

	validateInsert12(newNode);

	meta->setSize(meta->getSize() + 1);
}

long long FileRBTree::appendNode(FileNode* node) {
	return file->append(node, sizeof(*node));
}

void FileRBTree::flush() {
	FileMetadata fileMeta(meta);
	file->write(0, &fileMeta, sizeof(fileMeta));
	for (auto& i : *nodes) {
		Node* node = i.second;
		if (node->isModified()) {
			FileNode fileNode(node);
			file->write(node->getAddressSelf(), &fileNode, sizeof(fileNode));
			node->setNotModified();
		}
	}
}

FileRBTree::Node* FileRBTree::loadNode(long long nodeAddress) {
	auto& match = nodes->find(nodeAddress);
	if (match != nodes->end()) {
		return match->second;
	}

	FileNode fileNode;
	file->read(nodeAddress, &fileNode, sizeof(fileNode));
	Node* node = new Node(this, nodeAddress, &fileNode);
	(*nodes)[nodeAddress] = node;

	return node;
}

void FileRBTree::loadNode(long long nodeAddress, Node* node) {
	FileNode fileNode;
	file->read(nodeAddress, &fileNode, sizeof(fileNode));
	Node* newNode = new(node) Node(this, nodeAddress, &fileNode);
}

FileRBTree::Node* FileRBTree::getUncle(Node* node) {
	Node* parent = node->parent();
	Node* grandpa = parent->parent();
	if (grandpa->getAddressLeft() == parent->getAddressSelf()) {
		return grandpa->right();
	} else {
		return grandpa->left();
	}
}

void FileRBTree::rotateLeft(Node* node) {
	Node* right = node->right();
	Node* parent = node->parent();
	if (parent == 0) {
		meta->setRoot(right);
	} else {
		if (parent->getAddressLeft() == node->getAddressSelf()) {
			parent->setLeft(right);
		} else {
			parent->setRight(right);
		}
	}
	right->setParent(parent);

	Node* rightLeft = right->left();
	node->setRight(rightLeft);
	if (rightLeft != 0) {
		rightLeft->setParent(node);
	}

	right->setLeft(node);
	node->setParent(right);
}

void FileRBTree::rotateRight(Node* node) {
	Node* left = node->left();
	Node* parent = node->parent();
	if (parent == 0) {
		meta->setRoot(left);
	} else {
		if (parent->getAddressLeft() == node->getAddressSelf()) {
			parent->setLeft(left);
		} else {
			parent->setRight(left);
		}
	}
	left->setParent(parent);

	Node* leftRight = left->right();
	node->setLeft(leftRight);
	if (leftRight != 0) {
		leftRight->setParent(node);
	}

	left->setRight(node);
	node->setParent(left);
}

void FileRBTree::validateInsert12(Node* current) {
	Node* parent = current->parent();
	if (parent == 0) {
		current->setRed(false);
	} else if (parent->isRed()) {
		validateInsert3(current);
	}
}

void FileRBTree::validateInsert3(Node* current) {
	Node* uncle = getUncle(current);
	Node* parent = current->parent();
	if ((uncle != 0) && uncle->isRed() && parent->isRed()) {
		parent->setRed(false);
		uncle->setRed(false);
		Node* grandpa = parent->parent();
		grandpa->setRed(true);
		validateInsert12(grandpa);
	} else {
		validateInsert4(current);
	}
}

void FileRBTree::validateInsert4(Node* current) {
	Node* parent = current->parent();
	Node* grandpa = parent->parent();
	if ((current->getAddressSelf() == parent->getAddressRight()) && (parent->getAddressSelf() == grandpa->getAddressLeft())) {
		rotateLeft(parent);
		current = parent;
	} else if ((current->getAddressSelf() == parent->getAddressLeft()) && (parent->getAddressSelf() == grandpa->getAddressRight())) {
		rotateRight(parent);
		current = parent;
	}
	validateInsert5(current);
}

void FileRBTree::validateInsert5(Node* current) {
	Node* parent = current->parent();
	Node* grandpa = parent->parent();
	parent->setRed(false);
	grandpa->setRed(true);
	if (current->getAddressSelf() == parent->getAddressLeft()) {
		rotateRight(grandpa);
	} else {
		rotateLeft(grandpa);
	}
}

void FileRBTree::insertWithoutValidation(Node* newNode) {
	Node* current = meta->root();
	Node* prev = 0;
	while (true) {
		bool insertLeft = comparatorLT(newNode->getIndexData(), current->getIndexData());

		if (insertLeft) {
			if (current->getAddressLeft() == 0) {
				current->setLeft(newNode);
				newNode->setParent(current);
				newNode->setNext(current);
				newNode->setPrev(prev);
				if (prev != 0) {
					prev->setNext(newNode);
				}
				if (current == meta->first()) {
					meta->setFirst(newNode);
				}
				return;
			} else {
				current = current->left();
			}
		} else {
			prev = current;
			if (current->getAddressRight() == 0) {
				current->setRight(newNode);
				newNode->setParent(current);
				newNode->setAddressNext(current->getAddressNext());
				current->setNext(newNode);
				newNode->setPrev(prev);
				if (current == meta->last()) {
					meta->setLast(newNode);
				}
				return;
			} else {
				current = current->right();
			}
		}
	}
}

FileRBTree::Iterator::Iterator() {
	node = new Node(0, 0, 0);
}

FileRBTree::Iterator::~Iterator() {
	delete node;
}

bool FileRBTree::Iterator::isEmpty() {
	return node->getAddressSelf() == 0;
}

bool FileRBTree::Iterator::next() {
	if (node->getAddressSelf() == 0) {
		throw exception("Invalid iterator");
	}

	bool result = node->next(node);
	if (!result) {
		delete node;
		node = new Node(0, 0, 0);
	}

	return result;
}

long long FileRBTree::Iterator::getIndexData() {
	if (node->getAddressSelf() == 0) {
		throw exception("Invalid iterator");
	}
	return *(long long*)node->getIndexData();
}

FileRBTree::Metadata::Metadata(FileRBTree* tree, FileMetadata* fileMeta) {
	this->tree = tree;
	this->size = fileMeta->size;
	this->indexDataSize = fileMeta->indexDataSize;

	this->addressRoot = fileMeta->addressRoot;
	this->addressFirst = fileMeta->addressFirst;
	this->addressLast = fileMeta->addressLast;

	this->_first = 0;
	this->_last = 0;
	this->_root = 0;
}

FileRBTree::FileMetadata::FileMetadata(Metadata* src) {
	size = src->getSize();
	indexDataSize = src->getIndexDataSize();
	addressRoot = src->root() == 0 ? 0 : src->root()->getAddressSelf();
	addressFirst = src->first() == 0 ? 0 : src->first()->getAddressSelf();
	addressLast = src->last() == 0 ? 0 : src->last()->getAddressSelf();
}

FileRBTree::Node::Node(FileRBTree* tree, long long nodeAddress, FileNode* fileNode) {
	this->tree = tree;
	this->addressSelf = nodeAddress;
	this->addressParent = fileNode == 0 ? 0 : fileNode->addressParent;
	this->addressNext = fileNode == 0 ? 0 : fileNode->addressNext;
	this->addressPrev = fileNode == 0 ? 0 : fileNode->addressPrev;
	this->addressLeft = fileNode == 0 ? 0 : fileNode->addressLeft;
	this->addressRight = fileNode == 0 ? 0 : fileNode->addressRight;
	this->addressData = fileNode == 0 ? 0 : fileNode->addressData;
	this->indexData = fileNode == 0 ? 0 : fileNode->indexData;
	this->red = fileNode == 0 ? false : fileNode->red;
	this->modified = false;

	this->_parent = 0;
	this->_next = 0;
	this->_prev = 0;
	this->_left = 0;
	this->_right = 0;
}

FileRBTree::FileNode::FileNode(long long indexData, bool red) {
	this->addressParent = 0;
	this->addressNext = 0;
	this->addressPrev = 0;
	this->addressLeft = 0;
	this->addressRight = 0;
	this->addressData = 0;
	this->indexData = indexData;
	this->red = red;
}

FileRBTree::FileNode::FileNode(Node* node) {
	this->addressParent = node->getAddressParent();
	this->addressNext = node->getAddressNext();
	this->addressPrev = node->getAddressPrev();
	this->addressLeft = node->getAddressLeft();
	this->addressRight = node->getAddressRight();
	this->addressData = 0;
	this->indexData = *(long long*)node->getIndexData();
	this->red = node->isRed();
}

}