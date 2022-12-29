/*
* 아이템 속도 증가, 데미지 증가
* 총알 시스템 개편
* sound effect개선
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>//SoundBuffer 사용

using namespace sf;

struct Player {
	RectangleShape sprite;// sprite : 그림이 되는 부분
	int speed;
	int score;
	int life;
	float x, y;
};

//총알
struct Bullet {
	RectangleShape sprite;
	int is_fired;	//발사 여부
};

struct Enemy {
	RectangleShape sprite;
	SoundBuffer explosion_buffer;
	Sound explosion_sound;//소리에 대한 정보를 담음
	int speed;
	int score;
	int life;
	int respawn_time;
};

struct Textures {
	Texture bg;	//배경 이미지
	Texture gameover;	//게임오버 이미지
	Texture player;	//플레이어 이미지
	Texture enemy;
	Texture bullet;
};

//obj1과 obj2의 충돌 여부 충돌하면 1로 반환 아니면 0으로 반환
int is_collide(RectangleShape obj1, RectangleShape obj2) {
	return obj1.getGlobalBounds().intersects(obj2.getGlobalBounds());
}


//전역변수
const int ENEMY_NUM = 12;					//적의 최대 개수
const int BULLET_NUM = 50;// 총알 최대 갯수
const int W_WIDTH = 1200, W_HEIGHT = 600;	//창의 크기
const int GO_WIDTH = 320, GO_HEIGHT = 240;	//게임 오버 그림 크기

int main(void) {

	struct Textures t;
	t.bg.loadFromFile("./resources/images/background.png");
	t.gameover.loadFromFile("./resources/images/gameover.png");
	t.player.loadFromFile("./resources/images/player.png");
	t.enemy.loadFromFile("./resources/images/enemy.png");
	t.bullet.loadFromFile("./resources/images/bullet.png");


	srand(time(NULL));//랜덤 함수 사용

	//640 x 480 윈도우 화면 나옴
	//잠깐 떴다가 사라지는 건 return 0때문에 프로그램이 종료된 것
	RenderWindow window(VideoMode(W_WIDTH, W_HEIGHT), "AfterSchool");
	window.setFramerateLimit(60);//1초에 60장 보여준다. 플레이어가 빨리 가지 않도록 하기


	long start_time = clock();// 게임 시작 시간
	long spent_time;// 게임 진행 시간
	int fired_time = 0;
	int is_gameover = 0;

	//BGM
	SoundBuffer BGM_buffer;
	BGM_buffer.loadFromFile("./resources/sounds/bgm.ogg");
	Sound BGM_sound;
	BGM_sound.setBuffer(BGM_buffer);
	BGM_sound.setLoop(1);	//BGM무한 반복
	BGM_sound.play();

	// text 폰트
	Font font;
	font.loadFromFile("C:\\Windows\\Fonts\\Arial.ttf");//C드라이브에 있는 폰트 가져오기

	Text text;
	char info[40];
	text.setFont(font);//폰트 세팅
	text.setCharacterSize(24);//폰트 크기
	text.setFillColor(Color(255, 255, 255));//RGB로 흰색 표현
	text.setPosition(0, 0);//텍스트 위치 0,0


	//배경
	Sprite bg_sprite;
	bg_sprite.setTexture(t.bg);
	bg_sprite.setPosition(0, 0);

	//gameover
	Sprite gameover_sprite;
	gameover_sprite.setTexture(t.gameover);
	gameover_sprite.setPosition((W_WIDTH - GO_WIDTH) / 2, (W_HEIGHT - GO_HEIGHT) / 2);

	// 플레이어
	struct Player player;
	player.sprite.setTexture(&t.player);	//주소값으로 받아서 가져와야 한다.
	player.sprite.setSize(Vector2f(190, 167));//플레이어 사이즈
	player.sprite.setPosition(100, 100);//플레이어 시작 위치
	player.x = player.sprite.getPosition().x;	//플레이어 x좌표
	player.y = player.sprite.getPosition().y;	//플레이어 y좌표
	player.speed = 7;//플레이어 속도
	player.score = 0;//플레이어 점수
	player.life = 10;


	//총알
	int bullet_speed = 20;
	int bullet_idx = 0;
	int bullet_delay = 500;	// 딜레이 0.5초

	struct Bullet bullet[BULLET_NUM];
	for (int i = 0; i < BULLET_NUM; i++) {
		bullet[i].sprite.setTexture(&t.bullet);
		bullet[i].sprite.setSize(Vector2f(50, 55));
		bullet[i].sprite.setPosition(player.x + 115, player.y + 15);	//임시 테스트
		bullet[i].is_fired = 0;	//false니까 0으로 나타냄

	}


	// enemy
	struct Enemy enemy[ENEMY_NUM];

	for (int i = 0; i < ENEMY_NUM; i++)
	{
		enemy[i].sprite.setTexture(&t.enemy);
		// TODO : 굉장히 비효율적인 코드이므로 나중에 refactoring
		enemy[i].explosion_buffer.loadFromFile("./resources/sounds/rumble.ogg");
		enemy[i].explosion_sound.setBuffer(enemy[i].explosion_buffer);
		enemy[i].score = 100;
		enemy[i].respawn_time = 8;

		enemy[i].sprite.setSize(Vector2f(50, 50));
		enemy[i].sprite.setPosition(rand() % 300 + W_WIDTH * 0.9, rand() % 410);
		enemy[i].life = 1;
		enemy[i].speed = -(rand() % 10 + 1);
	}


	while (window.isOpen()) //윈도우창이 열려있는 동안 계속 반복
	{
		spent_time = clock() - start_time;// 시간이 지남에 따라 증가

		//총알이 플레이어럴 따라다닐 수 있도록 
		player.x = player.sprite.getPosition().x;	//플레이어 x좌표
		player.y = player.sprite.getPosition().y;	//플레이어 y좌표

		Event event;//이벤트 생성
		while (window.pollEvent(event)) //이벤트가 발생. 이벤트가 발생해야 event 초기화가 됨
		{
			switch (event.type)
			{
				//종료(x)버튼을 누르면 Event::Closed(0) 
			case Event::Closed://정수임
				window.close();//윈도우창이 닫힘
				break;
				//키보드를 눌렀을 때 
			case Event::KeyPressed:
				//case문 안에 변수를 선언할 때에는 중괄호를 쳐야 함
			{
				break;
			}
			}
		}

		//game 상태 update
		if (player.life <= 0) {
			is_gameover = 1;	//ture 임으로 1
		}

		//player 이동 범위 제한

		if (player.x < 0)
			player.sprite.setPosition(0, player.y);
		else if (player.x > W_WIDTH - 200)
			player.sprite.setPosition(W_WIDTH - 200, player.y);

		if (player.y < 0)
			player.sprite.setPosition(player.x, 0);
		else if (player.y > W_HEIGHT - 177)
			player.sprite.setPosition(player.x, W_HEIGHT - 177);

		//player업데이트
		//방향키
		if (Keyboard::isKeyPressed(Keyboard::Left))
		{
			player.sprite.move(-1 * player.speed, 0);//왼쪽 이동
		}
		if (Keyboard::isKeyPressed(Keyboard::Up))
		{
			player.sprite.move(0, -1 * player.speed);//위쪽 이동

		}
		if (Keyboard::isKeyPressed(Keyboard::Down))
		{
			player.sprite.move(0, player.speed);//아래쪽 이동
		}
		if (Keyboard::isKeyPressed(Keyboard::Right))
		{
			player.sprite.move(player.speed, 0);//오른쪽 이동
		}
		if (Keyboard::isKeyPressed(Keyboard::Space))
		{
			if (spent_time - fired_time > bullet_delay) {
				//총알이 발사 되어있지 않다면
				if (!bullet[bullet_idx].is_fired) {
					bullet[bullet_idx].sprite.setPosition(player.x + 50, player.y + 15);
					bullet[bullet_idx].is_fired = 1;
					bullet_idx++;	//다음 총알이 발사할 수 있도록
					fired_time = spent_time;	//장전시간
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
		//enemy와의 충돌
		//intersects : 플레이어와 적 사이에서 교집합이 있는가
		for (int i = 0; i < ENEMY_NUM; i++)
		{
			// 10초 마다 enemy가 젠
			if (spent_time % (1000 * enemy[i].respawn_time) < 1000 / 60) // 1초동안 60프레임이 반복되기 때문에
			{
				enemy[i].sprite.setSize(Vector2f(50, 50));
				enemy[i].sprite.setPosition(rand() % 300 + W_WIDTH * 0.9, rand() % 380);// 90%부터 적들이 나옴
				enemy[i].life = 1;
				// 10초마다 enemy 속도 +1
				enemy[i].speed = -(rand() % 3 + 1); //+(spent_time / 1000 / enemy[i].respawn_time));
			}
			if (enemy[i].life > 0)
			{
				// player과 enemy와의 충돌
				if (is_collide(player.sprite, enemy[i].sprite) || is_collide(bullet[i].sprite, enemy[i].sprite)) {

					enemy[i].life -= 1;//적의 생명 줄이기
					player.score += enemy[i].score;


					// TODO : 코드 refactoring 필요
					if (enemy[i].life == 0)
					{
						enemy[i].explosion_sound.play();

					}
				}
				//적이 왼쪽 끝에 진입하려는 순간
				else if (enemy[i].sprite.getPosition().x < 0) {
					player.life -= 1;
					enemy[i].life = 0;
				}
				// 총알과 enemy의 충돌
				for (int j = 0; j < BULLET_NUM; j++) {

					if (is_collide(bullet[j].sprite, enemy[i].sprite))
					{
						if (bullet[j].is_fired) {
							enemy[i].life -= 1;
							player.score += enemy[i].score;

							// TODO : 코드 refactoring 필요
							if (enemy[i].life == 0)
							{
								enemy[i].explosion_sound.play();
							}
							bullet[j].is_fired = 0;
						}
					}
				}
				enemy[i].sprite.move(enemy[i].speed, 0);
			}
		}

		// 시작 시간은 변하지 않음
		sprintf(info, "life: %d score: %d time: %d\n", player.life, player.score, spent_time / 1000);
		text.setString(info);

		window.clear(Color::Black);//플레이어 자체 제거 (배경 지우기)
		window.draw(bg_sprite);

		for (int i = 0; i < ENEMY_NUM; i++)
		{
			if (enemy[i].life > 0)  window.draw(enemy[i].sprite);//적 보여주기
		}
		//화면이 열려져 있는 동안 계속 그려야 함
		//draw는 나중에 호출할수록 우선순위가 높아짐
		window.draw(player.sprite);//플레이어 보여주기(그려주기)
		for (int i = 0; i < BULLET_NUM; i++)
			if (bullet[i].is_fired)
				window.draw(bullet[i].sprite);
		window.draw(text);


		if (is_gameover) {
			window.draw(gameover_sprite);
		}

		window.display();
	}

	return 0;
}