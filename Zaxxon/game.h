#pragma once
#include <algorithm>
#include <map>
#include <unordered_map>

class Game : public GameObject
{
	std::set<GameObject*> game_objects;
	std::set<GameObject*> game_objects_vfx; // TODO: temp

	IsoEngine* engine;
	AudioManager* audio_manager;
	
	Player* player;
	Camera* cam;

	ObjectPool<Laser> laser_pool;
	ObjectPool<Enemy> enemy_pool;
	ObjectPool<Obstacle> obstacle_pool;
	ObjectPool<VFX> vfx_pool; // For animations not connected to any game object

	Spritesheet* ss_ui;
	Spritesheet* ss_ui_altitude;
	bool game_over;
	unsigned int score = 0;
	int audio_bg_ch; // The audio channel for the background sound effect

public:

	virtual void Create(IsoEngine* engine, AudioManager* audio_manager)
	{
		SDL_Log("Game::Create");

		this->engine = engine;
		this->audio_manager = audio_manager;

		cam = new Camera();
		player = new Player();
		
	    // Player
		auto* player_behaviour = new PlayerBehaviourComponent();
		player_behaviour->Create(engine, player, &game_objects, &laser_pool);
		auto* player_coll = new CollideComponent();
		player_coll->Create(engine, player, &game_objects, { 10, -15, 10 });
		player_coll->AddCollObjects(reinterpret_cast<ObjectPool<GameObject>*>(&enemy_pool));
		player_coll->AddCollObjects(reinterpret_cast<ObjectPool<GameObject>*>(&obstacle_pool));
		auto* player_render = new RenderSpriteSheetComponent();
		player_render->Create(engine, player, &game_objects, "assets/sprites/player_ship-sheet.png", 12, &(cam->position));
		auto* shadow_render = new RenderComponent();
		shadow_render->Create(engine, player, &game_objects, "assets/sprites/player/player_shadow_blur.png", &(cam->position));
		shadow_render->setRenderFlat(true);
		player->Create();
		player->AddComponent(player_behaviour);
		player->AddComponent(player_coll);
		player->AddComponent(player_render);
		player->AddComponent(shadow_render);
		player->AddReceiver(this);

	    // Camera
		auto* cam_behaviour = new CameraBehaviourComponent();
		cam_behaviour->Create(engine, cam, &game_objects, &(player->isoPos));
		auto* cam_render = new RenderLevelComponent();
		cam_render->Create(engine, cam, &game_objects, "assets/sprites/levels/level_1.png", engine->getWidth(), engine->getHeight(), 0, LEVEL_HEIGHT - engine->getHeight());
		cam->Create();
		cam->AddComponent(cam_behaviour);
		cam->AddComponent(cam_render);
		
		// Laser
		laser_pool.Create(42);
		for (auto& laser : laser_pool.pool)
		{
			auto* behaviour = new LaserBehaviourComponent();
			behaviour->Create(engine, laser, &game_objects);
			auto* coll = new CollideComponent();
			coll->Create(engine, laser, &game_objects, { 5, -15, 5 });
			auto* render = new RenderSpriteSheetComponent();
			render->Create(engine, laser, &game_objects, "assets/sprites/player_projectile-sheet.png", 4, &(cam->position));
			render->addAnimation(0, {  LASER_ANIM_SEQ, 4, LASER_ANIM_FRAMETIME });
			laser->Create();
			laser->AddComponent(behaviour);
			laser->AddComponent(coll);
			laser->AddComponent(render);
		}

		// Enemy
		enemy_pool.Create(10);
		for (auto& enemy : enemy_pool.pool)
		{
			auto* behaviour = new EnemyBehaviourComponent();
			behaviour->Create(engine, enemy, &game_objects, &laser_pool);
			auto* coll = new CollideComponent();
			coll->Create(engine, enemy, &game_objects, { 10, -10, 10 });
			coll->AddCollObjects(reinterpret_cast<ObjectPool<GameObject>*>(&laser_pool));

			auto* render = new RenderSpriteSheetComponent();
			render->Create(engine, enemy, &game_objects, "assets/sprites/enemy_canon-sheet.png", 2, &(cam->position));
			enemy->Create();
			enemy->AddComponent(behaviour);
			enemy->AddComponent(coll);
			enemy->AddComponent(render);
			enemy->AddReceiver(this);
			game_objects.insert(enemy); // TODO: Fix? Should probably not be here, but works
		}

		// Obstacles
		obstacle_pool.Create(10);
		for (auto& obstacle : obstacle_pool.pool)
		{
			//auto* behaviour = new ObstacleBehaviourComponent();
			//behaviour->Create(engine, obstacle, &game_objects);
			auto* coll = new CollideComponent();
			coll->Create(engine, obstacle, &game_objects, { 30, -23, 20 }); //15, -10, 10; 15, 10, 15
			coll->AddCollObjects(reinterpret_cast<ObjectPool<GameObject>*>(&laser_pool));
			auto* render = new RenderSpriteSheetComponent();
			render->Create(engine, obstacle, &game_objects, "assets/sprites/obstacles-sheet.png", 4, &(cam->position));
			obstacle->Create();
			//obstacle->AddComponent(behaviour);
			obstacle->AddComponent(coll);
			obstacle->AddComponent(render);
			obstacle->AddReceiver(this);
			game_objects.insert(obstacle); // TODO: vet ej om jag bör ha det såhär... får va tillsvidare
		}

		// VFX (animations)
		vfx_pool.Create(15);
		for (auto& vfx : vfx_pool.pool)
		{
			auto* vfx_render = new RenderSpriteSheetComponent();
			vfx_render->Create(engine, vfx, &game_objects, "assets/sprites/vfx-sheet.png", 17, &(cam->position));
			vfx_render->addAnimation(ANIM_EXP_WEAK_ID, ANIM_EXP_WEAK_SEQ, 6, ANIM_EXP_WEAK_FRAMETIME);
			vfx_render->addAnimation(ANIM_EXP_STRONG_ID, ANIM_EXP_STRONG_SEQ, 4, ANIM_EXP_STRONG_FRAMETIME);
			vfx_render->addAnimation(ANIM_EXP_OTHER_ID, ANIM_EXP_OTHER_SEQ, 2, ANIM_EXP_OTHER_FRAMETIME);
			vfx->Create();
			vfx->AddComponent(vfx_render);
		}

		//game_objects.insert(cam);
		game_objects.insert(player);

		// Sprites for UI
		ss_ui = engine->createSpritsheet("assets/sprites/ui-sheet.png", 15);
		ss_ui_altitude = engine->createSpritsheet("assets/sprites/ui_altitude-sheet.png", 25);

		// Load audio files
		audio_manager->loadWAV("assets/sound/01.wav", WAV_LASER);
		audio_manager->loadWAV("assets/sound/04_low.wav", WAV_LOOP_ASTEROID_NOISE);
		audio_manager->loadWAV("assets/sound/11.wav", WAV_S_EXP);
		audio_manager->loadWAV("assets/sound/10.wav", WAV_M_EXP);
		audio_manager->loadMusic("assets/sound/ice-crap-zone.wav", WAV_SANIC_HS); // TEMP...
	}

	virtual void Init()
	{
		cam->Init(engine->getWidth(), engine->getHeight(), LEVEL_WIDTH, LEVEL_HEIGHT);
		player->Init();
		InitEntities();

		id = "Zaxxon";
		enabled = true;
		game_over = false;
		score = 0;

		audio_manager->playMusic(WAV_SANIC_HS, 1000, true); // TEMP...
		audio_bg_ch = audio_manager->playSound(WAV_LOOP_ASTEROID_NOISE, true);
	}

	virtual void Update(float dt)
	{
		IsoEngine::KeyStatus keys;
		engine->getKeyStatus(keys);
		if (keys.esc) {
			Destroy();
			engine->quit();
		}

		if (IsGameOver())
		{
			dt = 0.f;
			audio_manager->pauseChannel(audio_bg_ch);
			
			if (keys.enter)
			{
				Init();
				SDL_Log("Restarting game...");
			}
		}

		// TODO: Sort sprites
		//std::unordered_map<double, GameObject*> ordered;
		//for (auto go = game_objects.begin(); go != game_objects.end(); go++)
		//	ordered[(*go)->position.y] = (*go);
		//for (auto go = ordered.begin(); go != ordered.end(); ++go)
		//	(*go).second->Update(dt);

		cam->Update(dt);
		for (auto go : game_objects)
			go->Update(dt);
		for (auto go : game_objects_vfx)
			go->Update(dt);

		// Check player position, if at the end of level then move to beginning again
		if (player->isoPos.y < LEVEL_END_Y)
			LoopLevel();
	}

	virtual void Draw()
	{
		/* UI */
		char msg[128];
		int w = engine->getWidth(), h = engine->getHeight();

		// Score
		std::snprintf(msg, 128, "%*s%06u", -4, "TOP", 100);
		engine->drawText(8, 8, msg, { 0, 0xFF, 0xFF });
		std::snprintf(msg, 128, "%*s%06u", -5, "1UP", score);
		engine->drawText(8, 24, msg);

		// Fuel
		std::snprintf(msg, 128, "FUEL");
		engine->drawText(w / 2 - 16, h - 16, msg, { 0, 128, 0 });
		std::snprintf(msg, 128, "E                      F");
		engine->drawText(w / 2 + 32, h - 16, msg, { 0xFF, 0, 0 });

		int x = w / 2 + 46;
		int fuel = player->fuel;
		while (fuel > 0)
		{
			if (fuel >= 8 )
			{   // Draw full sized sprite
				ss_ui->draw(x, h - 14, 11);
				x += 8;
				fuel -= 8;
			}
			else
			{	// Draw non-full sized sprite based on how much fuel is left
				ss_ui->draw(x, h - 14, 3 + fuel);
				break;
			}
		}
		
		// Lifes
		for (int i = 0; i < player->lives; i++)
			ss_ui->draw((w / 2 - 16) + i * 18, h - 32, 13);


		// Altitude
		int altitude = std::max(5, static_cast<int>(player->isoPos.z) * (100 / 50)); // in percent
		int bot = h / 2 + ss_ui_altitude->h * 5;

		for (int i = 0; i < 10; i++)
			ss_ui_altitude->draw(4, bot - i * ss_ui_altitude->h, 9);

		x = 0;
		while (altitude >= 0)
		{
			if (altitude >= 10)
			{
				ss_ui_altitude->draw(4, bot - x, 3);
				x += ss_ui_altitude->h;
				altitude -= 10;
			}
			else
			{
				ss_ui_altitude->draw(4, bot - x, altitude + (altitude >= 8 ? 0 : 17));
				break;
			}

		}
		
		int L = bot + ss_ui_altitude->h;
		std::snprintf(msg, 128, "L");
		engine->drawText(4, L, msg, { 0, 128, 0 });
		std::snprintf(msg, 128, "H");
		engine->drawText(4, h - L, msg, { 0xFF, 0, 0 });

		if (IsGameOver())
		{
			std::snprintf(msg, 128, "***** GAME OVER :( *****");
			engine->drawText(DISPLAY_WIDTH / 2 - 90, DISPLAY_HEIGHT / 2, msg);
			std::snprintf(msg, 128, "Press ENTER to play again :)");
			engine->drawSmallText(DISPLAY_WIDTH / 2 - 65, DISPLAY_HEIGHT / 2 + 20, msg, { 0xFF, 0xFF, 0xFF, 0xE0 });
		}

		engine->swapBuffers();
		engine->clearWindow();
	}

	virtual void Receive(Message m)
	{
		if (IsGameOver())
			return;

		if (m.type == MessageType::PLAY_SOUND)
			audio_manager->playSound(m.data.i);

		else if (m.type == MessageType::GAME_OVER)
		{
		    game_over = true;
			InitVFX(ANIM_EXP_STRONG_ID, m.data.vec, true);
			audio_manager->playSound(WAV_M_EXP);
		}

		else if (m.type == MessageType::PLAYER_HIT)
		{
			SDL_Log("Game::Receive - PLAYER_HIT: %.2f, %.2f, %.2f", m.data.vec.x, m.data.vec.y, m.data.vec.z);

			// Show explosion and play sound effect
			InitVFX(ANIM_EXP_STRONG_ID, m.data.vec);
			audio_manager->playSound(WAV_M_EXP);
		}

		else if (m.type == MessageType::ENEMY_HIT)
		{
			SDL_Log("Game::Receive - ENEMY_HIT: %.2f, %.2f, %.2f", m.data.vec.x, m.data.vec.y, m.data.vec.z);
			
			score += 100;

			InitVFX(ANIM_EXP_WEAK_ID, m.data.vec);
			audio_manager->playSound(WAV_S_EXP);
		}

		else if (m.type == MessageType::OBSTACLE_HIT)
		{
			SDL_Log("Game::Receive - OBSTACLE_HIT: %.2f, %.2f, %.2f", m.data.vec.x, m.data.vec.y, m.data.vec.z);

			InitVFX(ANIM_EXP_OTHER_ID, m.data.vec);
			audio_manager->playSound(WAV_S_EXP);
		}

		// Probably don't use this MessageType here
		else if (m.type == MessageType::HIT)
		{
			SDL_Log("Game::Receive - HIT: %.2f, %.2f, %.2f", m.data.vec.x, m.data.vec.y, m.data.vec.z);
		}

	}

	virtual void Destroy()
	{
		SDL_Log("Game::Destroy");

		cam->Destroy();
		ss_ui->destroy();
		for (auto go = game_objects.begin(); go != game_objects.end(); go++)
			(*go)->Destroy();
	}

	bool IsGameOver()
	{
		return game_over;
	}

	void InitVFX(int id, IsoVector pos, bool loop_animation = false)
	{
		auto* vfx = vfx_pool.FirstAvailable();
		if (vfx != nullptr)
		{
			vfx->Init(id, pos, loop_animation);
			game_objects_vfx.insert(vfx);
		}
	}

	// Initialize enemies and obstacles
	// (only one type of each for now, should also probably be taken care of by some kind of EntitySpawner class)
	void InitEntities()
	{
		
		// Todo: Maybe define enemy values in constants.h instead then use a loop here instead
		/*auto* enemy = enemy_pool.FirstAvailable();
		if (enemy != nullptr) {
			enemy->Init(200, -800);
			game_objects.insert(enemy);
		}*/

		// Enemies (only one type for now)
		enemy_pool.pool[0]->Init({ 200, -800 });
		enemy_pool.pool[1]->Init({ 100, -500 });
		enemy_pool.pool[2]->Init({ 250, -400 });
		enemy_pool.pool[3]->Init({ 200, -700 });
		enemy_pool.pool[4]->Init({ 200, -880 });
		enemy_pool.pool[5]->Init({ 250, -900 });
		enemy_pool.pool[6]->Init({ 100, -990 });
		enemy_pool.pool[7]->Init({ 200, -1100 });
		enemy_pool.pool[8]->Init(IsoVector::from2DtoISO({ 830, 537 }, cam->get_level_offset()));

		// Obstacles
		obstacle_pool.pool[0]->Init({ 180, -550 });
		obstacle_pool.pool[1]->Init({ 230, -550 });
		obstacle_pool.pool[2]->Init({ 230, -800 });
	}

	// TODO: Fix better transition
	void LoopLevel()
	{
		player->isoPos.y -= (LEVEL_END_Y - LEVEL_START_Y);
		InitEntities();
	}
};
