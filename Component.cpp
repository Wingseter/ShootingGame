#include "Component.h"
#include "Actor.h"

Component::Component(Actor* owner, int updateOrder): pOwner(owner), updateOrder(updateOrder)
{
	pOwner->AddComponent(this);
}

Component::~Component()
{
	pOwner->RemoveComponent(this);
}

void Component::Update(float deltaTime)
{}
