#pragma once
#include "SDL.h"
#include <unordered_map>
#include <string>
#include <vector>

class Game
{
public:
	Game();
	bool Initialize() noexcept;
	void RunLoop() noexcept;
	void Shutdown() noexcept;

	void AddActor(class Actor* actor);
	void RemoveActor(class Actor* actor);

	void AddSprite(class SpriteComponent* sprite);
	void RemoveSprite(class SpriteComponent* sprite);

	SDL_Texture* GetTexture(const std::string& fileName);
private:
	void ProcessInput() noexcept;
	void UpdateGame() noexcept;
	void GenerateOutput() noexcept;
	void LoadData();
	void UnloadData();

	// 뒤의 맵을 로딩하기
	std::unordered_map<std::string, SDL_Texture*> textures;
	std::vector<class Actor*> actors;
	std::vector<class Actor*> pendingActors;
	std::vector<class SpriteComponent*> sprites;

	SDL_Window* pWindow;
	SDL_Renderer* renderer;
	Uint32 ticksCount;
	bool isRunning;
	bool updatingActors;

	class Player* pPlayer;
};
