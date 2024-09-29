#include <SFML\Graphics.hpp>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <sstream>
#include <iostream>
#include <sfml\audio.hpp>

int main() {

	srand(time(0));
	sf::Vector2i fieldSize(20, 15);
	
	sf::Image image;
	if (!image.loadFromFile("snake.jpg")) {
		return EXIT_FAILURE;
	}
	
	sf::SoundBuffer soundbuffer;
	if (!soundbuffer.loadFromFile("sound.mp3")) {
		return EXIT_FAILURE;
	}
	sf::Sound sound(soundbuffer);
	
	sf::Clock foodTime;

	int level = 10;
	
	sf::Texture blockTexture;
	if (!blockTexture.loadFromFile("block.png")) {
		return EXIT_FAILURE;
	}
	sf::Sprite block(blockTexture);

	sf::Vector2f blockSize(block.getLocalBounds().width, block.getLocalBounds().height);
	
	std::vector<sf::Vector2i> snake = { sf::Vector2i(5, 5) };
	

	sf::Vector2i food(rand() % fieldSize.x, rand() % fieldSize.y);

	sf::VideoMode mode(
		unsigned int(fieldSize.x * blockSize.x),
		unsigned int(fieldSize.y * blockSize.y));

	sf::Clock clock;
	
	enum class Direction { UP = 0, DOWN, LEFT, RIGHT };
	Direction direction = Direction::RIGHT;

	unsigned int score = 0;
	sf::Font font;
	if (!font.loadFromFile("arial.ttf")) {
		return EXIT_FAILURE;
	}

	bool restar = false;
	
	sf::Text text("score : 0", font, 25);
	text.setPosition(0, 0);

	sf::Text text2("food Time :", font, 25);
	text2.setPosition(0.f, 50.f);


	sf::RenderWindow window(mode, "SnakeGame");
	window.setIcon(image.getSize().x, image.getSize().y, image.getPixelsPtr());

	bool isDead = false;
	

	while (window.isOpen()) {
		sf::Event event;
		
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
			

			if (sf::Mouse::isButtonPressed(event.mouseButton.button)) {
				restar = true;
			}
			
			if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::Up) {
					direction = Direction::UP;
				}
				if (event.key.code == sf::Keyboard::Down) {
					direction = Direction::DOWN;
				}
				if (event.key.code == sf::Keyboard::Left) {
					direction = Direction::LEFT;
				}
				if (event.key.code == sf::Keyboard::Right) {
					direction = Direction::RIGHT;
				}
				if (sf::Keyboard::isKeyPressed(event.key.code) && isDead) {
					restar = true;
				}
			}
		}
		if (foodTime.getElapsedTime().asSeconds() > level) {
			food.x = rand() % fieldSize.x;
			food.y = rand() % fieldSize.y;
			foodTime.restart();
			
		}

		if (clock.getElapsedTime().asSeconds() >= 0.1f) {
			
			sf::Vector2i head = snake[0];
			
			if (direction == Direction::UP) {
				head.y--;
			}
			if (direction == Direction::DOWN) {
				head.y++;
			}
			if (direction == Direction::LEFT) {
				head.x--;
			}
			if (direction == Direction::RIGHT) {
				head.x++;
			}
			

			
			if (head.x < 0 || head.x >= fieldSize.x ||
				head.y < 0 || head.y >= fieldSize.y) {
				isDead = true;
			}
			if (!isDead) {

				if (food == head) {
					food.x = rand() % fieldSize.x;
					food.y = rand() % fieldSize.y;
					++score;
					std::stringstream strScore;
					strScore << "Score: " << score;
					text.setString(strScore.str());
					sound.play();
					if (level > 2) {
						level--;
					}
					foodTime.restart();
					
				}
				else {
					snake.pop_back();
				}
				
				for (const auto& body : snake) {
					if (head == body) {
						isDead = true;
					}
				}
				snake.insert(snake.begin(), head);
				
				clock.restart();
			}
		}
			if(isDead) {
				window.clear(sf::Color::Red);
				
				if (restar) {
					snake = { sf::Vector2i(3, 4) };
					restar = false;
					isDead = false;
					score = 0;
					std::stringstream strScore;
					strScore << "Score: " << score;
					text.setString(strScore.str());
					food.x = rand() % fieldSize.x;
					food.y = rand() % fieldSize.y;
					foodTime.restart();
					level = 10;
					continue;
				}
			}
			else {
				window.clear();
			}
			
			sf::Vector2f foodPos(food.x * blockSize.x, food.y * blockSize.y);
			block.setPosition(foodPos);
			window.draw(block);

			for (const auto& body : snake) {
				sf::Vector2f bodypos(body.x * blockSize.x, body.y * blockSize.y);
				block.setPosition(bodypos);
				window.draw(block);
			}
			window.draw(text);
			if (!isDead) {
				std::stringstream strTime;
				strTime << "food time : " << int(foodTime.getElapsedTime().asSeconds());
				text2.setString(strTime.str());
				window.draw(text2);
			}
			window.display();
	}

		return EXIT_SUCCESS;
}