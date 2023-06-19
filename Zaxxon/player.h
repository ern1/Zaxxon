#pragma once

class PlayerBehaviourComponent;

// the main player
class Player : public GameObject
{

public:
	int lives;	        // it's game over when goes to zero
	unsigned int fuel;  // TODO: make this more useful
	bool is_invincible; // true a short while after player take damage
    
	virtual ~Player() { SDL_Log("Player::~Player"); }

	virtual void Init()
	{
		SDL_Log("Player::Init");
		GameObject::Init();
		lives = PLAYER_LIVES;
		fuel = PLAYER_FUEL_MAX;
		is_invincible = false;
	}

	virtual void Receive(Message m)
	{
		if (m.type == MessageType::HIT)
		{
			SDL_Log("Player::Hit!");
			RemoveLife();

			if (lives < 0)
			{
				enabled = false;
				Send({ MessageType::GAME_OVER, lives });
			}
		}
	}

	void RemoveLife()
	{
		if (is_invincible) {
			return;
		}
		
		--lives;
		setInvincible(true);

		SDL_Log("Remaining lives: %d", lives);
	}

	void RemoveFuel()
	{
		if (--fuel <= 0)
			Send({ MessageType::GAME_OVER, fuel });
	}

	void setInvincible(bool state)
	{
		is_invincible = state;

		if (is_invincible)
		{
            // TODO: make player invincible for PLAYER_INVINCIBLE_TIME seconds
			is_invincible = false; // temp
		}
	}
};


class PlayerBehaviourComponent : public Component
{
	IsoVector move_speed;	 // speed of the players ship
	float energy;	         // used for laser (i dont know if that is even somewhat accurate to the original)
	double fuel_timer;       // how long until the next time fuel decreases
	float time_fire_pressed; // time from the last time the fire button was pressed
	ObjectPool<Laser>* laser_pool;

public:
	virtual ~PlayerBehaviourComponent() {}

	virtual void Create(IsoEngine* engine, GameObject* go, std::set<GameObject*>* game_objects, ObjectPool<Laser>* laser_pool)
	{
		Component::Create(engine, go, game_objects);
		this->laser_pool = laser_pool;
	}

	virtual void Init()
	{
		SDL_Log("PlayerBehaviourComponent::Init");

		go->isoPos.x = 200.0;  // left/right
		go->isoPos.y = -180.0; // forward/backward
		go->isoPos.z = 15.0;   // altitude
		go->position = go->isoPos.to2D();

		move_speed = { PLAYER_SPEED, PLAYER_SPEED, PLAYER_SPEED_Z };
		energy = PLAYER_ENERGY_MAX * PLAYER_ENERGY_REGEN_TIME;
		fuel_timer = 0.;
		time_fire_pressed = -10000.f;
	}

	virtual void Update(double dt)
	{
		IsoEngine::KeyStatus keys;
		engine->getKeyStatus(keys);

	    /* Based on current user input, a simple orientation vector is used to represent the 9 possible
	       directions of the player ship. No need to normalize it as we want a constant speed along the y-axis. */
		IsoVector dir(0, -1, 0);
		dir.x = -static_cast<int>(keys.left) + static_cast<int>(keys.right);
		dir.z = static_cast<int>(keys.up) - static_cast<int>(keys.down);

		// Set player ship's sprite based on its direction
		auto* go_render = go->GetComponent<RenderSpriteSheetComponent*>();
		if (go_render != nullptr)
		{
			int x = 0;
			switch (static_cast<int>(4 + dir.x + (dir.z * -3))) {
			    case 0: x = 5; break;
			    case 1: x = 4; break;
			    case 2: x = 8; break;
			    case 3: x = 6; break;
			    case 4: x = 0; break;
			    case 5: x = 9; break;
			    case 6: x = 2; break;
			    case 7: x = 1; break;
			    case 8: x = 9; break;
			    default: x = 0;
			}

			go_render->selectSprite(x);
		}
			
		// Update energy and fuel levels
		energy = std::min(energy + static_cast<float>(dt), PLAYER_ENERGY_MAX * PLAYER_ENERGY_REGEN_TIME);
		fuel_timer += dt;

		if (fuel_timer > PLAYER_FUEL_TIME / game_speed)
		{
			auto* player = dynamic_cast<Player*>(go);
			player->RemoveFuel();
			fuel_timer -= PLAYER_FUEL_TIME / game_speed; // reset timer
		}

		// Move/fire
		Move(dir * move_speed * dt * game_speed);
		go->position = go->isoPos.to2D(); // TODO: Better to do this in rendercomponent instead?

	    if (keys.fire && CanFire())
			Fire();
	}

	void Move(IsoVector dir)
	{
		go->isoPos += dir;

		// Make sure player is still within the bounds of the map, else move back
		if (go->isoPos.x < LEVEL_MIN_X || go->isoPos.x > LEVEL_MAX_X)
			go->isoPos.x -= dir.x;
		if (go->isoPos.z < SHIP_MIN_ALTITUDE || go->isoPos.z > SHIP_MAX_ALTITUDE)
			go->isoPos.z -= dir.z; // to low/high so move back
	}

	void Fire()
	{
		// Fetch a laser from the pool with lasers
		auto* laser = laser_pool->FirstAvailable();
		if (laser != nullptr)
		{
			laser->Init(go->isoPos + IsoVector{ 2.0, -20.0, 0.0 }, { 0, -1, 0 });
			game_objects->insert(laser);

			go->Send({ MessageType::PLAY_SOUND, WAV_LASER });
			energy -= PLAYER_ENERGY_REGEN_TIME;
			SDL_Log("laser - energy: %d (%f)", static_cast<int>(energy / PLAYER_ENERGY_REGEN_TIME), energy);
		}
	}

	bool CanFire()
	{
		if ((engine->getElapsedTime() - time_fire_pressed) < (PLAYER_FIRE_TIME_INTERVAL / game_speed)
			|| energy < PLAYER_ENERGY_REGEN_TIME)
			return false;

		time_fire_pressed = engine->getElapsedTime();

		return true;
	}
};
