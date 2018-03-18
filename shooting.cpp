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

	// sky texture
	if (!skyTexture.initialize(graphics, SKY_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing texture"));

	// cd texture
	if (!cdTexture.initialize(graphics, CD_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing texture"));

	// sky
	if (!sky.initialize(graphics, 0, 0, 0, &skyTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing imgae"));
																	  
	// cd															  
	if (!cd.initialize(graphics, 0, 0, 0, &cdTexture))				  
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing imgae"));

	
	cd.setX(GAME_WIDTH*0.5f - cd.getWidth()*0.5f);
	cd.setY(GAME_HEIGHT*0.5f - cd.getHeight()*0.5f);
	cd.setScale(0.5f);
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
	graphics->spriteBegin();
	sky.draw();
	cd.draw();

	graphics->spriteEnd();
}

void Shooting::releaseAll()
{
	skyTexture.onLostDevice();
	cdTexture.onLostDevice();

	Game::releaseAll();
	return;
}

void Shooting::resetAll()
{
	skyTexture.onResetDevice();
	cdTexture.onResetDevice();
	Game::resetAll();
	return;
}
