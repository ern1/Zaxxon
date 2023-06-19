#pragma once

class VFX : public GameObject
{
	RenderSpriteSheetComponent* render;
	
public:
	virtual ~VFX() { SDL_Log("VFX::~VFX"); }

	virtual void Init(int animation_id, IsoVector pos, bool loop_animation)
	{
		SDL_Log("VFX::Init");
		GameObject::Init();

		this->position = pos.to2D();

		render = this->GetComponent<RenderSpriteSheetComponent*>();
		render->startAnimation(animation_id, loop_animation);
	}

	virtual void Update(float dt)
	{
		GameObject::Update(dt);

		if (!render->isAnimationActive())
			enabled = false;
	}
};
