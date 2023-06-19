#pragma once

class ObstacleBehaviourComponent : public Component
{

public:
	virtual ~ObstacleBehaviourComponent() {}

	virtual void Create(IsoEngine* engine, GameObject* go, std::set<GameObject*>* game_objects)
	{
		Component::Create(engine, go, game_objects);

	}

	virtual void Init()
	{
		SDL_Log("ObstacleBehaviourComponent::Init");

	}

	virtual void Update(double dt)
	{
		
	}

};

class Obstacle : public GameObject
{

public:
	virtual ~Obstacle() { SDL_Log("Obstacle::~Obstacle"); }

	virtual void Init(IsoVector pos)
	{
		SDL_Log("Obstacle::Init");
		GameObject::Init();

		isoPos = pos;
		position = isoPos.to2D();
	}

	virtual void Receive(Message m)
	{
		if (m.type == MessageType::HIT)
		{
			SDL_Log("Obstacle::Hit!");

			Send({ MessageType::OBSTACLE_HIT, m.data.vec });
			//enabled = false;
		}
	}
};
