#include <iostream>

// last include (requires previous includes)
#include "globals.hpp"

sf::RectangleShape menu;
sf::RectangleShape background;
sf::RectangleShape outline;
sf::Text speech;

void resize(int width, int height) {
	// get ratio based on the original size
	float widthRatio = (float) gb::window.getSize().x / gb::gridWidth;	// TODO add bar widths
	float heightRatio = (float) gb::window.getSize().y / gb::gridHeight;
	int heightOffset = 0;
	int widthOffset = 0;
	float ratio = 1;
	
	// use the smaller ratio to update the window size
	if (heightRatio > widthRatio) {
		// black border up and down (undesirable)
		heightOffset = (height - width * gb::sizeY / gb::sizeX) / widthRatio / 2;
		ratio = widthRatio;
	}
	else {
		// black border left and right
		widthOffset = (width - height * gb::sizeX / gb::sizeY) / heightRatio / 2;
		ratio = heightRatio;
	}
	
	// set view
	gb::window.setView(sf::View(sf::FloatRect(-widthOffset, -heightOffset, width/ratio, height/ratio)));
	// resize background
	background.setPosition(-widthOffset, -heightOffset);
	background.setSize(sf::Vector2f(width/ratio, height/ratio));
	background.setTextureRect(sf::IntRect(0, 0, width/ratio, height/ratio));
	// resize outline
	outline.setPosition(0, 0);
	outline.setSize(sf::Vector2f(width/ratio - 2 * widthOffset, height/ratio - 2 * heightOffset));
	// resize menu
	menu.setPosition(-widthOffset, -heightOffset);
	menu.setSize(sf::Vector2f(width/ratio, height/ratio));
}

int main() {
	// define background
	sf::Texture backgroundTexture;
	backgroundTexture.loadFromFile(std::string(PATH) + "img/background.png");
	backgroundTexture.setRepeated(true);
	background.setTexture(&backgroundTexture);
	
	// define menu texture (to be used with the background)
	sf::Texture menuTexture;
	menuTexture.loadFromFile(std::string(PATH) + "img/titleScreen.png");
	menu.setTexture(&menuTexture);
	
	// define menu variables
	unsigned int currentLevel = 0;
	sf::Font font;
	font.loadFromFile(std::string(PATH) + "fonts/LiberationSerif-Regular.ttf");
	speech.setFont(font);
	speech.setColor(sf::Color(0x00, 0x00, 0x00));
	speech.setCharacterSize(16);
	speech.setPosition(sf::Vector2f(16, 16));
	
	// define outline
	outline.setOutlineColor(sf::Color(0x90, 0x90, 0x00));
	outline.setFillColor(sf::Color(0x00, 0x00, 0x00, 0x00));
	outline.setOutlineThickness(2.0f);
	
	// window settings
	//gb::window.setVerticalSyncEnabled(true);
	//gb::window.setFramerateLimit(30); // avoid noise ;)
	gb::window.setMouseCursorVisible(false);
	resize(gb::screenWidth, gb::screenHeight);
	bool focus = true;
	
	// define a clock to measure time
	sf::Clock clock;
	sf::Time deltaT = clock.restart();
	gb::clock.restart();

	gb::soundManager.playMusic(std::string(PATH) + "sound/backgroundFast.ogg");
	
	
	// main loop
	while (gb::window.isOpen()) {
		// poll events (do not use for input handling)
		sf::Event event;
		while (gb::window.pollEvent(event)) {
			if (event.type == sf::Event::Closed  || ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape))) {
				if (gb::inMenu) {
					gb::window.close();
				}
				else {
					gb::inMenu = true;
				}
			}
			if (event.type == sf::Event::LostFocus) {
				focus = false;
			}
			if (event.type == sf::Event::GainedFocus) {
				focus = true;
			}
			// maintain aspect ratio
			if (event.type == sf::Event::Resized) {
				int width = event.size.width;
				int height = event.size.height;
				resize(width, height);
			}
			if (gb::inMenu && event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Left) {
				if (currentLevel == 0) {
					currentLevel = gb::numLevels - 1;
				}
				else {
					--currentLevel;
				}
			}
			if (gb::inMenu && event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Right) {
				if (currentLevel == gb::numLevels - 1) {
					currentLevel = 0;
				}
				else {
					++currentLevel;
				}
			}
			if (gb::inMenu && event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Return) {
				gb::inMenu = false;
				gb::sceneManager.nextLevel(currentLevel);
			}
		}

		// retrieve input (either gamepad or keyboard)
		if (sf::Joystick::isConnected(0)) {
			// retrieve current gamepad gb::input
			gb::input[0] = sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::PovX) == -100;
			gb::input[1] = sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::PovX) == 100;
			gb::input[2] = sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::PovY) == -100;
			gb::input[3] = sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::PovY) == 100;
			// TODO input 4 - 7 buggy?
			for (int i = 0; i < 3; ++i) {
				gb::input[i + 4] = sf::Joystick::isButtonPressed(0, i);
			}
		}
		else {
			// retrieve current keyboard input
			for (int i = 0; i < INPUT_SIZE; ++i) {
				gb::input[i] = sf::Keyboard::isKeyPressed(gb::keyboardBinding[i]) && focus;
			}
		}
		
		// clear window content
		gb::window.clear();
		// reset clock and determine elapsed time since last frame
		deltaT = clock.restart();
		// update game with deltaT when focused
		
		if (gb::inMenu) {
			// do menu logic here
			speech.setString(
				"Current Level: " + std::to_string(currentLevel + 1) +
				"\nLEFT, RIGHT ARROW - choose level"+
				"\nRETURN - start the game" +
				"\nESC - quit the game" +
				"\n\nIngame usage:" +
				"\nARROW keys - move the character" +
				"\nSPACE key - proceed with dialogue" +
				"\nS key- skip dialogue" +
				"\nESC key- return to menu" +
				"\n(You may use a gamepad instead)" +
				"\n\n\nDeveloped by:\nAnnemarie Mattmann,\nJohannes Mattmann,\nMatthias Gazzari,\nMoritz Hagemann and\nSebastian Artz."
			);
			// draw menu and level number
			gb::window.draw(menu);
			gb::window.draw(speech);
		}
		else {
			gb::window.draw(background);
			if (gb::showOutline){
				gb::window.draw(outline);
			}
			if (focus) {
				gb::sceneManager.update(deltaT);
			}
		}
		gb::window.display();
	}
}
