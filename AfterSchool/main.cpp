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
};

//�Ѿ�
struct Bullet {
	RectangleShape sprite;
	int speed;
	int is_fired;	//�߻� ����

};

struct Enemy {
	RectangleShape sprite;
	SoundBuffer explosion_buffer;
	Sound explosion_sound;//�Ҹ��� ���� ������ ����
	int speed;
	int score;
	int life;
	int respawn_time;
};

//obj1�� obj2�� �浹 ���� �浹�ϸ� 1�� ��ȯ �ƴϸ� 0���� ��ȯ
int is_collide(RectangleShape obj1, RectangleShape obj2) {
	return obj1.getGlobalBounds().intersects(obj2.getGlobalBounds());
}


//��������
const int ENEMY_NUM = 12;					//���� �ִ� ����
const int W_WIDTH = 1200, W_HEIGHT = 600;	//â�� ũ��
const int GO_WIDTH = 320, GO_HEIGHT = 240;	//���� ���� �׸� ũ��

int main(void) {

	srand(time(NULL));//���� �Լ� ���

	//640 x 480 ������ ȭ�� ����
	//��� ���ٰ� ������� �� return 0������ ���α׷��� ����� ��
	RenderWindow window(VideoMode(W_WIDTH, W_HEIGHT), "AfterSchool");
	window.setFramerateLimit(60);//1�ʿ� 60�� �����ش�. �÷��̾ ���� ���� �ʵ��� �ϱ�


	long start_time = clock();// ���� ���� �ð�
	long spent_time;// ���� ���� �ð�
	int is_gameover = 0;

	//BGM
	SoundBuffer BGM_buffer;
	BGM_buffer.loadFromFile("./resources/sounds/bgm.ogg");
	Sound BGM_sound;
	BGM_sound.setBuffer(BGM_buffer);
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
	Texture bg_texture;
	bg_texture.loadFromFile("./resources/images/background.png");
	Sprite bg_sprite;
	bg_sprite.setTexture(bg_texture);
	bg_sprite.setPosition(0, 0);

	//gameover
	Texture gameover_texture;
	gameover_texture.loadFromFile("./resources/images/gameover.png");
	Sprite gameover_sprite;
	gameover_sprite.setTexture(gameover_texture);
	gameover_sprite.setPosition((W_WIDTH-GO_WIDTH)/2,(W_HEIGHT-GO_HEIGHT)/2);


	// �÷��̾�
	struct Player player;
	player.sprite.setSize(Vector2f(40, 40));//�÷��̾� ������
	player.sprite.setPosition(100, 100);//�÷��̾� ���� ��ġ
	player.sprite.setFillColor(Color::Red);//�÷��̾� ����
	player.x = player.sprite.getPosition().x;	//�÷��̾� x��ǥ
	player.y = player.sprite.getPosition().y;	//�÷��̾� y��ǥ
	player.speed = 7;//�÷��̾� �ӵ�
	player.score = 0;//�÷��̾� ����
	player.life = 10;


	//�Ѿ�
	struct Bullet bullet;
	bullet.sprite.setSize(Vector2f(10, 10));
	bullet.sprite.setPosition(player.x+50, player.y+15);	//�ӽ� �׽�Ʈ
	bullet.speed = 20;
	bullet.is_fired = 0;	//false�ϱ� 0���� ��Ÿ��


	// enemy

	struct Enemy enemy[ENEMY_NUM];

	for (int i = 0; i < ENEMY_NUM; i++)
	{
		// TODO : ������ ��ȿ������ �ڵ��̹Ƿ� ���߿� refactoring
		enemy[i].explosion_buffer.loadFromFile("./resources/sound/rumble.flac");
		enemy[i].explosion_sound.setBuffer(enemy[i].explosion_buffer);
		enemy[i].score = 100;
		enemy[i].respawn_time = 8;

		enemy[i].sprite.setSize(Vector2f(70, 70));
		enemy[i].sprite.setPosition(rand() % 300 + W_WIDTH*0.9, rand() % 410);
		enemy[i].life = 1;
		enemy[i].sprite.setFillColor(Color::Yellow);//�� ����
		enemy[i].speed = -(rand() % 10 + 1);
	}


	//���� ��Ű�� �����? -> ���� �ݺ�
	while (window.isOpen()) //������â�� �����ִ� ���� ��� �ݺ�
	{
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
				////spaceŰ ������ ��� enemy �ٽ� ����
				//if (event.key.code == Keyboard::Space)
				//{
				//	for (int i = 0; i < ENEMY_NUM; i++)
				//	{
		

				//	}
				//}
				break;
			}
			}
		}

		spent_time = clock() - start_time;// �ð��� ������ ���� ����

		//�Ѿ��� �÷��̾ ����ٴ� �� �ֵ��� 
		player.x = player.sprite.getPosition().x;	//�÷��̾� x��ǥ
		player.y = player.sprite.getPosition().y;	//�÷��̾� y��ǥ

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
			//�Ѿ��� �߻� �Ǿ����� �ʴٸ�
			if(!bullet.is_fired )
			bullet.sprite.setPosition(player.x + 50, player.y + 15);
			bullet.is_fired = 1;
			
		}

		//enemy���� �浹
		//intersects : �÷��̾�� �� ���̿��� �������� �ִ°�
		for (int i = 0; i < ENEMY_NUM; i++)
		{
			// 10�� ���� enemy�� ��
			if (spent_time % (1000 * enemy[i].respawn_time) < 1000 / 60) // 1�ʵ��� 60�������� �ݺ��Ǳ� ������
			{
				enemy[i].sprite.setSize(Vector2f(70, 70));
				enemy[i].sprite.setFillColor(Color::Yellow);//�� ����
				enemy[i].sprite.setPosition(rand() % 300 + W_WIDTH * 0.9, rand() % 380);// 90%���� ������ ����
				enemy[i].life = 1;
				// 10�ʸ��� enemy �ӵ� +1
				enemy[i].speed = -(rand() % 3 + 1 + (spent_time / 1000 / enemy[i].respawn_time));
			}
			if (enemy[i].life > 0)
			{
				// enemy���� �浹
				if (is_collide(player.sprite, enemy[i].sprite) || is_collide(bullet.sprite, enemy[i].sprite)){
			
					enemy[i].life -= 1;//���� ���� ���̱�
					player.score += enemy[i].score;
					

					// TODO : �ڵ� refactoring �ʿ�
					if (enemy[i].life == 0)
					{
						enemy[i].explosion_sound.play();
						
					}
				}
				//���� ���� ���� �����Ϸ��� ����
				else if (enemy[i].sprite.getPosition().x < 0) {
					player.life -= 1;
					enemy[i].life = 0;
				}

				enemy[i].sprite.move(enemy[i].speed, 0);
			}

		}
		if (bullet.is_fired) {
			bullet.sprite.move(bullet.speed, 0);
			if (bullet.sprite.getPosition().x > W_WIDTH)
				bullet.is_fired = 0;
		}


		if (player.life <=0) {
			is_gameover = 1;	//ture ������ 1
		}

		// ���� �ð��� ������ ����
		sprintf(info, "life: %d score: %d time: %d\n", player.life,player.score, spent_time / 1000);
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
		if (bullet.is_fired)
			window.draw(bullet.sprite);
		window.draw(text);
		window.draw(bullet.sprite);


		if (is_gameover) {
			window.draw(gameover_sprite);
		}

		window.display();
	}

	return 0;
}