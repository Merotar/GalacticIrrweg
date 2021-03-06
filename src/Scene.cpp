/*
 * Scene.cpp
 *
 *  Created on: 24.01.2015
 *      Author: sartz
 */

#include "Scene.hpp"
#include "Tile.hpp"
#include "globals.hpp"
#include <iostream>
#include "GUI.hpp"
#include <math.h>
#include "globals.hpp"
#include "KeyItem.hpp"

Scene::Scene() {
	// TODO Auto-generated constructor stub
	gameBoard.resize(gb::sizeX * gb::sizeY * gb::largeTileSizeX * gb::largeTileSizeY);
	textBox = new TextBox();
	leaved = false;
	highscore  = 0;
	fooexit = false;
}

Scene::~Scene() {
	// TODO Auto-generated destructor stub
}


GameObject* Scene::getTile(int x, int y)
{
	if (x + y*gb::sizeX < (int)gameBoard.size())
	{
		return gameBoard[x + y * gb::sizeX * gb::largeTileSizeX];
	}
	return 0;
}


void Scene::setTile(GameObject* obj, int x, int y)
{
	gameBoard[x + y * gb::sizeX * gb::largeTileSizeX] = obj;
}

void Scene::setGUI(GUI* obj)
{
	gui = obj;
}

const std::vector<GameObject*> &Scene::getGameBoard() const
{
	return gameBoard;
}

void Scene::switchLargeTile(int x1, int y1, int x2, int y2)
{
	int startX1 = x1*gb::largeTileSizeX;
	int startY1 = y1*gb::largeTileSizeY;
	int startX2 = x2*gb::largeTileSizeX;
	int startY2 = y2*gb::largeTileSizeY;

	sf::Vector2f orthogonal, dir;
	float length;
	float momMax = 80.f;

	for (int x=0;x<gb::largeTileSizeX;x++)
	{
		for (int y=0;y<gb::largeTileSizeY;y++)
		{
			sf::Vector2f tmpPos = getTile(startX1+x, startY1+y)->getPosition();
			sf::Vector2f tmpPos2 = getTile(startX2+x, startY2+y)->getPosition();
//			getTile(startX2+x, startY2+y)->setPosition(tmpPos.x, tmpPos.y);
//			getTile(startX1+x, startY1+y)->setPosition(tmpPos2.x, tmpPos2.y);

			TileFlightData tmp;
			tmp.startPos = tmpPos;
			tmp.currentPos = tmpPos;
			tmp.targetPos = tmpPos2;
			tmp.tile = getTile(startX1+x, startY1+y);
			dir = tmp.targetPos - tmp.currentPos;
			dir *= 0.2f;
			orthogonal.x = -dir.y;
			orthogonal.y = dir.x;
			length = sqrt(orthogonal.x*orthogonal.x+orthogonal.y*orthogonal.y);
			orthogonal = orthogonal * 1.f/length;
			tmp.momentum.x = momMax * (2.f * (1.f*rand() / RAND_MAX) - 1.f);
			tmp.momentum.y = momMax * (2.f * (1.f*rand() / RAND_MAX) - 1.f);
			tmp.scale = 1.f;
			tileAnimationPos.push_back(tmp);

			tmp.startPos = tmpPos2;
			tmp.currentPos = tmpPos2;
			tmp.targetPos = tmpPos;
			tmp.tile = getTile(startX2+x, startY2+y);
			dir = tmp.targetPos -tmp.currentPos;
			length = sqrt(dir.x*dir.x+dir.y*dir.y);
			dir = dir * 1.f/length;
			orthogonal.x = -dir.y;
			orthogonal.y = dir.x;
//			length = sqrt(orthogonal.x*orthogonal.x+orthogonal.y*orthogonal.y);
//			orthogonal = orthogonal * 1.f/length;
			tmp.momentum.x = momMax * (2.f * (1.f*rand() / RAND_MAX) - 1.f);
			tmp.momentum.y = momMax * (2.f * (1.f*rand() / RAND_MAX) - 1.f);
			tmp.scale = 1.f;
//			tmp.momentum = orthogonal * 40.f * (2.f * (1.f*rand() / RAND_MAX) - 1.f);
			tileAnimationPos.push_back(tmp);
		}
	}
	tileAnimationTime = 1.5f;

}

void Scene::updateTileAnimation(sf::Time deltaT)
{
	float dt = deltaT.asSeconds() * 1000;
	tileAnimationTime -= dt / 1000;
	float scaleMax = 1.4;
		for(std::vector<TileFlightData>::iterator itIt = tileAnimationPos.begin() ; itIt != tileAnimationPos.end() ; ) {
			TileFlightData &tmpObj = (*itIt);
			tmpObj.momentum = tmpObj.momentum * 0.95f + (tmpObj.targetPos - tmpObj.currentPos)* 0.02f;
			sf::Vector2f dir = tmpObj.momentum;
			tmpObj.currentPos += dt*dir * 0.01f;
			sf::Vector2f distVec1 = tmpObj.currentPos- tmpObj.startPos;
			sf::Vector2f distVec2 = tmpObj.currentPos- tmpObj.targetPos;
			sf::Vector2f distTotalVec = tmpObj.startPos- tmpObj.targetPos;
			float dist1 = sqrt(distVec1.x*distVec1.x+distVec1.y*distVec1.y);
			float dist2 = sqrt(distVec2.x*distVec2.x+distVec2.y*distVec2.y);
			float distTotal = sqrt(distTotalVec.x*distTotalVec.x+distTotalVec.y*distTotalVec.y);
			tmpObj.scale = (std::min(dist1,dist2)+distTotal) / distTotal;
			tmpObj.tile->mySprite->setScale(scaleMax*tmpObj.scale, scaleMax*tmpObj.scale);
			tmpObj.tile->setPosition(tmpObj.currentPos.x, tmpObj.currentPos.y);

			// delete animation if target is reached
			if (dir.x*dir.x+dir.y*dir.y < 10 || tileAnimationTime < 0)
			{
				tmpObj.tile->setPosition(tmpObj.targetPos.x, tmpObj.targetPos.y);
				tmpObj.tile->mySprite->setScale(1, 1);
				itIt = tileAnimationPos.erase(itIt);
			}
			else
			{
				itIt ++;
			}
		}
}

void Scene::update(sf::Time deltaT)
{
//	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
//	{
//		sf::Vector2i globalPosition = sf::Mouse::getPosition(gb::window);
//
//		sf::Vector2f localPosition;
//		localPosition.x = 1.f*globalPosition.x/(gb::pixelSizeX);
//		localPosition.y = 1.f*globalPosition.y/(gb::pixelSizeY);
//		std::cout<<localPosition.x<<", "<<localPosition.y<<std::endl;
//	}
	/*for (std::vector<GameObject*>::iterator it = gameBoard.begin();it != gameBoard.end(); it++)
	{
		(*it)->update(deltaT);
		std::cout << (*it) << std::endl;
	}*/
	if (highscore != 0)
	{
		highscore->update(deltaT);
		return;
	}
	if (leaved && !textBox->enabled())
	{
		finishLevel();
	}

	updateTileAnimation(deltaT);

	for(auto& obj: gameBoard) {
		obj->update(deltaT);
	}

	for(auto& obj: items) {
		obj->update(deltaT);
	}
	player->update(deltaT);
	if (gui != 0)
	{
		gui->update(deltaT);
	}

	sf::Font font;
	font.loadFromFile(std::string(PATH) + "fonts/LiberationSerif-Regular.ttf");

	sf::Text level;
	level.setFont(font);
	level.setPosition(gb::gridWidth + 2, gb::gridHeight - 32);
	level.setString(std::to_string(gb::sceneManager.getCurrentLevelNumber()+1));
	gb::window.draw(level);
	textBox->update(deltaT);
	if (!fooexit){
		for(std::vector<Item*>::iterator itIt = items.begin() ; itIt != items.end() ; ) {
			if (player->intersects(**itIt))
			{
				if ((*itIt)->applyEffect())
				{
					fooexit = true;
					return;
				}
				if ((*itIt)->collectable)
				{
					itIt = items.erase(itIt);
				}
				else
				{
					itIt ++;
				}
			}
			else
			{
				itIt ++;
			}
		}
	}
}
void Scene::finishLevel()
{
	highscore = new Highscore(gb::sceneManager.getCurrentLevelNumber()+1);
	highscore->save();
	highscore->load();
}


bool Scene::readyToLeave() const
{
	int size = items.size();
	int keysInLevel = 0;
	for(int i = 0;i < size;i++)
	{
		if (dynamic_cast<KeyItem*>(items[i]))
		{
			keysInLevel++;
		}
	}
	return (keysInLevel == 0);
}


void Scene::leave()
{
	if (!readyToLeave()) return;
	leaved = true;
	textBox->triggerText("end");
	if (!textBox->enabled())
	{
		finishLevel();
	}
}
