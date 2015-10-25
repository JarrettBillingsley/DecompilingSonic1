enum
{
	ColClass_Enemy =   0x00,
	ColClass_Powerup = 0x40,
	ColClass_Hurt =    0x80,
	ColClass_Special = 0xC0,

	ColClass_Mask =    0xC0,
};

enum
{
	ColSize_Invalid = 0x00,
	ColSize_20x20 =   0x01,
	ColSize_12x20 =   0x02,
	ColSize_20x12 =   0x03,
	ColSize_4x16 =    0x04,
	ColSize_12x18 =   0x05,
	ColSize_16x16 =   0x06,
	ColSize_6x6 =     0x07,
	ColSize_24x12 =   0x08,
	ColSize_12x16 =   0x09,
	ColSize_16x12 =   0x0A,
	ColSize_8x8_A =   0x0B,
	ColSize_20x16 =   0x0C,
	ColSize_20x8 =    0x0D,
	ColSize_14x14 =   0x0E,
	ColSize_24x24_A = 0x0F,
	ColSize_40x16 =   0x10,
	ColSize_16x24 =   0x11,
	ColSize_8x16 =    0x12,
	ColSize_32x112 =  0x13,
	ColSize_64x32 =   0x14,
	ColSize_128x32 =  0x15,
	ColSize_32x32 =   0x16,
	ColSize_8x8_B =   0x17,
	ColSize_4x4 =     0x18,
	ColSize_32x8 =    0x19,
	ColSize_12x12 =   0x1A,
	ColSize_8x4 =     0x1B,
	ColSize_24x4 =    0x1C,
	ColSize_40x4 =    0x1D,
	ColSize_4x8 =     0x1E,
	ColSize_4x24 =    0x1F,
	ColSize_4x40 =    0x20,
	ColSize_4x32 =    0x21,
	ColSize_24x24_B = 0x22,
	ColSize_12x24 =   0x23,
	ColSize_72x8 =    0x24,

	ColSize_Mask =    0x3F,
};

const ubyte Sizes[][] =
{
	{   0,   0 },  // 0x00 (unused)
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

	for(int i = 0; i < NumLevelObjects)
	{
		auto obj = &v_lvlobjspace[i];

		if(Object_IsVisible(obj) && obj->colType != 0)
		{
			auto colSize = obj->colType & ColSize_Mask;
			auto size = Sizes[colSize];
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
					switch(obj->colType & ColClass_Mask)
					{
						case ColClass_Enemy:
							React_Enemy(player, obj);
							break;

						case ColClass_Powerup:
							if(colSize == ColSize_16x16)
								React_Monitor(player, obj);
							else if(VAR_W(player, Player_InvincibilityW) < 0x90) // sonic NOT invincible?
								obj->routine += 2;
							break;

						case ColClass_Hurt:
							React_ChkHurt(player, obj);
							break;

						case ColClass_Special:
							React_Special(player, obj, playerHeight - (objBottom - playerTop), playerLeft);
							break;
					}

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
		if(DecToZero(obj->colProp))
			BSET(obj->status, ObjStatus_Dead);
	}
	else
	{
		BSET(obj->status, ObjStatus_Dead);

		// Hopping on successive things increases point bonus
		auto pointsIdx = v_itembonus > 6 ? 6 : v_itembonus;
		v_itembonus += 2;
		VAR_W(obj, 0x3E) = pointsIdx;
		AddPoints((v_itembonus >= 32) ? 1000 : Points[pointsIdx / 2]);

		// Change object to explosion
		obj->id = ID_ExplosionItem;
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
	auto colSize = obj->colType & ColSize_Mask;

	if(colSize == ColSize_8x8_A) // Caterkiller
		React_Caterkiller(player, obj);
	else if(colSize == ColSize_20x16) // Yadrin (spiny-backed crab thing)
	{
		// Have to check for spine collision

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
	else if(colSize == ColSize_8x8_B || colSize == ColSize_4x32) // pinball bumper (SYZ) or breaking waterway pole (LZ)
		obj->colProp++;
}

void React_Caterkiller(Object* player, Object* obj)
{
	BSET(obj->status, ObjStatus_Dead);
	React_ChkHurt(player, obj);
}

void React_ChkHurt(Object* player, Object* obj)
{
	if(v_invinc || VAR_W(player, Player_InvincibilityW) != 0) // invincible or hurt-flashing?
		return;

	HurtSonic(player, obj);
}