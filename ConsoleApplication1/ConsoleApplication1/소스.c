#define CHILD_SIZE 4		//CHILS_SIZE는 4보다 커야한다.
#define MAX_VALUE 200000000
#define KEY_TYPE int
#define VALUE_TYPE void*

#include <stdlib.h>
#include <string.h>

//FOR DEBUG
#include <stdio.h>

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

KEY_TYPE tempKeyArr[CHILD_SIZE * 2];
void* tempValueArr[CHILD_SIZE * 2];

//Root Node에 대한 설정 수정 요망@@@@@@@@@@@@@@@@
Node* root;
void Insert(Node* root2, KEY_TYPE inputKey, void* inputValue) {
	Node* current = root;
	char insertComplete = 0;
	char doingSpilt = 0;
	while (insertComplete == 0) {
		for (int i = 0; i < CHILD_SIZE; i++) {
			if ( (current->isLeaf && current->key[i] >= inputKey) || current->key[i] > inputKey) {
				if(!(current->isLeaf) && !doingSpilt)	//Not LeafNode
				{	
					current = ((Node**)current->child)[i];
					break;
				}
				else {								//LeafNode
					//Move Back Key
					int howManyMove = CHILD_SIZE - i - 1;
					if (howManyMove > 0) {
						memcpy(tempKeyArr, &(current->key[i]), sizeof(KEY_TYPE) * howManyMove);
						memcpy(&(current->key[i + 1]), tempKeyArr, sizeof(KEY_TYPE) * howManyMove);
					}
					current->key[i] = inputKey;

					//Move Back Child
					if (current->isLeaf) {
						if (howManyMove > 0) {
							memcpy(tempValueArr, &((Node**)current->child)[i], sizeof(void*) * howManyMove);
							memcpy(&((Node**)current->child)[i + 1], tempValueArr, sizeof(void*) * howManyMove);
						}
						current->child[i] = inputValue;
					}
					else {
						if (howManyMove > 0) {
							memcpy(tempValueArr, &((Node**)current->child)[i + 1], sizeof(void*) * howManyMove);
							memcpy(&((Node**)current->child)[i + 2], tempValueArr, sizeof(void*) * howManyMove);
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
							((Node*)((Node*)inputValue)->child[c])->parent = ((Node*)inputValue);
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


int Delete(Node* root2, KEY_TYPE deletedKey) {
	Node* current = root;
	char doingDirectoryDelete = 0;
	char isDistribute = 0;
	int isDistributeChangedKey = 0;
	
	//Find and Delete
	while (1) {
		for (int i = 0; i < CHILD_SIZE; i++) {
			if ((current->isLeaf && current->key[i] >= deletedKey) || current->key[i] > deletedKey) {
				if (!(current->isLeaf) && !doingDirectoryDelete) {
					current = ((Node**)current->child)[i];
					
					i = -1;
					continue;
				}
				else {
					if (!doingDirectoryDelete) {
						//Compare current key to deletedKey
						if (current->key[i] != deletedKey) {
							return 0;
						}

						//LeafNode data Delete
						free(current->child[i]);

						int howManyCopy = CHILD_SIZE - i - 1;
						if (howManyCopy > 0) {
							//move key
							memcpy(tempKeyArr, &current->key[i + 1], sizeof(KEY_TYPE) * howManyCopy);
							memcpy(&current->key[i], tempKeyArr, sizeof(KEY_TYPE) * howManyCopy);
							//move data
							memcpy(tempValueArr, &current->child[i + 1], sizeof(void*) * howManyCopy);
							memcpy(&current->child[i], tempValueArr, sizeof(void*) * howManyCopy);
						}
					}
					else {
						if (!isDistribute) {
							//Directory Data 삭제.
							int howManyCopy = CHILD_SIZE - i;
							if (howManyCopy > 0) {
								//move key
								memcpy(tempKeyArr, &current->key[i], sizeof(KEY_TYPE) * howManyCopy);
								memcpy(&current->key[i - 1], tempKeyArr, sizeof(KEY_TYPE) * howManyCopy);
								//move data
								memcpy(tempValueArr, &current->child[i + 1], sizeof(void*) * howManyCopy);
								memcpy(&current->child[i], tempValueArr, sizeof(void*) * howManyCopy);
							}
						}
						else {
							//Directory Data 변경.			
							current->key[i - 1] = isDistributeChangedKey;

							return 1;
						}
					}
					break;	//Go to Space check
				}
			}
		}

		//Space check
		char isEmpty = 0;
		if (current->isLeaf) {
			if (current->key[CHILD_SIZE / 2 - 1] == MAX_VALUE) {
				isEmpty = 1;
			}
		}
		else {
			if (current->key[CHILD_SIZE / 2 - 2] == MAX_VALUE) {
				isEmpty = 1;
			}
		}

		//Merge
		if (isEmpty) {
			doingDirectoryDelete = 1;

			//Find leftChild
			Node* leftChild = NULL;
			KEY_TYPE parentKey;
			if (current->parent != NULL) {
				Node* tempParent = current->parent;
				int k = 0;
				while (tempParent->key[k] <= deletedKey) {
					k++;
				}
				if (k - 1 >= 0) {
					leftChild = tempParent->child[k - 1];
					parentKey = tempParent->key[k - 1];
				}
			}
			else {	//current is root
				//get root's child size
				int rootSize = 0;
				while (current->key[rootSize] != MAX_VALUE) {
					rootSize++;
				}

				//change root to child[0]
				if (rootSize == 0) {
					if (root->isLeaf) {
						return 1;
					}
					else {
						Node* temp = root;
						root = current->child[0];
						root->parent = NULL;
						free(temp);					}
				}

				return 1;
			}
			
			//get left and right
			Node* left,* right;		//merged left & merged right
			if (leftChild == NULL) {		//if current is first child
				//Merge current, rightChild
				left = current;
				right = ((Node*)current->parent)->child[1];
				parentKey = ((Node*)left->parent)->key[0];
			}
			else {
				//Merge leftChild, current
				left = leftChild;
				right = current;
			}
			
			//get left's size & right's size
			int leftSize = 0, rightSize = 0;
			while (left->key[leftSize] != MAX_VALUE) {
				leftSize++;
			}
			while (right->key[rightSize] != MAX_VALUE) {
				rightSize++;
			}

			//Merge to tempArr
			memset(tempKeyArr, 0, sizeof(tempKeyArr));
			if (left->isLeaf) {
				memcpy(tempKeyArr, left->key, sizeof(KEY_TYPE) * leftSize);
				memcpy(&tempKeyArr[leftSize], right->key, sizeof(KEY_TYPE) * rightSize);		
				memcpy(tempValueArr, left->child, sizeof(void*) * leftSize);
				memcpy(&tempValueArr[leftSize], right->child, sizeof(void*) * rightSize);
			}
			else {
				memcpy(tempKeyArr, left->key, sizeof(KEY_TYPE) * leftSize);
				memcpy(&tempKeyArr[leftSize], &parentKey, sizeof(KEY_TYPE));
				memcpy(&tempKeyArr[leftSize + 1], right->key, sizeof(KEY_TYPE) * rightSize);
				memcpy(tempValueArr, left->child, sizeof(void*) * (leftSize + 1));
				memcpy(&tempValueArr[leftSize + 1], right->child, sizeof(void*) * (rightSize + 1));
			}

			//Check isDistribute
			if (tempKeyArr[CHILD_SIZE - 1] == 0) {
				if (left->isLeaf) {
					//copy tempValue & tempKey to left value & key
					memcpy(&left->key[leftSize], &tempKeyArr[leftSize], sizeof(KEY_TYPE) * rightSize);
					memcpy(&left->child[leftSize], &tempValueArr[leftSize], sizeof(void*) * rightSize);

					left->child[CHILD_SIZE] = right->child[CHILD_SIZE];
				}
				else {
					//copy tempValue & tempKey to left value & key
					memcpy(&left->key[leftSize], &tempKeyArr[leftSize], sizeof(KEY_TYPE) * (rightSize + 1));
					memcpy(&left->child[leftSize + 1], &tempValueArr[leftSize + 1], sizeof(void*) * (rightSize + 1));

					//set Parent
					for (int k = leftSize; k < CHILD_SIZE; k++) {
						((Node*)left->child[k])->parent = left;

						if (left->key[k] == MAX_VALUE)
							break;
					}
				}
				if(right->key[0] != MAX_VALUE)
					deletedKey = right->key[0];

				current = left;
				free(right);
			}
			else {
				isDistribute = 1;

				//copy left and right
				//각각의 size를 가지고 있다면 쉽게 변경 가능@@@@@@@
				for (int j = 0; j < CHILD_SIZE; j++) {
					left->key[j] = MAX_VALUE;
					right->key[j] = MAX_VALUE;
				}
				if (left->isLeaf) {	
					int howManyCopy = (leftSize + rightSize) / 2;

					//copy tempValue & tempKey to left
					memcpy(left->key, tempKeyArr, sizeof(KEY_TYPE) * howManyCopy);
					memcpy(left->child, tempValueArr, sizeof(void*) * howManyCopy);

					//copy tempValue & tempKey to right
					memcpy(right->key, &tempKeyArr[howManyCopy], sizeof(KEY_TYPE) * ((leftSize + rightSize) - howManyCopy));
					memcpy(right->child, &tempValueArr[howManyCopy], sizeof(void*) * ((leftSize + rightSize) - howManyCopy));

					isDistributeChangedKey = right->key[0];
				}
				else {
					int howManyCopy = (leftSize + rightSize + 1) / 2;

					//copy tempValue & tempKey to left 
					memcpy(left->key, tempKeyArr, sizeof(KEY_TYPE) * howManyCopy);
					memcpy(left->child, tempValueArr, sizeof(void*) * (howManyCopy + 1));

					isDistributeChangedKey = tempKeyArr[howManyCopy];

					//copy tempValue & tempKey to right
					memcpy(right->key, &tempKeyArr[howManyCopy + 1], sizeof(KEY_TYPE) * ((leftSize + rightSize + 1) - howManyCopy - 1));
					memcpy(right->child, &tempValueArr[howManyCopy + 1], sizeof(void*) * ((leftSize + rightSize + 1) - howManyCopy));

					//set parent
					for (int k = 0; k < CHILD_SIZE; k++) {
						((Node*)left->child[k])->parent = left;

						if (left->key[k] == MAX_VALUE)
							break;
					}
					for (int k = 0; k < CHILD_SIZE; k++) {
						((Node*)right->child[k])->parent = right;

						if (right->key[k] == MAX_VALUE)
							break;
					}
				}

				if (deletedKey < right->key[0] && right->key[0] != MAX_VALUE) {
					deletedKey = right->key[0];
				}
			}

			current = current->parent;
			continue;
		}
		return 1;
	}
	return 0;
}

Data* Search(Node* root2, KEY_TYPE searchKey) {
	Node* current = root;
	while (1) {
		for (int i = 0; i < CHILD_SIZE; i++) {
			if (current->key[i] > searchKey || (current->key[i] >= searchKey && current->isLeaf)) {
				if (!current->isLeaf) {
					current = current->child[i];
					break;
				}
				else {
					if (current->key[i] == searchKey) {
						return current->child[i];
					}
					else {
						return NULL;
					}
				}
			}
		}
	}

	return NULL;
}

//FOR DEBUG
void print(Node* root) {
	if (root->isLeaf) {
		printf("Leaf Node\n");
	}
	else {
		printf("Directory Node\n");

		for (int i = 0; i < CHILD_SIZE; i++) {
			if (root->key[i] == MAX_VALUE) {
				if (root != ((Node*)root->child[i])->parent) {
					printf("FUCK!!@@@@@@@@@@@@@@@@@@\n");
					printf("FUCK!!@@@@@@@@@@@@@@@@@@\n");
					while (1) {

					}

				}
				break;
			}
				

			if (root != ((Node*)root->child[i])->parent) {
				printf("FUCK!!@@@@@@@@@@@@@@@@@@\n");
				printf("FUCK!!@@@@@@@@@@@@@@@@@@\n");
				while (1) {

				}

			}

			for (int j = 0; j < CHILD_SIZE; j++) {
				if (((Node*)root->child[i])->key[j] == MAX_VALUE)
					break;

				if (root->key[i] <= ((Node*)root->child[i])->key[j] && ((Node*)root->child[i])->isLeaf) {
					printf("FUCK!!@@@@@@@@@@@@@@@@@@\n");
					while (1) {

					}
				}
			}
		}


	}
	if (root->parent != NULL) {
		printf("parent is %d\n", ((Node*)root->parent)->key[0]);
	}

	for (int i = 0; i < CHILD_SIZE - 1; i++) {
		if (root->key[i] == MAX_VALUE) {
			printf("M ");
		}else {
			printf("%d ", root->key[i]);
		}
	}
	printf("\n");
	for (int i = 0; i < CHILD_SIZE - 1; i++) {
		if (root->key[i] == MAX_VALUE) {
			//printf("M ");
		}
		else {
			//printf("%d ", ((Data*)root->child[i])->testN);
		}
	}
	//printf("\n");

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

	char isPrint = 1;
	int max = 300;
	int insertedArr[1000];
	memset(insertedArr, 0, sizeof(insertedArr));
	for (int i = 0; i < max; i++) {
		int key = rand() % max;

		if (insertedArr[key] > 0 || key == 0)
			continue;

		insertedArr[key]++;

		Data* d = (Data*)malloc(sizeof(Data));
		d->testN = key;

		if(isPrint)
			printf("insert %d\n", key);

		Insert(root, key, d);
		
		if (isPrint) {
			print(root);
			printf("=========================================\n");
		}
	}

	rand();
	for (int i = 0; i < max; i++) {
		int key = rand() % max;

		if (key == 0)
			continue;

		if (insertedArr[key] > 0 || key == 0) {
			insertedArr[key]--;
		}

		if (isPrint)
			printf("delete %d\n", key);

		Delete(root, key);

		if (isPrint) {
		print(root);
		printf("=========================================\n");
		}
	}

	for (int i = 0; i < max; i++) {
		int key = rand() % max;

		if (key == 0)
			continue;

		if (isPrint)
			printf("search %d\n", key);

		Data* temp = Search(root, key);

		if (temp == NULL) {
			printf("No\n");

			if (insertedArr[key] > 0) {
				printf("FFFFFFUUUUUCCCCKKK@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
				while (1) {

				}
			}
		}
		else {
			printf("%d\n", temp->testN);

			if (insertedArr[key] <= 0) {
				printf("FFFFFFUUUUUCCCCKKK@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
				while (1) {

				}
			}
		}

		if (isPrint) {
			print(root);
			printf("=========================================\n");
		}
	}

	print(root);

	/*
	int insertedArr[] = { 1, 2, 3, 4, 5, 6, 7, 12, 13, 14 };
	for (int i = 0; i < 20; i++) {
		char temp = 0;
		for (int j = 0; j < sizeof(insertedArr) / sizeof(int); j++) {
			if (i == insertedArr[j]) {
				temp = 1;
			}
		}
		if (!temp)	continue;

		Data* d = (Data*)malloc(sizeof(Data));
		d->testN = i;

		Insert(root, i, d);
	}

	Delete(root, 7);
	Data* d = (Data*)malloc(sizeof(Data));
	d->testN = 17;
	Insert(root, 17, d);
	Delete(root, 2);


	print(root);
	printf("=========================================\n");
	*/

	while (1) {
		int key;
		char input = 'a';
		while (1) {
			scanf("%c %d", &input, &key);

			if (input == '\n') {
				continue;
			}else {
				break;
			}
		}

		if (input == 'i') {
			Data* d = (Data*)malloc(sizeof(Data));
			d->testN = key;

			Insert(root, key, d);
		}else if(input == 'd'){
			Delete(root, key);
		}
		else if (input == 's') {
			Data* temp = Search(root, key);
			if (temp == NULL) {
				printf("No\n");
			}else 
				printf("%d\n", temp->testN);
		}

		print(root);
		printf("=========================================\n");
	}
	
}