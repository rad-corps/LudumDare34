//PlayerProjectile

#include "GameObject.h"
#include <vector>
#include "Platform.h"

#pragma once

class PlayerProjectile : public GameObject
{

public:
	PlayerProjectile();
	~PlayerProjectile();	

	void Shoot(Vector2 pos_, Vector2 direction_, int playerID_);
	virtual void Draw();
	void Update(float delta_, std::vector<Platform> terrain_);
	int PlayerID();


private:
	
	Vector2 velocity;
	//float* sprite;
	float uv[4];
	static bool init;
	SDL_Rect collider;
	int playerID;
	bool collidedWithPlatform;
};