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

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBox.x), float(tileMapDispl.y + posBox.y)));
}

void Box::update(int deltaTime)
{
	posBox.y += FALL_STEP;
	map->collisionMoveDown(posBox, glm::ivec2(48, 48), &posBox.y);

	if (posBox.y < 360) {
		PosAux = posPlayer1.x;
		posPlayer1 = Game::instance().getPosPlayer1();
		//el player esta movent cap a la dreta ->
		if (posPlayer1.x - PosAux > 0) {
			int difx = posBox.x - posPlayer1.x;
			if (difx < 48 && difx > 0) {
				//el jugador esta a la mateixa x, comprovem si es troba a dalt
				int dify = posBox.y - posPlayer1.y;
				if (dify < 58 && dify > 38) {
					posPlayer1.y = posBox.y + 48;
					Game::instance().setPosition1(posPlayer1);
				}//comprovem si es troba a la mateixa altura
				else if (dify < 38 && dify > -5) {
					posBox.x += 2;
					if (map->collisionMoveRight(posBox, glm::ivec2(48, 48))) {
						posPlayer1.x -= 2;
						posBox.x -= 2;
						Game::instance().setPosition1(posPlayer1);
					}
				}
			}
		}
		//el player esta movent cap a la esquerra <-
		else if (posPlayer1.x - PosAux < 0) {
			int difx = posPlayer1.x - posBox.x;
			if (difx < 48 && difx > 0) {
				//el jugador esta a la mateixa x, comprovem si es troba a dalt
				int dify = posBox.y - posPlayer1.y;
				if (dify < 58 && dify > 38) {
					posPlayer1.y = posBox.y + 48;
					Game::instance().setPosition1(posPlayer1);
				}
				else if (dify < 38 && dify > -5) {
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
	else {
		posPlayer2 = Game::instance().getPosPlayer2();
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
