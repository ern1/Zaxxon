#pragma once

// TODO: Improve how handling of different coordinates are done in the future. Camera class should probably be changed/replaced.
// - kanske inte borde vara ett gameobject, utan bara en klass som man kallar på för att sortera objekt, räkna vad på karten som ska renderas är, etc?
// - skulle jag kunna ändra så jag typ har "LevelController" eller något istället för Camera och CameraComponent istället för CameraBehaviorComponent? (dvs så CameraComponent används av LevelController)

class CameraBehaviourComponent : public Component
{
	IsoVector* refPos; // the coordinate the camera follow
	//int offset_y;
	//int h, w;

public:
	virtual ~CameraBehaviourComponent() {}

	virtual void Create(IsoEngine* engine, GameObject* go, std::set<GameObject*>* game_objects, IsoVector* ref_position)
	{
		Component::Create(engine, go, game_objects);
		refPos = ref_position;
		//this->offset_y = offset_y;
	}

	//virtual void Init()
	//{
	//	SDL_Log("CameraBehaviourComponent::Init");
	//}

	virtual void Update(double dt)
	{
		//SDL_Log("CameraBehaviourComponent::Update");
		go->isoPos = *refPos;

	    // Offset
		go->isoPos.x = 0.0;
		go->isoPos.y -= DISPLAY_HEIGHT / 2 - 20.; // 110
		go->isoPos.z = 0.0;

		go->position = go->isoPos.to2D();
	}
};

class Camera : public GameObject
{
	double w, h;             // viewport size
	double level_w, level_h; // size of level
	Vector2D level_offset;   // origin of coordinate system

public:
	virtual ~Camera() { SDL_Log("Camera::~Camera"); }

	Vector2D get_level_offset() { return level_offset; }
	
	//virtual void Create(AvancezLib* engine)
	//{
	//	SDL_Log("Level::Create");
	//}

	virtual void Init(double w, double h, double level_w, double level_h)
	{
		SDL_Log("Camera::Init");
		GameObject::Init();

		this->level_w = level_w;
		this->level_h = level_h;
		this->w = w;
		this->h = h;
		this->level_offset = { 0.0, this->level_h - this->h };
	}
};
