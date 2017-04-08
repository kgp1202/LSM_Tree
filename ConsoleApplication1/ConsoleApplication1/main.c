#include "bptree.h"

int insertedArr[300];

int main() {
	Level* level1 = bptree_init(300);
	
	char isPrint = 1;
	int max = 300;
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

		bptree_insert(level1, key, d);
		
		if (isPrint) {
			print(level1->root);
			printf("=========================================\n");
		}
	}

	print(level1->root);
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

		bptree_delete(level1, key);

		if (isPrint) {
			print(level1->root);
			printf("=========================================\n");
		}
	}



	for (int i = 0; i < max; i++) {
		int key = rand() % max;

		if (key == 0)
			continue;

		if (isPrint)
			printf("search %d\n", key);

		Data* temp = bptree_search(level1, key);

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
			print(level1->root);
			printf("=========================================\n");
		}
	}

	print(level1->root);
}