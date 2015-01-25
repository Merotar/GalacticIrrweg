#include "globals.hpp"

sf::Keyboard::Key keyboardBinding[] = {
	sf::Keyboard::Left,
	sf::Keyboard::Right,
	sf::Keyboard::Up,
	sf::Keyboard::Down,
	sf::Keyboard::S,
	sf::Keyboard::Space
};


sf::RenderWindow window(sf::VideoMode(screenWidth, screenHeight), "Galactic Irrweg");
SceneManager sceneManager;
TextureManager textureManager;
#ifndef _WIN32
	SoundManager soundManager;
#else
	SoundManager soundManager(false);
#endif

bool focus = true;
bool inMenu = true;
InputType input;
sf::Clock globalClock;
