#include <stdio.h>

//�������� ��������.
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