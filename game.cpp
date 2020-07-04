#include "Game.h"
#include <algorithm>
#include "Actor.h"
#include "SpriteComponent.h"
#include "Player.h"
#include "BGSpriteComponent.h"
#include "SDL_image.h"

Game::Game()
	:pWindow(nullptr)
	,renderer(nullptr)
	,updatingActors(false)
	,isRunning(true)
{}

bool Game::Initialize() noexcept
{
	if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) != 0)
	{
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}

	pWindow = SDL_CreateWindow("Shooting Game", 100, 100, 1024, 768, 0);
	if (!pWindow)
	{
		SDL_Log("Failed to create window %s", SDL_GetError());
		return false;
	}

	renderer = SDL_CreateRenderer(pWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!renderer)
	{
		SDL_Log("Failed to create renderer : %s", SDL_GetError());
		return false;
	}
	
	if (IMG_Init(IMG_INIT_PNG) == 0)
	{
		SDL_Log("Unable to initialize SDl_Image: %s", SDL_GetError());
		return false;
	}

	LoadData();

	ticksCount = SDL_GetTicks();

	return true;
}

void Game::RunLoop() noexcept
{
	while (isRunning)
	{
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}

void Game::Shutdown() noexcept
{
	UnloadData();
	IMG_Quit();
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(pWindow);
	SDL_Quit();
}

void Game::AddActor(Actor* actor)
{
	if (updatingActors)
	{
		pendingActors.emplace_back(actor);
	}
	else
	{
		actors.emplace_back(actor);
	}
}

void Game::RemoveActor(Actor* actor)
{
	auto iter = std::find(pendingActors.begin(), pendingActors.end(), actor);
	if (iter != pendingActors.end())
	{
		std::iter_swap(iter, pendingActors.end() - 1);
		pendingActors.pop_back();
	}

	iter = std::find(actors.begin(), actors.end(), actor);
	if (iter != actors.end())
	{
		std::iter_swap(iter, actors.end() - 1);
		actors.pop_back();
	}
}

void Game::AddSprite(SpriteComponent* sprite)
{
	int myDrawOrder = sprite->GetDrawOrder();
	auto iter = sprites.begin();
	for (; iter != sprites.end(); ++iter)
	{
		if (myDrawOrder < (*iter)->GetDrawOrder())
		{
			break;
		}
	}

	sprites.insert(iter, sprite);
}

void Game::RemoveSprite(SpriteComponent* sprite)
{
	auto iter = std::find(sprites.begin(), sprites.end(), sprite);
	sprites.erase(iter);
}

SDL_Texture* Game::GetTexture(const std::string& fileName)
{
	SDL_Texture* tex = nullptr;

	auto iter = textures.find(fileName);
	if (iter != textures.end())
	{
		tex = iter->second;
	}
	else 
	{
		SDL_Surface* surf = IMG_Load(fileName.c_str());
		if (!surf)
		{
			SDL_Log("Failed to load texture file %s", fileName.c_str());
			return nullptr;
		}

		tex = SDL_CreateTextureFromSurface(renderer, surf);
		SDL_FreeSurface(surf);
		if (!tex)
		{
			SDL_Log("Failed to convert curface to texture for %s", fileName.c_str());
			return nullptr;
		}
		textures.emplace(fileName.c_str(), tex);
	}
	return tex;
}

void Game::ProcessInput() noexcept
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			isRunning = false;
			break;
		}
	}

	const Uint8* state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_ESCAPE])
	{
		isRunning = false;
	}

	pPlayer->ProcessKeyboard(state);
}

void Game::UpdateGame() noexcept
{
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), ticksCount + 16));
	
	float deltaTime = (SDL_GetTicks() - ticksCount) / 1000.0f;

	if (deltaTime > 0.05f)
	{
		deltaTime = 0.05f;
	}

	ticksCount = SDL_GetTicks();
	
	updatingActors = true;
	for (auto actor : actors)
	{
		actor->Update(deltaTime);
	}
	updatingActors = false;

	for (auto pending: pendingActors)
	{
		actors.emplace_back(pending);
	}
	pendingActors.clear();

	std::vector<Actor*> deadActors;
	for (auto actor : actors)
	{
		if (actor->GetState() == Actor::EDead)
		{
			deadActors.emplace_back(actor);
		}
	}

	for (auto actor : deadActors)
	{
		delete actor;
	}
}

void Game::GenerateOutput() noexcept
{
	SDL_SetRenderDrawColor(renderer,0,0,255,255);
	SDL_RenderClear(renderer);

	for (auto sprite : sprites)
	{
		sprite->Draw(renderer);
	}
	SDL_RenderPresent(renderer);
}

void Game::LoadData()
{
	pPlayer = new Player(this);
	pPlayer->SetPosition(Vector2(100.0f, 384.0f));
	pPlayer->SetScale(1.5f);

	Actor* temp = new Actor(this);
	temp->SetPosition(Vector2(512.0f, 384.0f));

	BGSpriteComponent* bg = new BGSpriteComponent(temp);
	bg->SetScreenSize(Vector2(1024.0f, 768.0f));
	std::vector<SDL_Texture*> bgtexs = {
		GetTexture("Assets/background1.png"),
		GetTexture("Assets/background2.png")
	};
	bg->SetBGTextures(bgtexs);
	bg->SetScrollSpeed(-100.0f);

	bg = new BGSpriteComponent(temp, 50);
	bg->SetScreenSize(Vector2(1024.0f, 768.0f));

	bgtexs = {
		GetTexture("Assets/Stars.png"),
		GetTexture("Assets/Stars.png")
	};
	bg->SetBGTextures(bgtexs);
	bg->SetScrollSpeed(-200.0f);
}

void Game::UnloadData()
{
	while (!actors.empty())
	{
		delete actors.back();
	}
	for (auto i : textures)
	{
		SDL_DestroyTexture(i.second);
	}
	textures.clear();
}
