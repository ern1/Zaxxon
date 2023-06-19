#pragma once

#include "isoVector.h"

/****** DISPLAY ETC ******/
// default 224 x 256 (hade ungefär kamera-offset 30, -120 då)
const unsigned int DISPLAY_WIDTH  = 448;
const unsigned int DISPLAY_HEIGHT = 256;

/****** CAMERA / LEVEL ******/
// (currently only with support for one level in mind. Level info should probably be stored in separate files?)
const int LEVEL_HEIGHT  = 1008;
const int LEVEL_WIDTH   = 1816;
const int LEVEL_MIN_X   = 100;   // iso
const int LEVEL_MAX_X   = 300;   // iso
const int LEVEL_END_Y   = -1250; // iso
const int LEVEL_START_Y = -180;  // iso

/****** PLAYER ******/
const unsigned int PLAYER_LIVES              = 3;
const double       PLAYER_SPEED              = 70.0;
const double       PLAYER_SPEED_Z            = 40.0;
const unsigned int PLAYER_ENERGY_MAX         = 5;
const float        PLAYER_ENERGY_REGEN_TIME  = 0.3f;
const unsigned int PLAYER_FUEL_MAX           = 16 * 8;
const float        PLAYER_FUEL_TIME          = 0.5f;    // time for fuel to decrease by 1 unit
const float        PLAYER_FIRE_TIME_INTERVAL = .15f;
const double       PLAYER_INVINCIBLE_TIME    = 1.;
const double       PLAYER_INVINCIBLE_RENDER_INTERVAL = 0.2;

/****** WEAPONS / SHIPS ******/
const double       LASER_SPEED       = 350.0;
const double       LASER_DISTANCE    = 500.0; // this makes sense for now
const int          SHIP_MIN_ALTITUDE = 10;
const int          SHIP_MAX_ALTITUDE = 50;

/****** ANIMATIONS ******/
int          LASER_ANIM_SEQ[]          = { 0, 1, 2, 3 };
const double LASER_ANIM_FRAMETIME      = 0.07;
const int    ANIM_EXP_WEAK_ID          = 0;
int          ANIM_EXP_WEAK_SEQ[]       = { 5, 4, 3, 2, 1, 0 };
const double ANIM_EXP_WEAK_FRAMETIME   = 0.05;
const int    ANIM_EXP_STRONG_ID        = 1;
int          ANIM_EXP_STRONG_SEQ[]     = { 6, 7, 8, 9 };
const double ANIM_EXP_STRONG_FRAMETIME = 0.1;
const int    ANIM_EXP_OTHER_ID = 2;
int          ANIM_EXP_OTHER_SEQ[] = { 9, 8 };
const double ANIM_EXP_OTHER_FRAMETIME = 0.1;

/****** AUDIO ******/
const unsigned int WAV_HOMING_MISSILE      = 3;   // 0  - Homing Missile
const unsigned int WAV_BASE_MISSILE        = 2;   // 1  - Base Missile
const unsigned int WAV_LASER               = 1;   // 2  - Laser (force field)
const unsigned int WAV_BATTLESHIP          = 0;   // 3  - Battleship (end of level boss)
const unsigned int WAV_S_EXP               = 11;  // 4  - S-Exp (enemy explosion)
const unsigned int WAV_M_EXP               = 10;  // 5  - M-Exp (ship explosion)
const unsigned int WAV_CANNON              = 8;   // 6  - Cannon (ship fire)
const unsigned int WAV_SHOT                = 23;  // 7  - Shot (enemy fire)
const unsigned int WAV_ALARM_2             = 21;  // 8  - Alarm 2 (target lock)
const unsigned int WAV_ALARM_3             = 20;  // 9  - Alarm 3 (low fuel)
const unsigned int WAV_INIT_BG_NOISE       = 5;   // 10 - initial background noise
const unsigned int WAV_LOOP_ASTEROID_NOISE = 4;   // 11 - looped asteroid noise
const unsigned int WAV_SANIC_HS            = 94;
