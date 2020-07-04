#include "AnimSpriteComponent.h"

AnimSpriteComponent::AnimSpriteComponent(Actor* owner, int drawOrder): SpriteComponent(owner, drawOrder)
	, currFrame(0.0f)
	, animFPS(24.0f)
{
}

void AnimSpriteComponent::Update(float deltaTime)
{
	SpriteComponent::Update(deltaTime);

	if (animTextures.size() > 0)
	{
		currFrame += animFPS * deltaTime;

		while (currFrame >= animTextures.size())
		{
			currFrame -= animTextures.size();
		}

		SetTexture(animTextures[static_cast<int>(currFrame)]);
	}
}

void AnimSpriteComponent::SetAnimTextures(const std::vector<SDL_Texture*>& textures)
{
	animTextures = textures;
	if (animTextures.size() > 0)
	{
		currFrame = 0.0f;
		SetTexture(animTextures[0]);
	}
}
