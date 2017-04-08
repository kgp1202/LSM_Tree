#pragma once
#define CHILD_SIZE 7		//CHILS_SIZE는 4보다 커야한다.
#define MAX_VALUE 200000000
#define KEY_TYPE int
#define VALUE_TYPE void*

#include <stdlib.h>
#include <string.h>

typedef struct _Data {
	int testN;
} Data;

typedef struct _Node {
	void* parent;
	void* child[CHILD_SIZE + 1];
	KEY_TYPE key[CHILD_SIZE];
	char isLeaf;
} Node;

typedef struct _Level {
	Node* root;
	int bptree_size;
	int bptree_max_size;
} Level;

KEY_TYPE tempKeyArr[CHILD_SIZE * 2];
void* tempValueArr[CHILD_SIZE * 2];

Level* bptree_init(int max_size);

Node* bptree_init_node();
Node* bptree_init_leaf_node();

int bptree_insert(Level* bptree, KEY_TYPE inputKey, void* inputValue);
int bptree_delete(Level* bptree, KEY_TYPE deletedKey);
Data* bptree_search(Level* bptree, KEY_TYPE searchKey);

//FOR DEBUG
#include <stdio.h>
void print(Node* root);