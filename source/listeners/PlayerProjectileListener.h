//PlayerProjectileListener.h

#pragma once

#include "../GLAH/Vector.h"

class PlayerProjectileListener
{
public:
	virtual void PlayerProjectileFired(Vector2 pos_, Vector2 velocity_, float gravity_, int playerID_) = 0;

	//CannonListener();
	//virtual ~CannonListener(); 

private:
	

};