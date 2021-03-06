#ifndef _BOX_INCLUDE
#define _BOX_INCLUDE


#include "Sprite.h"
#include "TileMap.h"


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class Box
{

public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	void update(int deltaTime);
	void render();

	void setTileMap(TileMap* tileMap);
	void setPosition(const glm::vec2& pos);

	glm::ivec2 getPosition();

private:

	glm::ivec2 tileMapDispl, posBox;
	int Vjump, startY, pos2Y;
	bool Player_up, baixant;
	int PosAux;
	glm::ivec2 posPlayer1, posPlayer2;
	Texture spritesheet;
	Sprite* sprite;
	TileMap* map;
};


#endif // _BOX_INCLUDE
#pragma once
