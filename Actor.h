#pragma once
#include <vector>
#include "Math.h"
class Actor
{
public:
	enum State
	{
		EActive,
		EPaused,
		EDead
	};

	Actor(class Game* game);
	virtual ~Actor();

	void Update(float deltaTime);
	void UpdateComponents(float deltaTime);
	virtual void UpdateActor(float deltaTime);

	const Vector2& GetPosition() const { return position; }
	void SetPosition(const Vector2& pos) { position = pos; }
	float GetScale() const { return scale; }
	void SetScale(float scale) const { scale = scale; }
	float GetRotation() const { return rotation; }
	void SetRotation(float rotation) { rotation = rotation; }

	State GetState() const { return state; }
	void SetState(State state) { state = state; }

	class Game* GetGame() { return game; }

	void AddComponent(class Component* component);
	void RemoveComponent(class Component* component);
	
private:
	State state;
	Vector2 position;
	float scale;
	float rotation;

	std::vector<class Component*> components;
	class Game* game;
};