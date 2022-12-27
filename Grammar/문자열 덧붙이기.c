#include <stdio.h>
#include <string.h>
#include <stdlib.h> //itoa사용

int main() {

	char str[30] = "SCORE : ";
	int score = 300;
	
	char str2[30];
	//정수를 문자열로 (변환할 정수, 적용대상, 진수)
	itoa(score, str2, 10);

	strcat(str, str2);
	printf("%s", str);


	return 0;
	

}