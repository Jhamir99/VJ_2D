#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Arrow.h"
#include "Game.h"
#include "windows.h" 


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4





void Arrow::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	//Botó actual
	cButton = 0;

	//Nombre de botons
	nButtons = 4;

	//espai entre butons
	spcButtons = 91;

	bJumping = false;
	spritesheet.loadFromFile("images/pointer.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(1., 1.), &spritesheet, &shaderProgram);

	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posArrow.x), float(tileMapDispl.y + posArrow.y)));
}

void Arrow::update(int deltaTime)
{
	sprite->update(deltaTime);
	if (Game::instance().getSpecialKey(GLUT_KEY_UP))
	{
		if (cButton == 0) {
			cButton = nButtons - 1;
			posArrow.y += cButton* spcButtons;

		}
		else {
			posArrow.y -= spcButtons;
			cButton--;
		}

		Sleep(200);
	}
	else if (Game::instance().getSpecialKey(GLUT_KEY_DOWN))
	{
		if (cButton == nButtons-1) {
			cButton = 0;
			posArrow.y -= (nButtons - 1) * spcButtons;

		}
		else {
			posArrow.y += spcButtons;
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
	
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posArrow.x), float(tileMapDispl.y + posArrow.y)));
}

void Arrow::render()
{
	sprite->render();
}

void Arrow::setTileMap(TileMap* tileMap)
{
	map = tileMap;
}

void Arrow::setPosition(const glm::vec2& pos)
{
	posArrow = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posArrow.x), float(tileMapDispl.y + posArrow.y)));
}




