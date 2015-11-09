void LZWaterFeatures()
{
	if(v_zone == Zone_LZ)
	{
		if(!f_nobgscroll && !Player_IsDead())
		{
			LZWindTunnels();
			LZWaterSlides();
			LZDynamicWater();
		}

		v_waterpos1 = (v_oscillate[2] / 2) + v_waterpos2;

		if(v_waterpos1 > v_screenposy)
		{
			v_hbla_line = min(v_waterpos1 - v_screenposy, 223);
			f_wtr_state = false;
		}
		else
		{
			v_hbla_line = 223;
			f_wtr_state = true;
		}
	}
}

const ushort WaterHeight[] = { 0xB8, 0x328, 0x900, 0x228 };

void LZDynamicWater()
{
	switch(v_act)
	{
		case Act_1: DynWater_LZ1();  break;
		case Act_2: DynWater_LZ2();  break;
		case Act_3: DynWater_LZ3();  break;
		case Act_4: DynWater_SBZ3(); break;
	}

	if(v_waterpos3 < v_waterpos2)
		v_waterpos2 -= f_water ? 1 : 0;
	else if(v_waterpos3 > v_waterpos2)
		v_waterpos2 += f_water ? 1 : 0;
}

void DynWater_LZ1()
{
	switch(v_wtr_routine)
	{
		case 0:
			if(v_screenposx < 0x600) v_waterpos3 = 0xB8;
			else if(v_player->y >= 0x200)
			{
				if(v_screenposx < 0xC00) v_waterpos3 = 0x108;
				else if(v_screenposx < 0x1080) v_waterpos3 = 0x318;
				else if(v_screenposx < 0x1380)
				{
					f_switch[5] = 0x80;
					v_waterpos3 = 0x5C8;
				}
				else
				{
					v_waterpos3 = 0x3A8

					if(v_waterpos2 == v_waterpos3)
						v_wtr_routine = 1;
				}
			}
			else if(v_screenposx < 0xC80) v_waterpos3 = 0x108;
			else if(v_screenposx < 0x1500) v_waterpos3 = 0xE8;
			else v_waterpos3 = 0x108;
			break;

		case 1:
			if(v_player->y < 0x2E0)
			{
				if(v_screenposx < 0x1300) v_waterpos3 = 0x3A8;
				else
				{
					v_waterpos3 = 0x108
					v_wtr_routine = 2;
				}
			}
			break;

		case 2:
			break;
	}
}

void DynWater_LZ2()
{
	if(v_screenposx < 0x500) v_waterpos3 = 0x328;
	else if(v_screenposx < 0xB00) v_waterpos3 = 0x3C8;
	else v_waterpos3 = 0x428;
}

void DynWater_LZ3()
{
	switch(v_wtr_routine)
	{
		case 0:
			if(v_screenposx < 0x600) v_waterpos2 = v_waterpos3 = 0x900;
			else if(v_player->y >= 0x3C0 && v_player->y < 0x600)
			{
				v_waterpos2 = v_waterpos3 = 0x4C8;
				v_lvllayout[1][6] = 0x4B;
				v_wtr_routine = 1;
				PlaySound_Special(SFX_Rumbling);
			}
			break;

		case 1:

			if(v_screenposx < 0x770) v_waterpos3 = 0x4C8;
			else if(v_screenposx < 0x1400) v_waterpos3 = 0x308;
			else if(v_waterpos3 == 0x508 || v_player->y >= 0x600 || v_player->y < 0x280)
			{
				v_waterpos3 = 0x508;
				v_waterpos2 = v_waterpos3;

				if(v_screenposx >= 0x1770)
					v_wtr_routine = 2;
			}
			break;

		case 2:
			if(v_screenposx < 0x1860) v_waterpos3 = 0x508;
			else v_waterpos3 = 0x188;

			if(v_screenposx >= 0x1AF0 || v_waterpos2 == v_waterpos3)
				v_wtr_routine = 3;
			break;

		case 3:
			if(v_screenposx < 0x1AF0) v_waterpos2 = v_waterpos3 = 0x188;
			else if(v_screenposx < 0x1BC0) v_waterpos2 = v_waterpos3 = 0x900;
			else
			{
				v_wtr_routine = 4;
				v_waterpos2 = 0x7C0;
				v_waterpos3 = 0x608;
				f_switch[8] = 1;
			}
			break;

		case 4:
			if(v_screenposx >= 0x1E00) v_waterpos3 = 0x128;
	}
}

void DynWater_SBZ3()
{
	if(v_screenposx < 0xF00) v_waterpos3 = 0x228;
	else v_waterpos3 = 0x4C8;
}

const ushort LZWind_Data[][4] =
{
	// left    top    right   bottom
	{ 0x0A80, 0x0300, 0x0C10, 0x0380 }, // act 1 values (set 1)
	{ 0x0F80, 0x0100, 0x1410, 0x0180 }, // act 1 values (set 2)
	{ 0x0460, 0x0400, 0x0710, 0x0480 }, // act 2 values
	{ 0x0A20, 0x0600, 0x1610, 0x06E0 }, // act 3 values
	{ 0x0C80, 0x0600, 0x13D0, 0x0680 }, // SBZ act 3 values
};

void LZWindTunnels()
{
	if(v_debuguse)
		return;

	if(v_act == 0)
	{
		LZDoWindTunnel(LZWind_Data[0]);
		LZDoWindTunnel(LZWind_Data[1]);
	}
	else
		LZDoWindTunnel(LZWind_Data[v_act + 1]);

	if(f_wtunnelmode)
	{
		f_wtunnelmode = false;
		v_player->anim = 0;
	}
}

//                            a2
void LZDoWindTunnel(ushort* coords)
{
	if(v_player->x >= coords[0] && v_player->x < coords[2] && v_player->y >= coords[1] && v_player->y < coords[3])
	{
		if(v_vbla_count & 0x3F) // does VInt counter fall on 0, 0x40, 0x80 or 0xC0?
			PlaySound(SFX_Waterfall);

		if(f_wtunnelallow)
		{
			if(!Player_IsControllable())
				f_wtunnelmode = false;
			else
			{
				f_wtunnelmode = true;

				// This is so weird. It's just checking the x-coord, but the register that held the player's x position
				// has the vblank count & 0x3F in the low byte.. STRANGE. maybe just a little bug that escaped detection
				if((((v_player->x & ~0xFF) | (v_vbla_count & 0x3F)) - 0x80) < coords[0])
				{
					if(v_act == Act_2)
						v_player->y -= 2;
					else
						v_player->y += 2;
				}

				v_player->x += 4
				v_player->velX = 0x400;
				v_player->velY = 0;
				Player_SetAnimFloat();
				Player_SetInAir();

				if(v_jpadhold2 & Buttons_Up)
					v_player->y--;
				else if(v_jpadhold2 & Buttons_Down)
					v_player->y++;
			}
		}
	}
}

const byte SlideVelocities[] = { 11, -12, -11, -10, 10, -11, 10 };
const ubyte SlideChunks[] = { 4, 8, 75, 76, 3, 7, 2 };

void LZWaterSlides()
{
	if(!Player_IsInAir())
	{
		auto chunk = v_lvllayout[(HIWORD(v_player->y) >> 8) & 7][HIWORD(v_player->x) & 0x7F];

		for(int i = 0; i < 7; i++)
		{
			if(chunk == SlideChunks[i])
			{
				v_player->inertia = SlideVelocities[i] << 8;
				Player_SetFlipped(v_player->inertia < 0);
				Player_SetAnimSlide();
				f_jumponly = true;

				if((v_vbla_byte & 0x1F) == 0)
					PlaySound(SFX_Waterfall);

				return;
			}
		}
	}

	if(f_jumponly)
	{
		f_jumponly = false;
		VAR_W(v_player, 0x3E) = 5;
	}
}