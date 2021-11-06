#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"


void Game::init()
{
	bPlay = true;
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	scene.init();
}

bool Game::update(int deltaTime)
{
	scene.update(deltaTime);
	
	return bPlay;
}

void Game::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	scene.render();
}

void Game::keyPressed(int key)
{
	if(key == 27) // Escape code
		bPlay = false;
	keys[key] = true;
}

void Game::keyReleased(int key)
{
	keys[key] = false;
}

void Game::specialKeyPressed(int key)
{
	specialKeys[key] = true;
}

void Game::specialKeyReleased(int key)
{
	specialKeys[key] = false;
}

void Game::mouseMove(int x, int y)
{
}

void Game::mousePress(int button)
{
}

void Game::mouseRelease(int button)
{
}

bool Game::getKey(int key) const
{
	return keys[key];
}

bool Game::getSpecialKey(int key) const
{
	return specialKeys[key];
}

void Game::showMenu()
{
	scene.showMenu();
}

void Game::showInstructions()
{
	scene.showInstructions();
}

void Game::showGame()
{
	scene.showGame();
}

void Game::showCredits()
{
	scene.showCredits();
};

void Game::resetPlayer() {
	scene.initGame();
}

void Game::playDeathSound() {
	scene.playDeathSound();
}

glm::ivec2 Game::getPosPlayer1()
{
	return scene.getPosPlayer1();
}

glm::ivec2 Game::getPosPlayer2()
{
	return scene.getPosPlayer2();
}

void Game::setPosition1(const glm::vec2& pos)
{
	scene.setPosition1(pos);
}
void Game::setPosition2(const glm::vec2& pos)
{
	scene.setPosition2(pos);
}

void Game::enable_doubleJump(int pj) {
	scene.enable_doubleJump(pj);
}
