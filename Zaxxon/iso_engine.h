#pragma once

#include "SDL.h"
#include "SDL_ttf.h"
#include "sprite.h"

class IsoEngine
{
public:
	// Destroys the avancez library instance
	void destroy();

	// Destroys the avancez library instance and exits
	void quit();

	// Creates the main window. Returns true on success.
	bool init(int width, int height);

	// set the render scale and rezies the window (integer scaling only)
	bool setRenderScale(int scale);

	// Clears the screen and draws all sprites and texts which have been drawn
	// since the last update call.
	// If update returns false, the application should terminate.
	void processInput();

	void swapBuffers();

	void clearWindow();

	// Create a sprite given a string.
	// All sprites are 32*32 pixels.
	Sprite* createSprite_BMP(const char* path); 
	Sprite* createSprite(const char* path);
	Spritesheet* createSpritsheet(const char* path, int num_sprite);

	// Draws the given text (default color = black)
	void drawText(int x, int y, const char* msg, SDL_Color color = { 0xff, 0xff, 0xff });
	void drawSmallText(int x, int y, const char* msg, SDL_Color color = { 0xff, 0xff, 0xff });

	// Return the total time spent in the game, in seconds.
	float getElapsedTime();

	struct KeyStatus
	{
		KeyStatus() : up(), down(), left(), right(), fire(), enter(), esc()
		{
		}

	    bool up;    // up arrow
		bool down;  // down arrow
		bool left;  // left arrow
		bool right; // right arrow
		bool fire;  // space
		bool enter; // enter button
		bool esc;   // escape button
	};

	// Returns the keyboard status. If a flag is set, the corresponding key is being held down.
	void getKeyStatus(KeyStatus& keys);

	int getWidth() {
		return width;
	}

	int getHeight() {
		return height;
	}

private:
	SDL_Window * window = nullptr;
	SDL_Renderer * renderer = nullptr;

	TTF_Font* font = nullptr;
	TTF_Font* font_small = nullptr;

	KeyStatus key;

	int width, height;
};

