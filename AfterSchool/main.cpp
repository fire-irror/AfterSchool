/*
* ������ �ӵ� ����, ������ ����
* �Ѿ� �ý��� ����
* sound effect����
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>//SoundBuffer ���

using namespace sf;

struct Player {
	RectangleShape sprite;// sprite : �׸��� �Ǵ� �κ�
	int speed;
	int score;
	int life;
	float x, y;
	int speed_max;
};

//�Ѿ�
struct Bullet {
	RectangleShape sprite;
	int is_fired;	//�߻� ����
};

struct Enemy {
	RectangleShape sprite;
	int speed;	//0
	int life;	//1
};

enum item_type {
	SPEED,
	DELAY
};

struct Item {
	RectangleShape sprite;
	int delay;
	int is_presented;	//�������� ������ Ȯ��
	long presented_time;
	Sound sound;
	enum item_type type;
};

struct SBuffers {
	SoundBuffer BGM;
	SoundBuffer rumble;
	SoundBuffer item_delay;
	SoundBuffer item_speed;


};

struct Textures {
	Texture bg;	//��� �̹���
	Texture gameover;	//���ӿ��� �̹���
	Texture player;	//�÷��̾� �̹���
	Texture enemy;	//�� �̹���
	Texture bullet;	//�Ѿ� �̹���
	Texture item_delay;	//���� ������
	Texture item_speed;	//�̼� ������

};

//obj1�� obj2�� �浹 ���� �浹�ϸ� 1�� ��ȯ �ƴϸ� 0���� ��ȯ
int is_collide(RectangleShape obj1, RectangleShape obj2) {
	return obj1.getGlobalBounds().intersects(obj2.getGlobalBounds());
}


//��������
const int ENEMY_NUM = 12;					//���� �ִ� ����
const int BULLET_NUM = 50;// �Ѿ� �ִ� ����
const int ITEM_NUM = 2;	//item�ִ� ���� ����
const int W_WIDTH = 1200, W_HEIGHT = 600;	//â�� ũ��
const int GO_WIDTH = 320, GO_HEIGHT = 240;	//���� ���� �׸� ũ��


int main(void) {

	struct Textures t;
	t.bg.loadFromFile("./resources/images/background.png");
	t.gameover.loadFromFile("./resources/images/gameover.png");
	t.player.loadFromFile("./resources/images/player.png");
	t.enemy.loadFromFile("./resources/images/enemy.png");
	t.bullet.loadFromFile("./resources/images/bullet.png");
	t.item_delay.loadFromFile("./resources/images/item_delay.png");
	t.item_speed.loadFromFile("./resources/images/item_speed.png");	

	struct SBuffers sb;
	sb.BGM.loadFromFile("./resources/sounds/bgm.ogg");
	sb.item_delay.loadFromFile("./resources/sounds/item_delay.ogg");
	sb.item_speed.loadFromFile("./resources/sounds/item_speed.wav");
	sb.rumble.loadFromFile("./resources/sounds/rumble.ogg");


	srand(time(NULL));//���� �Լ� ���

	//640 x 480 ������ ȭ�� ����
	//��� ���ٰ� ������� �� return 0������ ���α׷��� ����� ��
	RenderWindow window(VideoMode(W_WIDTH, W_HEIGHT), "AfterSchool");
	window.setFramerateLimit(60);//1�ʿ� 60�� �����ش�. �÷��̾ ���� ���� �ʵ��� �ϱ�


	long start_time = clock();// ���� ���� �ð�
	long spent_time;// ���� ���� �ð�
	int fired_time = 0;
	int is_gameover = 0;

	//BGM
	Sound BGM_sound;
	BGM_sound.setVolume(55);
	BGM_sound.setBuffer(sb.BGM);
	BGM_sound.setLoop(1);	//BGM���� �ݺ�
	BGM_sound.play();

	// text ��Ʈ
	Font font;
	font.loadFromFile("C:\\Windows\\Fonts\\Arial.ttf");//C����̺꿡 �ִ� ��Ʈ ��������

	Text text;
	char info[40];
	text.setFont(font);//��Ʈ ����
	text.setCharacterSize(24);//��Ʈ ũ��
	text.setFillColor(Color(255, 255, 255));//RGB�� ��� ǥ��
	text.setPosition(0, 0);//�ؽ�Ʈ ��ġ 0,0


	//���
	Sprite bg_sprite;
	bg_sprite.setTexture(t.bg);
	bg_sprite.setPosition(0, 0);

	//gameover
	Sprite gameover_sprite;
	gameover_sprite.setTexture(t.gameover);
	gameover_sprite.setPosition((W_WIDTH - GO_WIDTH) / 2, (W_HEIGHT - GO_HEIGHT) / 2);

	// �÷��̾�
	struct Player player;
	player.sprite.setTexture(&t.player);	//�ּҰ����� �޾Ƽ� �����;� �Ѵ�.
	player.sprite.setSize(Vector2f(165, 160));//�÷��̾� ������
	player.sprite.setPosition(100, 100);//�÷��̾� ���� ��ġ
	player.x = player.sprite.getPosition().x;	//�÷��̾� x��ǥ
	player.y = player.sprite.getPosition().y;	//�÷��̾� y��ǥ
	player.speed_max = 15;
	player.speed = 7;//�÷��̾� �ӵ�
	player.score = 0;//�÷��̾� ����
	player.life = 10;

	//�Ѿ�
	int bullet_speed = 20;
	int bullet_idx = 0;
	int bullet_delay = 500;	// ������ 0.5��
	int bullet_delay_max = 500;
	struct Bullet bullet[BULLET_NUM];
	for (int i = 0; i < BULLET_NUM; i++) {
		bullet[i].sprite.setTexture(&t.bullet);
		bullet[i].sprite.setSize(Vector2f(55, 60));
		bullet[i].sprite.setPosition(player.x + 115, player.y + 15);	//�ӽ� �׽�Ʈ
		bullet[i].is_fired = 0;	//false�ϱ� 0���� ��Ÿ��

	}


	//enemy
	struct Enemy enemy[ENEMY_NUM];
	Sound enemy_explosion_sound;
	enemy_explosion_sound.setBuffer(sb.rumble);
	int enemy_score = 100;
	int enemy_respawn_time = 8;

	for (int i = 0; i < ENEMY_NUM; i++)
	{
		enemy[i].sprite.setTexture(&t.enemy);

		enemy[i].sprite.setSize(Vector2f(60, 60));
		enemy[i].sprite.setPosition(rand() % 300 + W_WIDTH * 0.9, rand() % 410);
		enemy[i].life = 1;
		enemy[i].speed = -(rand() % 10 + 1);
	}

	//item
	struct Item item[ITEM_NUM];
	item[0].sprite.setTexture(&t.item_speed);
	item[0].delay = 25000;	// 25��
	item[0].type = SPEED;
	item[0].sound.setBuffer(sb.item_speed);
	item[1].sprite.setTexture(&t.item_delay);
	item[1].delay = 20000;
	item[1].type = DELAY;
	item[1].sound.setBuffer(sb.item_delay);

	for (int i = 0; i < ITEM_NUM; i++) {
		item[i].sprite.setSize(Vector2f(65, 70));
		item[i].is_presented = 0;
		item[i].presented_time = 0;
	}
	while (window.isOpen()) //������â�� �����ִ� ���� ��� �ݺ�
	{
		spent_time = clock() - start_time;// �ð��� ������ ���� ����

		//�Ѿ��� �÷��̾ ����ٴ� �� �ֵ��� 
		player.x = player.sprite.getPosition().x;	//�÷��̾� x��ǥ
		player.y = player.sprite.getPosition().y;	//�÷��̾� y��ǥ

		Event event;//�̺�Ʈ ����
		while (window.pollEvent(event)) //�̺�Ʈ�� �߻�. �̺�Ʈ�� �߻��ؾ� event �ʱ�ȭ�� ��
		{
			switch (event.type)
			{
				//����(x)��ư�� ������ Event::Closed(0) 
			case Event::Closed://������
				window.close();//������â�� ����
				break;
				//Ű���带 ������ �� 
			case Event::KeyPressed:
				//case�� �ȿ� ������ ������ ������ �߰�ȣ�� �ľ� ��
			{
				break;
			}
			}
		}

		//game ���� update
		if (player.life <= 0) {
			is_gameover = 1;	//ture ������ 1
		}

		//player �̵� ���� ����

		if (player.x < 0)
			player.sprite.setPosition(0, player.y);
		else if (player.x > W_WIDTH - 200)
			player.sprite.setPosition(W_WIDTH - 200, player.y);

		if (player.y < 0)
			player.sprite.setPosition(player.x, 0);
		else if (player.y > W_HEIGHT - 177)
			player.sprite.setPosition(player.x, W_HEIGHT - 177);

		//player������Ʈ
		//����Ű
		if (Keyboard::isKeyPressed(Keyboard::Left))
		{
			player.sprite.move(-1 * player.speed, 0);//���� �̵�
		}
		if (Keyboard::isKeyPressed(Keyboard::Up))
		{
			player.sprite.move(0, -1 * player.speed);//���� �̵�

		}
		if (Keyboard::isKeyPressed(Keyboard::Down))
		{
			player.sprite.move(0, player.speed);//�Ʒ��� �̵�
		}
		if (Keyboard::isKeyPressed(Keyboard::Right))
		{
			player.sprite.move(player.speed, 0);//������ �̵�
		}
		if (Keyboard::isKeyPressed(Keyboard::Space))
		{
			if (spent_time - fired_time > bullet_delay) {
				//�Ѿ��� �߻� �Ǿ����� �ʴٸ�
				if (!bullet[bullet_idx].is_fired) {
					bullet[bullet_idx].sprite.setPosition(player.x + 50, player.y + 15);
					bullet[bullet_idx].is_fired = 1;
					bullet_idx++;	//���� �Ѿ��� �߻��� �� �ֵ���
					bullet_idx = bullet_idx % BULLET_NUM;
					fired_time = spent_time;	//�����ð�
				}
			}
		}
		for (int i = 0; i < BULLET_NUM; i++) {
			if (bullet[i].is_fired) {
				bullet[i].sprite.move(bullet_speed, 0);
				if (bullet[i].sprite.getPosition().x > W_WIDTH) {
					bullet[i].is_fired = 0;
				}
			}
		}
		//enemy���� �浹
		//intersects : �÷��̾�� �� ���̿��� �������� �ִ°�
		for (int i = 0; i < ENEMY_NUM; i++)
		{
			// 10�� ���� enemy�� ��
			if (spent_time % (1000 * enemy_respawn_time) < 1000 / 60) // 1�ʵ��� 60�������� �ݺ��Ǳ� ������
			{
				//������ �������� ���� ���� ������ ��Ű�ڴ�.
				if (!is_gameover) {

					enemy[i].sprite.setSize(Vector2f(60, 60));
					enemy[i].sprite.setPosition(rand() % 300 + W_WIDTH * 0.9, rand() % 380);// 90%���� ������ ����
					enemy[i].life = 1;
					// 10�ʸ��� enemy �ӵ� +1
					enemy[i].speed = -(rand() % 3 + 1); //+(spent_time / 1000 / enemy[i].respawn_time));
				}
			}
			if (enemy[i].life > 0)
			{
				// player�� enemy���� �浹
				if (is_collide(player.sprite, enemy[i].sprite) || is_collide(bullet[i].sprite, enemy[i].sprite)) {

					enemy[i].life -= 1;//���� ���� ���̱�
					player.score += enemy_score;

					if (enemy[i].life == 0)
					{
						enemy_explosion_sound.play();

					}
				}
				//���� ���� ���� �����Ϸ��� ����
				else if (enemy[i].sprite.getPosition().x < 0) {
					player.life -= 1;
					enemy[i].life = 0;
				}
				// �Ѿ˰� enemy�� �浹
				for (int j = 0; j < BULLET_NUM; j++) {

					if (is_collide(bullet[j].sprite, enemy[i].sprite))
					{
						if (bullet[j].is_fired) {
							enemy[i].life -= 1;
							player.score += enemy_score;

							// TODO : �ڵ� refactoring �ʿ�
							if (enemy[i].life == 0)
							{
								enemy_explosion_sound.play();
							}
							bullet[j].is_fired = 0;
						}
					}
				}
				enemy[i].sprite.move(enemy[i].speed, 0);
			}
		}

		//item update
		for (int i = 0; i < ITEM_NUM; i++) {
			if (!item[i].is_presented) {
				if (spent_time - item[i].presented_time > item[i].delay) {

					item[i].sprite.setPosition((rand() % W_WIDTH) * 0.8, (rand() % W_HEIGHT) * 0.8);
					item[i].is_presented = 1;
				}
			}
			if (item[i].is_presented) {
				//�浹 �� ������ȿ���� �ְ� �������
				if (is_collide(player.sprite, item[i].sprite)) {
					switch (item[i].type) {

					case 0: //player move speed
						player.speed += 3;
						if (player.speed_max < player.speed) {
							player.speed = player.speed_max;
						}

						break;
					case 1:	//player attact speed
						bullet_delay -= 100;
						if (bullet_delay < bullet_delay_max) {
							bullet_delay = bullet_delay_max;
						}
						break;
					}
					item[i].is_presented = 0;
					item[i].presented_time = spent_time;
					item[i].sound.play();
				}
			}
		}
		// ���� �ð��� ������ ����
		sprintf(info, "life: %d score: %d time: %d\n", player.life, player.score, spent_time / 1000);
		text.setString(info);

		window.clear(Color::Black);//�÷��̾� ��ü ���� (��� �����)
		window.draw(bg_sprite);

		for (int i = 0; i < ENEMY_NUM; i++)
		{
			if (enemy[i].life > 0)  window.draw(enemy[i].sprite);//�� �����ֱ�
		}
		//ȭ���� ������ �ִ� ���� ��� �׷��� ��
		//draw�� ���߿� ȣ���Ҽ��� �켱������ ������
		window.draw(player.sprite);//�÷��̾� �����ֱ�(�׷��ֱ�)
		for (int i = 0; i < BULLET_NUM; i++)
			if (bullet[i].is_fired)
				window.draw(bullet[i].sprite);
		for (int i = 0; i < ITEM_NUM; i++) {
			if (item[i].is_presented)
				window.draw(item[i].sprite);
		}
	

		window.draw(text);


		if (is_gameover) {
			window.draw(gameover_sprite);
		}

		window.display();
	}

	return 0;
}