#pragma once

#include <SDL2/SDL_mixer.h>

class GLAHSound
{
public:
	static void Init();

	static void PlaySoundHitPlayer();
	static void PlaySoundJump();
	static void PlaySoundPickup();
	static void PlaySoundProjectileLand();
	static void PlaySoundProjectileShoot();
	static void PlaySoundShowScores();
	static void PlaySoundStartNewRound();

private:
	GLAHSound();
	~GLAHSound();

	static Mix_Chunk *soundHitPlayer;
	static Mix_Chunk *soundJump;
	static Mix_Chunk *soundPickup;
	static Mix_Chunk *soundProjectileLand;
	static Mix_Chunk *soundProjectileShoot;
	static Mix_Chunk *soundShowScores;
	static Mix_Chunk *soundStartNewRound;
};

