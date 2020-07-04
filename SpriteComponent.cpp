#include "SpriteComponent.h"
#include "Actor.h"
#include "Game.h"

SpriteComponent::SpriteComponent(Actor* owner, int drawOrder)
	:Component(owner)
	,texture(nullptr)
	,drawOrder(drawOrder)
	,texWidth(0)
	,texHeight(0)
{
	pOwner->GetGame()->AddSprite(this);
}

SpriteComponent::~SpriteComponent()
{
	pOwner->GetGame()->RemoveSprite(this);
}

void SpriteComponent::Draw(SDL_Renderer* renderer)
{
	if (texture)
	{
		SDL_Rect r;
		r.w = static_cast<int>(texWidth * pOwner->GetScale());
		r.h = static_cast<int>(texHeight * pOwner->GetScale());
		r.x = static_cast<int>(pOwner->GetPosition().x - r.w / 2);
		r.y = static_cast<int>(pOwner->GetPosition().y - r.h / 2);

		SDL_RenderCopyEx(renderer, texture, nullptr, &r, -Math::ToDegrees(pOwner->GetRotation()), nullptr, SDL_FLIP_NONE);
	}
}

void SpriteComponent::SetTexture(SDL_Texture* texture)
{
	texture = texture;
	SDL_QueryTexture(texture, nullptr, nullptr, &texWidth, &texHeight);
}
