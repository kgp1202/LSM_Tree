#define CHILD_SIZE 4
#define MAX_VALUE 200000000
#define KEY_TYPE int
#define VALUE_TYPE void*

#include <stdlib.h>
#include <string.h>

//FOR DEBUG
#include <stdio.h>

int tempBloomFilter = 123;

typedef struct _Node {
	void* parent;
	void* child[CHILD_SIZE + 1];
	KEY_TYPE key[CHILD_SIZE];
	char isLeaf;
} Node;


typedef struct _Data {
	int testN;
} Data;

Node* init(int max_size) {

}

Node* initNode() {
	Node* retNode = (Node*)malloc(sizeof(Node));

	for (int i = 0; i < CHILD_SIZE; i++) {
		retNode->key[i] = MAX_VALUE;
	}
	retNode->isLeaf = 0;
	retNode->parent = NULL;

	return retNode;
}
Node* initLeafNode() {
	Node* retNode = (Node*)malloc(sizeof(Node));
	
	for (int i = 0; i < CHILD_SIZE; i++) {
		retNode->key[i] = MAX_VALUE;
	}
	retNode->isLeaf = 1;
	retNode->parent = NULL;

	return retNode;
}

KEY_TYPE InsertTempKey[CHILD_SIZE];
void* InsertTempValue[CHILD_SIZE];

//Root Node에 대한 설정 수정 요망@@@@@@@@@@@@@@@@
Node* root;
void Insert(Node* root2, KEY_TYPE inputKey, void* inputValue) {
	Node* current = root;
	char insertComplete = 0;
	char doingSpilt = 0;
	while (insertComplete == 0) {
		for (int i = 0; i < CHILD_SIZE; i++) {
			if (current->key[i] > inputKey) {
				if(!(current->isLeaf) && !doingSpilt)	//Not LeafNode
				{	
					current = ((Node**)current->child)[i];
					break;
				}
				else {								//LeafNode
					//Move Back Key
					int howManyMove = CHILD_SIZE - i - 1;
					if (howManyMove > 0) {
						memcpy(InsertTempKey, &(current->key[i]), sizeof(KEY_TYPE) * howManyMove);
						memcpy(&(current->key[i + 1]), InsertTempKey, sizeof(KEY_TYPE) * howManyMove);
					}
					current->key[i] = inputKey;

					//Move Back Child
					if (current->isLeaf) {
						if (howManyMove > 0) {
							memcpy(InsertTempValue, &((Node**)current->child)[i], sizeof(void*) * howManyMove);
							memcpy(&((Node**)current->child)[i + 1], InsertTempValue, sizeof(void*) * howManyMove);
						}
						current->child[i] = inputValue;
					}
					else {
						if (howManyMove > 0) {
							memcpy(InsertTempValue, &((Node**)current->child)[i + 1], sizeof(void*) * howManyMove);
							memcpy(&((Node**)current->child)[i + 2], InsertTempValue, sizeof(void*) * howManyMove);
						}
						current->child[i + 1] = inputValue;

						((Node*)inputValue)->parent = current;
					}
					
					
					//Spilt Check
					if (current->key[CHILD_SIZE - 1] != MAX_VALUE) {
						doingSpilt = 1;
						inputKey = current->key[CHILD_SIZE / 2];

						//set InputValue
						if (current->isLeaf) {		
							//create LeafNode
							inputValue = (void*)initLeafNode();

							//connect each other 
							((Node*)inputValue)->child[CHILD_SIZE] = current->child[CHILD_SIZE];
							current->child[CHILD_SIZE] = inputValue;

							//copy to inputValue
							int copyStart = CHILD_SIZE / 2;
							int howManyCopy = CHILD_SIZE - CHILD_SIZE / 2;
							memcpy(((Node*)inputValue)->child, &current->child[copyStart], sizeof(void*) * howManyCopy);
							memcpy(((Node*)inputValue)->key, &current->key[copyStart], sizeof(KEY_TYPE) * howManyCopy);
							for (int t = copyStart; t < CHILD_SIZE; t++) {
								current->key[t] = MAX_VALUE;
							}
						}
						else {
							//create Node
							inputValue = (void*)initNode();

							//copy to inputValue
							int copyStart = CHILD_SIZE / 2 + 1;
							int valueHowManyCopy = CHILD_SIZE - CHILD_SIZE / 2;
							int keyHowManyCopy = valueHowManyCopy - 1;
							memcpy(((Node*)inputValue)->child, &current->child[copyStart], sizeof(void*) * valueHowManyCopy);
							memcpy(((Node*)inputValue)->key, &current->key[copyStart], sizeof(KEY_TYPE) * keyHowManyCopy);
							for (int t = copyStart - 1; t < CHILD_SIZE; t++) {
								current->key[t] = MAX_VALUE;
							}
							
							//set child's parent
							int c = 0;
							while (((Node*)inputValue)->key[c] != MAX_VALUE) {
								((Node*)((Node*)inputValue)->child[c++])->parent = ((Node*)inputValue);
							}
						}	
						
						//root expension
						if (current->parent == NULL) {
							current->parent = initNode();
							root = current->parent;
							root->child[0] = current;
						}
						current = (Node*)current->parent;
					}
					else {
						insertComplete = 1;
					}
					break;
				}
			}
		}
	}
}

void Delete() {

}

void Search() {

}

//FOR DEBUG
void print(Node* root) {
	if (root->isLeaf) {
		printf("Leaf Node\n");
		//printf("parent is %d\n", ((Node*)root->parent)->key[0]);
	}
	else {
		printf("Directory Node\n");
	}

	for (int i = 0; i < CHILD_SIZE - 1; i++) {
		if (root->key[i] == MAX_VALUE) {
			printf("M ");
		}else {
			printf("%d ", root->key[i]);
		}
	}
	printf("\n");

	if (root->isLeaf) {
		return;
	}
	for (int i = 0; i < CHILD_SIZE ; i++) {
		if (root->key[i] == MAX_VALUE) {
			print((Node*)root->child[i]);
			return;
		}
		else {
			print((Node*)root->child[i]);
		}
	}
}

int main() {
	root = initLeafNode();
	root->parent = NULL;

	for (int i = 0; i < 10; i++) {
		Data* d = (Data*)malloc(sizeof(Data));
		d->testN = i;

		Insert(root, i + 1, d);
	}


	print(root);
}