#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Player.h"
#include "Game.h"


#define JUMP_SPEED 14
#define JUMP_A 0.7
#define FALL_STEP 4


enum PlayerAnims
{
	STAND, MOVE_LEFT, MOVE_RIGHT, JUMP
};


void Player::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	bJumping = false;
	spritesheet.loadFromFile("images/drake.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(48, 48), glm::vec2(0.0833, 0.125), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(4);

	sprite->setAnimationSpeed(STAND, 1);
	sprite->addKeyframe(STAND, glm::vec2(0.75f, 0.5f));
	sprite->addKeyframe(STAND, glm::vec2(0.8333f, 0.5f));
	sprite->addKeyframe(STAND, glm::vec2(0.9166f, 0.5f));

	sprite->setAnimationSpeed(MOVE_LEFT, 8);
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.75f, 0.625f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.8333f, 0.625f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.9166f, 0.625f));

	sprite->setAnimationSpeed(MOVE_RIGHT, 8);
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.75f, 0.75f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.8333f, 0.75f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.9166f, 0.75f));

	sprite->setAnimationSpeed(JUMP, 10);
	sprite->addKeyframe(JUMP, glm::vec2(0.75f, 0.875f));
	sprite->addKeyframe(JUMP, glm::vec2(0.8333f, 0.875f));
	sprite->addKeyframe(JUMP, glm::vec2(0.9166f, 0.875f));
		
	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	
}

void Player::update(int deltaTime)
{
	sprite->update(deltaTime);
	if(Game::instance().getSpecialKey(GLUT_KEY_LEFT))
	{
		if(sprite->animation() != MOVE_LEFT)
			sprite->changeAnimation(MOVE_LEFT);
		posPlayer.x -= 2;
		if(map->collisionMoveLeft(posPlayer, glm::ivec2(48, 48)))
		{
			posPlayer.x += 2;
		}
	}
	else if(Game::instance().getSpecialKey(GLUT_KEY_RIGHT))
	{
		if(sprite->animation() != MOVE_RIGHT)
			sprite->changeAnimation(MOVE_RIGHT);
		posPlayer.x += 2;
		if(map->collisionMoveRight(posPlayer, glm::ivec2(48, 48)))
		{
			posPlayer.x -= 2;
		}
	}
	else
	{
		if(sprite->animation() == MOVE_LEFT)
			sprite->changeAnimation(STAND);
		else if (sprite->animation() == MOVE_RIGHT)
			sprite->changeAnimation(STAND);
		else if (sprite->animation() == JUMP)
			sprite->changeAnimation(STAND);
	}
	
	if(bJumping)
	{
		if (sprite->animation() != JUMP)
			sprite->changeAnimation(JUMP);

		Vjump -= JUMP_A;
		posPlayer.y -= int(Vjump);

		if (map->collisionJumpUp(posPlayer, glm::ivec2(48, 48), &posPlayer.y, Vjump))
			Vjump = 0;

		bJumping = !map->collisionJumpDown(posPlayer, glm::ivec2(48, 48), &posPlayer.y, Vjump);
	}
	else
	{
		posPlayer.y += FALL_STEP;
		map->collisionMoveDown(posPlayer, glm::ivec2(48, 48), &posPlayer.y);
		if (Game::instance().getSpecialKey(GLUT_KEY_UP))
		{
			bJumping = true;
			Vjump = JUMP_SPEED;
			startY = posPlayer.y;
		}
	}
	
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));

	if (posPlayer.y > 360) {
		Game::instance().playDeathSound();
		Game::instance().resetPlayer();		
	}
}

void Player::render()
{
	sprite->render();
}

void Player::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void Player::setPosition(const glm::vec2 &pos)
{
	posPlayer = pos;
	bJumping = false;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

glm::ivec2 Player::getPosition()
{
	return posPlayer;
}






