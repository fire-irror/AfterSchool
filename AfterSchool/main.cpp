#include <stdio.h>
#include <SFML/Graphics.hpp>

using namespace sf;

int main(void) {
	//윈도우 창 생성
	RenderWindow window(VideoMode(640, 480), "AfterSchool");
	window.setFramerateLimit(60);

	//사각형 창 그리기
	RectangleShape player;
	player.setSize(Vector2f(40, 40));
	player.setPosition(100, 100);
	window.draw(player);
	
	//사각형 색상 변경
	player.setFillColor(Color::Red);
	
	//플레이어 스피드 3으로 초기화
	int player_speed = 5;

	//윈도우가 열려있을 때 까지 반복
	while (window.isOpen()) {
		
		Event event;
		while (window.pollEvent(event)) {
			
			//종료(x)버튼을 누르면  
			switch (event.type) {
			case Event::Closed:
			window. close();
			}
		}
		//플레이어 움직임 구현
		//else를 쓰지 않으면 중간으로 나갈 수 있음 제약이 줄어든다.
		if (Keyboard::isKeyPressed(Keyboard::Left)) {
			player.move(-player_speed, 0);
		}
		if (Keyboard::isKeyPressed(Keyboard::Up)) {
			player.move(0, -player_speed);
		}
		if (Keyboard::isKeyPressed(Keyboard::Down)) {
			player.move(0, player_speed);
		}
		if (Keyboard::isKeyPressed(Keyboard::Right)) {
			player.move(player_speed, 0);
		}
		//60분에 1초마다 그렸다 지웠다를 반복하게 된다. 
		window.clear(Color::Black);

		window.draw(player);
		window.display();
	}
	return 0;
}