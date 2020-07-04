#pragma once
#include "Component.h"
#include "SDL.h"

class SpriteComponent : public Component
{
public:
	SpriteComponent(class Actor* owner, int drawOrder = 100);
	~SpriteComponent();

	virtual void Draw(SDL_Renderer* renderer);
	virtual void SetTexture(SDL_Texture* texture);

	int GetDrawOrder() const { return drawOrder; }
	int GetTexHeight() const { return texHeight; }
	int GetTexWidth() const { return texWidth; }
protected:
	SDL_Texture* pTexture;
	int drawOrder;
	int texWidth;
	int texHeight;
};