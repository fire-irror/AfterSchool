#include <stdio.h>
#include <SFML/Graphics.hpp>

using namespace sf;

int main(void) {
	//������ â ����
	RenderWindow window(VideoMode(640, 480), "AfterSchool");

	//�簢�� â �׸���
	RectangleShape player;
	player.setSize(Vector2f(40, 40));
	player.setPosition(100, 100);
	window.draw(player);
	player.setFillColor(Color::Red);

	//�����찡 �������� �� ���� �ݺ�
	while (window.isOpen()) {
		
		Event event;
		while (window.pollEvent(event)) {
			
			//����(x)��ư�� ������  
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