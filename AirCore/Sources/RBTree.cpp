#include "RBTree.h"

namespace AirDB {

RBTree::RBTree(Comparator comparatorLT, Comparator comparatorEQ, int nodeDataSize) {
	this->allocator = new ChunkedAllocator();
	this->root = 0;
	this->first = 0;
	this->last = 0;
	this->comparatorLT = comparatorLT;
	this->comparatorEQ = comparatorEQ;
	this->size = 0;
	this->nodeDataSize = nodeDataSize;
}

RBTree::~RBTree() {
	delete allocator;
}

void* RBTree::add(void* nodeData) {
	Node* newNode = (Node*)allocator->allocate(getNodeSize());
	memcpy(newNode->getData(), nodeData, nodeDataSize);
	newNode->red = true;

	if (root == 0) {
		root = newNode;
		first = root;
		last = root;
	} else {
		insertWithoutValidation(newNode);
	}

	validateInsert12(newNode);

	size++;

	return newNode->getData();
}

bool RBTree::remove(void* nodeData) {
	Node* node = findNode(nodeData);
	if (node == 0) {
		return false;
	}

	if ((node->left != 0) && (node->right != 0)) {
		Node* next = node->next;
		memcpy(node->getData(), next->getData(), nodeDataSize);
		node = next;
	}

	Node* child = node->left == 0 ? node->right : node->left;
	if (!node->red) {
		if (child != 0) {
			child->red = false;
		} else {
			validateDelete2(node);
		}
	}

	removeWithoutValidation(node, child);

	ChunkedAllocator::Chunk* chunk = allocator->getTail();
	Node* tail = (Node*)((char*)chunk->getData() + chunk->getUsed() - getNodeSize());
	move(tail, node);
	allocator->shrink(getNodeSize());

	size--;
	return true;
}

RBTree::Node* RBTree::getUncle(Node* node) {
	Node* parent = node->parent;
	Node* grandpa = parent->parent;
	if (grandpa->left == parent) {
		return grandpa->right;
	} else {
		return grandpa->left;
	}
}

RBTree::Node* RBTree::getSibling(Node* node) {
	Node* parent = node->parent;
	if (parent->left == node) {
		return parent->right;
	} else {
		return parent->left;
	}
}

void RBTree::insertWithoutValidation(Node* newNode) {
	Node* current = root;
	Node* prev = 0;
	while (true) {
		bool insertLeft = comparatorLT(newNode->getData(), current->getData());

		if (insertLeft) {
			if (current->left == 0) {
				current->left = newNode;
				newNode->parent = current;
				newNode->next = current;
				newNode->prev = prev;
				if (prev != 0) {
					prev->next = newNode;
				}
				if (current == first) {
					first = newNode;
				}
				return;
			} else {
				current = current->left;
			}
		} else {
			prev = current;
			if (current->right == 0) {
				current->right = newNode;
				newNode->parent = current;
				newNode->next = current->next;
				current->next = newNode;
				newNode->prev = prev;
				if (current == last) {
					last = newNode;
				}
				return;
			} else {
				current = current->right;
			}
		}
	}
}

void RBTree::validateInsert12(Node* current) {
	if (current->parent == 0) {
		current->red = false;
	} else if (current->parent->red) {
		validateInsert3(current);
	}
}

void RBTree::validateInsert3(Node* current) {
	Node* uncle = getUncle(current);
	Node* parent = current->parent;
	if ((uncle != 0) && uncle->red && parent->red) {
		parent->red = false;
		uncle->red = false;
		Node* grandpa = parent->parent;
		grandpa->red = true;
		validateInsert12(grandpa);
	} else {
		validateInsert4(current);
	}
}

void RBTree::validateInsert4(Node* current) {
	Node* parent = current->parent;
	Node* grandpa = parent->parent;
	if ((current == parent->right) && (parent == grandpa->left)) {
		rotateLeft(parent);
		current = parent;
	} else if ((current == parent->left) && (parent == grandpa->right)) {
		rotateRight(parent);
		current = parent;
	}
	validateInsert5(current);
}

void RBTree::validateInsert5(Node* current) {
	Node* parent = current->parent;
	Node* grandpa = parent->parent;
	parent->red = false;
	grandpa->red = true;
	if (current == parent->left) {
		rotateRight(grandpa);
	} else {
		rotateLeft(grandpa);
	}
}

void RBTree::rotateLeft(Node* node) {
	Node* right = node->right;
	Node* parent = node->parent;
	if (parent == 0) {
		root = right;
	} else {
		if (parent->left == node) {
			parent->left = right;
		} else {
			parent->right = right;
		}
	}
	right->parent = node->parent;

	node->right = right->left;
	if (right->left != 0) {
		right->left->parent = node;
	}

	right->left = node;
	node->parent = right;
}

void RBTree::rotateRight(Node* node) {
	Node* left = node->left;
	Node* parent = node->parent;
	if (parent == 0) {
		root = left;
	} else {
		if (parent->left == node) {
			parent->left = left;
		} else {
			parent->right = left;
		}
	}
	left->parent = parent;

	node->left = left->right;
	if (left->right != 0) {
		left->right->parent = node;
	}

	left->right = node;
	node->parent = left;
}

long long RBTree::getSize() {
	return size;
}

RBTree::Node* RBTree::findNode(void* nodeData) {
	Node* current = root;
	while (current != 0) {
		void* currData = current->getData();
		if (comparatorEQ(currData, nodeData)) {
			break;
		} else {
			if (comparatorLT(currData, nodeData)) {
				current = current->right;
			} else {
				current = current->left;
			}
		}
	}
	return current;
}

void* RBTree::find(void* nodeData) {
	Node* node = findNode(nodeData);
	return node == 0 ? 0 : node->getData();
}

void RBTree::removeWithoutValidation(Node* node, Node* child) {
	Node* prev = node->prev;
	Node* next = node->next;
	if (prev != 0) {
		prev->next = next;
	}
	if (next != 0) {
		next->prev = prev;
	}
	if (first == node) {
		first = next;
	}
	if (last == node) {
		last = prev;
	}

	Node* parent = node->parent;
	if (parent == 0) {
		root = child;
	} else {
		if (parent->left == node) {
			parent->left = child;
		}
		else {
			parent->right = child;
		}
	}
	if (child != 0) {
		child->parent = parent;
	}
}

void RBTree::move(Node* src, Node* dest) {
	if (src == dest) {
		return;
	}

	if (root == src) {
		root = dest;
	}
	if (first == src) {
		first = dest;
	}
	if (last == src) {
		last = dest;
	}

	Node* parent = src->parent;
	if (parent != 0) {
		if (parent->left == src) {
			parent->left = dest;
		} else {
			parent->right = dest;
		}
	}

	if (src->left != 0) {
		src->left->parent = dest;
	}
	if (src->right != 0) {
		src->right->parent = dest;
	}

	if (src->prev != 0) {
		src->prev->next = dest;
	}
	if (src->next != 0) {
		src->next->prev = dest;
	}

	memcpy(dest, src, getNodeSize());
}

void RBTree::validateDelete2(Node* node) {
	Node* parent = node->parent;
	if (node->parent == 0) {
		return;
	}

	Node* sibling = getSibling(node);
	if (sibling->red) {
		parent->red = true;
		sibling->red = false;
		if (node == parent->left) {
			rotateLeft(parent);
		} else {
			rotateRight(parent);
		}
	}
	
	validateDelete3(node);
}

void RBTree::validateDelete3(Node* node) {
	Node* sibling = getSibling(node);
	Node* parent = node->parent;

	if (((sibling->left == 0) || !sibling->left->red) &&
		((sibling->right == 0) || !sibling->right->red)) {
		sibling->red = true;
		if (parent->red) {
			parent->red = false;
		} else {
			validateDelete2(parent);
		}
	} else {
		validateDelete5(node);
	}
}

void RBTree::validateDelete5(Node* node) {
	Node* sibling = getSibling(node);
	Node* parent = node->parent;
	Node* left = sibling->left;
	Node* right = sibling->right;

	if ((node == parent->left) && (left != 0) && left->red) {
		sibling->red = true;
		left->red = false;
		rotateRight(sibling);
	} else
	if ((node == parent->right) && (right != 0) && right->red) {
		sibling->red = true;
		right->red = false;
		rotateLeft(sibling);
	}

	validateDelete6(node);
}

void RBTree::validateDelete6(Node* node) {
	Node* sibling = getSibling(node);
	Node* parent = node->parent;

	sibling->red = parent->red;
	parent->red = false;

	if (node == parent->left) {
		sibling->right->red = false;
		rotateLeft(parent);
	} else {
		sibling->left->red = false;
		rotateRight(parent);
	}
}

bool RBTree::begin(Iterator* it) {
	it->node = first;
	return it->node != 0;
}

RBTree::Iterator::Iterator(Node* node) {
	this->node = node;
}

}