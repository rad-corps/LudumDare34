#include "Goal.h"
#include "../GLAH/GLAHGraphics.h"
#include "../spritesheet/SpriteSheet.h"
#include "../spritesheet/UVTranslator.h"
#include "../globals/consts.h"


Goal::Goal(void)
{
	active = true;

	UVTranslator trans(TERR_SPRITESHEET_W,TERR_SPRITESHEET_H,TERR_TILE_S,TERR_TILE_S);
	//translator.GetUV(UVs,51,38);
	trans.GetUV(UVs,10,0);

	width = TILE_S;
	height = TILE_S;
}


Goal::~Goal(void)
{
}

void
Goal::Update(float delta_)
{
	
}

void Goal::Draw()
{
	SetSpriteUVCoordinates	( SpriteSheet::Sprite(), UVs);
	MoveSprite(SpriteSheet::Sprite(), pos.x, pos.y);
	DrawSprite(SpriteSheet::Sprite());
}