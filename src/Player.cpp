#include "Player.hpp"
#include "globals.hpp"

void Player::update (sf::Time deltaTime) {
	// get input from globals and process:
	sf::Vector2f tmpPos = getPosition();
	int width = getWidth();
	int height = getHeight();
	int dir = -1;
	if (input[0]) { tmpPos.x -= 1; dir = 3; }
	if (input[1]) { tmpPos.x += 1; dir = 2; }
	if (input[2]) { tmpPos.y -= 1; dir = 1; }
	if (input[3]) { tmpPos.y += 1; dir = 0; }
	
	if (tmpPos.x > screenWidth) tmpPos.x -= screenWidth;
	if (tmpPos.x + width < 0)  tmpPos.x += screenWidth;
	if (tmpPos.y > screenHeight) tmpPos.y -= screenHeight;
	if (tmpPos.y + height < 0)  tmpPos.y += screenHeight;
	
	setPosition(tmpPos.x, tmpPos.y);
	if (dir > -1) 
	{
		animationStep += 1;
		direction = dir;
	}
	if (animationStep / slowFactor > 2) animationStep = 0; // animationStep wird immer um 1 hochgezählt, aber effektiv um den Faktor slowFactor verlangsamt
	//check for collisions:
	
	
	if (mySprite != 0)
	{
		mySprite->setTextureRect(sf::IntRect(0,0, 16, 32));
		window.draw(*mySprite);
	}
}
