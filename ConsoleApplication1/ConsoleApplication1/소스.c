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

//	memset(retNode->key, MAX_VALUE, sizeof(KEY_TYPE) * CHILD_SIZE);
	for (int i = 0; i < CHILD_SIZE; i++) {
		retNode->key[i] = MAX_VALUE;
	}
	retNode->isLeaf = 0;

	return retNode;
}
Node* initLeafNode() {
	Node* retNode = (Node*)malloc(sizeof(Node));
	
	//memset(&retNode->key[0], MAX_VALUE, sizeof(KEY_TYPE) * (CHILD_SIZE));
	for (int i = 0; i < CHILD_SIZE; i++) {
		retNode->key[i] = MAX_VALUE;
	}
	retNode->isLeaf = 1;

	return retNode;
}

KEY_TYPE InsertTempKey[CHILD_SIZE];
void* InsertTempValue[CHILD_SIZE];

void Insert(Node* root, KEY_TYPE inputKey, void* inputValue) {
	Node* current = root;
	char insertComplete = 0;
	char doingSpilt = 0;
	while (insertComplete == 0) {
		for (int i = 0; i < CHILD_SIZE; i++) {
			if (current->key[i] >= inputKey) {
				if(!(current->isLeaf) && !doingSpilt)	//Not LeafNode
				{	
					current = ((Node**)current->child)[i];
				}
				else {								//LeafNode
					//Move Back Key & Child
					memcpy(InsertTempKey, &(current->key[i]), sizeof(KEY_TYPE) * (CHILD_SIZE - i - 1));
					memcpy(InsertTempValue, &((Node**)current->child)[i], sizeof(void*) * (CHILD_SIZE - i - 1));
					if (i + 1 < CHILD_SIZE) {
						memcpy(&(current->key[i + 1]), InsertTempKey, sizeof(KEY_TYPE) * (CHILD_SIZE - i - 1));
						memcpy(&((Node**)current->child)[i + 1], InsertTempValue, sizeof(void*) * (CHILD_SIZE - i - 1));
					}
					current->key[i] = inputKey;
					current->child[i] = inputValue;
					
					//Spilt Check
					if (current->key[CHILD_SIZE - 1] != MAX_VALUE) {
						doingSpilt = 1;

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
							memset(&current->child[copyStart], MAX_VALUE, sizeof(KEY_TYPE) * howManyCopy);
						}
						else {
							//create Node
							inputValue = (void*)initNode();

							//copy to inputValue
							int copyStart = CHILD_SIZE / 2 + 1;
							int keyHowManyCopy = CHILD_SIZE - CHILD_SIZE / 2;
							int valueHowManyCopy = keyHowManyCopy + 1;
							memcpy(((Node*)inputValue)->child, &current->child[copyStart], sizeof(void*) * valueHowManyCopy);
							memcpy(((Node*)inputValue)->key, &current->key[copyStart], sizeof(KEY_TYPE) * keyHowManyCopy);
							memset(&current->child[copyStart], MAX_VALUE, sizeof(KEY_TYPE) * keyHowManyCopy);

							//set child's parent
							int c = 0;
							while (((Node*)inputValue)->key[c] != MAX_VALUE) {
								((Node*)((Node*)inputValue)->child[c++])->parent = ((Node*)inputValue);
							}
						}
						
						inputKey = current->key[CHILD_SIZE / 2];
						
						//root expension
						if (current->parent == NULL) {

						fefefe]
						else {
									
						}
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
	for (int i = 0; i < CHILD_SIZE - 1; i++) {
		if (root->key[i] == MAX_VALUE) {
			return;
		}
		else {
			print((Node*)root->child[i]);
		}
	}
}

int main() {
	Node* root = initLeafNode();
	root->parent = NULL;

	for (int i = 0; i < 4; i++) {
		Data* d = (Data*)malloc(sizeof(Data));
		d->testN = i;

		Insert(root, i + 1, d);
	}


	print(root);
}