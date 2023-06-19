#pragma once

// TODO: Använd denna istället, men använd type-objekt enligt kursbok? (http://gameprogrammingpatterns.com/type-object.html)
// TODO: Räcker väl dock med att behavior-komponenten tar olika former (genom type-objekt)?
//class Projectile : public GameObject
//{
//
//public:
//	IsoVector init_pos; // isometric
//	IsoVector dir;		// isometric
//
//	virtual void Init(IsoVector pos, IsoVector direction)
//	{
//		//SDL_Log("Projectile::Init");
//		GameObject::Init();
//		this->isoPos = pos;
//		this->init_pos = isoPos;
//		this->dir = direction.normal();
//	}
//
//	virtual void Receive(Message m)
//	{
//		if (!enabled)
//			return;
//
//		if (m.type == MessageType::HIT)
//		{
//			enabled = false;
//			SDL_Log("Projectile::Hit");
//		}
//	}
//};
//
//class ProjectileBehaviourComponent : public Component
//{
//
//public:
//	virtual void Update(double dt)
//	{
//		auto* projectile = (Projectile*)go;
//
//		go->isoPos += projectile->dir * (LASER_SPEED * dt);
//
//		// TODO: FIXA (tänkte fel då player också rör sig framåt ju då laser hinner ju)
//		//if (go->isoPos > LASER_DISTANCE)
//		if (projectile->init_pos.distance(go->isoPos) > LASER_DISTANCE)
//			go->enabled = false;
//
//		go->position = go->isoPos.to2D();
//	}
//};
