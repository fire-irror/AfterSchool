#include <stdio.h>

int main() {
	char str[30] = "score : ";
	int str_len = 0;
	while (str[str_len] != '\0')
		str_len++;

	char str2[30] = "300";
	int str2_len = 0;
	while (str[str2_len] != '\0')
		str2_len++;

	for (int i = 0; i < str2_len; i++)
		str[i + str_len] = str2[i];
	str[str_len + str2_len] = '\0';
	printf("%s", str);

	int score = 300;


}