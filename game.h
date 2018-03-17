#ifndef _GAME_H
#define _GAME_H
#define WIN32_LEAN_AND_MEAN

class Game;

#include <Windows.h>
#include <Mmsystem.h>
#include "graphics.h"
#include "input.h"
#include "constants.h"
#include "gameError.h"

class Game
{
protected:
	Graphics * graphics;
	Input *input;
	HWND hwnd;
	HRESULT hr;
	LARGE_INTEGER timeStart;
	LARGE_INTEGER timeEnd;
	LARGE_INTEGER timeFreq;
	float frameTime;
	float fps;
	DWORD sleepTime;
	bool paused;
	bool initialized;

public:
	Game();

	virtual ~Game();

	// 메시지 핸들러
	LRESULT messageHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	virtual void initialize(HWND hwnd);

	virtual void run(HWND);

	virtual void releaseAll();

	virtual void resetAll();

	virtual void deleteAll();

	virtual void renderGame();

	virtual void handleLostGraphicsDevice();

	Graphics * getGraphics() { return graphics; }

	Input* getInput() { return input; }

	void exitGame() { PostMessage(hwnd, WM_DESTROY, 0, 0); }

	virtual void update() = 0;
	
	virtual void ai() = 0;

	virtual void collisions() = 0;

	virtual void render() = 0;

};

#endif