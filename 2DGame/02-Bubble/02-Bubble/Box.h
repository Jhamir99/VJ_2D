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


private:

	glm::ivec2 tileMapDispl, posPlayer;
	int Vjump, startY;
	Texture spritesheet;
	Sprite* sprite;
	TileMap* map;
};


#endif // _BOX_INCLUDE
#pragma once
