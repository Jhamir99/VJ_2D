#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"
#include <Windows.h>
#include <MMSystem.h>



#define SCREEN_X 0
#define SCREEN_Y 10

#define INIT_PLAYER_X_TILES 18
#define INIT_PLAYER_Y_TILES 7

#define INIT_PLAYER2_X_TILES 18
#define INIT_PLAYER2_Y_TILES 14

#define DELAY 1000
#define LEVEL_MAX 6

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
	initShaders();
	initGame();
	initMenu();
	initInstructions();
	initCredits();
	initBackground();
	currentTime = 0.0f;
	special_time = 0.0f;
	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
}

void Scene::initMenu() {
	bMenu = true;
	bArrow = true;
	spritesheet.loadFromFile("images/menu.png", TEXTURE_PIXEL_FORMAT_RGBA);
	menu = Sprite::createSprite(glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT), glm::vec2(1.0, 1.0), &spritesheet, &texProgram);

	//texHand.loadFromFile("images/pointer.png", TEXTURE_PIXEL_FORMAT_RGBA);
	arrow = new GameObj();
	arrow->init_arrow(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	arrow->setPosition(glm::vec2(13 * map->getTileSize(), (11 * map->getTileSize())-68));
}

void Scene::initInstructions() {
	bInstructions = false;
	texIns.loadFromFile("images/instructions.jpg", TEXTURE_PIXEL_FORMAT_RGB);
	instructions = Sprite::createSprite(glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT), glm::vec2(1.0, 1.0), &texIns, &texProgram);
}

void Scene::initCredits() {
	bCredits = false;
	texCred.loadFromFile("images/credits.png", TEXTURE_PIXEL_FORMAT_RGB);
	credits = Sprite::createSprite(glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT), glm::vec2(1.0, 1.0), &texCred, &texProgram);
}

void Scene::initBackground() {
	texbackground.loadFromFile("images/background4.jpg", TEXTURE_PIXEL_FORMAT_RGB);
	background = Sprite::createSprite(glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT), glm::vec2(1.0, 1.0), &texbackground, &texProgram);
}

void Scene::initGame() {
	

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
		case 4:
			lever = new GameObj();
			lever->init_lever(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
			posLever = glm::vec2(16 * map->getTileSize(), 5 * map->getTileSize() - 14);
			lever->setPosition(posLever);
			blever = true;
			lever_trigger = false;
			box = new Box();
			box->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
			box->setPosition(glm::vec2(10 * map->getTileSize(), 14 * map->getTileSize()));
			box->setTileMap(map);
			bBox = true;
			break;
		case 6:
			box = new Box();
			box->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
			box->setPosition(glm::vec2(13 * map->getTileSize(), 8 * map->getTileSize()));
			box->setTileMap(map);
			bBox = true;

			//tp
			tp = new GameObj();
			tp->init_tp(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
			posTp = glm::vec2(10 * map->getTileSize(), 9 * map->getTileSize() + 4);
			tp->setPosition(posTp);

			tp2 = new GameObj();
			tp2->init_tp2(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
			tp2->setPosition(glm::vec2(15 * map->getTileSize(), 12 * map->getTileSize()-4));

			btp = true;
			tp_up = true;

			break;
		default:
			bBox = false;
			blever = false;
			lever_trigger = false;
			btp = false;
	}

	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
	currentTime = 0.0f;
	prev_time = 0.0f;


	//Position goals
	posGoals1 = vector<glm::ivec2>(LEVEL_MAX);
	posGoals2 = vector<glm::ivec2>(LEVEL_MAX);

	posGoals1[0] = glm::ivec2(map->getTileSize() * 27, map->getTileSize() * 5);
	posGoals2[0] = glm::ivec2(map->getTileSize() * 27, SCREEN_HEIGHT-(map->getTileSize() * 8));

	posGoals1[1] = glm::ivec2(map->getTileSize() * 11, map->getTileSize() * 7);
	posGoals2[1] = glm::ivec2(map->getTileSize() * 16, SCREEN_HEIGHT - (map->getTileSize() * 11));

	posGoals1[2] = glm::ivec2(map->getTileSize() * 27, map->getTileSize() * 8);
	posGoals2[2] = glm::ivec2(map->getTileSize() * 27, SCREEN_HEIGHT - (map->getTileSize() * 10));

	posGoals1[3] = glm::ivec2(map->getTileSize() * 20, map->getTileSize() * 4);
	posGoals2[3] = glm::ivec2(map->getTileSize() * 28, SCREEN_HEIGHT - (map->getTileSize() * 4));

	posGoals1[4] = glm::ivec2(map->getTileSize() * 8, map->getTileSize() * 8);
	posGoals2[4] = glm::ivec2(map->getTileSize() * 23, SCREEN_HEIGHT - (map->getTileSize() * 11));

	posGoals1[5] = glm::ivec2(map->getTileSize() * 31, map->getTileSize() * 6);
	posGoals2[5] = glm::ivec2(map->getTileSize() * 31, SCREEN_HEIGHT - (map->getTileSize() * 9));


	posCactusD.push_back(glm::ivec2((map->getTileSize() * 23) + 24, (map->getTileSize() * 7) - 2));
	posCactusD.push_back(glm::ivec2((map->getTileSize() * 24) + 24, (map->getTileSize() * 7) - 2));
	posCactusD.push_back(glm::ivec2((map->getTileSize() * 25) + 24, (map->getTileSize() * 7) - 2));


	posCactus.push_back(glm::ivec2((map->getTileSize() * 19) + 24, (map->getTileSize() * 7) + 16));
	posCactus.push_back(glm::ivec2((map->getTileSize() * 20) + 24, (map->getTileSize() * 7) + 16));
	posCactus.push_back(glm::ivec2((map->getTileSize() * 21) + 24, (map->getTileSize() * 7) + 16));
	posCactus.push_back(glm::ivec2((map->getTileSize() * 24) + 24, (map->getTileSize() * 8) + 16));
	posCactus.push_back(glm::ivec2((map->getTileSize() * 25) + 24, (map->getTileSize() * 8) + 16));
	posCactus.push_back(glm::ivec2((map->getTileSize() * 12) + 24, (map->getTileSize() * 13) + 16));
	posCactus.push_back(glm::ivec2((map->getTileSize() * 13) + 24, (map->getTileSize() * 13) + 16));

	//flags
	flag = new GameObj();
	flag->init_flag(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	flag->setPosition(glm::vec2(posGoals1[level-1].x + 32, posGoals1[level-1].y +2));

	flag_reverse = new GameObj();
	flag_reverse->init_flag_reverse(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	flag_reverse->setPosition(glm::vec2(posGoals2[level - 1].x + 32, posGoals2[level - 1].y+16));

	//Cactus
	cactus = new GameObj();
	cactus->init_cactus(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);

	cactusD = new GameObj();
	cactusD->init_cactusD(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);

	
}

void Scene::update(int deltaTime)
{
	currentTime += deltaTime;
	if (wait) {
		player->update_win_animation(deltaTime);
		player2->update_win_animation(deltaTime);
		flag->update_animation(deltaTime);
		flag_reverse->update_animation(deltaTime);

		if (currentTime - prev_time > DELAY) {
			wait = false;
			if (++level <= LEVEL_MAX) initGame();
			else {
				init();
				showCredits();
			}
		}
	}
	else if (player_dead) {
		player->update_death_animation(deltaTime);
		player2->update_death_animation(deltaTime);

		if (currentTime - prev_time > DELAY/2) {
			PlaySound(TEXT("audio/song.wav"), NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
			player_dead = false;
			initGame();
		}
	}
	else {
		if (!bMenu) {
			player->update(deltaTime);
			player2->update(deltaTime);
			flag->update_animation(deltaTime);
			flag_reverse->update_animation(deltaTime);
			if (bBox) box->update(deltaTime);
			if (btp) {
				tp->update_animation(deltaTime);
				tp2->update_animation(deltaTime);
				if (tp_up && aprox(posTp, box->getPosition())) {
					tp->change_animation();
					tp2->change_animation();
					box->setPosition(glm::vec2(15 * map->getTileSize(), 13 * map->getTileSize()));
					prev_time = currentTime;
					tp_up = false;
				}
				else if (!tp_up && currentTime - prev_time > DELAY) {
					tp->change_animation();
					tp2->change_animation();
					}
			}
			if (blever && !lever_trigger) 
				if (aprox(posLever, getPosPlayer1())) {
					lever->change_animation();
					map->destroy_barrier(glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
					lever_trigger = true;
				}
					
			if (goal()) {
				wait = true;
				prev_time = currentTime;
				flag->change_animation();
				flag_reverse->change_animation();
			}

			//Touch cactus or fall to water
			if (player->getPosition().y > 360 || player2 -> getPosition().y < 360 || (!godmode && (touch_cactus(player->getPosition()) || touch_cactus(player2->getPosition())))) {
				prev_time = currentTime;
				playDeathSound();
				reset_player();
			}

			//Jump Level & godmode
			if (Game::instance().getKey('g') && currentTime - special_time > DELAY) {
				godmode = !godmode;
				player->setPosition(glm::vec2(player->getPosition().x, player->getPosition().y-3));
				special_time = currentTime;
			}
			else if (Game::instance().getKey('1') && currentTime - special_time > DELAY) {
				special_time = currentTime;
				level = 1;
				initGame();
			}
			else if (Game::instance().getKey('2') && currentTime - special_time > DELAY) {
				special_time = currentTime;
				level = 2;
				initGame();
			}
			else if (Game::instance().getKey('3') && currentTime - special_time > DELAY) {
				special_time = currentTime;
				level = 3;
				initGame();
			}
			else if (Game::instance().getKey('4') && currentTime - special_time > DELAY) {
				special_time = currentTime;
				level = 4;
				initGame();
			}
			else if (Game::instance().getKey('5') && currentTime - special_time > DELAY) {
				special_time = currentTime;
				level = 5;
				initGame();
			}
			else if (Game::instance().getKey('6') && currentTime - special_time > DELAY) {
				special_time = currentTime;
				level = 6;
				initGame();
			}
			else if (Game::instance().getKey('b') && currentTime - special_time > DELAY) {
				special_time = currentTime;
				map->destroy_barrier(glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
			}

		}
		if (bMenu) menu->update(deltaTime);
		if (bArrow) arrow->update(deltaTime);
		if (bCredits) credits->update(deltaTime);
		if (bInstructions) instructions->update(deltaTime);

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

	

	if (!bMenu) {
		background->render();
		map->render();
		player->render();
		player2->render();
		flag->render();

		flag_reverse->render();
		if (bBox) box->render();
		if (btp) {
			tp->render();
			tp2->render();
		}

		switch (level) {
		case 3:
			for (glm::ivec2 pc : posCactusD) {
				cactusD->setPosition(pc);
				cactusD->render();
			}
			break;

		case 5:
			for (glm::ivec2 pc : posCactus) {
				cactus->setPosition(pc);
				cactus->render();
			}
			break;
		default:
			;
		}
		if (blever) 
			lever->render();
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

void Scene::initGameShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/textureG.vert");
	if (!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/textureG.frag");
	if (!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if (!texProgram.isLinked())
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
	initGameShaders();
	bMenu = false;
	bArrow = false;
}

void Scene::showCredits()
{
	bCredits = true;
};

void Scene::playDeathSound() {
	PlaySound(TEXT("audio/oof.wav"), NULL, SND_ASYNC | SND_FILENAME);
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

void Scene::enable_doubleJump(int pj) {
	if (pj == 1)
		player->enable_doubleJump();
	else
		player2->enable_doubleJump();
}

bool Scene::goal() {
	return aprox(getPosPlayer1(), posGoals1[level-1]) && aprox(getPosPlayer2(), posGoals2[level-1]);
}

bool Scene::aprox(glm::ivec2 posPlayer, glm::ivec2 posGoal) {
	bool h = (posPlayer.x - posGoal.x) >= 0 && (posPlayer.x - posGoal.x) < map->getTileSize();
	bool v = (posPlayer.y - posGoal.y) >= 0 && (posPlayer.y - posGoal.y) < map->getTileSize();
	return h && v;
}

bool Scene::touch_cactus(glm::ivec2 posPlayer) {
	bool h = false, v = false;

	switch (level) {
		case 3:
			for (glm::ivec2 pc : posCactusD) {
				h = v = false;
				h = (posPlayer.x + 32) - pc.x > 0 && posPlayer.x - (pc.x+48) < 0;
				v = (posPlayer.y) - (pc.y + 24) < 0;
				if (h && v) break;
			}
			break;

		case 5:
			for (glm::ivec2 pc : posCactus) {
				h = v = false;
				h = (posPlayer.x + 32) - pc.x > 0 && posPlayer.x - (pc.x + 48) < 0;
				v = (posPlayer.y + 32) - (pc.y + 16) > 0 && (posPlayer.y + 32) - (pc.y + 16) < 32;
				if (h && v) break;
			}
			break;
	}

	return h && v;
}

void Scene::reset_player()
{
	player_dead = true;
}
