// There is evidence that they originally planned on having much larger special stages, probably 128x128. For one, many
// of the address calculations for accessing stage layout data use 0xFF0000 as the base address, even though the stage
// data is only loaded starting at 0xFF1000. It clears the area from 0xFF0000 to 0xFF1000 before loading the stage data.
// Also, the stage layouts are only 64x64, but the layout data uses 64 128-tile rows. If they did use 128x128 tiles,
// it would neatly fit in the area from 0xFF0000 to 0xFF4000 (where the tile info resides).

enum
{
	SS_InitialRotate = 64,
	SS_TileSize = 24,

	SS_LayoutRows = 128,
	SS_LayoutColumns = 128,
	SS_LayoutUsedRows = 64,
	SS_LayoutUsedColumns = 64,
	SS_LayoutRowStart = 32,
	SS_NumAnimations = 32,
	SS_PosBufferSize = 16,
	SS_NumWallTileTypes = 36,
};

struct SS_Animation
{
	ubyte id;
	ubyte _unused;
	ubyte timer;
	ubyte frame;
	uint* tile;
};

struct SSTileInfo
{
	ubyte* map;
	ushort frame;
	ushort vram;
};

enum : ubyte
{
	SSObj_None =        0x00,

	SSObj_Bumper =      0x25,

	SSObj_Goal =        0x27,
	SSObj_1Up =         0x28,
	SSObj_Up =          0x29,
	SSObj_Down =        0x2A,
	SSObj_R =           0x2B,
	SSObj_Peppermint =  0x2C,
	SSObj_Glass4Hit =   0x2D,
	SSObj_Glass3Hit =   0x2E,
	SSObj_Glass2Hit =   0x2F,
	SSObj_Glass1Hit =   0x30,
	SSObj_RHit =        0x31,
	SSObj_BumpHit1 =    0x32,
	SSObj_BumpHit2 =    0x33,

	SSObj_Ring =        0x3A,
	SSObj_Emerald1 =    0x3B,
	SSObj_Emerald2 =    0x3C,
	SSObj_Emerald3 =    0x3D,
	SSObj_Emerald4 =    0x3E,
	SSObj_Emerald5 =    0x3F,
	SSObj_Emerald6 =    0x40,
	SSObj_GhostBlock =  0x41,
	SSObj_RingColl1 =   0x42,
	SSObj_RingColl2 =   0x43,
	SSObj_RingColl3 =   0x44,
	SSObj_RingColl4 =   0x45,
	SSObj_1UpColl1 =    0x46,
	SSObj_1UpColl2 =    0x47,
	SSObj_1UpColl3 =    0x48,
	SSObj_1UpColl4 =    0x49,
	SSObj_GhostTurner = 0x4A,
	SSObj_GlassHit1 =   0x4B,
	SSObj_GlassHit2 =   0x4C,
	SSObj_GlassHit3 =   0x4D,
	SSObj_GlassHit4 =   0x4E,

	SSObj_MAX = SSObj_GlassHit4,
	SSObj_COUNT
};

enum : ubyte
{
	SSAni_RingSparks = 1,
	SSAni_Bumper = 2,
	SSAni_1Up = 3,
	SSAni_Reverse = 4,
	SSAni_EmeraldSparks = 5,
	SSAni_GlassBlock = 6,
};

const ubyte SSAni_RingData[] =  { SSObj_RingColl1, SSObj_RingColl2, SSObj_RingColl3, SSObj_RingColl4, 0, 0 };
const ubyte SSAni_BumpData[] =  { SSObj_BumpHit1, SSObj_BumpHit2, SSObj_BumpHit1, SSObj_BumpHit2, 0, 0 };
const ubyte SSAni_1UpData[] =   { SSObj_1UpColl1, SSObj_1UpColl2, SSObj_1UpColl3, SSObj_1UpColl4, 0, 0 };
const ubyte SSAni_RevData[] =   { SSObj_R, SSObj_RHit, SSObj_R, SSObj_RHit, 0, 0 };
const ubyte SSAni_EmerData[] =  { SSObj_1UpColl1, SSObj_1UpColl2, SSObj_1UpColl3, SSObj_1UpColl4, 0, 0 };
const ubyte SSAni_GlassData[] = { SSObj_GlassHit1, SSObj_GlassHit2, SSObj_GlassHit3, SSObj_GlassHit4,
									SSObj_GlassHit1, SSObj_GlassHit2, SSObj_GlassHit3, SSObj_GlassHit4, 0, 0 };

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

const Point16 SS_StartLoc[] =
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

// Took a little liberty here; the original code encodes the frame as the top 8 bits of the map, and then "unpacks" this
// data into v_sstileinfo in SS_Load. This is ugly and unneccessary in an HLL representation so I just store it in the
// runtime format and just memcpy it over.
const SSTileInfo SS_MapIndex[] =
{
	{ Map_SSWalls,   0, 0x0142 }, // 01
	{ Map_SSWalls,   0, 0x0142 },
	{ Map_SSWalls,   0, 0x0142 },
	{ Map_SSWalls,   0, 0x0142 },
	{ Map_SSWalls,   0, 0x0142 },
	{ Map_SSWalls,   0, 0x0142 },
	{ Map_SSWalls,   0, 0x0142 },
	{ Map_SSWalls,   0, 0x0142 }, // 08
	{ Map_SSWalls,   0, 0x0142 },
	{ Map_SSWalls,   0, 0x2142 },
	{ Map_SSWalls,   0, 0x2142 },
	{ Map_SSWalls,   0, 0x2142 },
	{ Map_SSWalls,   0, 0x2142 },
	{ Map_SSWalls,   0, 0x2142 },
	{ Map_SSWalls,   0, 0x2142 },
	{ Map_SSWalls,   0, 0x2142 }, // 10
	{ Map_SSWalls,   0, 0x2142 },
	{ Map_SSWalls,   0, 0x2142 },
	{ Map_SSWalls,   0, 0x4142 },
	{ Map_SSWalls,   0, 0x4142 },
	{ Map_SSWalls,   0, 0x4142 },
	{ Map_SSWalls,   0, 0x4142 },
	{ Map_SSWalls,   0, 0x4142 },
	{ Map_SSWalls,   0, 0x4142 }, // 18
	{ Map_SSWalls,   0, 0x4142 },
	{ Map_SSWalls,   0, 0x4142 },
	{ Map_SSWalls,   0, 0x4142 },
	{ Map_SSWalls,   0, 0x6142 },
	{ Map_SSWalls,   0, 0x6142 },
	{ Map_SSWalls,   0, 0x6142 },
	{ Map_SSWalls,   0, 0x6142 },
	{ Map_SSWalls,   0, 0x6142 }, // 20
	{ Map_SSWalls,   0, 0x6142 },
	{ Map_SSWalls,   0, 0x6142 },
	{ Map_SSWalls,   0, 0x6142 },
	{ Map_SSWalls,   0, 0x6142 },
	{ Map_Bump,      0, 0x023B },
	{ Map_SS_R,      0, 0x0570 },
	{ Map_SS_R,      0, 0x0251 },
	{ Map_SS_R,      0, 0x0370 }, // 28
	{ Map_SS_Up,     0, 0x0263 },
	{ Map_SS_Down,   0, 0x0263 },
	{ Map_SS_R,      0, 0x22F0 },
	{ Map_SS_Glass,  0, 0x0470 },
	{ Map_SS_Glass,  0, 0x05F0 },
	{ Map_SS_Glass,  0, 0x65F0 },
	{ Map_SS_Glass,  0, 0x25F0 },
	{ Map_SS_Glass,  0, 0x45F0 }, // 30
	{ Map_SS_R,      0, 0x02F0 },
	{ Map_Bump,      1, 0x023B },
	{ Map_Bump,      2, 0x023B },
	{ Map_SS_R,      0, 0x0797 },
	{ Map_SS_R,      0, 0x07A0 },
	{ Map_SS_R,      0, 0x07A9 },
	{ Map_SS_R,      0, 0x0797 },
	{ Map_SS_R,      0, 0x07A0 }, // 38
	{ Map_SS_R,      0, 0x07A9 },
	{ Map_Ring,      0, 0x27B2 },
	{ Map_SS_Chaos3, 0, 0x0770 },
	{ Map_SS_Chaos3, 0, 0x2770 },
	{ Map_SS_Chaos3, 0, 0x4770 },
	{ Map_SS_Chaos3, 0, 0x6770 },
	{ Map_SS_Chaos1, 0, 0x0770 },
	{ Map_SS_Chaos2, 0, 0x0770 }, // 40
	{ Map_SS_R,      0, 0x04F0 },
	{ Map_Ring,      4, 0x27B2 },
	{ Map_Ring,      5, 0x27B2 },
	{ Map_Ring,      6, 0x27B2 },
	{ Map_Ring,      7, 0x27B2 },
	{ Map_SS_Glass,  0, 0x23F0 },
	{ Map_SS_Glass,  1, 0x23F0 },
	{ Map_SS_Glass,  2, 0x23F0 }, // 48
	{ Map_SS_Glass,  3, 0x23F0 },
	{ Map_SS_R,      2, 0x04F0 },
	{ Map_SS_Glass,  0, 0x05F0 },
	{ Map_SS_Glass,  0, 0x65F0 },
	{ Map_SS_Glass,  0, 0x25F0 },
	{ Map_SS_Glass,  0, 0x45F0 }, // 4E
};

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
	v_player->id = ID::SonicSpecial;
	PalCycle_SS();
	v_ssangle = 0;
	v_ssrotate = SS_InitialRotate;
	PlaySound(BGM::SS);
	v_btnpushtime1 = 0;
	v_btnpushtime2 = DemoDataPtr[6][1] - 1;
	v_rings = 0;
	v_lifecount = 0;
	v_debuguse = 0;
	v_demolength = 30 * OneSecond;

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

	v_demolength = OneSecond;
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

		if(TimerZero(v_palchgspeed, 2)))
			WhiteOut_ToWhite();
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
	// Just using this as a temp buffer
	auto gfxBuffer = (ubyte*)v_sslayout;

	// Birds and fish
	EniDec(Eni_SSBg1, gfxBuffer, 0x4051);

	uint vramLoc = 0x50000001;
	uint gfxLoc = gfxBuffer + 0x80;

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

					srcLoc = gfxBuffer;
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
	EniDec(Eni_SSBg2, gfxBuffer, 0x400);
	TilemapToVRAM(gfxBuffer, 0x40000003, 63, 31);
	TilemapToVRAM(gfxBuffer, 0x50000003, 63, 63);
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

void SS_ShowLayout(ushort* spriteBuffer)
{
	SS_AniWallsRings();
	SS_UpdateAnims();

	// First: calculate all the tile screen positions based on the rotation angle
	int cosine;
	auto sine = CalcSine(v_ssangle & 0xFC, &cosine);
	auto xstep = sine * SS_TileSize;
	auto ystep = cosine * SS_TileSize;
	auto x = -(v_screenposx % SS_TileSize) - 180;
	auto y = -(v_screenposy % SS_TileSize) - 180;

	for(int i = 0; i < SS_PosBufferSize; i++)
	{
		// This is a typical 2D rotation
		auto xp = (-sine * y) + (cosine * x);
		auto yp = (cosine * y) + (sine * x);

		for(int j = 0; j < SS_PosBufferSize; j++)
		{
			v_ssposbuffer[i][j].x = xp >> 8;
			v_ssposbuffer[i][j].y = yp >> 8;
			xp += ystep;
			yp += xstep;
		}

		y += SS_TileSize;
	}

	// Next: actually draw the special stage tiles (as sprites!)
	for(int i = 0, row = v_screenposy / SS_TileSize; i < SS_PosBufferSize; i++, row++)
	{
		for(int j = 0, col = v_screenposx / SS_TileSize; j < SS_PosBufferSize; j++, col++)
		{
			auto tile = v_sslayout[row][col];
			auto xpos = v_ssposbuffer[i][j].x + 0x120;
			auto ypos = v_ssposbuffer[i][j].y + 0xF0;

			if(tile != SSObj_None && tile <= SSObj_MAX && ypos >= 0x70 && ypos < 0x170 && xpos >= 0x70 && xpos < 0x1D0)
			{
				auto tileInfo = v_sstileinfo[tile];
				auto mapping = tileInfo.map[tileInfo.frame];
				auto numPieces = *mapping++;
				EnqueueSpriteUnflipped(tileInfo.vram, mapping, numPieces, xpos, ypos, v_spritecount, spriteBuffer);
			}
		}
	}

	// Terminate sprite link list
	if(v_spritecount == MaxSprites)
		spriteBuffer[-5] = 0;
	else
		spriteBuffer[0] = 0;
}

void SS_AniWallsRings()
{
	// Update wall tile frame numbers according to stage rotation
	auto frame = (v_ssangle / 4) & 0xF;

	for(int i = 0; i < SS_NumWallTileTypes; i++)
		v_sstileinfo[i].frame = frame;

	if(TimerNeg(v_ani1_time, 7))
		v_ani1_frame = (v_ani1_frame + 1) & 3;

	v_sstileinfo[58].frame = v_ani1_frame;

	if(TimerNeg(v_ani2_time, 7))
		v_ani2_frame = (v_ani2_frame + 1) & 1;

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

	if(TimerNeg(v_ani3_time, 4))
		v_ani3_frame = (v_ani3_frame + 1) & 3;

	v_sstileinfo[45].frame = v_ani3_frame;
	v_sstileinfo[46].frame = v_ani3_frame;
	v_sstileinfo[47].frame = v_ani3_frame;
	v_sstileinfo[48].frame = v_ani3_frame;

	if(TimerNeg(v_ani0_time, 7))
		v_ani0_frame = (v_ani0_frame + 1) & 7;

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

SS_Animation* SS_FindAnimSlot()
{
	for(int i = 0; i < SS_NumAnimations; i++)
	{
		if(v_ssanimations[i].id == 0)
			return &v_ssanimations[i];
	}

	return nullptr;
}

ubyte* SS_UpdateAnim(SS_Animation* anim, int timerReset, const ubyte* animData)
{
	if(TimerNeg(anim->timer, timerReset))
	{
		*anim->tile = animData[anim->frame++];

		if(*anim->tile == 0)
		{
			auto ret = anim->tile;
			memset(anim, 0, sizeof(SS_Animation));
			return ret;
		}
	}

	return nullptr;
}

void SS_UpdateAnims()
{
	for(int i = 0; i < SS_NumAnimations; i++)
	{
		switch(v_ssanimations[i]->id)
		{
			case SSAni_RingSparks: SS_UpdateAnim(&v_ssanimations[i], 5, SSAni_RingData);  break;
			case SSAni_1Up:        SS_UpdateAnim(&v_ssanimations[i], 5, SSAni_1UpData);   break;
			case SSAni_GlassBlock: SS_UpdateAnim(&v_ssanimations[i], 1, SSAni_GlassData); break;

			case SSAni_Bumper:
				if(auto tile = SS_UpdateAnim(&v_ssanimations[i], 7, SSAni_BumpData))
					*tile = SSObj_Bumper;
				break;

			case SSAni_Reverse:
				if(auto tile = SS_UpdateAnim(&v_ssanimations[i], 7, SSAni_BumpData))
					*tile = SSObj_R;
				break;

			case SSAni_EmeraldSparks:
				if(SS_UpdateAnim(&v_ssanimations[i], 5, SSAni_EmerData))
				{
					v_player->routine = Routine_ExitStage;
					PlaySound_Special(SFX::SSGoal);
				}
				break;

			case 0: break;
		}
	}
}

void SS_Load()
{
_retry:
	auto ssidx = v_lastspecial++;

	if(v_lastspecial == NumEmeralds)
		v_lastspecial = 0;

	if(v_emeralds != NumEmeralds)
	{
		for(int i = v_emeralds - 1; i >= 0; i--)
		{
			if(v_emldlist[i] == 0)
				goto _retry;
		}
	}

	v_player->x = SS_StartLoc[ssidx].x;
	v_player->y = SS_StartLoc[ssidx].y;

	// Just using v_sstileinfo as a temp buffer
	ubyte* decompressedLayout = (ubyte*)v_sstileinfo;
	EniDec(SS_LayoutIndex[ssidx], decompressedLayout, 0);
	memset(v_sslayout, 0, 0x1000);

	// Have to expand 64-byte rows to 128-byte rows
	for(int i = 0; i < SS_LayoutUsedRows; i++)
		memcpy(v_sslayout[i + SS_LayoutRowStart], decompressedLayout[i * SS_LayoutUsedColumns], SS_LayoutUsedColumns);

	memcpy(v_sstileinfo, SS_MapIndex, SSObj_COUNT * sizeof(SSTileInfo));
	memset(v_ssanimations, 0, SS_NumAnimations * sizeof(SS_Animation));
}