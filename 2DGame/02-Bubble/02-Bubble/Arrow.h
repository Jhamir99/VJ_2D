#ifndef _ARROW_INCLUDE
#define _ARROW_INCLUDE

#include "Sprite.h"
#include "TileMap.h"


class Arrow
{

public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	void update(int deltaTime);
	void render();

	void setTileMap(TileMap* tileMap);
	void setPosition(const glm::vec2& pos);

private:
	bool bJumping;
	glm::ivec2 tileMapDispl, posArrow;
	int jumpAngle, startY;
	Texture spritesheet;
	Sprite* sprite;
	TileMap* map;
	int nButtons, cButton, spcButtons;
};
#endif