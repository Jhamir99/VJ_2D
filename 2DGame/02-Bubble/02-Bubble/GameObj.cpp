#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "GameObj.h"
#include "Game.h"
#include "windows.h" 


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4





void GameObj::init_arrow(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	//Botó actual
	cButton = 0;

	//Nombre de botons
	nButtons = 4;

	//espai entre butons
	spcButtons = 91;

	spritesheet.loadFromFile("images/pointer.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(1., 1.), &spritesheet, &shaderProgram);

	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posGameObj.x), float(tileMapDispl.y + posGameObj.y)));
}

void GameObj::init_flag(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram) 
{
	spritesheet.loadFromFile("images/flag.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(1., 1.), &spritesheet, &shaderProgram);
	tileMapDispl = tileMapPos;
}

void GameObj::init_flag_reverse(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	spritesheet.loadFromFile("images/flag_reverse.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(1., 1.), &spritesheet, &shaderProgram);
	tileMapDispl = tileMapPos;
}

void GameObj::update(int deltaTime)
{
	sprite->update(deltaTime);
	if (Game::instance().getSpecialKey(GLUT_KEY_UP))
	{
		if (cButton == 0) {
			cButton = nButtons - 1;
			posGameObj.y += cButton* spcButtons;

		}
		else {
			posGameObj.y -= spcButtons;
			cButton--;
		}

		Sleep(200);
	}
	else if (Game::instance().getSpecialKey(GLUT_KEY_DOWN))
	{
		if (cButton == nButtons-1) {
			cButton = 0;
			posGameObj.y -= (nButtons - 1) * spcButtons;

		}
		else {
			posGameObj.y += spcButtons;
			cButton++;
		}

		Sleep(200);
	}
	
	else if (Game::instance().getKey(13))
	{
		if (cButton == nButtons - 1) {
			//exit
			Game::instance().keyPressed(27);
		}
		else if(cButton == 0) {
			//show game
			Game::instance().showGame();
		}
		else if (cButton == 1) {
			//show instructions
			Game::instance().showInstructions();
		}
		else if (cButton == 2) {
			//show credits
			Game::instance().showCredits();
		}
		Sleep(200);
	} 

	else if (Game::instance().getKey(77) || Game::instance().getKey(109)) {
		Game::instance().showMenu();
	}
	
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posGameObj.x), float(tileMapDispl.y + posGameObj.y)));
}

void GameObj::render()
{
	sprite->render();
}

void GameObj::setTileMap(TileMap* tileMap)
{
	map = tileMap;
}

void GameObj::setPosition(const glm::vec2& pos)
{
	posGameObj = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posGameObj.x), float(tileMapDispl.y + posGameObj.y)));
}




