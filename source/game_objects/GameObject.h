//////////////////////////////////////////////////////////////////////////
// Monster Mayhem
// Game Code By Adam Hulbert
// For AIE Advanced Diploma - Practice Production Term 4 2014
// 22/12/2014
// GameObject.h
/////////////////////////////////////////////////////////////////////////

#pragma once

#include "../GLAH/Vector.h"
#include "../math/Rect.h"
#include "SDL2/SDL.h"

//trivial change

class GameObject
{
public:
	GameObject(void);
	
	//destructor must be virtual to ensure it is called
	virtual ~GameObject(void);

	//pure virtuals
	//virtual void Update(float delta_);
	virtual void Draw() = 0;

	float Width();
	float Height();

	bool IsActive();
	void SetActive(bool active_);
	Vector2 Pos();

	SDL_Rect GetRect();

	int Row();
	int Col();

	void SetPos(int col_, int row_);

protected:
	Vector2 pos;
	int width;
	int height;
	bool active;
};

