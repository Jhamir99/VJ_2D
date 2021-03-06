#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE


#include "Sprite.h"
#include "TileMap.h"


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class Player
{

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void update_win_animation(int deltaTime);
	void update_death_animation(int deltaTime);
	void render();
	
	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);


	void enable_doubleJump();

	glm::ivec2 getPosition();
	
private:
	bool bJumping, bDoubleJump;
	bool GodMode;
	glm::ivec2 tileMapDispl, posPlayer;
	int startY;
	float Vjump;
	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;
};


#endif // _PLAYER_INCLUDE


