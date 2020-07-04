#include "BGSpriteComponent.h"
#include "Actor.h"

BGSpriteComponent::BGSpriteComponent(Actor* owner, int drawOrder)
	:SpriteComponent(owner, drawOrder)
	, scrollSpeed(0.0f)
{}

void BGSpriteComponent::Update(float deltaTime)
{
	SpriteComponent::Update(deltaTime);
	for (auto& bg : BGTextures)
	{
		bg.offset.x += scrollSpeed * deltaTime;
		if (bg.offset.x < -screenSize.x)
		{
			bg.offset.x = (BGTextures.size() - 1) * screenSize.x - 1;
		}
	}
}

void BGSpriteComponent::Draw(SDL_Renderer* renderer)
{
	for (auto& bg : BGTextures)
	{
		SDL_Rect r;
		r.w = static_cast<int>(screenSize.x);
		r.h = static_cast<int>(screenSize.y);

		r.x = static_cast<int>(pOwner->GetPosition().x - r.w / 2 + bg.offset.x);
		r.y = static_cast<int>(pOwner->GetPosition().y - r.w / 2 + bg.offset.y);

		SDL_RenderCopy(renderer,
			bg.texture,
			nullptr,
			&r
		);
	}
}

void BGSpriteComponent::SetBGTextures(const std::vector<SDL_Texture*>& textures)
{
	int count = 0;
	for (auto tex : textures)
	{
		BGTexture temp;
		temp.texture = tex;
		temp.offset.x = count * screenSize.x;
		temp.offset.y = 0;
		BGTextures.emplace_back(temp);
		count++;
	}
}
