#include <stdio.h>
#include <string.h>

int main() {
	char str[30] = "score : ";
	int str_len = strlen(str);

	char str2[30] = "300";
	int str2_len = strlen(str2);
	
	for (int i = 0; i < str2_len; i++)
		str[i + str_len] = str2[i];
	str[str_len + str2_len] = '\0';
	printf("%s", str);

	int score = 300;


}