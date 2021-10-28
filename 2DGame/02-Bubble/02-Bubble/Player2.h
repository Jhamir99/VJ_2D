#ifndef _PLAYER2_INCLUDE
#define _PLAYER2_INCLUDE


#include "Sprite.h"
#include "TileMap.h"


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class Player2
{

public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	void update(int deltaTime);
	void render();

	void setTileMap(TileMap* tileMap);
	void setPosition(const glm::vec2& pos);

	glm::ivec2 getPosition();

private:
	bool bJumping;
	glm::ivec2 tileMapDispl, posPlayer;
	int startY;
	float Vjump;
	Texture spritesheet;
	Sprite* sprite;
	TileMap* map;

};


#endif // _PLAYER2_INCLUDE
#pragma once
