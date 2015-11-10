#define PLAYER_WIDTH 16
#define PLAYER_HALFWIDTH (PLAYER_WIDTH / 2)

enum : ubyte
{
	PlayerRoutine_Init = 0,
	PlayerRoutine_Normal = 2,
	PlayerRoutine_Hurt = 4,
	PlayerRoutine_Dead = 6,
	PlayerRoutine_LevelReset = 8,

	Player_InvincibilityW = 0x30,   // 0x01..0x8F means flashing from being hurt; >= 0x90 means invincible
	Player_InvTimeW = 0x32,         // Timer for invincibility
	Player_ShoeTimeW = 0x34,        // Timer for speed shoes
	Player_LastAngle1B = 0x36,      // Last floor angle (3 = "no angle")
	Player_LastAngle2B = 0x37,      // Last floor angle (secondary, when standing on two tiles) (3 = "no angle")

	// This variable is weird. When sonic is alive, it's accessed as a byte, only set to 0, and checked in a few places
	// when doing physics related to slopes. My guess is that these are the remnants of some previous use of this var.
	Player_DeathOrigYW = 0x38,      // Stores original Y pos when dying

	Player_DeathResetTimerW = 0x3A, // When you die, how long before you respawn
	Player_IsJumpingB = 0x3C,       // true if the player is in the air because they jumped
	Player_StandingObjectB = 0x3D,  // Index of object last stood on
	Player_SomeTimerW = 0x3E,       // ?? Something to do with locking L/R controls after falling off a slope?
};
