#include <stdio.h>
#include <string.h>
#include <stdlib.h> //itoa���

int main() {

	char str[30];
	int score = 300;

	//���ڿ��� ���
	sprint(str, "score : %d", score);
	
	printf("%s", str);


	return 0;
	

}