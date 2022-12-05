// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Now Start From this state I will change Everything
// ----------------------------------------------------------------

#include "Game.h"

int main(int argc, char** argv)
{
	Game game;

	// Initialize
	bool success = game.Initialize();
	if (success)
	{
		printf("Start Game\n");
		game.RunLoop();
	}
	game.Shutdown();
	return 0;
}
