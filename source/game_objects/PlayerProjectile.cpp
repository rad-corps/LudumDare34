//PlayerProjectile

#include "../spritesheet/UVTranslator.h"
#include "../spritesheet/SpriteSheet.h"
#include "PlayerProjectile.h"
#include <iostream>
#include "../globals/consts.h"
#include "../math/Collision.h"
#include "../GLAH/GLAHSound.h"

//float PlayerProjectile::uv[4];
bool PlayerProjectile::init = false;

using namespace std;

PlayerProjectile::PlayerProjectile()
{
	active = false;

	if ( !init ) 
	{
		UVTranslator trans(64,16,16,16);
		trans.GetUV(uv, 0, 0);
		init = true;
	}

	width = PROJECTILE_SZ * FileSettings::GetFloat("SCALE_W");
	height = PROJECTILE_SZ * FileSettings::GetFloat("SCALE_H");;
}

PlayerProjectile::~PlayerProjectile()
{
	cout << "Projectile Created" << endl; 
}

void PlayerProjectile::Shoot(Vector2 pos_, Vector2 direction_, float gravity_, int playerID_)
{
//	timer = 0.0f;
	gravity = gravity_;
	active = true;
	pos = pos_;
	
	velocity = direction_ * 2000;

	UVTranslator trans(64, 16, 16, 16);
	trans.GetUV(uv, 0, playerID_);
	playerID = playerID_;
	collidedWithPlatform = false;
}

int PlayerProjectile::PlayerID()
{
	return playerID;
}


void
PlayerProjectile::Draw()
{
	if ( active )
	{
		SetSpriteUVCoordinates(SpriteSheet::ProjectileSprite(), uv);
		MoveSprite(SpriteSheet::ProjectileSprite(), pos.x, pos.y);
		DrawSprite(SpriteSheet::ProjectileSprite());

#ifdef SHOW_COLLIDERS
		DrawRect(GetRect());
#endif

	}
}

void
PlayerProjectile::Update(float delta_, std::vector<Platform> platforms_)
{
	if (active && !collidedWithPlatform)
	{
		//apply gravity
		Vector2 gravityVec(0, gravity);
		velocity += gravityVec;

		pos += velocity * delta_;

		//if it no longer collides with the screen, deactivate
		if (!Collision::RectCollision(GetScreenRect(), GetRect()))
		{
			cout << "PlayerProjectile left screen" << endl;
			active = false;
		}

		//if the projectile collides with a platform
		for (auto &platform : platforms_)
		{
			if (platform.HasCollider() && platform.TileType() != 0)
			{
				if (Collision::RectCollision(*platform.Collider(), GetRect()))
				{
					collidedWithPlatform = true;
				}
			}

			//only check these tiletypes when velocity.y > 0
			if (platform.HasCollider() && platform.TileType() == 0)
			{
				if (velocity.y > 0.0f)
				{
					if (Collision::RectCollision(*platform.Collider(), GetRect()))
					{
						collidedWithPlatform = true;
						GLAHSound::PlaySoundProjectileLand();
					}
				}
			}
		}
	}
}