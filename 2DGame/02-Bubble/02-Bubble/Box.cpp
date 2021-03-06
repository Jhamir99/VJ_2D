#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Box.h"
#include "Game.h"


#define FALL_STEP 4

void Box::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	spritesheet.loadFromFile("images/Box2.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(48, 48), glm::vec2(1, 1), &spritesheet, &shaderProgram);

	posPlayer1 = Game::instance().getPosPlayer1();
	posPlayer2 = Game::instance().getPosPlayer2();
	tileMapDispl = tileMapPos;
	baixant = false;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBox.x), float(tileMapDispl.y + posBox.y)));
}

void Box::update(int deltaTime)
{
	Player_up = false;

	//la caixa es troba en l'aigua, ha de poder moure els dos
	if (posBox.y > 320 && posBox.y < 340) {
		if (posBox.y > 335 && baixant)
			baixant = false;
		else if (posBox.y < 325 && !baixant)
			baixant = true;

		if (baixant)
			pos2Y += 1;
		else 
			pos2Y -= 1;

		posBox.y = pos2Y / 2;

		//mirem player1
		PosAux = posPlayer1.x;
		posPlayer1 = Game::instance().getPosPlayer1();
		int difx = posPlayer1.x - posBox.x;
		if (difx < 44 && difx > -44) {
			int dify = posBox.y - posPlayer1.y;
			if (dify < 58 && dify > 25) {
				if (!Game::instance().getSpecialKey(GLUT_KEY_UP)) {
					posPlayer1.y = posBox.y - 48;
					Game::instance().setPosition1(posPlayer1);
					Game::instance().enable_doubleJump(1);
					Player_up = true;
				}
				else if (dify < 43) {
					posPlayer1.y = posBox.y - 48;
					Game::instance().setPosition1(posPlayer1);
					Game::instance().enable_doubleJump(1);
					Player_up = true;
				}
			}
		}

		//mirem player 2
		PosAux = posPlayer2.x;
		posPlayer2 = Game::instance().getPosPlayer2();
		difx = posPlayer2.x - posBox.x;
		if (difx < 44 && difx > -44) {
			int dify = posPlayer2.y - posBox.y;
			if (dify < 58 && dify > 25) {
				if (!Game::instance().getSpecialKey(GLUT_KEY_UP))
				{
					posPlayer2.y = posBox.y + 48;
					Game::instance().setPosition2(posPlayer2);
					Game::instance().enable_doubleJump(2);
					Player_up = true;
				}
				else if (dify < 43) {
					posPlayer2.y = posBox.y + 48;
					Game::instance().setPosition2(posPlayer2);
					Game::instance().enable_doubleJump(2);
					Player_up = true;
				}
			}
		}

	}
	else if (posBox.y < 340) { //pot moure player 1
		posBox.y += FALL_STEP;
		map->collisionMoveDown(posBox, glm::ivec2(48, 48), &posBox.y);
		pos2Y = 2 * posBox.y;

		PosAux = posPlayer1.x;
		posPlayer1 = Game::instance().getPosPlayer1();
		
	    //mirem si esta a sobre
		int difx = posPlayer1.x - posBox.x;
		if (difx < 44 && difx > -44) {
			int dify = posBox.y - posPlayer1.y;
			if (dify < 58 && dify > 25) {
				if (!Game::instance().getSpecialKey(GLUT_KEY_UP)){
					posPlayer1.y = posBox.y - 48;
					Game::instance().setPosition1(posPlayer1);
					Game::instance().enable_doubleJump(1);
					Player_up = true;
				}
				else if (dify < 43) {
					posPlayer1.y = posBox.y - 48;
					Game::instance().setPosition1(posPlayer1);
					Game::instance().enable_doubleJump(1);
					Player_up = true;
				}
			}
		}
		if (difx < 48 && difx > -48 && !Player_up) {
			int dify = posBox.y - posPlayer1.y;
			if (dify < 38 && dify > -5) {
				//el player esta movent cap a la dreta ->
				if (posPlayer1.x - PosAux > 0) {
					difx = posBox.x - posPlayer1.x;
					if (difx < 48 && difx > -4) {
						//el jugador esta tocant la caixa
						posBox.x += 2;
						if (map->collisionMoveRight(posBox, glm::ivec2(48, 48))) {
							posPlayer1.x -= 2;
							posBox.x -= 2;
							Game::instance().setPosition1(posPlayer1);
						}
					}
				}
				//el player esta movent cap a la esquerra <-
				else if(PosAux - posPlayer1.x > 0){
					if (difx < 48 && difx > -4) {
						//el jugador esta tocant la caixa
						posBox.x -= 2;
						if (map->collisionMoveLeft(posBox, glm::ivec2(48, 48))) {
							posPlayer1.x += 2;
							posBox.x += 2;
							Game::instance().setPosition1(posPlayer1);
							
						}
					}
				}
			}
		}
	}
	else { //pot moure player 2
		posBox.y -= FALL_STEP;
		map->collisionMoveUp(posBox, glm::ivec2(48, 48), &posBox.y);
		PosAux = posPlayer2.x;
		posPlayer2 = Game::instance().getPosPlayer2();

		//mirem si esta a sobre
		int difx = posPlayer2.x - posBox.x;
		if (difx < 44 && difx > -44) {
			int dify = posPlayer2.y - posBox.y;
			if (dify < 58 && dify > 25) {
				if (!Game::instance().getSpecialKey(GLUT_KEY_UP))
				{
					posPlayer2.y = posBox.y + 48;
					Game::instance().setPosition2(posPlayer2);
					Game::instance().enable_doubleJump(2);
					Player_up = true;
				}
				else if (dify < 43) {
					posPlayer2.y = posBox.y + 48;
					Game::instance().setPosition2(posPlayer2);
					Game::instance().enable_doubleJump(2);
					Player_up = true;
				}
			}
		}
		else if(difx < 48 && difx > -48 && !Player_up){
			int dify = posPlayer2.y - posBox.y;
			if (dify < 38 && dify > -5) {
				//el player esta movent cap a la dreta ->
				if (posPlayer2.x - PosAux > 0) {
					difx = posBox.x - posPlayer2.x;
					if (difx < 48 && difx > -4) {
						//el jugador esta tocant la caixa
						posBox.x += 2;
						if (map->collisionMoveRight(posBox, glm::ivec2(48, 48))) {
							posPlayer2.x -= 2;
							posBox.x -= 2;
							Game::instance().setPosition2(posPlayer2);
						}
					}
				}
				//el player esta movent cap a la esquerra <-
				else if (PosAux - posPlayer2.x > 0) {
					if (difx < 48 && difx > -4) {
						//el jugador esta tocant la caixa
						posBox.x -= 2;
						if (map->collisionMoveLeft(posBox, glm::ivec2(48, 48))) {
							posPlayer2.x += 2;
							posBox.x += 2;
							Game::instance().setPosition2(posPlayer2);

						}
					}
				}
			}
		}
		pos2Y = 2 * posBox.y;
	}

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBox.x), float(tileMapDispl.y + posBox.y)));
}

void Box::render()
{
	sprite->render();
}

void Box::setTileMap(TileMap* tileMap)
{
	map = tileMap;
}

void Box::setPosition(const glm::vec2& pos)
{
	posBox = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBox.x), float(tileMapDispl.y + posBox.y)));
}

glm::ivec2 Box::getPosition()
{
	return posBox;
}
