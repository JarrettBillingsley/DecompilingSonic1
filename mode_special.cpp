void GM_Special()
{
	PlaySound_Special(SFX::EnterSS);
	PaletteWhiteOut();
	DISABLE_INTERRUPTS();

	// TODO:
	// lea	(vdp_control_port).l,a6
	// move.w	#0x8B03,(a6)	; line scroll mode
	// move.w	#0x8004,(a6)	; 8-colour mode
	// move.w	#0x8A00+175,(v_hbla_hreg).w
	// move.w	#0x9011,(a6)	; 128-cell hscroll size
	// move.w	(v_vdp_buffer1).w,d0
	// andi.b	#0xBF,d0
	// move.w	d0,(vdp_control_port).l

	ClearScreen();
	ENABLE_INTERRUPTS();
	FillVRAM(0, 0x6FFF, 0x5000);

// SS_WaitForDMA:
// 	move.w	(a5),d1		; read control port (0xC00004)
// 	btst	#1,d1		; is DMA running?
// 	bne.s	SS_WaitForDMA	; if yes, branch

	// move.w	#0x8F02,(a5)	; set VDP increment to 2 bytes
	SS_BGLoad()
	QuickPLC(PLC::SpecialStage);

	Clear_Objects();
	Clear_F700_F800()
	Clear_FE60_FF00();
	Clear_NGFX_Buffer();
	f_wtr_state = false;
	f_restart = 0;
	PalLoad1(Palette::Special);
	SS_Load();
	v_screenposx = 0;
	v_screenposy = 0;
	v_player.id = ID::SonicSpecial;
	PalCycle_SS();
	v_ssangle = 0;
	v_ssrotate = 64;
	PlaySound(BGM::SS);
	v_btnpushtime1 = 0;
	v_btnpushtime2 = DemoDataPtr[6][1] - 1;
	v_rings = 0;
	v_lifecount = 0;
	v_debuguse = 0;
	v_demolength = 1800;

	if(f_debugcheat && v_jpadhold1 & Buttons::A)
		f_debugmode = true;

	// TODO:
	// move.w	(v_vdp_buffer1).w,d0
	// ori.b	#0x40,d0
	// move.w	d0,(vdp_control_port).l
	PaletteWhiteIn();

	// ===========================================================================
	// Main Special Stage loop
	// ===========================================================================

	do
	{
		PauseGame();
		WaitForVBlank(VBlank_Special);
		MoveSonicInDemo();
		v_jpadhold2 = v_jpadhold1;
		ExecuteObjects();
		SS_ShowLayout(BuildSprites());
		SS_BGAnimate();

		if(f_demo && v_demolength == 0)
		{
			v_gamemode = GameMode::Sega;
			return;
		}
	} while(v_gamemode == GameMode::Special);

	if(f_demo)
	{
		if(v_gamemode == GameMode::Level)
			v_gamemode = GameMode::Sega;

		return;
	}

	v_gamemode = GameMode::Level;

	if(v_zone >= Zone::SBZ)
	{
		v_zone = 0;
		v_act = 0;
	}


	v_demolength = 60;
	v_pfade_start = 63;
	v_palchgspeed = 0;

	do
	{
		WaitForVBlank(VBlank_SSFade);
		MoveSonicInDemo();
		v_jpadhold2 = v_jpadhold1;
		ExecuteObjects();
		SS_ShowLayout(BuildSprites());
		SS_BGAnimate();

		if(TimerZero(v_palchgspeed)))
		{
			v_palchgspeed = 2;
			WhiteOut_ToWhite();
		}
	} while(v_demolength);

	DISABLE_INTERRUPTS();
	// TODO:
	// lea	(vdp_control_port).l,a6
	// move.w	#0x8200+(vram_fg>>10),(a6) ; set foreground nametable address
	// move.w	#0x8400+(vram_bg>>13),(a6) ; set background nametable address
	// move.w	#0x9001,(a6)		; 64-cell hscroll size
	ClearScreen();
	NemDec(Nem_TitleCard, 0xB000);
	Hud_Base();
	ENABLE_INTERRUPTS();
	PalLoad2(Palette::SSResult);
	NewPLC(PLC::Main);
	AddPLC(PLC::SSResult);
	f_scorecount = 1;
	f_endactbonus = 1;
	v_ringbonus = v_rings * 10;
	PlaySound_Special(BGM::GotThrough);

	Clear_Objects();
	v_objspace[23].id = ID::SSResult;

	do
	{
		PauseGame();
		WaitForVBlank(VBlank_SSResults);
		ExecuteObjects();
		BuildSprites();
		RunPLC();
	} while(!f_restart && v_plc_buffer != 0);

	PlaySound_Special(SFX::EnterSS);
	PaletteWhiteOut();
}

void SS_BGLoad()
{
	// Birds and fish
	EniDec(Eni_SSBg1, 0xFF0000, 0x4051);

	uint vramLoc = 0x50000001;
	uint gfxLoc = 0xFF0080;

	for(int i = 6; i >= 0; i--)
	{
		auto destLoc = vramLoc;
		auto someFlag = (i >= 3) ? 0 : 1

		for(int j = 3; j >= 0; j--)
		{
			for(int k = 7; k >= 0; k--)
			{
				auto srcLoc = gfxLoc;
				someFlag ^= 1;

				if(someFlag == 0)
				{
					if(i != 6)
						goto loc_48F2;

					srcLoc = 0xFF0000;
				}

				TilemapToVRAM(srcLoc, destLoc, 7, 7);

			loc_48F2:
				destLoc += 0x100000;
			}

			destLoc += 0x3800000;
			someFlag ^= 1;
		}

		vramLoc += 0x10000000;

		if(vramLoc & 0x80000000)
			vramLoc = WSWAP(WSWAP(vramLoc) + 0xC000);

		gfxLoc += 0x80;
	}

	// Clouds
	EniDec(Eni_SSBg2, 0xFF0000, 0x400);
	TilemapToVRAM(0xFF0000, 0x40000003, 63, 31);
	TilemapToVRAM(0xFF0000, 0x50000003, 63, 63);
}


const ubyte byte_4CB8[] = { 9, 0x28, 0x18, 0x10, 0x28, 0x18, 0x10, 0x30, 0x18, 8, 0x10, 0 };
const ubyte byte_4CC4[] = { 6, 0x30, 0x30, 0x30, 0x28, 0x18, 0x18, 0x18 };
const ubyte byte_4CCC[] = { 8, 2, 4, 0xFF, 2, 3, 8, 0xFF, 4, 2, 2, 3, 8, 0xFD, 4, 2, 2, 3, 2, 0xFF };

// don't even pretend to know what's doing on in this
void SS_BGAnimate()
{
	if(v_FFFFF7A0 == 0) // set in PalCycle_SS from the plane A nametable offset
	{
		// ??? these variables seem to have something to do with BG layer deformation
		v_FFFFF70C = 0;
		v_FFFFF618 = 0;
	}

	if(v_FFFFF7A0 < 8) // always true?
	{
		if(v_FFFFF7A0 == 6)
		{
			v_FFFFF718++;
			v_FFFFF70C++;
			v_FFFFF618 = v_FFFFF70C;
		}

		d0 = WSWAP(-v_FFFFF708) //w
		a1 = byte_4CCC //b
		a3 = v_ngfx_buffer //w

		for(int i = 0; i < 10; i++)
		{
			d0 = CalcSine((d0 & 0xFFFF0000) | a3[1]) //w (idx is w)
			d2 = *a1++ //b
			d0 *= d2
			d0 >>= 8 // arithmetic
			*a3++ = d0 //w
			d2 = *a1++ //b
			*a3 += d2 //w
		}

		a3 = v_ngfx_buffer //w
		a2 = byte_4CB8
	}
	else // never taken?
	{
		if(v_FFFFF7A0 == 12)
		{
			v_FFFFF718--;
			a3 = v_ngfx_buffer + 0x100 //l
			d2 = 0x18000

			for(int i = 0; i < 7; i++)
			{
				*a3 -= d2 //l
				a3++
				d2 -= 0x2000
			}
		}

		a3 = v_ngfx_buffer + 0x100 //w
		a2 = byte_4CC4 //b
	}

	d0 = WSWAP(-v_FFFFF718)
	d3 = *a2++ //b
	d2 = (-v_FFFFF70C & 0xFF) << 2 //w

	for(; d3 >= 0; d3--)
	{
		d0 = *a3++ //w
		a3++
		d1 = *a2++ - 1 //b

		for(; d1 >= 0; d1--)
		{
			v_hscrolltablebuffer[d2] = d0 //l; idx is bytes
			d2 = (d2 + 4) & 0x3FC //w
		}
	}
}

//
void SS_ShowLayout(ushort* spriteBuffer)
{
	SS_AniWallsRings();
	SS_AniItems();

	// First: calculate all the tile screen positions based on the rotation angle
	short* posBuffer = 0xFFFF8000;
	int cosine;
	auto sine = CalcSine(v_ssangle & 0xFC, &cosine);
	auto xstep = sine * 0x18;
	auto ystep = cosine * 0x18;
	auto x = ((v_screenposx / 0x18) << 16) | (-(v_screenposx % 0x18) - 180);
	auto y = ((v_screenposy / 0x18) << 16) | (-(v_screenposy % 0x18) - 180);

	for(int i = 0; i < 16; i++)
	{
		// This is a typical 2D rotation
		auto xp = (-sine * y) + (cosine * x);
		auto yp = (cosine * y) + (sine * x);

		for(int j = 0; j < 16; j++)
		{
			*posBuffer++ = xp >> 8;
			*posBuffer++ = yp >> 8;
			xp += ystep;
			yp += xstep;
		}

		y += 0x18;
	}

	// Next: actually draw the special stage tiles (as sprites!)
	auto layout = &0xFFFF0000[((v_screenposy / 0x18) * 0x80) + (v_screenposx / 0x18)];
	posBuffer = 0xFFFF8000;

	for(int i = 0; i < 16; i++)
	{
		for(int j = 0; j < 16; j++, posBuffer += 2)
		{
			auto tile = *layout++;
			auto xpos = posBuffer[0] + 0x120;
			auto ypos = posBuffer[1] + 0xF0;

			if(tile == 0 || tile > 0x4E || ypos < 0x70 || ypos >= 0x170 || xpos < 0x70 || xpos >= 0x1D0)
				continue;

			auto tileInfo = 0xFFFF4000[tile];
			auto mapping = tileInfo.map[tileInfo.frame];
			auto numPieces = *mapping++;
			EnqueueSpriteUnflipped(tileInfo.vram, mapping, numPieces, xpos, ypos, v_spritecount, spriteBuffer);
		}

		// next row
		layout += 0x70;
	}

	// Terminate sprite link list
	if(v_spritecount == 0x50)
		spriteBuffer[-5] = 0;
	else
		spriteBuffer[0] = 0;
}

const ushort SS_WaRiVramSet[] =
{
	0x0142, 0x6142, 0x0142, 0x0142, 0x0142, 0x0142, 0x0142, 0x6142,
	0x0142, 0x6142, 0x0142, 0x0142, 0x0142, 0x0142, 0x0142, 0x6142,
	0x2142, 0x0142, 0x2142, 0x2142, 0x2142, 0x2142, 0x2142, 0x0142,
	0x2142, 0x0142, 0x2142, 0x2142, 0x2142, 0x2142, 0x2142, 0x0142,
	0x4142, 0x2142, 0x4142, 0x4142, 0x4142, 0x4142, 0x4142, 0x2142,
	0x4142, 0x2142, 0x4142, 0x4142, 0x4142, 0x4142, 0x4142, 0x2142,
	0x6142, 0x4142, 0x6142, 0x6142, 0x6142, 0x6142, 0x6142, 0x4142,
	0x6142, 0x4142, 0x6142, 0x6142, 0x6142, 0x6142, 0x6142, 0x4142,
};

void SS_AniWallsRings()
{
	// Update wall tile frame numbers according to stage rotation
	auto frame = (v_ssangle / 4) & 0xF;

	for(int i = 0; i < 36; i++)
		v_sstileinfo[i].frame = frame;

	if(TimerNeg(v_ani1_time))
	{
		v_ani1_time = 7;
		v_ani1_frame = (v_ani1_frame + 1) & 3;
	}

	v_sstileinfo[58].frame = v_ani1_frame;

	if(TimerNeg(v_ani2_time))
	{
		v_ani2_time = 7;
		v_ani2_frame = (v_ani2_frame + 1) & 1;
	}

	v_sstileinfo[39].frame = v_ani2_frame;
	v_sstileinfo[41].frame = v_ani2_frame;
	v_sstileinfo[42].frame = v_ani2_frame;
	v_sstileinfo[44].frame = v_ani2_frame;
	v_sstileinfo[59].frame = v_ani2_frame;
	v_sstileinfo[60].frame = v_ani2_frame;
	v_sstileinfo[61].frame = v_ani2_frame;
	v_sstileinfo[62].frame = v_ani2_frame;
	v_sstileinfo[63].frame = v_ani2_frame;
	v_sstileinfo[64].frame = v_ani2_frame;

	if(TimerNeg(v_ani3_time))
	{
		v_ani3_time = 4;
		v_ani3_frame = (v_ani3_frame + 1) & 3;
	}

	v_sstileinfo[45].frame = v_ani3_frame;
	v_sstileinfo[46].frame = v_ani3_frame;
	v_sstileinfo[47].frame = v_ani3_frame;
	v_sstileinfo[48].frame = v_ani3_frame;

	if(TimerNeg(v_ani0_time))
	{
		v_ani0_time = 7
		v_ani0_frame = (v_ani0_frame + 1) & 7;
	}

	for(int i = 0, tile = 2, vram = v_ani0_frame; i < 4; i++, vram += 8, tile += 9)
	{
		v_sstileinfo[tile].vram = SS_WaRiVramSet[vram];
		v_sstileinfo[tile + 1].vram = SS_WaRiVramSet[vram + 1];
		v_sstileinfo[tile + 2].vram = SS_WaRiVramSet[vram + 2];
		v_sstileinfo[tile + 3].vram = SS_WaRiVramSet[vram + 3];
		v_sstileinfo[tile + 4].vram = SS_WaRiVramSet[vram + 4];
		v_sstileinfo[tile + 5].vram = SS_WaRiVramSet[vram + 5];
		v_sstileinfo[tile + 6].vram = SS_WaRiVramSet[vram + 6];
		v_sstileinfo[tile + 7].vram = SS_WaRiVramSet[vram + 7];
	}
}

// I don't think this function actually DOES anything.......
void SS_RemoveCollectedItem()
{
	// for(int i = 0; i < 32; i++)
	// {
	// 	if(0xFF4400[i * 8] == 0)
	// 		return;
	// }
}

const ubyte SS_AniRingData[] =  { 0x42, 0x43, 0x44, 0x45, 0, 0 };
const ubyte SS_AniBumpData[] =  { 0x32, 0x33, 0x32, 0x33, 0, 0 };
const ubyte SS_Ani1UpData[] =   { 0x46, 0x47, 0x48, 0x49, 0, 0 };
const ubyte SS_AniRevData[] =   { 0x2B, 0x31, 0x2B, 0x31, 0, 0 };
const ubyte SS_AniEmerData[] =  { 0x46, 0x47, 0x48, 0x49, 0, 0 };
const ubyte SS_AniGlassData[] = { 0x4B, 0x4C, 0x4D, 0x4E, 0x4B, 0x4C, 0x4D, 0x4E, 0, 0 };

struct SS_Object
{
	ubyte id;
	ubyte unused;
	ubyte timer;
	ubyte frame;
	uint* gfxmaybe;
};

void SS_AniItems()
{
	SS_Object* obj = 0xFF4400;

	for(int i = 0; i < 32; i++, obj++)
	{
		switch(obj->id)
		{
			case SS_AniRingSparks:
				if(TimerNeg(obj->timer))
				{
					obj->timer = 5;
					*obj->gfxmaybe = SS_AniRingData[obj->frame++]

					if(*obj->gfxmaybe == 0)
						memset(a0, 0, sizeof(SS_Object));
				}
				break;

			case SS_AniBumper:
				if(TimerNeg(obj->timer))
				{
					obj->timer = 7;
					auto frame = SS_AniBumpData[obj->frame++];

					if(frame == 0)
					{
						memset(a0, 0, sizeof(SS_Object));
						obj->gfxmaybe = 0x25;
					}
					else
						obj->gfxmaybe = frame;
				}
				break;

			case SS_Ani1Up:
				if(TimerNeg(obj->timer))
				{
					obj->timer = 5;
					*obj->gfxmaybe = SS_Ani1UpData[obj->frame++]

					if(*obj->gfxmaybe == 0)
						memset(a0, 0, sizeof(SS_Object));
				}
				break;

			case SS_AniReverse:
				if(TimerNeg(obj->timer))
				{
					obj->timer = 7;
					auto frame = SS_AniBumpData[obj->frame++];

					if(frame == 0)
					{
						memset(a0, 0, sizeof(SS_Object));
						obj->gfxmaybe = 0x2B;
					}
					else
						obj->gfxmaybe = frame;
				}
				break;

			case SS_AniEmeraldSparks:
				if(TimerNeg(obj->timer))
				{
					obj->timer = 5;

					*obj->gfxmaybe = SS_AniEmerData[obj->frame++]

					if(*obj->gfxmaybe == 0)
					{
						memset(a0, 0, sizeof(SS_Object));
						v_FFFFD024 = 4;
						PlaySound_Special(SFX::SSGoal);
					}
				}
				break;

			case SS_AniGlassBlock:
				if(TimerNeg(obj->timer))
				{
					obj->timer = 1;
					*obj->gfxmaybe = SS_AniGlassData[obj->frame++]

					if(*obj->gfxmaybe == 0)
						memset(a0, 0, sizeof(SS_Object));
				}
				break;

			case 0: break;
		}
	}
}

const ubyte Map_SSWalls[][] =
{
	{ 1, 0xF4, 0xA, 0, 0x00, 0xF4 },
	{ 1, 0xF0, 0xF, 0, 0x09, 0xF0 },
	{ 1, 0xF0, 0xF, 0, 0x19, 0xF0 },
	{ 1, 0xF0, 0xF, 0, 0x29, 0xF0 },
	{ 1, 0xF0, 0xF, 0, 0x39, 0xF0 },
	{ 1, 0xF0, 0xF, 0, 0x49, 0xF0 },
	{ 1, 0xF0, 0xF, 0, 0x59, 0xF0 },
	{ 1, 0xF0, 0xF, 0, 0x69, 0xF0 },
	{ 1, 0xF0, 0xF, 0, 0x79, 0xF0 },
	{ 1, 0xF0, 0xF, 0, 0x89, 0xF0 },
	{ 1, 0xF0, 0xF, 0, 0x99, 0xF0 },
	{ 1, 0xF0, 0xF, 0, 0xA9, 0xF0 },
	{ 1, 0xF0, 0xF, 0, 0xB9, 0xF0 },
	{ 1, 0xF0, 0xF, 0, 0xC9, 0xF0 },
	{ 1, 0xF0, 0xF, 0, 0xD9, 0xF0 },
	{ 1, 0xF0, 0xF, 0, 0xE9, 0xF0 },
};

const ubyte Map_SS_R[][] =
{
	{ 1,  0xF4, 0xA, 0, 0, 0xF4 },
	{ 1,  0xF4, 0xA, 0, 9, 0xF4 },
	{ 0 }
};

const ubyte Map_SS_Glass[][] =
{
	{ 1, 0xF4, 0xA, 0x00, 0, 0xF4 },
	{ 1, 0xF4, 0xA, 0x08, 0, 0xF4 },
	{ 1, 0xF4, 0xA, 0x18, 0, 0xF4 },
	{ 1, 0xF4, 0xA, 0x10, 0, 0xF4 },
};

const ubyte Map_SS_Up[][] =
{
	{ 1, 0xF4, 0xA, 0, 0x00, 0xF4 },
	{ 1, 0xF4, 0xA, 0, 0x12, 0xF4 },
};

const ubyte Map_SS_Down[][] =
{
	{ 1, 0xF4, 0xA, 0, 0x09, 0xF4 },
	{ 1, 0xF4, 0xA, 0, 0x12, 0xF4 },
};

const ubyte Map_SS_Chaos1[][] =
{
	{ 1, 0xF8, 5, 0, 0x00, 0xF8 },
	{ 1, 0xF8, 5, 0, 0x0C, 0xF8 },
};

const ubyte Map_SS_Chaos2[][] =
{
	{ 1, 0xF8, 5, 0, 0x04, 0xF8 },
	{ 1, 0xF8, 5, 0, 0x0C, 0xF8 },
};

const ubyte Map_SS_Chaos3[][] =
{
	{ 1, 0xF8, 5, 0, 0x08, 0xF8 },
	{ 1, 0xF8, 5, 0, 0x0C, 0xF8 },
};

struct StartPos
{
	ushort x, y;
};

const StartPos SS_StartLoc[] =
{
	{ 0x03D0, 0x02E0 },
	{ 0x0328, 0x0574 },
	{ 0x04E4, 0x02E0 },
	{ 0x03AD, 0x02E0 },
	{ 0x0340, 0x06B8 },
	{ 0x049B, 0x0358 },
};

const ubyte** SS_LayoutIndex[] =
{
	SS_1,
	SS_2,
	SS_3,
	SS_4,
	SS_5,
	SS_6,
};

struct SSTileInfo
{
	ubyte* map;
	ushort frame;
	ushort vram;
};

struct SSMap
{
	ubyte* map;
	ubyte frame;
	ushort vram;
};

const SSMap SS_MapIndex[] =
{
	{ Map_SSWalls,   0, 0x0142 },
	{ Map_SSWalls,   0, 0x0142 },
	{ Map_SSWalls,   0, 0x0142 },
	{ Map_SSWalls,   0, 0x0142 },
	{ Map_SSWalls,   0, 0x0142 },
	{ Map_SSWalls,   0, 0x0142 },
	{ Map_SSWalls,   0, 0x0142 },
	{ Map_SSWalls,   0, 0x0142 },
	{ Map_SSWalls,   0, 0x0142 },
	{ Map_SSWalls,   0, 0x2142 },
	{ Map_SSWalls,   0, 0x2142 },
	{ Map_SSWalls,   0, 0x2142 },
	{ Map_SSWalls,   0, 0x2142 },
	{ Map_SSWalls,   0, 0x2142 },
	{ Map_SSWalls,   0, 0x2142 },
	{ Map_SSWalls,   0, 0x2142 },
	{ Map_SSWalls,   0, 0x2142 },
	{ Map_SSWalls,   0, 0x2142 },
	{ Map_SSWalls,   0, 0x4142 },
	{ Map_SSWalls,   0, 0x4142 },
	{ Map_SSWalls,   0, 0x4142 },
	{ Map_SSWalls,   0, 0x4142 },
	{ Map_SSWalls,   0, 0x4142 },
	{ Map_SSWalls,   0, 0x4142 },
	{ Map_SSWalls,   0, 0x4142 },
	{ Map_SSWalls,   0, 0x4142 },
	{ Map_SSWalls,   0, 0x4142 },
	{ Map_SSWalls,   0, 0x6142 },
	{ Map_SSWalls,   0, 0x6142 },
	{ Map_SSWalls,   0, 0x6142 },
	{ Map_SSWalls,   0, 0x6142 },
	{ Map_SSWalls,   0, 0x6142 },
	{ Map_SSWalls,   0, 0x6142 },
	{ Map_SSWalls,   0, 0x6142 },
	{ Map_SSWalls,   0, 0x6142 },
	{ Map_SSWalls,   0, 0x6142 },
	{ Map_Bump,      0, 0x023B },
	{ Map_SS_R,      0, 0x0570 },
	{ Map_SS_R,      0, 0x0251 },
	{ Map_SS_R,      0, 0x0370 },
	{ Map_SS_Up,     0, 0x0263 },
	{ Map_SS_Down,   0, 0x0263 },
	{ Map_SS_R,      0, 0x22F0 },
	{ Map_SS_Glass,  0, 0x0470 },
	{ Map_SS_Glass,  0, 0x05F0 },
	{ Map_SS_Glass,  0, 0x65F0 },
	{ Map_SS_Glass,  0, 0x25F0 },
	{ Map_SS_Glass,  0, 0x45F0 },
	{ Map_SS_R,      0, 0x02F0 },
	{ Map_Bump,      1, 0x023B },
	{ Map_Bump,      2, 0x023B },
	{ Map_SS_R,      0, 0x0797 },
	{ Map_SS_R,      0, 0x07A0 },
	{ Map_SS_R,      0, 0x07A9 },
	{ Map_SS_R,      0, 0x0797 },
	{ Map_SS_R,      0, 0x07A0 },
	{ Map_SS_R,      0, 0x07A9 },
	{ Map_Ring,      0, 0x27B2 },
	{ Map_SS_Chaos3, 0, 0x0770 },
	{ Map_SS_Chaos3, 0, 0x2770 },
	{ Map_SS_Chaos3, 0, 0x4770 },
	{ Map_SS_Chaos3, 0, 0x6770 },
	{ Map_SS_Chaos1, 0, 0x0770 },
	{ Map_SS_Chaos2, 0, 0x0770 },
	{ Map_SS_R,      0, 0x04F0 },
	{ Map_Ring,      4, 0x27B2 },
	{ Map_Ring,      5, 0x27B2 },
	{ Map_Ring,      6, 0x27B2 },
	{ Map_Ring,      7, 0x27B2 },
	{ Map_SS_Glass,  0, 0x23F0 },
	{ Map_SS_Glass,  1, 0x23F0 },
	{ Map_SS_Glass,  2, 0x23F0 },
	{ Map_SS_Glass,  3, 0x23F0 },
	{ Map_SS_R,      2, 0x04F0 },
	{ Map_SS_Glass,  0, 0x05F0 },
	{ Map_SS_Glass,  0, 0x65F0 },
	{ Map_SS_Glass,  0, 0x25F0 },
	{ Map_SS_Glass,  0, 0x45F0 },
	{ nullptr, 0, 0 }
};

void SS_Load()
{
_retry:
	auto ssidx = v_lastspecial++;

	if(v_lastspecial == 6)
		v_lastspecial = 0;

	if(v_emeralds != 6)
	{
		for(int i = v_emeralds - 1; i >= 0; i--)
		{
			if(v_emldlist[i] == 0)
				goto _retry;
		}
	}

	v_player->x = SS_StartLoc[ssidx].x;
	v_player->y = SS_StartLoc[ssidx].y;

	EniDec(SS_LayoutIndex[ssidx], 0xFFFF4000, 0);
	memset(0xFF0000, 0, 0x1000);

	ubyte* layout = 0xFFFF1020;
	ubyte* decompressedLayout = 0xFFFF4000;

	// Not sure why they have 0x80 bytes for each row of the layout. Maybe they planned on having much larger stages?
	for(int i = 0; i < 64; i++)
	{
		memcpy(layout, decompressedLayout, 64);
		layout += 0x80;
		decompressedLayout += 0x40;
	}

	for(int i = 0; SS_MapIndex[i].map != nullptr; i++)
	{
		v_sstileinfo[i + 1].map = SS_MapIndex[i].map;
		v_sstileinfo[i + 1].frame = SS_MapIndex[i].frame;
		v_sstileinfo[i + 1].vram = SS_MapIndex[i].vram;
	}

	memset(0xFFFF4400, 0, 64 * sizeof(int));
}