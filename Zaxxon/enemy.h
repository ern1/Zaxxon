#pragma once

class EnemyBehaviourComponent : public Component
{
	float time_last_fire;	    // time from last shot
	ObjectPool<Laser>* laser_pool;

public:
	virtual ~EnemyBehaviourComponent() {}

	virtual void Create(IsoEngine* engine, GameObject* go, std::set<GameObject*>* game_objects, ObjectPool<Laser>* laser_pool)
	{
		Component::Create(engine, go, game_objects);
		this->laser_pool = laser_pool;
	}

	virtual void Init()
	{
		SDL_Log("EnemyBehaviourComponent::Init");
		time_last_fire = -1000.f;
	}

	virtual void Update(double dt)
	{
		if (CanFire())
			Fire();
	}

	void Move(IsoVector dir)
	{
		go->isoPos.x += dir.x;
		go->isoPos.y += dir.y;
		if (go->isoPos.z + dir.z > 10)
			go->isoPos.z += dir.z;
	}

	// TODO: Fix so enemies fire at player ship
	void Fire()
	{
		// fetch a laser from the pool with lasers
		auto* laser = laser_pool->FirstAvailable();
		if (laser != nullptr)
		{
			//laser->Init(go->isoPos, { 2, 20, 0 }, { 0,1,0 });
			laser->Init(go->isoPos + IsoVector{ 2, 20, 0 }, { 0, 1, 0 });
			game_objects->insert(laser);

			go->Send({ MessageType::PLAY_SOUND, WAV_LASER });
		}
	}

	bool CanFire()
	{
		//if ((engine->getElapsedTime() - time_last_fire) > 1.4f) {
		//	time_last_fire = engine->getElapsedTime();
		//	return true;
		//}
		return false;
	}
};

class Enemy : public GameObject
{

public:
	virtual ~Enemy() { SDL_Log("Enemy::~Enemy"); }

	virtual void Init(IsoVector pos)
	{
		SDL_Log("Enemy::Init");
		GameObject::Init();

		isoPos = pos;
		position = isoPos.to2D();
		int x = 0;
	}

	virtual void Receive(Message m)
	{
		if (m.type == MessageType::HIT)
		{
			SDL_Log("Enemy::Hit!");

			Send({ MessageType::ENEMY_HIT, m.data.vec });
			enabled = false;
		}
	}
};
