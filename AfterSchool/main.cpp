#include <stdio.h>
#include <SFML/Graphics.hpp>

using namespace sf;

int main(void) {
	//윈도우 창 생성
	RenderWindow window(VideoMode(640, 480), "AfterSchool");

	//윈도우가 열려있을 때 까지 반복
	while (window.isOpen()) {
		
		Event event;
		while (window.pollEvent(event)) {
			//종료(x)버튼을 누르면  
			if (event.type == Event::Closed)
				window.close(); //윈도우 창 닫기
		}
	}
	return 0;
}