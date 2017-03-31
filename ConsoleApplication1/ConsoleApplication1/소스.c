#define CHILD_SIZE 1024
#define MAX_VALUE 2000000000
#define KEY_TYPE int
#define VALUE_TYPE void*

#include <stdlib.h>
#include <string.h>

//FOR DEBUG
#include <stdio.h>

int tempBloomFilter = 123;

typedef struct _Node {
	Node* parent;
	void* child[CHILD_SIZE + 1];
	KEY_TYPE key[CHILD_SIZE + 1];
	char isLeaf;
} Node;

Node* initNode() {
	Node* retNode = (Node*)malloc(sizeof(Node));

	memset(retNode->key, MAX_VALUE, sizeof(KEY_TYPE) * (CHILD_SIZE + 1));
	retNode->isLeaf = 0;

	return retNode;
}
Node* initLeafNode() {
	Node* retNode = (Node*)malloc(sizeof(Node));

	memset(retNode->key, MAX_VALUE, sizeof(KEY_TYPE) * (CHILD_SIZE + 1));
	retNode->isLeaf = 1;

	return retNode;
}

Node* root;
KEY_TYPE InsertTempKey[CHILD_SIZE];
void* InsertTempValue[CHILD_SIZE];

void Insert(KEY_TYPE inputKey, VALUE_TYPE inputValue) {
	Node* current = root;
	char insertComplete = 0;
	char doingSpilt = 0;
	while (insertComplete != 0) {	//Find LeafNode
		for (int i = 0; i < CHILD_SIZE; i++) {
			if (current->key[i] >= inputKey) {
				if(!(current->isLeaf) && !doingSpilt)	//Not LeafNode
				{	
					current = ((Node**)current->child)[i];
				}
				else {								//LeafNode
					//Move Back Key
					memcpy(InsertTempKey, &(current->key[i]), sizeof(KEY_TYPE) * (CHILD_SIZE - i + 1));
					memcpy(&(current->key[i + 1]), InsertTempKey, sizeof(KEY_TYPE) * (CHILD_SIZE - i + 1));
					current->key[i] = inputKey;

					//Move Back Child
					memcpy(InsertTempValue, &((Node**)current->child)[i], sizeof(VALUE_TYPE) * (CHILD_SIZE - i + 1));
					memcpy(&((Node**)current->child)[i + 1], InsertTempValue, sizeof(VALUE_TYPE) * (CHILD_SIZE - i + 1));
					current->child[i] = inputValue;
					
					//Spilt Check
					if (current->key[CHILD_SIZE] != MAX_VALUE) {
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
						}
						
						inputKey = current->key[CHILD_SIZE / 2];
						current = current->parent;
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

int main() {
	root = initNode();

}