#ifndef _SHOOTING_H
#define _SHOOTING_H
#define WIN32_LEAN_AND_MEAN

#include "game.h"
#include "textureManager.h"
#include "image.h"

class Shooting : public Game
{
private:
	TextureManager skyTexture;
	TextureManager cdTexture;
	Image cd;
	Image sky;

public:
	Shooting();

	virtual ~Shooting();

	void initialize(HWND hwnd);
	void update();
	void ai();
	void collisions();
	void render();
	void releaseAll();
	void resetAll();
};

#endif