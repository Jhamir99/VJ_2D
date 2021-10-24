#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "Arrow.h"
#include "Player2.h"

// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class Scene
{

public:
	Scene();
	~Scene();

	void init();
	void initMenu();
	void initInstructions();
	void initCredits();
	void initGame();
	void update(int deltaTime);
	void render();


	//Custom
	void showMenu();
	void showInstructions();
	void showGame();
	void showCredits();

private:
	void initShaders();


private:
	TileMap *map;
	Player *player;
	Player2 *player2;

	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;
	Sprite *menu, *instructions, *credits;
	Texture spritesheet, texHand, texIns, texCred;
	Arrow *arrow;
	bool bMenu, bArrow, bInstructions, bCredits, bMusic = false;
};


#endif // _SCENE_INCLUDE

