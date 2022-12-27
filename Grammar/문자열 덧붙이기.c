#include <stdio.h>
#include <string.h>//문자열 라이브러리
//#include <stdlib.h>// itoa사용

int main() {


    char str[30];
    int score = 300;

    sprintf(str, "score : %d", score);
    printf("%s", str);

    return 0;

}