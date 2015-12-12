#include "EnemySpawner.h"
#include "../spritesheet/UVTranslator.h"
#include "../spritesheet/SpriteSheet.h"
#include "../globals/consts.h"

EnemySpawner::EnemySpawner(Enemy toClone_) : enemyMaster(toClone_)
{
	pos = toClone_.Pos();
	timer = 2.5f;

	UVTranslator trans(TERR_SPRITESHEET_W,TERR_SPRITESHEET_H,TERR_TILE_S,TERR_TILE_S);
	trans.GetUV(uv, 17,0);
}


EnemySpawner::~EnemySpawner(void)
{
}

void EnemySpawner::Update(float delta_, std::vector<Enemy>& enemyList_)
{
	timer += delta_;

	if ( timer > 5.f ) 
	{
		timer = 0.f;
		enemyList_.push_back(enemyMaster);
	}
}

void EnemySpawner::Draw()
{
	SetSpriteUVCoordinates(SpriteSheet::Sprite(),uv);	
	MoveSprite(SpriteSheet::Sprite(), pos.x, pos.y);
	DrawSprite(SpriteSheet::Sprite(), true);
}