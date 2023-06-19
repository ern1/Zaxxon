#pragma once

#include "SDL.h"

class Sprite
{
	SDL_Renderer* renderer;
	SDL_Texture* texture;

public:
	int w{}, h{};

	Sprite(SDL_Renderer* renderer, SDL_Texture* texture);
	Sprite() {};

	// Destroys the sprite instance
	void destroy();

	// Draw sprite at given position
	void draw(int x, int y);
	void draw_clip(int x, int y, SDL_Rect* clip);
};


// Not used by all different renderComponents yet as this was added later
class Spritesheet : public Sprite
{
	Sprite sprite;
	SDL_Rect clip{};
	int sprite_num;
	int index_selected;

public:
	Spritesheet(SDL_Renderer* renderer, SDL_Texture* texture, int sprite_num);

	void draw(int x, int y, int index);
	void draw_selected(int x, int y);
	void select_sprite(int index);
};
