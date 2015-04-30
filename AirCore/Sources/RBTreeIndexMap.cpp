#include "RBTreeIndexMap.h"

namespace AirDB {

RBTreeIndexMap::RBTreeIndexMap() {
	iterator = new RBTree::Iterator();
	data = new RBTree(RBTreeIndexMap::compare, 0, sizeof(Item));
}


RBTreeIndexMap::~RBTreeIndexMap() {
	delete iterator;
	delete data;
}

bool RBTreeIndexMap::compare(void* left, void* right) {
	return ((Item*)left)->key < ((Item*)right)->key;
}

void RBTreeIndexMap::add(LongIndex key, char* data) {
	Item item(key, data);
	this->data->add(&item);
}

bool RBTreeIndexMap::begin() {
	return data->begin(iterator);
}

LongIndex RBTreeIndexMap::getKey() {
	Item* item = (Item*)iterator->getData();
	return item->key;
}

int RBTreeIndexMap::getSize() {
	return (int)data->getSize();
}

}