#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <time.h>
#include <vector>
#include <deque>
#include <cmath>

using namespace std;
using namespace sf;

int main() {
	srand(static_cast<unsigned>(time(NULL)));

	const int H = 960;
	const int W = 1707;

	const Vector2i up(0, -1);
	const Vector2i down(0, 1);
	const Vector2i left(-1, 0);
	const Vector2i right(1, 0);

	Vector2i dir = right;
	deque<Vector2f> vBody;

	bool isRunning = false;
	//bool level = 0;
	bool addSeg = false;
	bool game_over = false;
	int score = 0;
	// lower = faster
	float tickRate = 0.1;
	float rateTimer = tickRate;

	Clock timer;

	///////
	RectangleShape boundryL(Vector2f(10, 790));
	boundryL.setFillColor(Color::Transparent);
	boundryL.setPosition(310, 35);

	RectangleShape boundryR(Vector2f(10, 790));
	boundryR.setFillColor(Color::Transparent);
	boundryR.setPosition(1400, 35);

	RectangleShape boundryT(Vector2f(1080, 10));
	boundryT.setFillColor(Color::Transparent);
	boundryT.setPosition(320, 30);

	RectangleShape boundryB(Vector2f(1080, 10));
	boundryB.setFillColor(Color::Transparent);
	boundryB.setPosition(320, 820);
	///////

	//creating window
	RenderWindow window(VideoMode(W, H), "Snake Game", Style::Titlebar | Style::Close);

	window.setFramerateLimit(60);

	Font font, font2;
	font.loadFromFile("font/SedgwickAveDisplay-Regular.ttf");
	font2.loadFromFile("font/Exo2-Italic-VariableFont_wght.ttf");

	Texture apple, head, body, welcome_background, game_background;
	apple.loadFromFile("images&sprites/apple.png");
	head.loadFromFile("images&sprites/head.png");
	body.loadFromFile("images&sprites/body.png");
	welcome_background.loadFromFile("images&sprites/welcome_backround.jpg");
	game_background.loadFromFile("images&sprites/game_background.png");

	Sprite sApple(apple), sHead(head), sWelcome_background(welcome_background), sGame_background(game_background), sBody(body);
	sHead.setOrigin(Vector2f(head.getSize().x / 2, head.getSize().y / 2));
	sApple.setOrigin(Vector2f(apple.getSize().x / 2, apple.getSize().y / 2));
	sApple.setPosition(300 + rand() % 1101, 40 + rand() % 781);
	sHead.setOrigin(Vector2f(head.getSize().x / 2, head.getSize().y / 2));
	sHead.setPosition(700, 300);

	Text text("WELCOME!!\nPRESS ENTER TO CONTINUE", font, 40);
	text.setPosition(W / 2 - 200, 200);
	text.setFillColor(Color::Black);

	Text text2("by:\nGeorge Hany\n", font2, 30);
	text2.setPosition(W / 2 - 100, 500);
	text2.setFillColor(Color::Black);
	text2.setStyle(Text::Bold);

	Text text3("select sound track using '1,2,3' keys.", font2, 20);
	text3.setPosition(1320, 20);
	text3.setFillColor(Color::Black);
	text3.setStyle(Text::Underlined);

	Text scoreBoard;
	scoreBoard.setPosition(580, 870);
	scoreBoard.setFillColor(Color::Blue);
	scoreBoard.setFont(font);
	scoreBoard.setCharacterSize(40);

	Text watch;
	watch.setPosition(900, 870);
	watch.setFillColor(Color::Blue);
	watch.setFont(font2);
	watch.setCharacterSize(40);

	SoundBuffer buffer;
	if (!buffer.loadFromFile("sounds/Swoosh.wav")) {
		return -1;
	}
	Sound swosh;
	swosh.setBuffer(buffer);

	Music music1;
	if (!music1.openFromFile("sounds/World of Magic.wav"))
		return -1;

	Music music2;
	if (!music2.openFromFile("sounds/Two-Places.ogg"))
		return -1;

	Music music3;
	if (!music3.openFromFile("sounds/Sanctum.wav"))
		return -1;

	/*music1.play();
	music1.setLoop(true);*/

	vBody.push_back(sHead.getPosition());

	while (window.isOpen()) {
		if (Keyboard::isKeyPressed(Keyboard::Num1)) { music1.play(); music1.setLoop(true); music2.stop(); music3.stop(); }
		if (Keyboard::isKeyPressed(Keyboard::Num2)) { music2.play(); music2.setLoop(true); music1.stop(); music3.stop(); }
		if (Keyboard::isKeyPressed(Keyboard::Num3)) { music3.play(); music3.setLoop(true); music1.stop(); music2.stop(); }

		Event ev;
		while (window.pollEvent(ev)) {
			if (ev.type == Event::Closed) {
				window.close();
			}
		}

		window.clear();

		if (!isRunning) {
			// Draw main menu
			window.draw(sWelcome_background);
			window.draw(text);
			window.draw(text2);
			window.draw(text3);

			if (Keyboard::isKeyPressed(Keyboard::Enter)) {
				isRunning = true;
				timer.restart();
			}
		}
		else {
			// Draw static sprites
			{
				window.draw(sGame_background);
				Time elapsed = timer.getElapsedTime();
				const unsigned int seconds = static_cast<unsigned int>(timer.getElapsedTime().asSeconds());

				watch.setString("Timer: " + to_string(seconds) + "s"); watch.setString("Timer: " + to_string(seconds) + "s");
				window.draw(watch);

				scoreBoard.setString(to_string(score));
				window.draw(scoreBoard);

				window.draw(boundryL);
				window.draw(boundryR);
				window.draw(boundryT);
				window.draw(boundryB);
			}

			// Handle Input
			{
				if (Keyboard::isKeyPressed(Keyboard::Up) || Keyboard::isKeyPressed(Keyboard::W) && (dir != down)) {
					dir = up;
					swosh.play();
					sHead.setRotation(270.0f);
				}
				if (Keyboard::isKeyPressed(Keyboard::Down) || Keyboard::isKeyPressed(Keyboard::S) && (dir != up)) {
					dir = down;
					swosh.play();
					sHead.setRotation(90.0f);
				}
				if (Keyboard::isKeyPressed(Keyboard::Left) || Keyboard::isKeyPressed(Keyboard::A) && (dir != right)) {
					dir = left;
					swosh.play();
					sHead.setRotation(180.0f);
				}
				if (Keyboard::isKeyPressed(Keyboard::Right) || Keyboard::isKeyPressed(Keyboard::D) && (dir != left)) {
					dir = right;
					swosh.play();
					sHead.setRotation(360.0f);
				}
			}

			// Handle out of bounds cases
			{
				if ((sHead.getPosition().x) <= 310.0f) {
					sHead.setPosition(1399.0f, sHead.getPosition().y);
					dir = left;
				}
				else if ((sHead.getPosition().x) >= 1400.0f) {
					sHead.setPosition(311.0f, sHead.getPosition().y);
					dir = right;
				}

				if ((sHead.getPosition().y) <= 30.0f) {
					sHead.setPosition(sHead.getPosition().x, 819.0f);
					dir = up;
				}
				else if ((sHead.getPosition().y) >= 820.0f) {
					sHead.setPosition(sHead.getPosition().x, 31.0f);
					dir = down;
				}
			}

			// Update Movement
			{
				Vector2f delta = Vector2f(dir.x * ((int)body.getSize().x), dir.y * ((int)body.getSize().y));
				Vector2f pos = sHead.getPosition() + delta;

				if (addSeg) {
					addSeg = false;
					vBody.push_front(pos);
				}

				rateTimer += 1.0f / 60.0f;
				if (rateTimer >= tickRate) {

					if (!vBody.empty()) {
						vBody.pop_back();
						vBody.push_front(pos);
						sHead.setPosition(vBody.front());
					}
					else {
						sHead.setPosition(pos);
					}

					rateTimer = 0.0f;
				}
			}

			// Handle apple collision
			if (sHead.getGlobalBounds().intersects(sApple.getGlobalBounds())) {
				int x = 300 + rand() % 1101;
				int y = 40 + rand() % 781;

				sApple.setPosition(x, y);
				addSeg = true;

				score++;
				if (score % 10 == 0) {
					tickRate -= 0.03;
				}
			}

			// Draw apple and snake
			{
				window.draw(sApple);
				window.draw(sHead);

				for (int i = 1; i < vBody.size(); i++) {
					Sprite sBody(body);
					sBody.setOrigin(body.getSize().x / 2.0f, body.getSize().y / 2.0f);
					sBody.setPosition(vBody[i].x, vBody[i].y);
					window.draw(sBody);
				}
			}
			for (int i = 2; i < vBody.size();i++) {
				if (sHead.getPosition().x == vBody[i].x && sHead.getPosition().y == vBody[i].y) {
					game_over = true;
				}
			}
			if (game_over) {
				window.clear();
				music1.stop();
				music2.stop();
				music3.stop();
				window.draw(sGame_background);

				Text text4("    GAME OVER\nthanks for playing! :)", font, 40);
				text4.setPosition(705, 400);
				text4.setFillColor(Color::Black);

				window.draw(text4);
			}
		}
		window.display();
	}
}