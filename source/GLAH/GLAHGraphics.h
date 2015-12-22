#pragma once
#include <SDL2/SDL.h>

#include <map>
#include <string>
//#include "SpriteFont.h"
//#include "Vertex.h"
#include "GLAHEntity.h"
#include "GLAHColour.h"

#include "InputListener.h"
#include "../math/Rect.h"

using namespace std;

	//input handling
void			AddInputListener(InputListener* inputListener_);
void			AddGameControllerListener(GameControllerListener* listener_, int playerID_);
void			RemoveGameControllerListener(GameControllerListener* listener);
void			RemoveInputListener();

//void			LoadAllGamePads();
//void			ClearGamePads();

 bool			IsKeyDown( SDL_Keycode a_iKey );
 //bool			IsGamePadButtonDown(SDL_GameControllerButton button_);

 void			GetMouseLocation( int& a_iMouseX, int& a_iMouseY );

 bool			GetMouseButtonDown( int a_iMouseButtonToTest );

 bool			GetMouseButtonReleased( int a_iMouseButtonToTest );

 void			SetQuit();


 Vector2			GetCameraPos();
 //void			SetBGColour(int r_, int g_, int b_);

	//create a sprite (returns a spriteID)
	//This also creates a GLAHEntity that maps to the sprite ID
	//See GLAHEntity for more info
	SDL_Texture* CreateSprite( const char* textureName_, //the filename/path of the sprite
								int width_, int height_, //width and height in pixels
								unsigned int parentSpriteID_ = 0, //sprite to parent to (0 if none)
								//Vector3 originOffset_ = Vector3(0.f,0.f,0.f), //rotation origin (bottom left by default)
								SColour colour_ = SColour(0xFF,0xFF,0xFF,0xFF)); //RGBA (white default)

	//get the viewport rect based on where the camera is. 
	SDL_Rect			GetScreenRect();
								

	//Move the sprite in world space to the absolute coordinate xPos_, yPos_
	void			MoveSprite				( SDL_Texture*, float xPos_, float yPos_ );

	void 			MoveCamera				(float xPos_, float yPos_);
	
	//move the sprite relative to its current position
	void			MoveSpriteRelative		( unsigned int spriteID_, float xMovement_, float yMovement_ = 0.0f, float rotation_ = 0.0f);
	
	//rotate the sprite to this rotation_
	void			RotateSprite					( SDL_Texture*, float rotation_ );
	
	//rotate the sprite relative to current rotation
	void			RotateSpriteRelative			( SDL_Texture*, float rotation_ );
		
	void			SetSpriteUVCoordinates	( SDL_Texture*, float* a_fUVVec4 );

	//draw the sprite to screen
	void			DrawSprite				( SDL_Texture* sprite_,  bool xFlip_ = false, float alpha_ = 1.0f, bool effectedByCamera = true);
	void			DrawRect(SDL_Rect);

	//Get information about the sprite based on spriteID_ 
	GLAHEntity		GetGLAHEntity			(SDL_Texture* spriteID_);

	//Scale sprite (both x and y scaled by scalar_)
//	void			ScaleSprite				( unsigned int spriteID_, float scalar_ );
	void			ScaleSprite				( SDL_Texture*, float scalarX_, float scalarY_ );

	bool			FrameworkUpdate();



	int				Initialise( int a_iWidth, int a_iHeight, bool a_bFullscreen = false, const char* a_pWindowTitle = nullptr );

	void			Shutdown();

	float			GetDeltaTime();

	SDL_Window*		GetWindow();

	//NOT YET IMPLEMENTED
	void			ClearScreen();	
	unsigned int	DuplicateSprite			( unsigned int spriteID_ );
	void			DestroySprite			( unsigned int spriteID_ );
	void			SetSpriteColour			( unsigned int spriteID_, SColour& colour_ );
	void			GetSpriteColour			( unsigned int spriteID_, SColour& colour_ );
	void			DrawString(char *, int xPos_, int yPos_, float size_ = 1.f, SColour colour_ = SColour(0xFF,0xFF,0xFF,0xFF));
	void			AddFont( char* fontName_ );
	void			SetFont( char* fontName_ );
	void			RemoveFont( char* fontName_ );
	//END OF NOT YET IMPLEMENTED

	void SetDrawColour(int r, int g, int b);
