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

bool PlayerFlipped()
{
	return BTST(v_player->status, ObjStatus::Flip);
}

bool PlayerInAir()
{
	return BTST(v_player->status, ObjStatus::Air);
}

bool PlayerUnderwater()
{
	return BTST(v_player->status, ObjStatus::Underwater);
}

bool PlayerDead()
{
	return v_player->routine >= PlayerRoutine_Dead;
}

bool PlayerControllable()
{
	return v_player->routine < PlayerRoutine_Hurt;
}

void SetPlayerHurt()
{
	v_player->routine = PlayerRoutine_Hurt;
}

void SetPlayerDead()
{
	v_player->routine = PlayerRoutine_Dead;
}

void SetPlayerAir()
{
	BSET(v_player->status, ObjStatus::Air);
}

void SetPlayerAnim_Drowning()
{
	v_player->anim = 0x17;
}

const ubyte Sizes[][] =
{
	{  20,  20 },  // 0x01
	{  12,  20 },  // 0x02
	{  20,  12 },  // 0x03
	{   4,  16 },  // 0x04
	{  12,  18 },  // 0x05
	{  16,  16 },  // 0x06
	{   6,   6 },  // 0x07
	{  24,  12 },  // 0x08
	{  12,  16 },  // 0x09
	{  16,  12 },  // 0x0A
	{   8,   8 },  // 0x0B
	{  20,  16 },  // 0x0C
	{  20,   8 },  // 0x0D
	{  14,  14 },  // 0x0E
	{  24,  24 },  // 0x0F
	{  40,  16 },  // 0x10
	{  16,  24 },  // 0x11
	{   8,  16 },  // 0x12
	{  32, 112 },  // 0x13
	{  64,  32 },  // 0x14
	{ 128,  32 },  // 0x15
	{  32,  32 },  // 0x16
	{   8,   8 },  // 0x17
	{   4,   4 },  // 0x18
	{  32,   8 },  // 0x19
	{  12,  12 },  // 0x1A
	{   8,   4 },  // 0x1B
	{  24,   4 },  // 0x1C
	{  40,   4 },  // 0x1D
	{   4,   8 },  // 0x1E
	{   4,  24 },  // 0x1F
	{   4,  40 },  // 0x20
	{   4,  32 },  // 0x21
	{  24,  24 },  // 0x22
	{  12,  24 },  // 0x23
	{  72,   8 },  // 0x24
};

// Original seems to return some flag in Carry but no code ever checks it, so..
//                        a0
void ReactToItem(Object* player)
{
	auto playerLeft = player->x - PLAYER_HALFWIDTH;
	auto playerTop = player->y - (player->height - 3);
	auto playerHeight = 2 * (player->height - 3);

	if(player->frame == Frame_Duck)
	{
		playerTop += 12;
		playerHeight = 20;
	}

	for(int i = 0; i < NUM_LEVEL_OBJECTS)
	{
		auto obj = &v_lvlobjspace[i];

		if(Object_IsVisible(obj) && obj->colType != 0)
		{
			auto size = Sizes[obj->colType & 0x3F];
			auto objLeft = obj->x - size[0];
			auto objRight = obj->x + size[0];
			auto leftDiff = objLeft - playerLeft;

			// Overlapping horizontally?
			if((leftDiff >= 0 && leftDiff <= PLAYER_WIDTH) || (objRight > playerLeft))
			{
				auto objTop = obj->y - size[1];
				auto objBottom = obj->y + size[1];
				auto topDiff = objTop - playerTop;

				// Overlapping vertically?
				if((topDiff >= 0 && topDiff <= playerHeight) || (objBottom > playerTop))
				{
					// Dispatch based on collision type
					if(obj->colType < 0x40)             // 00 .. 3F (enemies)
						return React_Enemy(player, obj);
					else if(obj->colType >= 0xC0)       // C0 .. FF (specials)
						return React_Special(player, obj, playerHeight - (objBottom - playerTop), playerLeft);
					else if(obj->colType >= 0x80)       // 80 .. BF (hurties)
						return React_ChkHurt(player, obj);
					else if((obj->colType & 0x3F) == 6) // 40 .. 7F (powerups)
						return React_Monitor(player, obj);
					else if(VAR_W(player, Player_InvincibilityW) < 0x90) // sonic NOT invincible?
						obj->routine += 2;

					return;
				}
			}
		}
	}
}

void React_Monitor(Object* player, object* obj)
{
	if(player->velY <= 0) // moving up?
	{
		// Bonk Sonic's head on the underside of the monitor
		if(player->y - 16 >= obj->y)
		{
			player->velY = -player->velY;
			obj->velY = -0x180;

			if(obj->routine2 == 0)
				obj->routine2 = 4;
		}
	}
	else if(player->anim == Anim::Roll)
	{
		// Pop the monitor
		player->velY = -player->velY;
		obj->routine += 2;
	}
}

// remember, the 0 at the end of the score is "fake", so these are 100, 200, 500, 1000
const ushort Points[] = { 10, 20, 50, 100 };

void React_Enemy(Object* player, Object* obj)
{
	if(!v_invinc && player->anim != Anim::Roll)
		React_ChkHurt(player, obj); // Sonic can only hurt things when rolled into a ball
	else if(obj->colProp != 0) // obj->colProp is used to count hits on Robotnik bosses
	{
		// Bounce sonic off boss
		player->velX = -player->velX / 2;
		player->velY = -player->velY / 2;
		obj->colType = 0;

		// Did we finish him off?
		obj->colProp--;
		if(obj->colProp == 0)
			BSET(obj->status, ObjStatus::Dead);
	}
	else
	{
		BSET(obj->status, ObjStatus::Dead);

		// Hopping on successive things increases point bonus
		auto pointsIdx = v_itembonus > 6 ? 6 : v_itembonus;
		v_itembonus += 2;
		VAR_W(obj, 0x3E) = pointsIdx;
		AddPoints((v_itembonus >= 32) ? 1000 : Points[pointsIdx / 2]);

		// Change object to explosion
		obj->id = ID::ExplosionItem;
		obj->routine = 0;

		// Bounce player
		if(player->velY < 0)
			player->velY += 0x100; // hitting from below, slow down speed a bit
		else if(player->y >= obj->y)
			player->velY -= 0x100; // hitting from side, bounce up a little
		else
			player->velY = -player->velY; // hitting from above, bounce up a lot
	}
}

void React_Special(Object* player, Object* obj, int ydiff, int playerLeft)
{
	if(obj->colType == 0xCB) // Caterkiller
		React_Caterkiller(player, obj);
	else if(obj->colType == 0xCC) // Yadrin (spiny-backed crab thing - have to check for spine collision)
	{
		// Is the player close enough to the top of the Yadrin?
		if(ydiff < 8)
		{
			auto spikeLeft = obj->x - (Object_IsFlipped(obj) ? 4 : 20);
			auto spikeRight = spikeLeft + 24;
			auto leftDiff = spikeLeft - playerLeft;

			// Overlapping horizontally?
			if((leftDiff >= 0 && leftDiff <= PLAYER_WIDTH) || (spikeRight > playerLeft))
			{
				React_ChkHurt(player, obj);
				return;
			}
		}

		// Otherwise, just react like normal
		React_Enemy(player, obj);
	}
	else if(obj->colType == 0xD7 || obj->colType == 0xE1) // pinball bumper (SYZ) or breaking waterway pole (LZ)
		obj->colProp++;
}

void React_Caterkiller(Object* player, Object* obj)
{
	BSET(obj->status, ObjStatus::Dead);
	React_ChkHurt(player, obj);
}

void React_ChkHurt(Object* player, Object* obj)
{
	if(v_invinc || VAR_W(player, Player_InvincibilityW) != 0) // invincible or hurt-flashing?
		return;

	HurtSonic(player, obj);
}

//                       a0             a2
void HurtSonic(Object* player, Object* obj)
{
	// If he has no shield..
	if(!v_shield)
	{
		// if he has rings, make the rings spill out
		if(v_rings)
		{
			if(auto ringLoss = FindFreeObj())
			{
				ringLoss->id = ID::RingLoss;
				ringLoss->x = player->x;
				ringLoss->y = player->y;
			}
		}
		else if(!f_debugmode) // otherwise, kill him (if debug mode isn't on)
		{
			KillSonic(player, obj);
			return;
		}
	}

	// Hurt sonic
	v_shield = 0;
	SetPlayerHurt();
	Sonic_ResetOnFloor(player);
	SetPlayerAir();

	// Bounce him away
	if(PlayerUnderwater())
	{
		player->velY = -0x200;
		player->velX = -0x100;
	}
	else
	{
		player->velY = -0x400;
		player->velX = -0x200;
	}

	// Make him bounce right if he's to the right of the object
	if(player->x > obj->x)
		player->velX = -player->velX;

	player->inertia = 0;
	player->anim = Anim::Hurt;
	VAR_W(player, Player_InvincibilityW) = 120; // 2 seconds of invincibility;

	// Bwah
	if(obj->id == ID::Spikes || obj->id == ID::Harpoon)
		PlaySound_Special(SFX::HitSpikes);
	else
		PlaySound_Special(SFX::Death);
}

//                       a0             a2
void KillSonic(Object* player, Object* killer)
{
	if(v_debuguse)
		return;

	// Set him up to do the death bounce
	v_invinc = 0;
	SetPlayerDead();
	Sonic_ResetOnFloor(player);
	SetPlayerAir();
	player->velY = -0x700;
	player->velX = 0;
	player->inertia = 0;
	VAR_W(player, 0x38) = player->y;
	player->anim = Anim::Death;
	BSET(player->gfx, 0x80);

	// Bwah
	if(killer->id == ID::Spikes)
		PlaySound_Special(SFX::HitSpikes);
	else
		PlaySound_Special(SFX::Death);
}