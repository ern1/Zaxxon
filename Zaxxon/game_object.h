#pragma once

#include <vector>
#include <set>
#include <string>

#include "vector2D.h"
#include "isoVector.h"

class Component;

enum class MessageType
{
	PLAY_SOUND,
	ANIMATION,
	PLAYER_HIT,   // the player was hit
	ENEMY_HIT,    // an enemy was hit
	OBSTACLE_HIT, // an obstacle was hit
	HIT,          // something collided with something else
	GAME_OVER,
	NO_MSG
};

struct Message
{
	MessageType type;
	union Data // std::variant seems nicer than this (but is from C++17)
	{
		int i;
		unsigned u;
		double d;
		IsoVector vec;

		Data(int i) : i(i) {}
        Data(unsigned u) : u(u) {}
		Data(double d) : d(d) {}
		Data(IsoVector vec) : vec(vec) {}
	}
	data;
};


class GameObject
{
protected:
	std::vector<GameObject*> receivers;
	std::vector<Component*> components;
	std::string id;

public:
	Vector2D position;
	IsoVector isoPos;
	bool enabled;

	virtual ~GameObject();

	virtual void Create();
	virtual void AddComponent(Component * component);

	virtual void Init();
	virtual void Update(float dt);
	virtual void Destroy();
	virtual void AddReceiver(GameObject *go);
	virtual void Receive(Message m) {}
	void Send(Message m);

	template<typename T>
	T GetComponent() {
		for (auto c : components) {
			T t = dynamic_cast<T>(c);
			if (t != nullptr)
				return t;
		}

		return nullptr;
	}

	// Could probably be useful for something?
	std::string getID() {
		return id;
	}
};
