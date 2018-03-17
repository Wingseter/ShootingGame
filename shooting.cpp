#include "shooting.h"

Shooting::Shooting()
{
}

Shooting::~Shooting()
{
	releaseAll();
}

void Shooting::initialize(HWND hwnd)
{
	Game::initialize(hwnd);
	return;
}

void Shooting::update()
{
}

void Shooting::ai()
{
}

void Shooting::collisions()
{
}

void Shooting::render()
{
}

void Shooting::releaseAll()
{
	Game::releaseAll();
	return;
}

void Shooting::resetAll()
{
	Game::resetAll();
	return;
}
