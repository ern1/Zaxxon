#define _CRT_SECURE_NO_WARNINGS

#include "stdio.h"
#include <set>

#include "iso_engine.h"
#include "audio_manager.h"
#include "object_pool.h"
#include "constants.h"

float game_speed = 1.f;
int render_scale = 2;

#include "component.h"
#include "game_object.h"

//#include "projectile.h"
#include "laser.h"
#include "player.h"
#include "enemy.h"
#include "obstacle.h"
#include "vfx.h"
#include "camera.h"

#include "game.h"

#define SHOW_FPS 1
#if SHOW_FPS
float frameTimeSum = 0.f, averageFPS = 0.f;
int frameCount = 0;
char buffer[32];
#endif

int main(int argc, char** argv)
{
	IsoEngine engine;
	AudioManager audio_manager;

	engine.init(DISPLAY_WIDTH, DISPLAY_HEIGHT);
	engine.setRenderScale(render_scale);

	audio_manager.init(80, 10);

	Game game;
	game.Create(&engine, &audio_manager);
	game.Init();

	float lastTime = engine.getElapsedTime();
	while (true)
	{
		float newTime = engine.getElapsedTime();
		float dt = newTime - lastTime;
		dt = dt * game_speed;
		lastTime = newTime;

#if SHOW_FPS
		frameTimeSum += dt;
#endif

		engine.processInput();
		game.Update(dt);
		game.Draw();

#if SHOW_FPS
		// Calculate average FPS once every second
		if (frameCount++ == 200) {
			averageFPS = static_cast<float>(frameCount) / frameTimeSum;
			frameTimeSum = 0.0;
			frameCount = 0;
		}
		std::snprintf(buffer, 20, "fps: %.2f", averageFPS);
		engine.drawSmallText(DISPLAY_WIDTH - 80, DISPLAY_HEIGHT - 32, buffer, { 128, 128, 128, 128 });
#endif

	}

	// Clean up
	game.Destroy();
	engine.destroy();
	audio_manager.destroy();

	return 0;
}
