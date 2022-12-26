#include <stdio.h>
#include <SFML/Graphics.hpp>

using namespace sf;

int main(void) {
	//������ â ����
	RenderWindow window(VideoMode(640, 480), "AfterSchool");
	window.setFramerateLimit(60);

	//�簢�� â �׸���
	RectangleShape player;
	player.setSize(Vector2f(40, 40));
	player.setPosition(100, 100);
	window.draw(player);
	
	//�簢�� ���� ����
	player.setFillColor(Color::Red);
	
	//�÷��̾� ���ǵ� 3���� �ʱ�ȭ
	int player_speed = 5;

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
		//�÷��̾� ������ ����
		//else�� ���� ������ �߰����� ���� �� ���� ������ �پ���.
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
		//60�п� 1�ʸ��� �׷ȴ� �����ٸ� �ݺ��ϰ� �ȴ�. 
		window.clear(Color::Black);

		window.draw(player);
		window.display();
	}
	return 0;
}