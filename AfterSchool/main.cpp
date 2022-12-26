#include <stdio.h>
#include <SFML/Graphics.hpp>

using namespace sf;

int main(void) {
	//윈도우 창 생성
	RenderWindow window(VideoMode(640, 480), "AfterSchool");

	//사각형 창 그리기
	RectangleShape player;
	player.setSize(Vector2f(40, 40));
	player.setPosition(100, 100);
	window.draw(player);
	player.setFillColor(Color::Red);

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
		window.draw(player);
		window.display();
	}
	return 0;
}