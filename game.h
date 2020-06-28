#pragma once
#include "SDL.h"

struct Vector2
{
	float x;
	float y;
};

class Game
{
public:
	Game();
	bool Initialize() noexcept;
	void RunLoop() noexcept;
	void Shutdown() noexcept;
private:
	void ProcessInput() noexcept;
	void UpdateGame() noexcept;
	void GenerateOutput() noexcept;

	SDL_Window* wnd;
	SDL_Renderer* renderer;
	Uint32 ticksCount;
	bool isRunning;

	int paddleDir;
	Vector2 paddlePos;
	Vector2 ballPos;
	Vector2 ballVel;
};