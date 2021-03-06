#include "SpriteSheet.h"
#include "../globals/consts.h"

bool SpriteSheet::init;
SDL_Texture* SpriteSheet::sprite;
SDL_Texture* SpriteSheet::fontSprite;
SDL_Texture* SpriteSheet::playerSprite;
SDL_Texture* SpriteSheet::platformSprite;
SDL_Texture* SpriteSheet::projectileSprite;

UVTranslator SpriteSheet::translator;



SpriteSheet::SpriteSheet(void)
{	
}

SpriteSheet::~SpriteSheet(void)
{	
}

void SpriteSheet::FillUV(float* uv_, SPRITE_TYPE st_)
{
	//get row and column based on st_
	int row = static_cast<int>(st_) / 50;
	int col = static_cast<int>(st_) % 50;

	//translate to a float*
	translator.GetUV(uv_, row, col);
}

void SpriteSheet::Init()
{
	if ( !init ) 
	{
		translator = UVTranslator(800, 1280, 16, 16);

		sprite = CreateSprite("./resources/images/simples_pimples2.png", TILE_S * FileSettings::GetFloat("SCALE_W"), TILE_S * FileSettings::GetFloat("SCALE_H"), 0);
		fontSprite = CreateSprite("./resources/images/courier.png", 16 * FileSettings::GetFloat("SCALE_W"), 16 * FileSettings::GetFloat("SCALE_H"), 0);
		playerSprite = CreateSprite("./resources/images/player.png", PLAYER_S * FileSettings::GetFloat("SCALE_W"), PLAYER_S * FileSettings::GetFloat("SCALE_H"), 0);
		//platformSprite = CreateSprite("./resources/images/platformTiles.png", TILE_S, TILE_S, 0);
		platformSprite = CreateSprite("./resources/images/platforms.png", TILE_S * FileSettings::GetFloat("SCALE_W"), TILE_S * FileSettings::GetFloat("SCALE_H"), 0);
		projectileSprite = CreateSprite("./resources/images/projectiles.png", PROJECTILE_SZ * FileSettings::GetFloat("SCALE_W"), PROJECTILE_SZ * FileSettings::GetFloat("SCALE_H"), 0);
		
		init = true;
	}
}

SDL_Texture* SpriteSheet::Sprite()
{
	if ( !init ) 
		return nullptr;
	return sprite;
}

SDL_Texture* SpriteSheet::FontSprite()
{
	if ( !init ) 
		return nullptr;
	return fontSprite;
}

SDL_Texture* SpriteSheet::ProjectileSprite()
{
	if (!init)
		return nullptr;
	return projectileSprite;
}

SDL_Texture* SpriteSheet::PlayerSprite()
{
	if ( !init ) 
		return nullptr;
	return playerSprite;
}

SDL_Texture* SpriteSheet::PlatformSprite()
{
	if ( !init ) 
		return nullptr;
	return platformSprite;
}