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
	STAND, MOVE_LEFT, MOVE_RIGHT, JUMP, WIN, DEATH
};


void Player::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	bJumping = false;
	GodMode = false;
	bDoubleJump = false;
	spritesheet.loadFromFile("images/drake.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(48, 48), glm::vec2(0.25, 0.125), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(6);

	sprite->setAnimationSpeed(STAND, 1);
	sprite->addKeyframe(STAND, glm::vec2(0.0f, 0.5f));
	sprite->addKeyframe(STAND, glm::vec2(0.25f, 0.5f));
	sprite->addKeyframe(STAND, glm::vec2(0.5, 0.5f));

	sprite->setAnimationSpeed(MOVE_LEFT, 8);
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.0f, 0.625f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.25f, 0.625f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.5f, 0.625f));

	sprite->setAnimationSpeed(MOVE_RIGHT, 8);
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.0f, 0.75f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25f, 0.75f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.5f, 0.75f));

	sprite->setAnimationSpeed(JUMP, 10);
	sprite->addKeyframe(JUMP, glm::vec2(0.0f, 0.875f));
	sprite->addKeyframe(JUMP, glm::vec2(0.25f, 0.875f));
	sprite->addKeyframe(JUMP, glm::vec2(0.5f, 0.875f));

	sprite->setAnimationSpeed(WIN, 6);
	sprite->addKeyframe(WIN, glm::vec2(0.0f, 0.5f));
	sprite->addKeyframe(WIN, glm::vec2(0.25f, 0.5f));
	sprite->addKeyframe(WIN, glm::vec2(0.5f, 0.5f));

	sprite->setAnimationSpeed(DEATH, 8);
	sprite->addKeyframe(DEATH, glm::vec2(0.5f, 0.5f));
	sprite->addKeyframe(DEATH, glm::vec2(0.75f, 0.5f));
	sprite->addKeyframe(DEATH, glm::vec2(0.75f, 0.625f));
	sprite->addKeyframe(DEATH, glm::vec2(0.75f, 0.625f));
	sprite->addKeyframe(DEATH, glm::vec2(0.75f, 0.625f));
		
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

		if (bDoubleJump && (Vjump < JUMP_SPEED - 14 * JUMP_A) && Game::instance().getSpecialKey(GLUT_KEY_UP))
		{
			sprite->changeAnimation(STAND);
			bDoubleJump = false;
			Vjump = JUMP_SPEED;
		}
	}
	else
	{
		posPlayer.y += FALL_STEP;
		if(map->collisionMoveDown(posPlayer, glm::ivec2(48, 48), &posPlayer.y))
			bDoubleJump = false;
		if (Game::instance().getSpecialKey(GLUT_KEY_UP))
		{
			bJumping = true;
			Vjump = JUMP_SPEED;
			startY = posPlayer.y;
		}
	}

	if (bDoubleJump && sprite->animation() == STAND)
		sprite->changeAnimation(WIN);

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));

	
}

void Player::update_win_animation(int deltaTime) {
	if (sprite->animation() != WIN)
		sprite->changeAnimation(WIN);
	sprite->update(deltaTime);
}

void Player::update_death_animation(int deltaTime)
{
	if (sprite->animation() != DEATH)
		sprite->changeAnimation(DEATH);
	sprite->update(deltaTime);
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

void Player::enable_doubleJump() {
	bDoubleJump = true;
}

glm::ivec2 Player::getPosition()
{
	return posPlayer;
}






