#ifndef _GameObj_INCLUDE
#define _GameObj_INCLUDE

#include "Sprite.h"
#include "TileMap.h"


class GameObj
{

public:
	void init_arrow(const glm::ivec2& tileMapPos,ShaderProgram& shaderProgram);
	void init_flag(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	void init_flag_reverse(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);

	void init_cactus(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	void init_cactusD(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	void init_cactusL(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);

	void init_lever(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);

	void update(int deltaTime);
	void update_animation(int deltaTime);
	void change_animation();
	void render();

	void setTileMap(TileMap* tileMap);
	void setPosition(const glm::vec2& pos);

private:
	bool bJumping;
	glm::ivec2 tileMapDispl, posGameObj;
	int jumpAngle, startY;
	Texture spritesheet;
	Sprite* sprite;
	TileMap* map;
	int nButtons, cButton, spcButtons;
};
#endif