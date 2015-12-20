#include "PSGameLoop.h"

//#include "AIE.h"
#include <string>
#include <cmath>
#include <time.h>
#include <assert.h>
#include <iostream>
#include <list>
#include <vector>
#include <string>

#include "../globals/consts.h"
#include "../game_objects/Player.h"
#include "../game_objects/Platform.h"
#include "../spritesheet/UVTranslator.h"
#include "../spritesheet/SpriteSheet.h"
#include "../data/DBLevel.h"
#include "PSMainMenu.h"


PSGameLoop::PSGameLoop(int level_, std::vector<Player*> players_) 
	: newProgramState(nullptr), gameOverTimer(0.0f), gameOver(false)
{
	players = players_;
	AddInputListener(this);
	cout << "PSGameLoop" << endl;
	level = level_;
	cout << endl << "-------Playing Game: ESC to quit-----------" << endl;
	DBLevel dbLevel;
	dbLevel.FillData(level, platforms);

	gameTimer = 0.f;
	
	//cannon.RegisterCannonListener(this);	

	//maximum of 20 player projectiles on screen at once
	playerProjectiles.resize(20);
	
	for (int i = 0; i < players.size(); ++i)
	{
		players[i]->InitListener(this);
	}

	SetDrawColour(89, 141, 179);	
}


PSGameLoop::~PSGameLoop(void)
{
	//kill the players
	for (auto &player : players)
	{
		delete player;
	}

	ClearGamePads();
}

void PSGameLoop::KeyDown(SDL_Keycode key_)
{
	if ( key_ == SDLK_ESCAPE ) 
		newProgramState = new PSMainMenu();

	if ( key_ == SDLK_r ) 
		newProgramState = new PSGameLoop(level, players);
}

void PSGameLoop::GamePadButtonDown(SDL_GameControllerButton button_)
{
	std::cout << "GamePadButtonDown(" << button_ << ");" << endl;
}

ProgramState* PSGameLoop::Update(float delta_)
{
	//reset the round when one player is left
	if (gameOver)
	{
		gameOverTimer += delta_;
		if (gameOverTimer > 3.0f)
		{
			gameOver = false;
			for (auto &player : players)
			{
				player->Reset();
				gameOverTimer = 0.0f;
			}
		}
	}

	int numPlayersAlive = 0;
	//update player	
	for (auto &player : players)
	{
		if (player->Update(delta_, platforms, playerProjectiles))
			numPlayersAlive++;
	}

	if (numPlayersAlive < 2)
	{
		gameOver = true;
	}

	//update platforms
	for ( auto &env : platforms )
		env.Update(delta_);		

	for ( auto &projectiles : playerProjectiles)
		projectiles.Update(delta_, platforms);

	return newProgramState;
}

//Draw Game
void PSGameLoop::Draw()
{
	for ( auto &projectiles : playerProjectiles)
		projectiles.Draw();

	for (auto &env : platforms )
		env.Draw();		

	for ( auto &player : players )
		player->Draw();			
}


void PSGameLoop::PlayerProjectileFired(Vector2 pos_, Vector2 velocity_, int playerID_)
{
	//find an inactive shell
	for ( auto &projectile : playerProjectiles )
	{
		if ( !projectile.IsActive() )
		{
			projectile.Shoot(pos_, velocity_, playerID_);
			return;
		}
	}
}