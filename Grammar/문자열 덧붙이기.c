#include <stdio.h>
#include <string.h>
#include <stdlib.h> //itoa���

int main() {

	char str[30] = "SCORE : ";
	int score = 300;
	
	char str2[30];
	//������ ���ڿ��� (��ȯ�� ����, ������, ����)
	itoa(score, str2, 10);

	strcat(str, str2);
	printf("%s", str);


	return 0;
	

}