#include <stdio.h>

struct Person {
	int hp;
	int speed;

};

int main() {

	struct Person player;
	int player_hp = 5;
	int player_speed = 7;

	struct Person enemy;
	int enemy_hp = 2;
	int enemy_speed = 5;
	printf("�÷��̾� ü��%d �÷��̾� �ӵ�%d\n", player_hp, player_speed);
	
	return 0;
}