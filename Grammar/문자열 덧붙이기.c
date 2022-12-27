#include <stdio.h>
#include <string.h>

int main() {

	char str[30] = "SCORE : ";
	char str2[30] = "300";
	strcat(str, str2);
	printf("%s", str);

	int score = 300;
	return 0;
	

}