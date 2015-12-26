#pragma once

#include <vector>
#include "ProgramState.h"
#include "../GLAH/GLAHGraphics.h"
#include "../GLAH/InputListener.h"
#include "../game_objects/Platform.h"
#include "../game_objects/Player.h"
//#include "../game_objects/EnemySpawner.h"
//#include "../game_objects/Goal.h"
#include "../game_objects/Camera.h"
#include "../game_objects/PlayerProjectile.h"
#include "../listeners/CannonListener.h"
#include "../listeners/PlayerProjectileListener.h"
#include "../GLAH/Vector.h"

enum GAME_STATE {
	GS_PLAYING,
	GS_SHOWSCORE
};

class PSGameLoop : public ProgramState,/*public CannonListener, */ public InputListener, public PlayerProjectileListener
{
public:
	//updateInterval defaults to 1/60th of a second (60FPS)
	//GameLoop(int screenW_, int screenH_, float updateInterval = 0.0166666666666666667f);
	PSGameLoop(int startingLevel, std::vector<Player*> players_);
	//PSGameLoop();
	~PSGameLoop(void);

	//void Run();

	//updates the entire game world
	virtual ProgramState* Update(float delta_);
	void Draw();

	//virtual void ShotFired(Vector2 pos_, Vector2 velocity_);
	virtual void PlayerProjectileFired(Vector2 pos_, Vector2 velocity_, float gravity_, int playerID_);
	virtual void KillEarned(int playerID_);
	virtual void KeyDown(SDL_Keycode key_);
	virtual void GamePadButtonDown(SDL_GameControllerButton button_);

private:
	
	void SpawnAllPlayers();
	void SpawnAllProjectiles();
	PlayerProjectile SpawnSingleProjectile(int playerID_);
	void Reset();
	float gameTimer;
	
	int level;

	std::vector<Player*> players;
	std::vector<Platform> platforms;
	std::vector<PlayerProjectile> playerProjectiles;

	ProgramState* newProgramState;
	float gameOverTimer; //
	float showKillsTimer; //inbetween rounds
	bool gameOver;

	DatabaseManager dm;
	int initialProjectiles;

	//Stuff for inbetween rounds
	std::vector<int> currentRoundKills;
	std::vector<int> cumulativeKills;

	float p1Sprite[4];
	float p2Sprite[4];
	float p3Sprite[4];
	float p4Sprite[4];

	GAME_STATE state;
	std::vector<GLText> playerKillsText;
};

