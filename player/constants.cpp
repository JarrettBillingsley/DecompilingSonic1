/*
SonicPlayer object variables:
============================================================

0x30(w): invincibility (0x01 .. 0x8F means flashing from being hurt; >= 0x90 means invincibility stars)
0x38(w): maybe stores sonic's original Y position when dying?
0x3D(b): index of object being stood on
*/

#define PLAYER_WIDTH 16
#define PLAYER_HALFWIDTH (PLAYER_WIDTH / 2)

enum : ubyte
{
	PlayerRoutine_Init = 0,
	PlayerRoutine_Normal = 2,
	PlayerRoutine_Hurt = 4,
	PlayerRoutine_Dead = 6,
	PlayerRoutine_LevelReset = 8,

	Player_InvincibilityW = 0x30,
	Player_StandingObjectB = 0x3D,
};
