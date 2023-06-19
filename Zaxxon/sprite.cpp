#include "sprite.h"
#include <cstddef>

Sprite::Sprite(SDL_Renderer* renderer, SDL_Texture* texture)
{
	this->renderer = renderer;
	this->texture = texture;
	SDL_QueryTexture(this->texture, NULL, NULL, &(this->w), &(this->h));
}

void Sprite::destroy()
{
	SDL_DestroyTexture(texture);
}

void Sprite::draw(int x, int y)
{
	SDL_Rect rect = { x, y, 0, 0 };
	SDL_QueryTexture(texture, NULL, NULL, &(rect.w), &(rect.h));

	//Render texture to screen
	SDL_RenderCopy(renderer, texture, NULL, &rect);
}

void Sprite::draw_clip(int x, int y, SDL_Rect* clip)
{
	SDL_Rect rect = { x, y, clip->w, clip->h };

	//Render texture to screen
	SDL_RenderCopy(renderer, texture, clip, &rect);
}

Spritesheet::Spritesheet(SDL_Renderer* renderer, SDL_Texture* texture, int sprite_num)
	: Sprite::Sprite(renderer, texture)
{
	this->sprite_num = sprite_num;
	clip = { 0, 0, w / sprite_num, h };
	index_selected = 0;
}

// Draw the sprite at the given position
void Spritesheet::draw(int x, int y, int index)
{
	clip = { index * clip.w, 0, w / sprite_num, h };
	draw_clip(x, y, &clip);
}

void Spritesheet::draw_selected(int x, int y)
{
	clip = { index_selected * clip.w, 0, w / sprite_num, h };
	draw_clip(x, y, &clip);
}

void Spritesheet::select_sprite(int index)
{
	index_selected = index;
}
