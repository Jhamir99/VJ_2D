#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"


#define SCREEN_X 32
#define SCREEN_Y 16

#define INIT_PLAYER_X_TILES 4
#define INIT_PLAYER_Y_TILES 25


Scene::Scene()
{
	map = NULL;
	player = NULL;
}

Scene::~Scene()
{
	if(map != NULL)
		delete map;
	if(player != NULL)
		delete player;
}


void Scene::init()
{
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
	map = TileMap::createTileMap("levels/level01.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	player->setTileMap(map);
	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
	currentTime = 0.0f;
}

void Scene::update(int deltaTime)
{
	currentTime += deltaTime;
	player->update(deltaTime);
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
	player->render();
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




