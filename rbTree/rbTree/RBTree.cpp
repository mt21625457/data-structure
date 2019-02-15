#include "RBTree.h"

#include <iostream>

mt::RBTree::RBTree()
{
}


mt::RBTree::~RBTree()
{
}

bool mt::RBTree::init()
{
	if (this->m_rbtree == nullptr) {
		
		this->m_rbtree = new rbtree;
		
		if (this->m_rbtree == nullptr) {
			return false;
		}

		this->m_rbtree->nil = new rbtree_node;
		this->m_rbtree->nil->basic = new rbtree_basic;
		this->m_rbtree->nil->bus = new rbtree_KeyValue;

		this->m_rbtree->nil->basic->color = mt::BLACK;
		this->m_rbtree->root = this->m_rbtree->nil;

	}
	return true;
}

bool mt::RBTree::insert(rbtree_node * node)
{
	if (this->m_rbtree == nullptr) {
		return false;
	}

	this->rbtree_insert(node);
	return true;
}

mt::rbtree_node * mt::RBTree::init_node(KEY_TYPE key, void * value)
{
	mt::rbtree_node* node = new mt::rbtree_node;
	node->basic = new rbtree_basic;
	node->bus = new rbtree_KeyValue;

	node->bus->key = key;
	node->bus->value = value;

	return node;
}

void mt::RBTree::del_node(rbtree_node * node)
{
}

mt::rbtree_node* mt::RBTree::find(KEY_TYPE key)
{
	return this->rbtree_search(key);
}

mt::rbtree_node * mt::RBTree::del(rbtree_node * node)
{
	return this->rbtree_delete(node);
}

void mt::RBTree::traversal()
{
	this->rbtree_traversal(this->m_rbtree->root);
}

void mt::RBTree::close()
{
	if (this->m_rbtree != nullptr) {
	
		delete this->m_rbtree ;
		if (this->m_rbtree->root != nullptr) {
			delete this->m_rbtree->root;
		}		
	}
}


void mt::RBTree::rbtree_insert(rbtree_node * node)
{
	rbtree_node * y = this->m_rbtree->nil;
	rbtree_node * x = this->m_rbtree->root;

	while (x != this->m_rbtree->nil) {
		y = x;
		if (node->bus->key < x->bus->key) {
			x = x->basic->left;
		}
		else if (node->bus->key > x->bus->key) {
			x = x->basic->right;
		}
		else {
			return;
		}
	}

	node->basic->parent = y;
	if (y == this->m_rbtree->nil) {
		this->m_rbtree->root = node;
	}
	else if (node->bus->key < y->bus->key) {
		y->basic->left = node;
	}
	else {
		y->basic->right= node;
	}

	node->basic->left = this->m_rbtree->nil;
	node->basic->right = this->m_rbtree->nil;
	node->basic->color = RED;

	this->rbtree_insert_fixup(node);
}

void mt::RBTree::rbtree_left_rotate(rbtree_node * node)
{
	rbtree_node * y = node->basic->right;

	/*1-1 1-2*/
	node->basic->right = y->basic->left;
	if (y->basic->left != this->m_rbtree->nil) {
		y->basic->left->basic->parent = node;
	}

	/*2-1 2-2*/
	y->basic->parent = node->basic->parent;
	if (node->basic->parent == this->m_rbtree->nil) {
		this->m_rbtree->root = y;
	}
	else if (node == node->basic->parent->basic->left) {
		node->basic->parent->basic->left = y;
	}
	else {
		node->basic->parent->basic->right = y;
	}

	/*3-1 3-2*/
	y->basic->left = node;
	node->basic->parent = y;
}

void mt::RBTree::rbtree_right_rotate(rbtree_node * node)
{
	rbtree_node * x = node->basic->left;

	/*1-1 1-2*/
	node->basic->left = x->basic->right;
	if (x->basic->right != this->m_rbtree->nil) {
		x->basic->right->basic->parent = node;
	}

	/*2-1 2-2*/
	x->basic->parent = node->basic->parent;
	if (node->basic->parent == this->m_rbtree->nil) {
		this->m_rbtree->root = x;
	}
	else if (node == node->basic->parent->basic->right) {
		node->basic->parent->basic->right = x;
	}
	else {
		node->basic->parent->basic->left = x;
	}

	/*3-1 3-2*/
	x->basic->right = node;
	node->basic->parent = x;

}

void mt::RBTree::rbtree_insert_fixup(rbtree_node * node)
{
	std::cout << "rbtree_insert_fixup --->" << std::endl;
	while (node->basic->parent->basic->color == RED) { //z ---> RED
		if (node->basic->parent == node->basic->parent->basic->parent->basic->left) {
			rbtree_node *y = node->basic->parent->basic->parent->basic->right;
			if (y->basic->color == RED) {
				node->basic->parent->basic->color = BLACK;
				y->basic->color = BLACK;
				node->basic->parent->basic->parent->basic->color = RED;

				node = node->basic->parent->basic->parent; //z --> RED
			}
			else {

				if (node == node->basic->parent->basic->right) {
					node = node->basic->parent;
					this->rbtree_left_rotate(node);
				}

				node->basic->parent->basic->color = BLACK;
				node->basic->parent->basic->parent->basic->color = RED;
				rbtree_right_rotate(node->basic->parent->basic->parent);
			}
		}
		else {
			rbtree_node *y = node->basic->parent->basic->parent->basic->left;
			if (y->basic->color == RED) {
				node->basic->parent->basic->color = BLACK;
				y->basic->color = BLACK;
				node->basic->parent->basic->parent->basic->color = RED;

				node = node->basic->parent->basic->parent; //z --> RED
			}
			else {
				if (node == node->basic->parent->basic->left) {
					node = node->basic->parent;
					rbtree_right_rotate(node);
				}

				node->basic->parent->basic->color = BLACK;
				node->basic->parent->basic->parent->basic->color = RED;
				rbtree_left_rotate(node->basic->parent->basic->parent);
			}
		}

	}

	this->m_rbtree->root->basic->color = BLACK;
}
mt::rbtree_node * mt::RBTree::rbtree_delete(rbtree_node * node)
{
	rbtree_node *y = this->m_rbtree->nil;
	rbtree_node *x = this->m_rbtree->nil;

	if ((node->basic->left == this->m_rbtree->nil) || (node->basic->right == this->m_rbtree->nil)) {
		y = node;
	}
	else {
		y = rbtree_successor(node);
	}

	if (y->basic->left != this->m_rbtree->nil) {
		x = y->basic->left;
	}
	else if (y->basic->right != this->m_rbtree->nil) {
		x = y->basic->right;
	}

	x->basic->parent = y->basic->parent;
	if (y->basic->parent == this->m_rbtree->nil) {
		this->m_rbtree->root = x;
	}
	else if (y == y->basic->parent->basic->left) {
		y->basic->parent->basic->left = x;
	}
	else {
		y->basic->parent->basic->right = x;
	}

	if (y != node) {
		node->bus->key = y->bus->key;
		node->bus->value = y->bus->value;
	}

	if (y->basic->color == BLACK) {
		this->rbtree_delete_fixup(x);
	}

	return y;
}
void mt::RBTree::rbtree_delete_fixup(rbtree_node * node)
{
	while ((node != this->m_rbtree->root) && (node->basic->color == BLACK)) {
		if (node == node->basic->parent->basic->left) {

			rbtree_node *w = node->basic->parent->basic->right;
			if (w->basic->color == RED) {
				w->basic->color = BLACK;
				node->basic->parent->basic->color = RED;

				rbtree_left_rotate(node->basic->parent);
				w = node->basic->parent->basic->right;
			}

			if ((w->basic->left->basic->color == BLACK) && (w->basic->right->basic->color == BLACK)) {
				w->basic->color = RED;
				node = node->basic->parent;
			}
			else {

				if (w->basic->right->basic->color == BLACK) {
					w->basic->left->basic->color = BLACK;
					w->basic->color = RED;
					this->rbtree_right_rotate(w);
					w = node->basic->parent->basic->right;
				}

				w->basic->color = node->basic->parent->basic->color;
				node->basic->parent->basic->color = BLACK;
				w->basic->right->basic->color = BLACK;
				rbtree_left_rotate(node->basic->parent);

				node = this->m_rbtree->root;
			}

		}
		else {

			rbtree_node *w = node->basic->parent->basic->left;
			if (w->basic->color == RED) {
				w->basic->color = BLACK;
				node->basic->parent->basic->color = RED;
				rbtree_right_rotate(node->basic->parent);
				w = node->basic->parent->basic->left;
			}

			if ((w->basic->left->basic->color == BLACK) && (w->basic->right->basic->color == BLACK)) {
				w->basic->color = RED;
				node = node->basic->parent;
			}
			else {

				if (w->basic->left->basic->color == BLACK) {
					w->basic->right->basic->color = BLACK;
					w->basic->color = RED;
					rbtree_left_rotate(w);
					w = node->basic->parent->basic->left;
				}

				w->basic->color = node->basic->parent->basic->color;
				node->basic->parent->basic->color = BLACK;
				w->basic->left->basic->color = BLACK;
				rbtree_right_rotate(node->basic->parent);

				node = this->m_rbtree->root;
			}

		}
	}

	node->basic->color = BLACK;
}
void mt::RBTree::rbtree_traversal(rbtree_node * node)
{
	if (node != this->m_rbtree->nil) {
		rbtree_traversal(node->basic->left);
		std::cout << "key:" << node->bus->key << " color:" << node->basic->color << std::endl;
		rbtree_traversal(node->basic->right);
	}
}

mt::rbtree_node * mt::RBTree::rbtree_search(KEY_TYPE key)
{
	rbtree_node *node = this->m_rbtree->root;
	while (node != this->m_rbtree->nil) {
		if (key < node->bus->key) {
			node = node->basic->left;
		}
		else if (key > node->bus->key) {
			node = node->basic->right;
		}
		else {
			return node;
		}
	}
	return this->m_rbtree->nil;
}

mt::rbtree_node * mt::RBTree::rbtree_successor(rbtree_node * node)
{
	rbtree_node *y = node->basic->parent;

	if (node->basic->right != this->m_rbtree->nil) {
		return rbtree_mini(node->basic->right);
	}

	while ((y != this->m_rbtree->nil) && (node == y->basic->right)) {
		node = y;
		y = y->basic->parent;
	}
	return y;
}

mt::rbtree_node * mt::RBTree::rbtree_mini(rbtree_node * node)
{
	while (node->basic->left != this->m_rbtree->nil) {
		node = node->basic->left;
	}
	return node;
}

mt::rbtree_node * mt::RBTree::rbtree_maxi(rbtree_node * node)
{
	while (node->basic->right != this->m_rbtree->nil) {
		node = node->basic->right;
	}
	return node;
}
                         
uint64_t mt::RBTree::MurmurHash2_64_64(const void * key, int len, unsigned int seed)
{
	const uint64_t m = 0xc6a4a7935bd1e995;
	const int r = 47;

	uint64_t h = seed ^ (len * m);

	const uint64_t * data = (const uint64_t *)key;
	const uint64_t * end = data + (len / 8);

	while (data != end)
	{
		uint64_t k = *data++;

		k *= m;
		k ^= k >> r;
		k *= m;

		h ^= k;
		h *= m;
	}

	const unsigned char * data2 = (const unsigned char*)data;

	switch (len & 7)
	{
	case 7: h ^= uint64_t(data2[6]) << 48;
	case 6: h ^= uint64_t(data2[5]) << 40;
	case 5: h ^= uint64_t(data2[4]) << 32;
	case 4: h ^= uint64_t(data2[3]) << 24;
	case 3: h ^= uint64_t(data2[2]) << 16;
	case 2: h ^= uint64_t(data2[1]) << 8;
	case 1: h ^= uint64_t(data2[0]);
		h *= m;
	};

	h ^= h >> r;
	h *= m;
	h ^= h >> r;

	return h;
}

uint64_t mt::RBTree::MurmurHash2_64_32(const void * key, int len, unsigned int seed)
{
	const unsigned int m = 0x5bd1e995;
	const int r = 24;

	unsigned int h1 = seed ^ len;
	unsigned int h2 = 0;

	const unsigned int * data = (const unsigned int *)key;

	while (len >= 8)
	{
		unsigned int k1 = *data++;
		k1 *= m; k1 ^= k1 >> r; k1 *= m;
		h1 *= m; h1 ^= k1;
		len -= 4;

		unsigned int k2 = *data++;
		k2 *= m; k2 ^= k2 >> r; k2 *= m;
		h2 *= m; h2 ^= k2;
		len -= 4;
	}

	if (len >= 4)
	{
		unsigned int k1 = *data++;
		k1 *= m; k1 ^= k1 >> r; k1 *= m;
		h1 *= m; h1 ^= k1;
		len -= 4;
	}

	switch (len)
	{
	case 3: h2 ^= ((unsigned char*)data)[2] << 16;
	case 2: h2 ^= ((unsigned char*)data)[1] << 8;
	case 1: h2 ^= ((unsigned char*)data)[0];
		h2 *= m;
	};

	h1 ^= h2 >> 18; h1 *= m;
	h2 ^= h1 >> 22; h2 *= m;
	h1 ^= h2 >> 17; h1 *= m;
	h2 ^= h1 >> 19; h2 *= m;

	uint64_t h = h1;

	h = (h << 32) | h2;

	return h;
}
