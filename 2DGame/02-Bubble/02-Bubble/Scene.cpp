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
	if(!bMusic) PlaySound(TEXT("audio/song.wav"), NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
	bMusic = true;

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

	texHand.loadFromFile("images/pointer.png", TEXTURE_PIXEL_FORMAT_RGBA);
	arrow = new Arrow();
	arrow->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	arrow->setPosition(glm::vec2(10 * map->getTileSize(), (15 * map->getTileSize())-75));
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
	map = TileMap::createTileMap("levels/lvl1_v2.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	map = TileMap::createTileMap("levels/level03.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);

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
	box = new Box();
	box->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	box->setPosition(glm::vec2((INIT_PLAYER_X_TILES-4) * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	box->setTileMap(map);

	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
	currentTime = 0.0f;
}

void Scene::update(int deltaTime)
{
	currentTime += deltaTime;
	if (!bMenu) player->update(deltaTime);
	if (!bMenu) player2->update(deltaTime);
	if (!bMenu) box->update(deltaTime);
	if (bMenu) menu->update(deltaTime);
	if (bArrow) arrow->update(deltaTime);
	if (bCredits) credits->update(deltaTime);
	if (bInstructions) instructions->update(deltaTime);
	
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
	if(!bMenu) player->render();
	if (!bMenu) player2->render();
	if (!bMenu) box->render();
	if(bMenu) menu->render();
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
	PlaySound(TEXT("audio/song.wav"), NULL, SND_ASYNC | SND_FILENAME);
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

