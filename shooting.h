#ifndef _SHOOTING_H
#define _SHOOTING_H
#define WIN32_LEAN_AND_MEAN

#include "game.h"

class Shooting : public Game
{
private:
	//todo
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