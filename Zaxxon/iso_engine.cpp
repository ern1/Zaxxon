#include "iso_engine.h"
#include "SDL_image.h"
#include <cstddef>

// Creates the main window. Returns true on success.
bool IsoEngine::init(int width, int height)
{
	SDL_Log("Initializing the engine...\n");

	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL failed the initialization: %s\n", SDL_GetError());
		return false;
	}

	// Create window
	window = SDL_CreateWindow("Zaxxon 2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);//SDL_WINDOW_FULLSCREEN_DESKTOP
	if (window == nullptr)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Window could not be created! SDL_Error: %s\n", SDL_GetError());
		return false;
	}

	// Create renderer for window
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE);
	if (renderer == nullptr)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Renderer could not be created! SDL Error: %s\n", SDL_GetError());
		return false;
	}

	if (IMG_Init(IMG_INIT_PNG) == 0)
	{
	    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Could not init PNG! SDL_Error: %s\n", IMG_GetError());
		return false;
	}

	TTF_Init();
	font = TTF_OpenFont("assets/fonts/NiseZaxxon.ttf", 12); //this opens a font style and sets a size
	if (font == nullptr)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "font cannot be created! SDL_Error: %s\n", TTF_GetError());
		return false;
	}

	font_small = TTF_OpenFont("assets/fonts/space_invaders.ttf", 8); //this opens a font style and sets a size
	if (font == nullptr)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "font cannot be created! SDL_Error: %s\n", TTF_GetError());
		return false;
	}

	// Initialize the keys
	//key.up = false; key.down = false; key.left = false;	key.right = false; key.fire = false; key.enter = false;  key.esc = false;

	SDL_RenderSetIntegerScale(renderer, SDL_TRUE);

	// Initialize renderer color
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xFF);

	//Clear screen
	SDL_RenderClear(renderer);

	this->width = width;
	this->height = height;

	SDL_Log("Engine up and running...\n");
	return true;
}

// Destroys the avancez library instance
void IsoEngine::destroy()
{
	SDL_Log("Shutting down the engine\n");

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	TTF_CloseFont(font);

    TTF_Quit();
	IMG_Quit();
    SDL_Quit();
}

void IsoEngine::quit() {
	destroy();
	exit(0);
}

bool IsoEngine::setRenderScale(int scale)
{
	int w, h;
	SDL_GetWindowSize(window, &w, &h);

	if (SDL_RenderSetScale(renderer, static_cast<float>(scale), static_cast<float>(scale)) != 0)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "could not set render scale! SDL_Error: %s\n", SDL_GetError());
		return false;
	}

	SDL_SetWindowSize(window, w * scale, h * scale);
	return true;
}

void IsoEngine::processInput()
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_KEYDOWN)
		{
			switch (event.key.keysym.sym)
			{
			case SDLK_ESCAPE:
			case SDLK_q:
				key.esc = true;
				break;
			case SDLK_RETURN:
				key.enter = true;
				break;
			case SDLK_SPACE:
				key.fire = true;
				break;
			case SDLK_UP:
				key.up = true;
				break;
			case SDLK_DOWN:
				key.down = true;
				break;
			case SDLK_LEFT:
				key.left = true;
				break;
			case SDLK_RIGHT:
				key.right = true;
				break;
			}
		}

		if (event.type == SDL_KEYUP)
		{
			switch (event.key.keysym.sym)
			{
			case SDLK_RETURN:
				key.enter = false;
				break;
			case SDLK_SPACE:
				key.fire = false;
				break;
			case SDLK_UP:
				key.up = false;
				break;
			case SDLK_DOWN:
				key.down = false;
				break;
			case SDLK_LEFT:
				key.left = false;
				break;
			case SDLK_RIGHT:
				key.right = false;
				break;
			}
		}

	}
}

void IsoEngine::swapBuffers() {
	//Update screen
	SDL_RenderPresent(renderer);
}

void IsoEngine::clearWindow() {
	//Clear screen
	SDL_RenderClear(renderer);
}

void IsoEngine::drawText(int x, int y, const char* msg, SDL_Color color)
{
	SDL_Surface* surface = TTF_RenderText_Solid(font, msg, color); // as TTF_RenderText_Solid could only be used on SDL_Surface then you have to create the surface first

	SDL_Texture* msg_texture = SDL_CreateTextureFromSurface(renderer, surface); //now you can convert it into a texture

	SDL_Rect dst_rect = { x, y, 0, 0 };
	SDL_QueryTexture(msg_texture, NULL, NULL, &dst_rect.w, &dst_rect.h);

	SDL_RenderCopy(renderer, msg_texture, NULL, &dst_rect);

	SDL_DestroyTexture(msg_texture);
	SDL_FreeSurface(surface);
}

void IsoEngine::drawSmallText(int x, int y, const char* msg, SDL_Color color)
{
	SDL_Surface* surface = TTF_RenderText_Solid(font_small, msg, color); // as TTF_RenderText_Solid could only be used on SDL_Surface then you have to create the surface first

	SDL_Texture* msg_texture = SDL_CreateTextureFromSurface(renderer, surface); //now you can convert it into a texture

	SDL_Rect dst_rect = { x, y, 0, 0 };
	SDL_QueryTexture(msg_texture, NULL, NULL, &dst_rect.w, &dst_rect.h);

	SDL_RenderCopy(renderer, msg_texture, NULL, &dst_rect);

	SDL_DestroyTexture(msg_texture);
	SDL_FreeSurface(surface);
}

float IsoEngine::getElapsedTime()
{
	return SDL_GetTicks() / 1000.f;
}

void IsoEngine::getKeyStatus(KeyStatus & keys)
{
	keys.up = key.up;
    keys.down = key.down;
	keys.left = key.left;
	keys.right = key.right;
	keys.fire = key.fire;
	keys.enter = key.enter;
	keys.esc = key.esc;
}


Sprite* IsoEngine::createSprite_BMP(const char* path)
{
	SDL_Surface* surface = SDL_LoadBMP(path);
	if (surface == NULL)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unable to load image %s! SDL_image Error: %s\n", path, SDL_GetError());
		return NULL;
	}

	//Create texture from surface pixels
	SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, surface);
	if (texture == NULL)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unable to create texture from %s! SDL Error: %s\n", path, SDL_GetError());
		return NULL;
	}

	//Get rid of old loaded surface
	SDL_FreeSurface(surface);

	return new Sprite(renderer, texture);
}

Sprite* IsoEngine::createSprite(const char* path)
{
	SDL_Surface* surface = IMG_Load(path);
	if (surface == nullptr)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unable to load image %s! SDL_image Error: %s\n", path, SDL_GetError());
		return nullptr;
	}

	//Create texture from surface pixels
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	if (texture == nullptr)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unable to create texture from %s! SDL Error: %s\n", path, SDL_GetError());
		return nullptr;
	}

	//Get rid of old loaded surface
	SDL_FreeSurface(surface);

	return new Sprite(renderer, texture);
}

Spritesheet* IsoEngine::createSpritsheet(const char* path, int num_sprite)
{
	SDL_Surface* surface = IMG_Load(path);
	if (surface == nullptr)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unable to load image %s! SDL_image Error: %s\n", path, SDL_GetError());
		return nullptr;
	}

	//Create texture from surface pixels
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	if (texture == nullptr)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unable to create texture from %s! SDL Error: %s\n", path, SDL_GetError());
		return nullptr;
	}

	//Get rid of old loaded surface
	SDL_FreeSurface(surface);

	return new Spritesheet(renderer, texture, num_sprite);
}
