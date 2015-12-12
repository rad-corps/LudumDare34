#pragma once

#include "ProgramState.h"
#include <vector>
#include "../game_objects/GLText.h"
#include "../game_objects/Player.h"

class PSControllerConnect : public ProgramState, public InputListener
{
public:
	PSControllerConnect(void);
	~PSControllerConnect(void);

	virtual ProgramState* Update(float delta_);
	virtual void Draw();

	virtual void KeyDown(SDL_Keycode key_);
	virtual void GamePadButtonDown(SDL_GameControllerButton button_);

private:
	ProgramState* newState;
	GLText menuText;
	std::vector<Player*> players;
};

