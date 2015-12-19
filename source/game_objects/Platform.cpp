#include "Platform.h"
//#include "aie.h"
#include "../spritesheet/SpriteSheet.h"
#include "../spritesheet/UVTranslator.h"
#include "../math/Collision.h"
#include "../globals/consts.h"

Platform::Platform(int col_, int row_, int tileType_) : tileType(tileType_), row(row_), col(col_), scaleX(1), scaleY(1)
{
	w = TILE_S * FileSettings::GetFloat("SCALE_W");
	h = TILE_S * FileSettings::GetFloat("SCALE_H");
	x = col_ * w;
	y = row_ * h;

	//dont set rect for tile type 1
	collider = nullptr;
	if ( tileType_ == 2 || tileType_ == 3 )
	{
		collider = new SDL_Rect();
		collider->w = w;
		collider->h = h;
		collider->x = x;
		collider->y = y;
	}
	if ( tileType_ == 0 )
	{
		collider = new SDL_Rect();
		collider->w = w;
		collider->h = 8; //TODO scale this down
		collider->x = x;
		collider->y = y;
	}
	
	InitialiseGraphic();
	isFalling = false;
	active = true;
}


Platform::~Platform(void)
{
}

float* Platform::UV()
{
	return uv;
}

bool Platform::Active()
{
	return active;
}

void Platform::InitialiseGraphic()
{
	UVTranslator trans(TERR_SPRITESHEET_W,TERR_SPRITESHEET_H,TERR_TILE_S,TERR_TILE_S);
	
	//get row and col based on tileType
	const int TERR_SPR_ROWS =  TERR_SPRITESHEET_H / TERR_TILE_S;
	const int TERR_SPR_COLS =  TERR_SPRITESHEET_W / TERR_TILE_S;

	int spriteSheetRow = tileType / TERR_SPR_ROWS;
	int spriteSheetCol = tileType % TERR_SPR_COLS;

	trans.GetUV(uv, spriteSheetRow, spriteSheetCol);
}

SDL_Rect* Platform::Collider()
{
	return collider;
}

void Platform::Fall()
{
	isFalling = true;
}

//scroll down
void Platform::IncrementTileType()
{
	++tileType;	
	InitialiseGraphic();
}

//scroll up
void Platform::DecrementTileType()
{
	--tileType;
	InitialiseGraphic();
}

void Platform::SetTileset(int tile_)
{
	tileType = tile_;
	InitialiseGraphic();
}

int Platform::TileType()
{
	return tileType;
}

int Platform::Row()
{
	return row;
}

int Platform::Col()
{
	return col;
}

void Platform::Draw(float alpha_)
{
	if ( !active ) 
		return;

	SetSpriteUVCoordinates(SpriteSheet::PlatformSprite(), uv);
	MoveSprite(SpriteSheet::PlatformSprite(), x, y);
	//ScaleSprite(SpriteSheet::Sprite(), scaleX, scaleY);
	DrawSprite(SpriteSheet::PlatformSprite(), false, alpha_, true);
	//ScaleSprite(SpriteSheet::Sprite(), 1, 1);
	
#ifdef SHOW_COLLIDERS	
	if ( collider != nullptr )
		DrawRect(*collider);
#endif
}

void Platform::Update(float delta_, std::vector<PlayerProjectile>& playerProjectiles_)
{
	//if ( !active ) 
	//	return;

	//if ( isFalling ) 
	//{
	//	centre.y -= delta_ * 100;
	//}

	//if ( tileType == ENVIRO_TILE::ROCK_BASE || tileType == ENVIRO_TILE::ROCK_SURFACE ) 
	//{
	//	for ( auto& shell : shells_ )
	//	{
	//		if ( Collision::RectCollision(shell.GetRect(), *this) ) 
	//		{
	//			active = false;
	//			shell.SetActive(false);
	//		}
	//	}
	//}
	//if ( tileType == ENVIRO_TILE::GREY_BRICK_BASE|| tileType == ENVIRO_TILE::GREY_BRICK_SURFACE) 
	//{
	//	for ( auto& shell : shells_ )
	//	{
	//		if ( Collision::RectCollision(shell.GetRect(), *this) ) 
	//		{
	//			shell.SetActive(false);
	//		}
	//	}
	//}
}
