#include "Player.h"
#include "AnimSpriteComponent.h"
#include "Game.h"

Player::Player(Game* game) : Actor(game), rightSpeed(0.0f), downSpeed(0.0f)
{
	AnimSpriteComponent* asc = new AnimSpriteComponent(this);
	std::vector<SDL_Texture*> anims = {
		game->GetTexture("Assets/player1.png"),
		game->GetTexture("Assets/player2.png"),
		game->GetTexture("Assets/player3.png"),
		game->GetTexture("Assets/player4.png")
	};
	asc->SetAnimTextures(anims);
}

void Player::UpdateActor(float deltaTime)
{
	Actor::UpdateActor(deltaTime);
	Vector2 pos = GetPosition();
	pos.x += rightSpeed * deltaTime;
	pos.y += downSpeed * deltaTime;

	if (pos.x < 25.0f)
	{
		pos.x = 25.0f;
	}
	else if (pos.x > 500.0f)
	{
		pos.x = 500.0f;
	}
	if (pos.y < 25.0f)
	{
		pos.y = 25.0f;
	}
	else if (pos.y > 743.0f)
	{
		pos.y = 743.0f;
	}
	SetPosition(pos);
}

void Player::ProcessKeyboard(const uint8_t* state)
{
	rightSpeed = 0.0f;
	downSpeed = 0.0f;

	if (state[SDL_SCANCODE_D])
	{
		rightSpeed += 250.0f;
	}
	if (state[SDL_SCANCODE_A])
	{
		rightSpeed -= 250.0f;
	}

	if (state[SDL_SCANCODE_S])
	{
		downSpeed += 300.0f;
	}
	if (state[SDL_SCANCODE_W])
	{
		downSpeed -= 300.0f;
	}
}
