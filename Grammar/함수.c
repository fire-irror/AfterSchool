#include <stdio.h>

//함수의 구성요소: 반환형 함수이름(매개변수)

int f(int x) {
	return 2 * x + 1;
}
int main(void) {

	printf("%d", f(1));
	
	return 0;

}