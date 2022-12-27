#include <stdio.h>
#include <string.h>
#include <stdlib.h> //itoa사용

int main() {

	char str[30];
	int score = 300;

	//문자열에 출력
	sprint(str, "score : %d", score);
	
	printf("%s", str);


	return 0;
	

}