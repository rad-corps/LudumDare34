#pragma once

#include "../math/Rect.h"
#include "../GLAH/GLAHGraphics.h"
#include "../GLAH/Vector.h"
#include "PlayerProjectile.h"
#include <vector>

class Platform
{

public:
	Platform(int col_, int row_, int tileType_);
	~Platform(void);

	void Draw(float alpha_ = 1.0f);
	void Update(float delta_, std::vector<PlayerProjectile>& playerProjectiles_);

	void Fall();
	bool Active();

	void IncrementTileType();
	void DecrementTileType();
	void SetTileset(int);
	int TileType();

	int Row();
	int Col();

	float* UV();

	int x, y, w, h;

	SDL_Rect* Collider();

private:
	void InitialiseGraphic();
	float uv[4];
	int scaleX, scaleY;
	unsigned int sprite;
	int tileType;
	int row, col;
	bool isFalling;
	bool active;

	SDL_Rect* collider;

//	GLAHGraphics * gfx;
};

