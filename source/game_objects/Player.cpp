#include "Player.h"
//#include "aie.h"
#include "../spritesheet/SpriteSheet.h"
#include "../spritesheet/UVTranslator.h"
#include "../math/Collision.h"
#include "../globals/consts.h"



Player::Player(int id_)
	: kills(0), alive(true), id(id_), projectiles(3), canWallJumpLeft(false), canWallJumpRight(false)
{
	AddGameControllerListener(this, id);

	Reset();

	velocity = Vector2(0,0);
	status = PLAYER_STATUS::STATIONARY;

	//initialise animations
	UVTranslator translator(512, 512, 32, 32);
	translator.GetUV(animStationary, id, 0);
	translator.GetUV(animMove1, id, 1);
	translator.GetUV(animMove2, id, 2);
	translator.GetUV(animMove3, id, 3);
	translator.GetUV(animDead, id, 4);
	
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

void Player::SetPos(Vector2 pos_)
{
	pos = pos_;
}

void Player::Reset()
{
	alive = true;
	pos = Vector2(200, 200);

	alive = true;

	////initialise colliders
	topCollider.w = COLLIDER_SZ;
	topCollider.h = COLLIDER_SZ;
	bottomCollider.w = PLAYER_S * FileSettings::GetFloat("SCALE_W") * 0.50f;;
	bottomCollider.h = COLLIDER_SZ;

	leftCollider.w = COLLIDER_SZ;
	leftCollider.h = PLAYER_S * FileSettings::GetFloat("SCALE_H") * 0.50f;
	rightCollider.w = COLLIDER_SZ;
	rightCollider.h = PLAYER_S * FileSettings::GetFloat("SCALE_H") * 0.50f;
	hitCollider.w = PLAYER_S * FileSettings::GetFloat("SCALE_W");
	hitCollider.h = PLAYER_S * FileSettings::GetFloat("SCALE_H");

	UpdateColliders();

	projectiles = 3;
}

Player::~Player(void)
{
	RemoveGameControllerListener(this);
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
	bottomCollider.x = pos.x + (PLAYER_S * FileSettings::GetFloat("SCALE_W")) * 0.25f;
	bottomCollider.y = pos.y + (PLAYER_S * FileSettings::GetFloat("SCALE_H")) + 3;
	leftCollider.x = pos.x;
	leftCollider.y = pos.y + (PLAYER_S * FileSettings::GetFloat("SCALE_H") * 0.25f);
	rightCollider.x = pos.x + (PLAYER_S * FileSettings::GetFloat("SCALE_W")) - COLLIDER_SZ;
	rightCollider.y = pos.y + (PLAYER_S * FileSettings::GetFloat("SCALE_H") * 0.25f);

	hitCollider.x = pos.x;
	hitCollider.y = pos.y;
}

void Player::HandleCollision(vector<Platform>& platform_, std::vector<PlayerProjectile>& projectiles_)
{
	//bool tempOnPlatform = false;
	onPlatform = false;
	//check collision	

	canWallJumpRight = false;
	canWallJumpLeft = false;
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
					MoveTo(Vector2(pos.x, env.Y() - (PLAYER_S * FileSettings::GetFloat("SCALE_H"))));
				}
			}

			//check left and right collision on types 2 and 3
			if  ( env.TileType() == 2 || env.TileType() == 3 )
			{
				if ( Collision::RectCollision(leftCollider, *env.Collider()))
				{
					velocity.x = 0;
					MoveTo(Vector2(env.X() + (TILE_S * FileSettings::GetFloat("SCALE_W")), pos.y));
					canWallJumpRight = true;
				}
				else if ( Collision::RectCollision(rightCollider, *env.Collider()))
				{
					velocity.x = 0;
					MoveTo(Vector2(env.X() - (PLAYER_S * FileSettings::GetFloat("SCALE_W")), pos.y));
					canWallJumpLeft = true;
					//cout << "canWallJumpLeft: " << canWallJumpLeft << endl;
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
				if (projectile.PlayerID() == id)
				{
					projectiles++;					
					projectile.SetActive(false);
				}
				else
				{
					alive = false;
				}				
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
	//cout << "GamePadButtonDown" << endl;
	buttonDown[button_] = true;
}

void Player::GamePadButtonUp(SDL_GameControllerButton button_)
{
	//cout << "GamePadButtonUp" << endl;
	buttonDown[button_] = false;
}

void Player::HandleInput(float delta_)
{
	if (id == 0)
	{

	}

	if ( buttonDown[ SDL_CONTROLLER_BUTTON_DPAD_LEFT ] )
	{
		faceLeft = true;
		velocity.x -= accelleration;
		if ( onPlatform ) 
			status = RUNNING;
	}
	else if ( buttonDown[ SDL_CONTROLLER_BUTTON_DPAD_RIGHT ] )
	{
		faceLeft = false;
		velocity.x += accelleration;
		
		if ( onPlatform ) 
			status = RUNNING;
	}
	else if (onPlatform)
	{
		velocity.x = 0.0f;
	}
	else if (!onPlatform)//drag
	{
		velocity.x *= 0.8f;
	}

	if (velocity.x > maxSpeed )
	{
		velocity.x = maxSpeed;
	}
	if (velocity.x < -maxSpeed)
	{
		velocity.x = -maxSpeed;
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
	
	//walljumping
	if (buttonDown[SDL_CONTROLLER_BUTTON_A] && canWallJumpLeft && !jumpHeld)
	{
		jumpHeld = true;
		status = JUMPING;

		//will only happen for one frame
		velocity.y = -jumpForce;
		velocity.x = -maxSpeed;
		canWallJumpLeft = false;
	}
	if (buttonDown[SDL_CONTROLLER_BUTTON_A] && canWallJumpRight&& !jumpHeld)
	{
		jumpHeld = true;
		status = JUMPING;

		//will only happen for one frame
		velocity.y = -jumpForce;
		velocity.x = maxSpeed;
		canWallJumpRight = false;
	}

	if ( !buttonDown[ SDL_CONTROLLER_BUTTON_A ] )
	{
		jumpHeld = false;
	}

	if ( buttonDown[ SDL_CONTROLLER_BUTTON_X ] && shootHeld == false )
	{
		shootHeld = true;
		
		//TODO REVISIT IMPORTANT SCALE PROJECTILE POSITION
		if (projectiles > 0)
		{
			--projectiles;
			Vector2 projectilePos = pos;

			//aiming 
			if (buttonDown[SDL_CONTROLLER_BUTTON_DPAD_LEFT] && buttonDown[SDL_CONTROLLER_BUTTON_DPAD_UP])
			{
				projectilePos.x -= PROJECTILE_SZ * FileSettings::GetFloat("SCALE_W") + PROJECTILE_OFFSET;
				playerProjectileListener->PlayerProjectileFired(projectilePos, Vector2(-PROJECTILE_POWER * 0.7f, -PROJECTILE_POWER * 0.7f), id);
			}
			else if (buttonDown[SDL_CONTROLLER_BUTTON_DPAD_LEFT] && buttonDown[SDL_CONTROLLER_BUTTON_DPAD_DOWN])
			{
				projectilePos.x -= PROJECTILE_SZ * FileSettings::GetFloat("SCALE_W") + PROJECTILE_OFFSET;
				playerProjectileListener->PlayerProjectileFired(projectilePos, Vector2(-PROJECTILE_POWER * 0.7f, PROJECTILE_POWER * 0.7f), id);
			}
			else if (buttonDown[SDL_CONTROLLER_BUTTON_DPAD_RIGHT] && buttonDown[SDL_CONTROLLER_BUTTON_DPAD_UP])
			{
				projectilePos.x += PLAYER_S * FileSettings::GetFloat("SCALE_W") + PROJECTILE_OFFSET;
				playerProjectileListener->PlayerProjectileFired(projectilePos, Vector2(PROJECTILE_POWER * 0.7f, -PROJECTILE_POWER * 0.7f), id);
			}
			else if (buttonDown[SDL_CONTROLLER_BUTTON_DPAD_RIGHT] && buttonDown[SDL_CONTROLLER_BUTTON_DPAD_DOWN])
			{
				projectilePos.x += PLAYER_S * FileSettings::GetFloat("SCALE_W") + PROJECTILE_OFFSET;
				playerProjectileListener->PlayerProjectileFired(projectilePos, Vector2(PROJECTILE_POWER * 0.7f, PROJECTILE_POWER * 0.7f), id);
			}
			else if (buttonDown[SDL_CONTROLLER_BUTTON_DPAD_RIGHT])
			{
				projectilePos.x += PLAYER_S * FileSettings::GetFloat("SCALE_W") + PROJECTILE_OFFSET;
				playerProjectileListener->PlayerProjectileFired(projectilePos, Vector2(PROJECTILE_POWER, 0.0f), id);
			}
			else if (buttonDown[SDL_CONTROLLER_BUTTON_DPAD_DOWN])
			{
				projectilePos.y += PLAYER_S * FileSettings::GetFloat("SCALE_H") + PROJECTILE_OFFSET;
				playerProjectileListener->PlayerProjectileFired(projectilePos, Vector2(0.0f, PROJECTILE_POWER), id);
			}
			else if (buttonDown[SDL_CONTROLLER_BUTTON_DPAD_LEFT])
			{
				projectilePos.x -= PROJECTILE_SZ * FileSettings::GetFloat("SCALE_W") + PROJECTILE_OFFSET;
				playerProjectileListener->PlayerProjectileFired(projectilePos, Vector2(-PROJECTILE_POWER, 0.0f), id);
			}
			else if (buttonDown[SDL_CONTROLLER_BUTTON_DPAD_UP])
			{
				projectilePos.y -= PLAYER_S * FileSettings::GetFloat("SCALE_H") + PROJECTILE_OFFSET;
				playerProjectileListener->PlayerProjectileFired(projectilePos, Vector2(0.0f, -PROJECTILE_POWER), id);
			}
			else if (faceLeft)
			{
				projectilePos.x -= PROJECTILE_SZ * FileSettings::GetFloat("SCALE_W") + PROJECTILE_OFFSET;
				playerProjectileListener->PlayerProjectileFired(projectilePos, Vector2(-PROJECTILE_POWER, 0.0f), id);
			}
			else
			{
				projectilePos.x += PLAYER_S * FileSettings::GetFloat("SCALE_W") + PROJECTILE_OFFSET;
				playerProjectileListener->PlayerProjectileFired(projectilePos, Vector2(PROJECTILE_POWER, 0.0f), id);
			}
			
		}

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
	if (id == 0)
	{
		return "Glen";
	}
	if (id == 1)
	{
		return "John";
	}
	if (id == 2)
	{
		return "Rex";
	}
	if (id == 3)
	{
		return "Paul";
	}
	return "Invalid playerNum";
}

//UPDATE AND DRAW FUNCTIONS
bool Player::Update(float delta_, vector<Platform>& platform_, std::vector<PlayerProjectile>& projectiles_ )
{
	prevX = pos.x;
	prevY = pos.y;
	
	if (alive)
		HandleInput(delta_);

	ApplyGravity();	
	ApplyVelocity(velocity);	
	playerSpeak.SetPos(pos + Vector2(13, -15));
	HandleCollision(platform_, projectiles_);
	UpdateAnimation(delta_);

	if (alive)
		return true;
	return false;
}


void Player::Draw()
{
	//set the UV
	SetSpriteUVCoordinates	( SpriteSheet::PlayerSprite(), currentAnimation);
	MoveSprite(SpriteSheet::PlayerSprite(), pos.x, pos.y);
	DrawSprite(SpriteSheet::PlayerSprite(), faceLeft, 1.0f, true);

#ifdef SHOW_COLLIDERS
	DrawRect(bottomCollider);
	DrawRect(leftCollider);
	DrawRect(rightCollider);
	DrawRect(topCollider);
	DrawRect(hitCollider);	
#endif
	
	playerSpeak.SetText(to_string(projectiles));
	playerSpeak.Draw();
}