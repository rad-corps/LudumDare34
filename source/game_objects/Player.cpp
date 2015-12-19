#include "Player.h"
//#include "aie.h"
#include "../spritesheet/SpriteSheet.h"
#include "../spritesheet/UVTranslator.h"
#include "../math/Collision.h"
#include "../globals/consts.h"



Player::Player(int id_)
	: kills(0), alive(true), id(id_)
{
//	gfx = GLAHGraphics::Instance();
//	inpt = GLAHInput::Instance();
	AddGameControllerListener(this, id);
	playerNum = id;

	Reset();

	velocity = Vector2(0,0);
	status = PLAYER_STATUS::STATIONARY;

	//initialise animations
	UVTranslator translator(512, 512, 32, 32);
	translator.GetUV(animStationary, playerNum, 0);
	translator.GetUV(animMove1, playerNum, 1);
	translator.GetUV(animMove2, playerNum, 2);
	translator.GetUV(animMove3, playerNum, 3);
	translator.GetUV(animDead, playerNum, 4);
	
	currentAnimation = animStationary;

	animationTimer = 0.0f;
	faceLeft = false;
	onPlatform = false;

	playerSpeak.SetAlignment(TEXT_ALIGNMENT::ALIGN_RIGHT);

	//get the players attributes from DB (assume only 1 row)
	char * error = "";
	dm.Select("./resources/db/dontpanic.db", "player_attributes", "*", "", "", error);
	gravity = dm.GetValueFloat(0, "gravity");
	maxSpeed = dm.GetValueFloat(0, "top_speed");
	jumpForce = dm.GetValueFloat(0, "jump_force");
	accelleration = dm.GetValueFloat(0, "accelleration");	
	terminalVelocity = dm.GetValueFloat(0, "terminal_velocity");	
}

void Player::Reset()
{
	alive = true;
	pos = Vector2(200, 200);

	alive = true;

	////initialise colliders
	topCollider.w = COLLIDER_SZ;
	topCollider.h = COLLIDER_SZ;
	bottomCollider.w = COLLIDER_SZ;
	bottomCollider.h = COLLIDER_SZ;

	leftCollider.w = COLLIDER_SZ;
	leftCollider.h = COLLIDER_SZ;
	rightCollider.w = COLLIDER_SZ;
	rightCollider.h = COLLIDER_SZ;
	hitCollider.w = PLAYER_S * FileSettings::GetFloat("SCALE_W");
	hitCollider.h = PLAYER_S * FileSettings::GetFloat("SCALE_H");

	UpdateColliders();
}

Player::~Player(void)
{
}

void Player::InitListener(PlayerProjectileListener* playerProjectileListener_)
{
	playerProjectileListener = playerProjectileListener_;
}


//must be called whenever players position changes
void Player::UpdateColliders()
{
	//update colliders
	topCollider.x = pos.x + (PLAYER_S * FileSettings::GetFloat("SCALE_W"))* 0.5f;
	topCollider.y = pos.y;
	bottomCollider.x = pos.x + (PLAYER_S * FileSettings::GetFloat("SCALE_W")) * 0.5f;
	bottomCollider.y = pos.y + (PLAYER_S * FileSettings::GetFloat("SCALE_H")) + 3;
	leftCollider.x = pos.x;
	leftCollider.y = pos.y + (PLAYER_S * FileSettings::GetFloat("SCALE_H")) * 0.5f;
	rightCollider.x = pos.x + (PLAYER_S * FileSettings::GetFloat("SCALE_W"));
	rightCollider.y = pos.y + (PLAYER_S * FileSettings::GetFloat("SCALE_H")) * 0.5f;


	hitCollider.x = pos.x;
	hitCollider.y = pos.y;
}

void Player::HandleCollision(vector<Platform>& platform_, std::vector<PlayerProjectile>& projectiles_)
{
	//bool tempOnPlatform = false;
	onPlatform = false;
	//check collision	
	for ( auto &env : platform_ )
	{
		if ( env.Active() && env.Collider() != nullptr)
		{			
			//only check bottom collision with platform if we are falling. 			
			if ( velocity.y > 0.1f ) 
			{
				if ( Collision::RectCollision(bottomCollider, *env.Collider()))
				{
					status = RUNNING;
					onPlatform = true;
					velocity.y = 0;
					//push him back up to the top of the platform
					MoveTo(Vector2(pos.x, env.y - (PLAYER_S * FileSettings::GetFloat("SCALE_H"))));
				}
			}

			//check left and right collision on types 2 and 3
			if  ( env.TileType() == 2 || env.TileType() == 3 )
			{
				if ( Collision::RectCollision(leftCollider, *env.Collider()))
				{
					UndoX();
					pos.x += 1;
				}
				if ( Collision::RectCollision(rightCollider, *env.Collider()))
				{
					UndoX();
					pos.x -= 1;
				}

				if ( Collision::RectCollision(topCollider, *env.Collider()))
				{
					if  (velocity.y < 0)
						velocity.y = 0;
						//velocity.y = -velocity.y;
				}
			}

		}
	}

	for ( auto &projectile : projectiles_ )
	{
		if (projectile.IsActive())
		{
			if (Collision::RectCollision(hitCollider, projectile.GetRect()))
			{
				cout << "Projectile Collided with player " << playerNum << endl; 
				alive = false;
			}
		}
	}

	if ( !onPlatform )
	{
		status = JUMPING;
	}
}

void Player::UndoX()
{
	MoveTo(Vector2(prevX, pos.y));
}

void Player::UndoY()
{
	MoveTo(Vector2(pos.x, prevY));
}

void Player::ApplyGravity()
{
	//gravity only if jumping
	if ( status == JUMPING )
	{
		if ( jumpHeld )
		{
			Vector2 gravityVec(0, (gravity * 0.5f)); 
			velocity += gravityVec;	
		}
		else
		{
			Vector2 gravityVec(0, (gravity)); 
			velocity += gravityVec;	
		}

		//dont go above terminal velocity
		if ( velocity.y > terminalVelocity )
			velocity.y = terminalVelocity;
		
	}
}

void Player::GamePadButtonDown(SDL_GameControllerButton button_)
{
	cout << "GamePadButtonDown" << endl;
	buttonDown[button_] = true;
}

void Player::GamePadButtonUp(SDL_GameControllerButton button_)
{
	cout << "GamePadButtonUp" << endl;
	buttonDown[button_] = false;
}

void Player::HandleInput(float delta_)
{
	if ( buttonDown[ SDL_CONTROLLER_BUTTON_DPAD_LEFT ] )
	{
		faceLeft = true;
		pos.x -= maxSpeed * delta_;
		if ( onPlatform ) 
			status = RUNNING;
	}
	else if ( buttonDown[ SDL_CONTROLLER_BUTTON_DPAD_RIGHT ] )
	{
		faceLeft = false;
		pos.x += maxSpeed * delta_;
		
		if ( onPlatform ) 
			status = RUNNING;
	}
	else if ( onPlatform ) 
	{
		status = STATIONARY;
	}

	//only jump if not already jumping
	if ( buttonDown[ SDL_CONTROLLER_BUTTON_A ] && status != PLAYER_STATUS::JUMPING && !jumpHeld)
	{
		jumpHeld = true;
		status = JUMPING;	
		
		//will only happen for one frame
		velocity.y -= jumpForce;
	}

	if ( !buttonDown[ SDL_CONTROLLER_BUTTON_A ] )
	{
		jumpHeld = false;
	}

	if ( buttonDown[ SDL_CONTROLLER_BUTTON_X ] && shootHeld == false )
	{
		shootHeld = true;
		cout << "SPACE PRESSED" << endl;
		
		//TODO REVISIT IMPORTANT SCALE PROJECTILE POSITION
		if ( faceLeft )
			playerProjectileListener->PlayerProjectileFired(pos - Vector2(64, 0), Vector2(-0.2, 0));
		else
			playerProjectileListener->PlayerProjectileFired(pos + Vector2(64, 0), Vector2(0.2, 0));

	}
	else if (!buttonDown[ SDL_CONTROLLER_BUTTON_X ])
	{
		shootHeld = false;
	}
}

void Player::UpdateAnimation(float delta_)
{
	if ( !alive ) 
	{
		currentAnimation = animDead;
		return;
	}
	//pick the animation
	animationTimer += delta_;
	if ( animationTimer > 0.1f )
	{
		animationTimer = 0.0f;

		if  ( status == STATIONARY ) 
			currentAnimation = animStationary;
		else if ( status == RUNNING ) 
		{
			if ( currentAnimation == animStationary ) 
				currentAnimation = animMove1;
			else if ( currentAnimation == animMove1 ) 
				currentAnimation = animMove2;
			else if ( currentAnimation == animMove2 ) 
				currentAnimation = animMove3;
			else if ( currentAnimation == animMove3 ) 
				currentAnimation = animStationary;
		}
		else if ( status == JUMPING )
		{
			currentAnimation = animMove1;
		}

		FPS = 1 / delta_;		
	}

}

bool Player::Update(float delta_, vector<Platform>& platform_, std::vector<PlayerProjectile>& projectiles_ )
{
	prevX = pos.x;
	prevY = pos.y;
	
	if (alive)
		HandleInput(delta_);

	ApplyGravity();	
	ApplyVelocity(velocity);	
	playerSpeak.SetPos(pos);
	HandleCollision(platform_, projectiles_);
	UpdateAnimation(delta_);

	if (alive)
		return true;
	return false;
}

void Player::MoveTo(Vector2 pos_)
{
	pos = pos_;
	UpdateColliders();
}

int Player::ID()
{
	return id;
}

void Player::ApplyVelocity(Vector2 velocity_)
{
	pos += velocity;
	UpdateColliders();
}

std::string Player::Name()
{ 
	if (playerNum == 0)
	{
		return "Glen";
	}
	if (playerNum == 1)
	{
		return "John";
	}
	if (playerNum == 2)
	{
		return "Rex";
	}
	if (playerNum == 3)
	{
		return "Paul";
	}
	return "Invalid playerNum";
}

void Player::Draw()
{
	//set the UV
	SetSpriteUVCoordinates	( SpriteSheet::PlayerSprite(), currentAnimation);
	MoveSprite(SpriteSheet::PlayerSprite(), pos.x, pos.y);
	DrawSprite(SpriteSheet::PlayerSprite(), faceLeft, 1.0f, true);
	//DrawString(to_string(FPS).c_str(), 50, 500);

	//update the text
	stringstream str;
	str << Name() << kills;
	guiText.SetText(str.str());
	if ( playerNum == 0 )
	{		
		guiText.SetPos(Vector2(50, 25));
		guiText.SetAlignment(TEXT_ALIGNMENT::ALIGN_LEFT);
	}
	if ( playerNum == 1)
	{
		guiText.SetPos(Vector2(FileSettings::GetInt("SCREEN_W")-50, 25));
		guiText.SetAlignment(TEXT_ALIGNMENT::ALIGN_RIGHT);
	}
	if (playerNum == 2)
	{
		guiText.SetPos(Vector2(50, FileSettings::GetInt("SCREEN_H") - 50));
		guiText.SetAlignment(TEXT_ALIGNMENT::ALIGN_LEFT);
	}
	if (playerNum == 3)
	{
		guiText.SetPos(Vector2(FileSettings::GetInt("SCREEN_W"), FileSettings::GetInt("SCREEN_H") - 50));
		guiText.SetAlignment(TEXT_ALIGNMENT::ALIGN_RIGHT);
	}

	guiText.Draw();

#ifdef SHOW_COLLIDERS
	DrawRect(bottomCollider);
	DrawRect(leftCollider);
	DrawRect(rightCollider);
	DrawRect(topCollider);
	DrawRect(hitCollider);	
#endif

	playerSpeak.Draw();
}