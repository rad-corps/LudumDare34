//////////////////////////////////////////////////////////////////////////
// Monster Mayhem
// Game Code By Adam Hulbert
// For AIE Advanced Diploma - Practice Production Term 4 2014
// 22/12/2014
// Collision.cpp
/////////////////////////////////////////////////////////////////////////

#include "Collision.h"
#include <iostream>

using namespace std;

Collision::Collision(void)
{
}


Collision::~Collision(void)
{
}
/*
//circle collision between two gameobjects
bool Collision::CheckCollision(GameObject* obj1_, GameObject* obj2_)
{
	if ( !obj1_->IsActive() || !obj2_->IsActive() )
		return false;

	//calculate a slight reduction in radius to account for some collision overlap
	float obj1_radius = obj1_->Width() / 2;
	float obj2_radius = obj2_->Width() / 2;
	obj1_radius *= 0.7f;
	obj2_radius *= 0.7f;

	float distance = sqrt(pow(obj1_->Pos().x - obj2_->Pos().x, 2) + pow(obj1_->Pos().y - obj2_->Pos().y, 2));
	if ( distance < obj1_radius + obj2_radius )
		return true;
	return false;
}
*/

////check rectangle collision between two rectangles
//int Collision::RectCollision(Rect platform_, Rect player_)
//{
//	//platform_.Scale(0.95f);
//	//player_.Scale(0.95f);
//
//	int ret = (int)COLLISION_TYPE::NO_COL;
//
//	if (platform_.Right() < player_.Left() 
//		|| player_.Right() < platform_.Left() 
//		|| platform_.Bottom() > player_.Top() 
//		|| platform_.Top() < player_.Bottom())
//		
//		return ret;
//
//	
//
//	//http://stackoverflow.com/questions/5062833/detecting-the-direction-of-a-collision
//
//	float b_collision = player_.Bottom() - platform_.Top();
//	float t_collision = platform_.Bottom() - player_.Top();
//	float l_collision = platform_.Right() - player_.Left();
//	float r_collision = player_.Right() - platform_.Left();
//
//	if (t_collision < b_collision && t_collision < l_collision && t_collision < r_collision )
//	{                           
//		ret |= COLLISION_TYPE::TOP_COLL;
//	}
//	if (b_collision < t_collision && b_collision < l_collision && b_collision < r_collision)                        
//	{
//		ret |= COLLISION_TYPE::BOT_COLL;
//	}
//	if (l_collision < r_collision && l_collision < t_collision && l_collision < b_collision)
//	{
//		ret |= COLLISION_TYPE::LEFT_COL;
//	}
//	if (r_collision < l_collision && r_collision < t_collision && r_collision < b_collision )
//	{
//		ret != COLLISION_TYPE::RIGHT_COL;
//	}
//
//	return ret;
//}

int Collision::RectCollision(SDL_Rect platform_, SDL_Rect player_)
{
	//platform_.Scale(0.95f);
	//player_.Scale(0.95f);

	if (platform_.x + platform_.w < player_.x 
		|| player_.x + player_.w < platform_.x 
		|| platform_.y + platform_.h < player_.y 
		|| platform_.y > player_.y + player_.h)
		
		return 0;
	return 1;	
}