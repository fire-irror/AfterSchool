#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
	int arr[5];
	srand(time(0));

	for (int i = 0; i < 5; i++) {
		//1~100까지 수를 랜덤으로 출력
		arr[i] = (rand()%100)+1;
		printf("%d\n", arr[i]);
	}
	return 0;
}