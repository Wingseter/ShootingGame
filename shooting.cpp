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

	// miku texture
	if (!mikuTexture.initialize(graphics, MIKU_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing texture"));


	// sky
	if (!sky.initialize(graphics, 0, 0, 0, &skyTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing imgae"));
	sky.setScale(0.8f);

	// cd															  
	if (!cd.initialize(graphics, 0, 0, 0, &cdTexture))				  
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing imgae"));

	cd.setX(GAME_WIDTH*0.5f - cd.getWidth()*0.5f / 2);
	cd.setY(GAME_HEIGHT*0.5f - cd.getHeight()*0.5f / 2);
	cd.setScale(0.5f);

	// miku
	if (!miku.initialize(graphics, MIKU_WIDTH, MIKU_HEIGHT, MIKU_COLS, &mikuTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing MIKU"));
	miku.setX(GAME_WIDTH / 4);                    // start above and left of planet
	miku.setY(GAME_HEIGHT / 4);
	miku.setFrames(MIKU_START_FRAME, MIKU_END_FRAME);   // animation frames
	miku.setCurrentFrame(MIKU_START_FRAME);     // starting frame
	miku.setFrameDelay(MIKU_ANIMATION_DELAY);
	miku.setDegrees(45.0f);                     // angle of ship
	return;
}

void Shooting::update()
{
	miku.update(frameTime);
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
	miku.draw();

	graphics->spriteEnd();
}

void Shooting::releaseAll()
{
	mikuTexture.onLostDevice();
	skyTexture.onLostDevice();
	cdTexture.onLostDevice();

	Game::releaseAll();
	return;
}

void Shooting::resetAll()
{
	mikuTexture.onResetDevice();
	skyTexture.onResetDevice();
	cdTexture.onResetDevice();
	Game::resetAll();
	return;
}
