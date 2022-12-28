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
	int speed;
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

//obj1과 obj2의 충돌 여부 충돌하면 1로 반환 아니면 0으로 반환
int is_collide(RectangleShape obj1, RectangleShape obj2) {
	return obj1.getGlobalBounds().intersects(obj2.getGlobalBounds());
}


//전역변수
const int ENEMY_NUM = 12;					//적의 최대 개수
const int W_WIDTH = 1200, W_HEIGHT = 600;	//창의 크기
const int GO_WIDTH = 320, GO_HEIGHT = 240;	//게임 오버 그림 크기

int main(void) {

	srand(time(NULL));//랜덤 함수 사용

	//640 x 480 윈도우 화면 나옴
	//잠깐 떴다가 사라지는 건 return 0때문에 프로그램이 종료된 것
	RenderWindow window(VideoMode(W_WIDTH, W_HEIGHT), "AfterSchool");
	window.setFramerateLimit(60);//1초에 60장 보여준다. 플레이어가 빨리 가지 않도록 하기


	long start_time = clock();// 게임 시작 시간
	long spent_time;// 게임 진행 시간
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


	// 플레이어
	struct Player player;
	player.sprite.setSize(Vector2f(40, 40));//플레이어 사이즈
	player.sprite.setPosition(100, 100);//플레이어 시작 위치
	player.sprite.setFillColor(Color::Red);//플레이어 색상
	player.x = player.sprite.getPosition().x;	//플레이어 x좌표
	player.y = player.sprite.getPosition().y;	//플레이어 y좌표
	player.speed = 7;//플레이어 속도
	player.score = 0;//플레이어 점수
	player.life = 10;


	//총알
	struct Bullet bullet;
	bullet.sprite.setSize(Vector2f(10, 10));
	bullet.sprite.setPosition(player.x+50, player.y+15);	//임시 테스트
	bullet.speed = 20;
	bullet.is_fired = 0;	//false니까 0으로 나타냄


	// enemy

	struct Enemy enemy[ENEMY_NUM];

	for (int i = 0; i < ENEMY_NUM; i++)
	{
		// TODO : 굉장히 비효율적인 코드이므로 나중에 refactoring
		enemy[i].explosion_buffer.loadFromFile("./resources/sound/rumble.flac");
		enemy[i].explosion_sound.setBuffer(enemy[i].explosion_buffer);
		enemy[i].score = 100;
		enemy[i].respawn_time = 8;

		enemy[i].sprite.setSize(Vector2f(70, 70));
		enemy[i].sprite.setPosition(rand() % 300 + W_WIDTH*0.9, rand() % 410);
		enemy[i].life = 1;
		enemy[i].sprite.setFillColor(Color::Yellow);//적 색상
		enemy[i].speed = -(rand() % 10 + 1);
	}


	//유지 시키는 방법은? -> 무한 반복
	while (window.isOpen()) //윈도우창이 열려있는 동안 계속 반복
	{
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
				////space키 누르면 모든 enemy 다시 출현
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

		spent_time = clock() - start_time;// 시간이 지남에 따라 증가

		//총알이 플레이어럴 따라다닐 수 있도록 
		player.x = player.sprite.getPosition().x;	//플레이어 x좌표
		player.y = player.sprite.getPosition().y;	//플레이어 y좌표

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
			//총알이 발사 되어있지 않다면
			if(!bullet.is_fired )
			bullet.sprite.setPosition(player.x + 50, player.y + 15);
			bullet.is_fired = 1;
			
		}

		//enemy와의 충돌
		//intersects : 플레이어와 적 사이에서 교집합이 있는가
		for (int i = 0; i < ENEMY_NUM; i++)
		{
			// 10초 마다 enemy가 젠
			if (spent_time % (1000 * enemy[i].respawn_time) < 1000 / 60) // 1초동안 60프레임이 반복되기 때문에
			{
				enemy[i].sprite.setSize(Vector2f(70, 70));
				enemy[i].sprite.setFillColor(Color::Yellow);//적 색상
				enemy[i].sprite.setPosition(rand() % 300 + W_WIDTH * 0.9, rand() % 380);// 90%부터 적들이 나옴
				enemy[i].life = 1;
				// 10초마다 enemy 속도 +1
				enemy[i].speed = -(rand() % 3 + 1 + (spent_time / 1000 / enemy[i].respawn_time));
			}
			if (enemy[i].life > 0)
			{
				// enemy와의 충돌
				if (is_collide(player.sprite, enemy[i].sprite) || is_collide(bullet.sprite, enemy[i].sprite)){
			
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

				enemy[i].sprite.move(enemy[i].speed, 0);
			}

		}
		if (bullet.is_fired) {
			bullet.sprite.move(bullet.speed, 0);
			if (bullet.sprite.getPosition().x > W_WIDTH)
				bullet.is_fired = 0;
		}


		if (player.life <=0) {
			is_gameover = 1;	//ture 임으로 1
		}

		// 시작 시간은 변하지 않음
		sprintf(info, "life: %d score: %d time: %d\n", player.life,player.score, spent_time / 1000);
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