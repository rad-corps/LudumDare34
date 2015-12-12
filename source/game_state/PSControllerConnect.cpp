#include "PSControllerConnect.h"
#include "PSLevelSelect.h"
#include <iostream>
#include <sstream>
#include "../globals/consts.h"
#include "../game_objects/Player.h"

PSControllerConnect::PSControllerConnect(void)
{
	AddInputListener(this);

	menuText.SetAlignment(TEXT_ALIGNMENT::ALIGN_CENTRE);
	menuText.SetHUD(true);
	menuText.SetPos(Vector2(SCREEN_W * 0.5f, 100));
	menuText.SetText("Controller Connect Screen");

	cout << "SDL_NumJoysticks()" << SDL_NumJoysticks() << endl;

	newState = nullptr;
}


PSControllerConnect::~PSControllerConnect(void)
{
}


void PSControllerConnect::KeyDown(SDL_Keycode key_)
{
	//create a player for each control detected. 
	for ( int i = 0; i < SDL_NumJoysticks(); ++i ) 
	{
		players.push_back(new Player(i));
	}
	
	//move onto the level select screen
	newState = new PSLevelSelect(players);
}

void PSControllerConnect::GamePadButtonDown(SDL_GameControllerButton button_) 
{

}


ProgramState* PSControllerConnect::Update(float delta_)
{
	//build the text
	stringstream text;
	text << SDL_NumJoysticks() << " Controllers are detected.";
	menuText.SetText(text.str());

	return newState;
}

void PSControllerConnect::Draw()
{
	menuText.Draw();
}