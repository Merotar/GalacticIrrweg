#include "globals.hpp"
#include "CoinItem.hpp"

CoinItem::CoinItem(sf::Sprite* sprite):Item(sprite,0,80,16,16){
}

void CoinItem::applyEffect()
{
	sceneManager.getCurrentScene().gui->addCoins(1);
};