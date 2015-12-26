#include "GLAHSound.h"

Mix_Chunk* GLAHSound::soundHitPlayer;
Mix_Chunk* GLAHSound::soundJump;
Mix_Chunk* GLAHSound::soundPickup;
Mix_Chunk* GLAHSound::soundProjectileLand;
Mix_Chunk* GLAHSound::soundProjectileShoot;
Mix_Chunk* GLAHSound::soundShowScores;
Mix_Chunk* GLAHSound::soundStartNewRound;

GLAHSound::GLAHSound()
{
}


GLAHSound::~GLAHSound()
{
}



void GLAHSound::Init()
{
	soundHitPlayer = Mix_LoadWAV("./resources/sounds/hitPlayer.wav");
	soundJump = Mix_LoadWAV("./resources/sounds/jump.wav");
	soundPickup = Mix_LoadWAV("./resources/sounds/pickup.wav");
	soundProjectileLand = Mix_LoadWAV("./resources/sounds/projectileLand.wav");
	soundProjectileShoot = Mix_LoadWAV("./resources/sounds/projectileShoot.wav");
	soundShowScores = Mix_LoadWAV("./resources/sounds/showScores.wav");
	soundStartNewRound = Mix_LoadWAV("./resources/sounds/startNewRound.wav");
}

void GLAHSound::PlaySoundHitPlayer()
{
	Mix_PlayChannel(-1, soundHitPlayer, 0);
}

void GLAHSound::PlaySoundJump()
{
	Mix_PlayChannel(-1, soundJump, 0);
}
void GLAHSound::PlaySoundPickup()
{
	Mix_PlayChannel(-1, soundPickup, 0);
}
void GLAHSound::PlaySoundProjectileLand()
{
	Mix_PlayChannel(-1, soundProjectileLand, 0);
}
void GLAHSound::PlaySoundProjectileShoot()
{
	Mix_PlayChannel(-1, soundProjectileShoot, 0);
}
void GLAHSound::PlaySoundShowScores()
{
	Mix_PlayChannel(-1, soundShowScores, 0);
}
void GLAHSound::PlaySoundStartNewRound()
{
	Mix_PlayChannel(-1, soundStartNewRound, 0);
}