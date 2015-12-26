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
#include "../math/Collision.h"
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
	playerProjectiles.resize(40);
	
	for (int i = 0; i < players.size(); ++i)
	{
		players[i]->InitListener(this);
	}

	SetDrawColour(89, 141, 179);	

	char * error = "";
	dm.Select("./resources/db/dontpanic.db", "game_attributes", "*", "", "", error);
	initialProjectiles = dm.GetValueFloat(0, "initial_projectiles");

	Reset();
}

PSGameLoop::~PSGameLoop(void)
{
	//kill the players
	for (auto &player : players)
	{
		delete player;
	}

	//	ClearGamePads();
}

void PSGameLoop::Reset()
{
	gameOver = false;
	for (auto &player : players)
	{
		player->Reset();
	}
	SpawnAllPlayers();
	SpawnAllProjectiles();
	gameOverTimer = 0.0f;
}

void PSGameLoop::SpawnAllProjectiles()
{
	int spawnPoints = players.size() * initialProjectiles;
	
	//inactivate all existing projectiles
	for (auto &projectile : playerProjectiles)
	{
		projectile.SetActive(false);
	}

	int projectileIter = 0;
	//spawn all projectiles
	for (auto &player : players)
	{
		for (int i = 0; i < initialProjectiles; ++i)
		{
			bool spawnAgain;
			PlayerProjectile projectile;

			do 
			{
				spawnAgain = false;

				projectile = SpawnSingleProjectile(player->ID());

				//check if this overlaps an existing tile
				for (auto &platform : platforms)
				{
					if (platform.Collider() != nullptr)
					{
						if (Collision::RectCollision(*platform.Collider(), projectile.GetRect()))
						{
							spawnAgain = true;
						}
					}
				}

			} while (spawnAgain);

			playerProjectiles[projectileIter] = projectile;
			++projectileIter;
		}
	}
}

PlayerProjectile PSGameLoop::SpawnSingleProjectile(int playerID_)
{
	PlayerProjectile projectile;
	//randomize a location
	int screenw = FileSettings::GetInt("SCREEN_W");
	int screenh = FileSettings::GetInt("SCREEN_H");

	int projX = rand() % screenw;
	int projY = rand() % screenh;

	projectile.Shoot(Vector2(projX, projY), Vector2(0, 0), 1.0f, playerID_);
	return projectile;
}

void PSGameLoop::SpawnAllPlayers()
{

	//determine where the spawn point positions are
	std::vector<Vector2> spawnPoints;

	for (auto& platform : platforms)
	{
		if (platform.TileType() == 4)
		{
			spawnPoints.push_back(Vector2(platform.X(), platform.Y() - 50));
		}
	}

	//randomise a spawn point and allocate it to player 1 .... 
	for (auto& player : players)
	{
		int num = rand() % spawnPoints.size();
		player->SetPos(spawnPoints[num]);
		spawnPoints.erase(spawnPoints.begin() + num);
	}
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
			Reset();
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


void PSGameLoop::PlayerProjectileFired(Vector2 pos_, Vector2 velocity_, float gravity_, int playerID_)
{
	//find an inactive shell
	for ( auto &projectile : playerProjectiles )
	{
		if ( !projectile.IsActive() )
		{
			projectile.Shoot(pos_, velocity_, gravity_, playerID_);
			return;
		}
	}
}