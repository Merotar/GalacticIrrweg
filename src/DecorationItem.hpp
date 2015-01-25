#pragma once

#include "Item.hpp"

class DecorationItem : public Item
{
public:
	DecorationItem(sf::Sprite* sprite, bool blocksPath, int texPosX, int texPosY, int texW, int texH);
	void applyEffect();
};
