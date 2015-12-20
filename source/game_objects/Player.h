#pragma once

#include "GameObject.h"
#include "Platform.h"
#include "../GLAH/Vector.h"
#include "../math/Rect.h"
#include <vector>
#include <map>
#include "GLText.h"

//#include "GLAH\GLAHInput.h"
#include "../GLAH/GLAHGraphics.h"
#include "../listeners/PlayerProjectileListener.h"
#include "../sqlite/DatabaseManager.h"
#include "PlayerProjectile.h"
#include "../GLAH/InputListener.h"


enum PLAYER_STATUS
{
	JUMPING,
	STATIONARY,
	RUNNING
};

class Player : public GameObject, GameControllerListener
{
public:
	Player(int id_);
	~Player(void);

	void InitListener(PlayerProjectileListener* playerProjectileListener_);

	bool Update(float delta_, std::vector<Platform>& platform_, std::vector<PlayerProjectile>& projectiles_);
	virtual void Draw();

	virtual void GamePadButtonDown(SDL_GameControllerButton button_);
	virtual void GamePadButtonUp(SDL_GameControllerButton button_);

	std::string Name();

	int ID();
	void Reset();

private:
	void ApplyVelocity(Vector2 velocity_);
	void MoveTo(Vector2 pos_);
	void UndoX();
	void UndoY();
	void HandleCollision(std::vector<Platform>& platform_, std::vector<PlayerProjectile>& projectiles_);
	void HandleInput(float delta_);
	void UpdateAnimation(float delta_);
	void ApplyGravity();
	void UpdateColliders();
	//Vector2 pos;
	float prevY;
	float prevX;
	Vector2 velocity;
	PLAYER_STATUS status;

	SDL_Rect topCollider;
	SDL_Rect bottomCollider;
	SDL_Rect leftCollider;
	SDL_Rect rightCollider;
	SDL_Rect hitCollider;

	float animStationary[4];
	float animMove1[4];
	float animMove2[4];
	float animMove3[4]; //copy of move1
	float animDead[4];
	float* currentAnimation;

	float animationTimer;

	float FPS;

	bool onPlatform;
	bool faceLeft;
	bool alive;
	bool jumpHeld;
	bool shootHeld;

	GLText playerSpeak;

	PlayerProjectileListener* playerProjectileListener;

	//database loaded values
	DatabaseManager dm;
	float gravity;
	float maxSpeed;
	float jumpForce;
	float accelleration;
	float terminalVelocity;

	std::map<SDL_GameControllerButton, bool> buttonDown;

	int playerNum;
	GLText guiText;
	int kills;
	int id;
};

