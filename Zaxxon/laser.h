#pragma once

// TODO: Gör så att denna kan användas till alla typer av projektiler (se projectile.h)
class Laser : public GameObject
{

public:
	IsoVector init_pos; // isometric
	IsoVector dir; // isometric

	virtual void Init(IsoVector pos, IsoVector direction)
	{
		//SDL_Log("Laser::Init");
		GameObject::Init();
		this->isoPos = pos;
		this->init_pos = isoPos;
		this->dir = direction.normal();
		
		auto* render = this->GetComponent<RenderSpriteSheetComponent*>();
		render->startAnimation(0);
	}

	virtual void Receive(Message m)
	{
		if (!enabled)
			return;

		if (m.type == MessageType::HIT)
		{
			enabled = false;
			SDL_Log("Laser::Hit");
		}
	}
};

class LaserBehaviourComponent : public Component
{

public:
	virtual void Update(double dt)
	{
		auto* laser = dynamic_cast<Laser*>(go);
		
		go->isoPos += laser->dir * (LASER_SPEED * dt);

		if (laser->init_pos.distance(go->isoPos) > LASER_DISTANCE)
			go->enabled = false;

		go->position = go->isoPos.to2D();
	}
};
