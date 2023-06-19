#include "component.h"
#include "game_object.h"
#include "iso_engine.h"

void Component::Create(IsoEngine* engine, GameObject* go, std::set<GameObject*>* game_objects)
{
	this->go = go;
	this->engine = engine;
	this->game_objects = game_objects;
}


void RenderComponent::Create(IsoEngine* engine, GameObject* go, std::set<GameObject*>* game_objects,
		const char* sprite_name, const Vector2D* ref_pos)
{
	SDL_Log("RenderComponent::Create");
	Component::Create(engine, go, game_objects);

	sprite = engine->createSprite(sprite_name);
	this->ref_pos = ref_pos;
	this->render_flat = false;
}

void RenderComponent::Update(double dt)
{
	if (!go->enabled)
		return;

	if (sprite)
	{
		// If a reference position is used, calculate a new position relative to that
		if (ref_pos != nullptr)
		{
			int posX = static_cast<int>(go->position.x - ref_pos->x);
			int posY = static_cast<int>(go->position.y - ref_pos->y + (render_flat ? go->isoPos.z : 0.0));
			sprite->draw(posX, posY);
		}
		else
		    sprite->draw(static_cast<int>(go->position.x),  static_cast<int>(go->position.y));
	}
}

void RenderComponent::Destroy()
{
	if (sprite != nullptr)
		sprite->destroy();
	sprite = nullptr;
}


void Animation::reset()
{
	current_frame = 0;
	current_frame_time = 0.0;
}

int Animation::getNextFrame(double dt)
{
	current_frame_time += dt;

	if (current_frame_time > time_per_frame)
	{
		// Change to next frame
		++current_frame;
		current_frame_time -= time_per_frame;
	}

	// Check if it's time start the animation from beginning or end it
	if (current_frame >= frame_num)
	{
	    if (loop)
			current_frame = 0;
		else
			return -1;
	}

	return frame_sequence[current_frame];
}


void RenderSpriteSheetComponent::Create(IsoEngine* engine, GameObject* go, std::set<GameObject*>* game_objects,
		const char* sprite_name, int sprite_num, const Vector2D* ref_pos)
{
	SDL_Log("RenderComponent::Create");
	Component::Create(engine, go, game_objects);

	sprite = engine->createSprite(sprite_name);
	this->ref_pos = ref_pos;
	this->render_flat = false;

    /* Set clip at index 0.
     * Width dividied by the number of sprites will give the width of a single sprite as all in the
     * same spritesheet will have the same width.
     * Height will always equal height of sprite as multiple rows in a spritesheet are not yet supported. */
	clip = { 0, 0, this->GetSprite()->w / sprite_num, this->GetSprite()->h };

	bool is_animation_running = false;
}

void RenderSpriteSheetComponent::Update(double dt)
{
	if (!go->enabled)
		return;

	if (sprite)
	{
		if (is_animation_active)
		{
            int next_frame = current_animation->getNextFrame(dt);
			if (next_frame == -1)
				is_animation_active = false;
			else
				selectSprite(next_frame);
		}

		if (ref_pos != nullptr)
		{
			int pos_x = static_cast<int>(go->position.x - ref_pos->x);
			int pos_y = static_cast<int>(go->position.y - ref_pos->y + (render_flat ? go->isoPos.z : 0.0));
			sprite->draw_clip(pos_x, pos_y, &clip);
		}
		else
			sprite->draw_clip(static_cast<int>(go->position.x), static_cast<int>(go->position.y), &clip);
	}
}

void RenderSpriteSheetComponent::selectSprite(int index)
{
	clip.x = index * clip.w;
}

void RenderSpriteSheetComponent::addAnimation(int id, const Animation& animation)
{
    animations.insert({ id, animation });
}

void RenderSpriteSheetComponent::addAnimation(int id, int* frame_sequence, int frame_num, double time_per_frame)
{
	addAnimation(id, { frame_sequence, frame_num, time_per_frame });
}

void RenderSpriteSheetComponent::startAnimation(int id, bool loop)
{
	// Find animation with specified id
	std::unordered_map<int, Animation>::const_iterator iter = animations.find(id);
	if (iter == animations.end())
		return;

	current_animation = &(const_cast<Animation&>(iter->second));;
	current_animation->loop = loop;
	current_animation->reset();
	is_animation_active = true;
}

void RenderSpriteSheetComponent::stopAnimation()
{
	is_animation_active = false;
}


void RenderLevelComponent::Create(IsoEngine* engine, GameObject* go, std::set<GameObject*>* game_objects,
		const char* sprite_name, int clip_w, int clip_h, int offset_x, int offset_y)
{
	SDL_Log("RenderLevelComponent::Create");
	Component::Create(engine, go, game_objects);

	sprite = engine->createSprite(sprite_name);
	clip = { 0, 0, clip_w, clip_h };
	
	this->offset_x = offset_x;
	this->offset_y = offset_y;
}

void RenderLevelComponent::Update(double dt)
{
	if (!go->enabled)
		return;

	if (sprite)
	{
		clip.x = offset_x + static_cast<int>(go->position.x);
		clip.y = offset_y + static_cast<int>(go->position.y);
		sprite->draw_clip(0, 0, &clip);
	}
}


void CollideComponent::Create(IsoEngine* engine, GameObject* go, std::set<GameObject*>* game_objects, IsoVector size)
{
	SDL_Log("CollideComponent::Create");
	Component::Create(engine, go, game_objects);
	this->size = size;
}

void CollideComponent::Update(double dt)
{
	// TODO: Would probably be better if the center position was set when this component was created (gets a bit weird now, and simplifies and would simplify/improve many other things in this project)
    IsoVector go_center = { go->isoPos.x + size.x / 2, go->isoPos.y + size.y / 2, go->isoPos.z + size.z / 2 };
    const double go_diag = go->isoPos.distance(go->isoPos + size);

	for (auto& object_pool : coll_objects_pools)
	{
		for (unsigned i = 0; i < object_pool->pool.size(); i++)
		{
			GameObject* go0 = object_pool->pool[i];
			auto* go0_coll = go0->GetComponent<CollideComponent*>();

			if (go0->enabled)
			{
				// TODO: Fix so collision detection work more like expected (right now it's shit - all objects are treated as spheres and sizes are only roughly estimated)
				IsoVector go0_center = { go0->isoPos.x + go0_coll->size.x / 2, go0->isoPos.y + go0_coll->size.y / 2, go0->isoPos.z + go0_coll->size.z / 2 };

				// Calculate the distance between the objects and its diagonal length
				double dist = go_center.distance(go0_center);
				double go0_diag = go0->isoPos.distance(go0->isoPos + go0_coll->size);

				// Collision if the distance between the objects is less than their combined half-diagonal
				if (dist < (go_diag + go0_diag) / 2)
				{
                 	go->Receive({ MessageType::HIT, go0->isoPos });
					go0->Receive({ MessageType::HIT, go->isoPos });
				}

				// OLD
				//if (((go0->isoPos.x > go->isoPos.x - 20) && (go0->isoPos.x < go->isoPos.x + 20)) &&
				//	((go0->isoPos.y > go->isoPos.y - 20) && (go0->isoPos.y < go->isoPos.y + 20)) &&
				//	((go0->isoPos.z > go->isoPos.z - 20) && (go0->isoPos.z < go->isoPos.z + 20)))	
			}
		}
	}
}

void CollideComponent::AddCollObjects(ObjectPool<GameObject>* coll_objects)
{
	coll_objects_pools.push_back(coll_objects);
}
