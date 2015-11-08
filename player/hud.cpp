
void HUD_Update()
{
	if(f_debugmode)
		HudDb_XY();
	else if(f_scorecount)
	{
		f_scorecount = false;
		Hud_Score(0xDC80, v_score);
	}

	if(f_ringcount)
	{
		if(BTST(f_ringcount, 0x80))
			Hud_DrawZeroRings();

		f_ringcount = false;
		Hud_Rings(0xDF40, v_rings);
	}

	if(!f_debugmode && f_timecount && !f_pause)
	{
		if(v_timemin == 9 && v_timesec == 59 && v_timecent == 59)
		{
			// Time over
			f_timecount = false;
			Player_Kill(v_player, v_player);
			f_timeover = true;
			return;
		}

		if(IncTo(v_timecent, 60))
		{
			v_timecent = 0;

			if(IncTo(v_timesec, 60))
			{
				v_timesec = 60;
				IncTo(v_timemin, 9);
			}

			Hud_Mins(0xDE40, v_timemin);
			Hud_Secs(0xDEC0, v_timesec);
		}
	}

	// Dupe
	if(f_lifecount)
	{
		f_lifecount = false;
		Hud_Lives();
	}

	if(f_endactbonus)
	{
		f_endactbonus = false;
		VDP_SetAddr(0xAE00, VDP_VRAM_Write);
		Hud_TimeRingBonus(v_timebonus);
		Hud_TimeRingBonus(v_ringbonus);
	}
}

const ubyte Hud_TilesBase[15] = { 0x16, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0, 0x14, 0, 0, 0xFF, 0xFF, 0 };
const ubyte Hud_TilesZero[3] = { 0xFF, 0xFF, 0 };

void Hud_DrawZeroRings()
{
	VDP_SetAddr(0xDF40, VDP_VRAM_Write);
	Hud_DrawFixedTiles(Hud_TilesZero, sizeof(Hud_TilesZero));
}

void Hud_Init()
{
	Hud_Lives()
	VDP_SetAddr(0xDC40, VDP_VRAM_Write);
	Hud_DrawFixedTiles(Hud_TilesBase, sizeof(Hud_TilesBase));
}

//                               a2          d2
void Hud_DrawFixedTiles(ubyte* tiles, uint count)
{
	for(int i = 0; i < count; i++)
	{
		if(tiles[i] & 0x80)
		{
			for(int j = 0; j < 16; j++)
				VDP_Data(0);
		}
		else
		{
			for(int j = 0; j < 16; j++)
				VDP_Data(Art_Hud[tiles[i]][j]);
		}
	}
}

void HudDb_XY()
{
	VDP_SetAddr(0xDC40, VDP_VRAM_Write);
	HudDb_XY2(v_screenposx, v_player->x);
	HudDb_XY2(v_screenposy, v_player->y);
}

void HudDb_XY2(int v1, int v2)
{
	for(int i = 0; i < 8; i++)
	{
		// This looks HORRENDOUS but it's getting alternating descending nybbles from each v1 and v2
		auto digit = ((i & 1 ? v2 : v1) >> (8 * (3 - i))) & 0xF;

		if(digit >= 10)
			digit += 7;

		for(int j = 0; j < 8; j++)
			VDP_Data(Art_Text[digit][j]);
	}
}

const uint Hud_100000[] =  { 100000, 10000, 1000, 100, 10, 1 };
const uint Hud_10000[] =   {         10000, 1000, 100, 10, 1 };
const uint Hud_1000[] =    {                1000, 100, 10, 1 };
const uint Hud_100[] =     {                      100, 10, 1 };
const uint Hud_10[] =      {                           10, 1 };
const uint Hud_1[] =       {                               1 };

//                   d0         d1
void Hud_Rings(uint addr, uint val)
{
	Hud_DrawNumber(addr, val, Hud_100, 3);
}

//                   d0         d1
void Hud_Score(uint addr, uint val)
{
	Hud_DrawNumber(addr, val, Hud_100000, 6);
}

//                        d0         d1          a2            d6
void Hud_DrawNumber(uint addr, uint val, uint* radix, uint numDigits)
{
	bool drewFirstDigit = false;

	for(int i = 0; i < numDigits; i++)
	{
		int digit = 0;

		while(val >= radix[i])
		{
			val -= radix[i]
			digit++;
		}

		if(digit != 0)
			drewFirstDigit = true;

		if(drewFirstDigit)
		{
			VDP_SetAddr(addr + (i * 0x40), VDP_VRAM_Write);

			for(int j = 0; j < 16; j++)
				VDP_Data(Art_Hud[digit][j]);
		}
	}
}

//                  d0         d1
void Hud_Mins(uint addr, uint val)
{
	Hud_DrawNumberWithLeading0s(addr, val, Hud_1, 1);
}

void Hud_Secs()
{
	Hud_DrawNumberWithLeading0s(addr, val, Hud_10, 2);
}

//                                     d0         d1           a2           d6
void Hud_DrawNumberWithLeading0s(uint addr, uint val, uint* radix, uint numDigits)
{
	for(int i = 0; i < numDigits; i++)
	{
		int digit = 0;

		while(val >= radix[i])
		{
			val -= radix[i]
			digit++;
		}

		VDP_SetAddr(addr + (i * 0x40), VDP_VRAM_Write);

		for(int j = 0; j < 16; j++)
			VDP_Data(Art_Hud[digit][j]);
	}
}

void Hud_TimeRingBonus()
{
	bool drewFirstDigit = false;

	for(int i = 0; i < 4; i++)
	{
		int digit = 0;

		while(val >= Hud_1000[i])
		{
			val -= Hud_1000[i]
			digit++;
		}

		if(digit != 0)
			drewFirstDigit = true;

		if(drewFirstDigit)
		{
			for(int j = 0; j < 16; j++)
				VDP_Data(Art_Hud[digit][j]);
		}
		else
		{
			for(int j = 0; j < 16; j++)
				VDP_Data(0);
		}
	}
}

void Hud_Lives()
{
	uint val = v_lives;
	bool drewFirstDigit = false;

	for(int i = 0; i < 2; i++)
	{
		VDP_SetAddr(0xFBA0 + (i * 0x40), VDP_VRAM_Write);
		int digit = 0;

		while(val >= Hud_10[i])
		{
			val -= Hud_10[i]
			digit++;
		}

		if(digit != 0)
			drewFirstDigit = true;

		if(i == 1 || drewFirstDigit)
		{
			for(int j = 0; j < 8; j++)
				VDP_Data(Art_LivesNums[digit][j]);
		}
		else
		{
			for(int j = 0; j < 8; j++)
				VDP_Data(0);
		}
	}
}