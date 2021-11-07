#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "GameObj.h"
#include "Player2.h"
#include "Box.h"

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
	void initBackground();
	void initGame();
	void update(int deltaTime);
	void render();


	//Custom
	void showMenu();
	void showInstructions();
	void showGame();
	void showCredits();

	void playDeathSound();

	void setPosition1(const glm::vec2& pos);
	void setPosition2(const glm::vec2& pos);

	glm::ivec2 getPosPlayer1();
	glm::ivec2 getPosPlayer2();

	void enable_doubleJump(int pj);

	bool goal();

	bool aprox(glm::ivec2 posPlayer, glm::ivec2 posGoal);

	bool touch_cactus(glm::ivec2 posPlayer);

	void reset_player();

private:
	void initShaders();

	void initGameShaders();


private:
	TileMap *map;
	Player *player;
	Player2 *player2;
	Box* box;
	float prev_time;
	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;
	Sprite *menu, *instructions, *credits, *background;
	Texture spritesheet, texHand, texIns, texCred, texbackground;
	GameObj *arrow, *flag, *flag_reverse, *lever, *cactus, *cactusD, *tp, *tp2;
	glm::ivec2 posLever, posTp;
	bool bMenu, bArrow, bInstructions, bCredits, bMusic = false, bBox, blever, lever_trigger, btp;
	bool wait, tp_up, player_dead, godmode = false;
	int level;

	//goal positions
	vector<glm::ivec2> posGoals1, posGoals2, posCactus, posCactusD, posCactusL;
};


#endif // _SCENE_INCLUDE