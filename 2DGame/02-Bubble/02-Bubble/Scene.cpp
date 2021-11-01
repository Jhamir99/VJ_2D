#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"
#include <Windows.h>
#include <MMSystem.h>



#define SCREEN_X 0
#define SCREEN_Y 0

#define INIT_PLAYER_X_TILES 20
#define INIT_PLAYER_Y_TILES 5

#define INIT_PLAYER2_X_TILES 20
#define INIT_PLAYER2_Y_TILES 15

#define LEVEL_MAX 5

Scene::Scene()
{
	map = NULL;
	player = NULL;
	player2 = NULL;
	box = NULL;
}

Scene::~Scene()
{
	if(map != NULL)
		delete map;
	if(player != NULL)
		delete player;
	if (player2 != NULL)
		delete player2;
	if (box != NULL)
		delete box;
}


void Scene::init()
{
	PlaySound(TEXT("audio/song.wav"), NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
	
	level = 1;

	initGame();
	initMenu();
	initInstructions();
	initCredits();
	currentTime = 0.0f;
	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
}

void Scene::initMenu() {
	bMenu = true;
	bArrow = true;
	spritesheet.loadFromFile("images/menu.png", TEXTURE_PIXEL_FORMAT_RGB);
	menu = Sprite::createSprite(glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT), glm::vec2(1.0, 1.0), &spritesheet, &texProgram);

	//texHand.loadFromFile("images/pointer.png", TEXTURE_PIXEL_FORMAT_RGBA);
	arrow = new GameObj();
	arrow->init_arrow(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	arrow->setPosition(glm::vec2(13 * map->getTileSize(), (11 * map->getTileSize())-68));
}

void Scene::initInstructions() {
	bInstructions = false;
	texIns.loadFromFile("images/instructions.png", TEXTURE_PIXEL_FORMAT_RGB);
	instructions = Sprite::createSprite(glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT), glm::vec2(1.0, 1.0), &texIns, &texProgram);
}

void Scene::initCredits() {
	bCredits = false;
	texCred.loadFromFile("images/credits.png", TEXTURE_PIXEL_FORMAT_RGB);
	credits = Sprite::createSprite(glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT), glm::vec2(1.0, 1.0), &texCred, &texProgram);
}

void Scene::initGame() {
	initShaders();

	string sLevel = "levels/LEVEL" + to_string(level) + ".txt";
	map = TileMap::createTileMap(sLevel, glm::vec2(SCREEN_X, SCREEN_Y), texProgram);

	//player1
	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	player->setTileMap(map);

	//player2
	player2 = new Player2();
	player2->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	player2->setPosition(glm::vec2(INIT_PLAYER2_X_TILES * map->getTileSize(), INIT_PLAYER2_Y_TILES * map->getTileSize()));
	player2->setTileMap(map);

	//Box
	switch (level) {
		default:
			bBox = false;
	}

	if (bBox) {
		box = new Box();
		box->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		box->setPosition(glm::vec2((INIT_PLAYER_X_TILES - 4) * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
		box->setTileMap(map);
	}

	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
	currentTime = 0.0f;


	//Position goals
	posGoals1 = vector<glm::ivec2>(LEVEL_MAX);
	posGoals2 = vector<glm::ivec2>(LEVEL_MAX);

	posGoals1[0] = glm::ivec2(map->getTileSize() * 27, map->getTileSize() * 5);
	posGoals2[0] = glm::ivec2(map->getTileSize() * 27, SCREEN_HEIGHT-(map->getTileSize() * 8));

	posGoals1[1] = glm::ivec2(map->getTileSize() * 11, map->getTileSize() * 7);
	posGoals2[1] = glm::ivec2(map->getTileSize() * 16, SCREEN_HEIGHT - (map->getTileSize() * 11));

	posGoals1[2] = glm::ivec2(map->getTileSize() * 27, map->getTileSize() * 8);
	posGoals2[2] = glm::ivec2(map->getTileSize() * 27, SCREEN_HEIGHT - (map->getTileSize() * 11));

	posGoals1[3] = glm::ivec2(map->getTileSize() * 20, map->getTileSize() * 4);
	posGoals2[3] = glm::ivec2(map->getTileSize() * 17, SCREEN_HEIGHT - (map->getTileSize() * 11));

	posGoals1[4] = glm::ivec2(map->getTileSize() * 8, map->getTileSize() * 8);
	posGoals2[4] = glm::ivec2(map->getTileSize() * 23, SCREEN_HEIGHT - (map->getTileSize() * 11));

	//flags
	flag = new GameObj();
	flag->init_flag(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	flag->setPosition(glm::vec2(posGoals1[level-1].x + 32, posGoals1[level-1].y + 32));

	flag_reverse = new GameObj();
	flag_reverse->init_flag_reverse(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	flag_reverse->setPosition(glm::vec2(posGoals2[level - 1].x + 32, posGoals2[level - 1].y+16));
}

void Scene::update(int deltaTime)
{
	currentTime += deltaTime;
	if (!bMenu) {
		player->update(deltaTime);
		player2->update(deltaTime);
		if(bBox) box->update(deltaTime);
	}
	if (bMenu) menu->update(deltaTime);
	if (bArrow) arrow->update(deltaTime);
	if (bCredits) credits->update(deltaTime);
	if (bInstructions) instructions->update(deltaTime);

	if (goal()) {
		if (++level <= LEVEL_MAX) initGame();
		else init();
	}
	
}

void Scene::render()
{
	glm::mat4 modelview;

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);

	map->render();

	if (!bMenu) {
		player->render();
		player2->render();
		flag->render();
		flag_reverse->render();
		if (bBox) box->render();
	}
	else menu->render();

	if(bArrow) arrow->render();
	if(bCredits) credits->render();
	if(bInstructions) instructions->render();
}

void Scene::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if(!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if(!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if(!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}



void Scene::showMenu()
{
	bMenu = true;
	bArrow = true;
	bInstructions = false;
	bCredits = false;
}

void Scene::showInstructions()
{
	bInstructions = true;
}

void Scene::showGame()
{
	bMenu = false;
	bArrow = false;
}

void Scene::showCredits()
{
	bCredits = true;
};

void Scene::playDeathSound() {
	PlaySound(TEXT("audio/oof.wav"), NULL, SND_FILENAME);
}

void Scene::setPosition1(const glm::vec2& pos)
{
	player->setPosition(pos);
}

void Scene::setPosition2(const glm::vec2& pos)
{
	player2->setPosition(pos);
}

glm::ivec2 Scene::getPosPlayer1()
{
	return player->getPosition();
}

glm::ivec2 Scene::getPosPlayer2()
{
	return player2->getPosition();
}


bool Scene::goal() {
	return aprox(getPosPlayer1(), posGoals1[level-1]) && aprox(getPosPlayer2(), posGoals2[level-1]);
}

bool Scene::aprox(glm::ivec2 posPlayer, glm::ivec2 posGoal) {
	bool h = (posPlayer.x - posGoal.x) >= 0 && (posPlayer.x - posGoal.x) < map->getTileSize();
	bool v = (posPlayer.y - posGoal.y) >= 0 && (posPlayer.y - posGoal.y) < map->getTileSize();
	return h && v;
}