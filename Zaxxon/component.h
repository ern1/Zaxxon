#pragma once

#include <set>
#include <vector>
#include <unordered_map>
#include <vector>
#include "object_pool.h"
#include "game_object.h"

class GameObject;
class IsoEngine;
class Sprite;

class Component
{
protected:
	IsoEngine* engine;	// used to access the engine
	GameObject* go;		// the game object this component is part of
	std::set<GameObject*>* game_objects;	// the global container of game objects

public:
	virtual ~Component() {}

	virtual void Create(IsoEngine* engine, GameObject* go, std::set<GameObject*>* game_objects);
	virtual void Init() {}
	virtual void Update(double dt) = 0;
	virtual void Receive(Message m) {}
	virtual void Destroy() {}
};


class RenderComponent : public Component
{
protected:
	Sprite* sprite;
	const Vector2D* ref_pos; // used to render at a position relative to 'camera' position
	bool render_flat;		 // Ignore isometric z-value (for the player ship's shadow etc)

public:
	virtual void Create(IsoEngine* engine, GameObject* go, std::set<GameObject*> * game_objects,
		const char* sprite_name, const Vector2D* ref_pos);
	virtual void Update(double dt);
	virtual void Destroy();

	Sprite* GetSprite() { return sprite; }
	void setRenderFlat(bool render_flat) { this->render_flat = render_flat; }
};


struct Animation
{
	// General info about an animation sequence
	std::vector<int> frame_sequence; // uses indices of the spritesheet
	int frame_num;                   // number of frame in sequence
	double time_per_frame;
	
	// Used when an animation is running
	int current_frame = 0;
	double current_frame_time = 0;
	bool loop; // add support other loops (i.e., one that reverses as it reach the end)

	Animation(int* frame_sequence, int frame_num, double time_per_frame)
		: frame_num(frame_num), time_per_frame(time_per_frame)
	{
		for (int i = 0; i < frame_num; i++)
			this->frame_sequence.push_back(frame_sequence[i]);
	};

	// Resets the animation to frame 0
	void reset();

	// Returns the index of the next frame (index on the spritesheet) to be rendered, returns -1 if finished
	int getNextFrame(double dt);
};


// TODO: Change so that this uses the Spritesheet class
// TODO: Storing a copy of an animation sequence locally doesnt make sense, if it's not related to the current state of an animation it might as well be accessible globally by all renderComponents.
class RenderSpriteSheetComponent : public RenderComponent
{
	SDL_Rect clip;

	bool is_animation_active = false;
	std::unordered_map<int, Animation> animations;
	Animation* current_animation = nullptr;

public:
	// Could take rows and cols instead of sprite_num, but having all sprites on a spritesheet laid out horizontal is enough for now
	virtual void Create(IsoEngine* engine, GameObject* go, std::set<GameObject*>* game_objects,
		const char* sprite_name, int sprite_num, const Vector2D* ref_pos);
	virtual void Update(double dt);
	void selectSprite(int index);

	// Animation stuff
	void addAnimation(int id, const Animation& animation);
	void addAnimation(int id, int* frame_sequence, int frame_num, double time_per_frame);
	void startAnimation(int id, bool loop = false);
	void stopAnimation();
	bool isAnimationActive() { return is_animation_active; }
};

// Basically a simpler version of RenderSpriteSheetComponent with offset added
// (Not that necessary as the same functionality easily could be added to RenderSpriteSheetComponent, and the Camera class should be remade anyway...)
class RenderLevelComponent : public RenderComponent
{
	SDL_Rect clip;
	int offset_x, offset_y;

public:
	void Create(IsoEngine* engine, GameObject* go, std::set<GameObject*>* game_objects,
		const char* sprite_name, int clip_w, int clip_h, int offset_x, int offset_y);
	virtual void Update(double dt);
};



class CollideComponent : public Component
{
	IsoVector size;

protected:
	std::vector<ObjectPool<GameObject>*> coll_objects_pools; // collision will be tested with these objects

public:
	virtual void Create(IsoEngine* engine, GameObject* go, std::set<GameObject*>* game_objects, IsoVector size);
	virtual void Update(double dt);
	void AddCollObjects(ObjectPool<GameObject>* coll_objects);
};
