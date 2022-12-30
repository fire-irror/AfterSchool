#include <stdio.h>

//가독성이 좋아진다.
enum enemy_item_type {
	DELAY,	//0
	SPEED	//1
};

int main() {  
	enum item_type it = SPEED;
	{
		printf("%d\n", it);
	};
	return 0;
}