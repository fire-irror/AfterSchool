#include <stdio.h>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <stdlib.h>
#include <time.h>

using namespace sf;

int main(void) {

    RenderWindow window(VideoMode(1200, 800), "Card_Game");// 화면 크기, 제목
    window.setFramerateLimit(60);// 60초마다 한 번씩 돌아간다

    // f : 소수
    // Vector2i : 정수
    Vector2i mouse_pos;// 마우스 좌표
    int click_cnt = 0;    //마우스 누른 횟수

    while (window.isOpen())
    {
        mouse_pos = Mouse::getPosition(window);// 마우스 좌표 실시간으로 받기

        Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
            case Event::Closed:
                window.close();
                break;
            }
        }
        if (Mouse::isButtonPressed(Mouse::Left)) {
            click_cnt++;
        }
        printf("(%d, %d) 클릭횟수 %d\n"
            , mouse_pos.x, mouse_pos.y,click_cnt);// 마우스 x값 y값 출력
    }

    return 0;
}