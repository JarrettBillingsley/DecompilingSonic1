struct LevSel_Ptr
{
	int zone;
	int act;
};

const LevSel_Ptr LevSel_Ptrs[] =
{
	{ Zone::GHZ, 0 },
	{ Zone::GHZ, 1 },
	{ Zone::GHZ, 2 },
	{ Zone::MZ,  0 },
	{ Zone::MZ,  1 },
	{ Zone::MZ,  2 },
	{ Zone::SYZ, 0 },
	{ Zone::SYZ, 1 },
	{ Zone::SYZ, 2 },
	{ Zone::LZ,  0 },
	{ Zone::LZ,  1 },
	{ Zone::LZ,  2 },
	{ Zone::SLZ, 0 },
	{ Zone::SLZ, 1 },
	{ Zone::SLZ, 2 },
	{ Zone::SBZ, 0 },
	{ Zone::SBZ, 1 },
	{ Zone::LZ,  3 },
	{ Zone::SBZ, 2 },
	{ Zone::SS,  0 }, // Special Stage
	{ 0x80,      0 }, // Sound Test
};

const ubyte LevSelCode_J[] =  { Buttons::Up, Buttons::Dn, Buttons::Dn, Buttons::Dn, Buttons::L, Buttons::R, 0, 0xFF };
const ubyte LevSelCode_US[] = { Buttons::Up, Buttons::Dn, Buttons::L, Buttons::R, 0, 0xFF };

void GM_Title()
{
	PlaySound_Special(BGM::Stop);
	ClearPLC();
	PaletteFadeOut();
	DISABLE_INTERRUPTS();
	SoundDriverLoad();

	// TODO:
	// lea	(vdp_control_port).l,a6
	// move.w	#0x8004,(a6)	; 8-colour mode
	// move.w	#0x8200+(vram_fg>>10),(a6) ; set foreground nametable address
	// move.w	#0x8400+(vram_bg>>13),(a6) ; set background nametable address
	// move.w	#0x9001,(a6)	; 64-cell hscroll size
	// move.w	#0x9200,(a6)	; window vertical position
	// move.w	#0x8B03,(a6)
	// move.w	#0x8720,(a6)	; set background colour (palette line 2, entry 0)

	f_wtr_state = 0;
	ClearScreen();
	Clear_Objects();

	NemDec(Nem_JapNames, 0);
	NemDec(Nem_CreditText, 0x14C0);
	EniDec(Eni_JapNames, 0xFF0000, 0);
	CopyTilemap(0xFF0000, 0xC000, 0x27, 0x1B);
	Clear_Palette(v_pal_dry_dup);
	PalLoad1(Palette::Sonic);

	v_objspace[2].id = ID::CreditsText;
	ExecuteObjects();
	BuildSprites();
	PaletteFadeIn();

	DISABLE_INTERRUPTS();
	NemDec(Nem_TitleFg, 0x4000);
	NemDec(Nem_TitleSonic, 0x6000);
	NemDec(Nem_TitleTM, 0xA200);

	CopyToVRAM(Art_Text, 0xD000, 0x290 * sizeof(ushort));

	v_lastlamp = 0;
	v_debuguse = 0;
	f_demo = 0;
	v_FFFFFFEA = 0;
	v_zone = 0;
	v_pcyc_time = 0;

	LevelSizeLoad();
	DeformLayers();

	EniDec(Blk16_GHZ, v_16x16, 0);
	KosDec(Blk256_GHZ, v_256x256);
	LevelLayoutLoad();
	PaletteFadeOut();
	DISABLE_INTERRUPTS();
	ClearScreen();

	// TODO:
	// lea	(vdp_control_port).l,a5
	// lea	(vdp_data_port).l,a6
	// lea	(0xFFFFF708).w,a3
	// lea	(v_lvllayout+0x40).w,a4
	// move.w	#0x6000,d2
	// bsr.w	DrawChunks

	EniDec(Eni_Title, 0xFF0000, 0);
	CopyTilemap(0xFF0000, 0xC206, 0x21, 0x15);
	NemDec(Nem_GHZ_1st, 0);
	PalLoad1(Palette::Title);
	PlaySound_Special(BGM::Title);

	f_debugmode = 0;
	v_demolength = 0x178;

	memset(&v_objspace[2], 0, 32);
	v_objspace[1].id = ID::TitleSonic;
	v_objspace[2].id = ID::PSBTM; // "PRESS START BUTTON"

	if(v_megadrive & 0x80)
	{
		// Outside Japan, load the TM object
		v_objspace[3].id = ID::PSBTM;
		v_objspace[3].frame = 3;
	}

	v_objspace[4].id = ID::PSBTM;
	v_objspace[4].frame = 2; // sprite line limiter..?

	ExecuteObjects();
	DeformLayers();
	BuildSprites();
	NewPLC(PLC::Main);
	v_title_dcount = 0;
	v_title_ccount = 0;

	// TODO:
	// move.w	(v_vdp_buffer1).w,d0
	// ori.b	#0x40,d0
	// move.w	d0,(vdp_control_port).l

	PaletteFadeIn();

	do
	{
		WaitForVBlank(VBlank_Title);
		ExecuteObjects()
		DeformLayers();
		BuildSprites();
		PCycle_Title();
		RunPLC();

		v_objspace[0].x += 2;

		if(v_objspace[0].x >= 7168)
		{
			v_gamemode = GameMode::Sega;
			return;
		}

		auto levSelCode = (v_megadrive & 0x80) ? LevSelCode_US : LevSelCode_J;
		auto buttons = v_jpadpress1 & Buttons::Dir;

		if(buttons == levSelCode[v_title_dcount])
		{
			v_title_dcount++;

			if(buttons == 0)
			{
				auto count = (v_title_ccount >> 1) & 3;

				if(count == 0)
					f_levselcheat = true;
				else if(v_megadrive & 0x80)
				{
					// International version
					f_debugcheat = true;
					f_slomocheat = true;
				}
				else
				{
					/// Japanese version
					if(count == 1)
						f_slomocheat = true;
					else if(count == 2)
						f_debugcheat = true;
					else
						f_creditscheat = true;
				}

				PlaySound_Special(SFX::Ring);
			}
		}
		else if(buttons != 0 && v_title_dcount != 9) // seems a little arbitrary...
			v_title_dcount = 0;

		if(v_jpadpress1 & Buttons::C)
			v_title_ccount++;

		if(v_demolength == 0)
		{
			if(GotoDemo()) // returns true if demo fadeout was interrupted by start button
				break;

			return;
		}
	} while(!(v_jpadpress1 & Buttons::Start));

	if(f_levselcheat && v_jpadhold1 & Buttons::A)
	{
		PalLoad2(Palette::LevelSel);

		// TODO:
		// This clears the scrolling data (presumably to make all the shit show up in the right place)
	// 	lea	(v_hscrolltablebuffer).w,a1
	// 	moveq	#0,d0
	// 	move.w	#0xDF,d1

	// Tit_ClrScroll1:
	// 	move.l	d0,(a1)+
	// 	dbf	d1,Tit_ClrScroll1 ; clear scroll data (in RAM)

	// 	move.l	d0,(v_scrposy_dup).w
	// 	DISABLE_INTERRUPTS();
	// 	lea	(vdp_data_port).l,a6
	// 	locVRAM	0xE000
	// 	move.w	#0x3FF,d1

	// Tit_ClrScroll2:
	// 	move.l	d0,(a6)
	// 	dbf	d1,Tit_ClrScroll2 ; clear scroll data (in VRAM)

		LevSelDisplayText();

		while(true)
		{
			WaitForVBlank(VBlank_Title);
			LevSelControls();
			RunPLC();

			if(v_plc_buffer == 0 && v_jpadpress1 & (Buttons::ABC | Buttons::Start))
			{
				if(v_levselitem == 0x14) // sound test
				{
					auto sound = v_levselsound + 0x80;

					if(f_creditscheat)
					{
						if(sound == 0x9F)
						{
							v_gamemode = GameMode::Ending;
							v_zone = Zone::EndZ;
							v_act = 0;
							return;
						}
						else if(sound == 0x9E)
						{
							v_gamemode = GameMode::Credits;
							PlaySound_Special(BGM::Credits);
							v_creditsnum = 0;
							return;
						}
					}

					if(sound < 0x94 || sound >= 0xA0)
						PlaySound_Special(sound);
				}
				else
				{
					auto ptr = LevSel_Ptrs[v_levselitem];

					if(ptr.zone != -1) // not on the sound test?
					{
						if(ptr.zone == Zone::SS) // special stage?
						{
							v_gamemode = GameMode::Special;
							v_zone = 0;
							v_lives = 3;
							v_rings = 0;
							v_time = 0;
							v_score = 0;

							// rev 1 only
							v_scorelife = 5000;
							return;
						}
						else
						{
							v_zone = ptr.zone;
							v_act = ptr.act;
							break;
						}
					}
				}
			}
		}
	}

	v_gamemode = GameMode::Level;
	v_lives = 3;
	v_rings = 0;
	v_time = 0;
	v_score = 0;
	v_lastspecial = 0;
	v_emeralds = 0;
	Clear_Emerald_List();
	v_continues = 0;

	// rev 1 only
	v_scorelife = 5000;
	PlaySound_Special(BGM::Fade);
}

LevSel_Ptr Demo_Levels[] =
{
	{ Zone::GHZ,  0 },
	{ Zone::MZ,   0 },
	{ Zone::SYZ,  0 },
	{ Zone::SS,   0 },
};

bool GotoDemo()
{
	v_demolength = 30;

	do
	{
		WaitForVBlank(VBlank_Title);
		DeformLayers();
		PaletteCycle();
		RunPLC();

		v_objspace[0].x += 2;

		if(v_objspace[0].x >= 7168)
		{
			v_gamemode = GameMode::Sega;
			return false;
		}

		if(v_jpadpress1 & Buttons::Start)
			return true;
	} while(v_demolength > 0);

	PlaySound_Special(BGM::Fade);
	auto level = Demo_Levels[v_demonum & 3];
	v_zone = level.zone;
	v_demonum = (v_demonum + 1) & 3;
	f_demo = true;
	v_gamemode = GameMode::Demo;

	if(level.zone == Zone::SS)
	{
		v_gamemode = GameMode::Special;
		v_zone = 0;
		v_lastspecial = 0;
	}

	v_lives = 3;
	v_rings = 0;
	v_time = 0;
	v_score = 0;
	v_scorelife = 5000; // rev 1

	return false;
}

void LevSelControls()
{
	auto buttonsPressed = v_jpadpress1 & (Buttons::Up | Buttons::Dn);

	if(buttonsPressed || TimerZero(v_levseldelay, 11))
	{
		auto buttonsHeld = v_jpadhold1 & (Buttons::Up | Buttons::Dn);

		if(buttonsHeld)
		{
			if(buttonsHeld & Buttons:Up)
				DecWrap(v_levselitem, 0x14);

			if(buttonsHeld & Buttons::Dn)
				IncWrap(v_levselitem, 0x14);

			LevSelDisplayText();
			return;
		}
	}

	if(v_levselitem == 0x14) // on sound test?
	{
		buttonsPressed = v_jpadpress1 & (Buttons::L | Buttons::R);

		if(buttonsPressed)
		{
			if(buttonsPressed & Buttons::L)
				DecWrap(v_levselsound, 0x4F);

			if(buttonsPressed & Buttons::R)
				IncWrap(v_levselsound, 0x4F);

			LevSelDisplayText();
		}
	}
}


void LevSelDisplayText()
{
	lea	(LevelMenuText).l,text

	// TODO:
// textpos:	= (0x40000000+((0xE210&0x3FFF)<<16)+((0xE210&0xC000)>>14))
// 				; 0xE210 is a VRAM address

	for(int i = 0, textpos = 0; i < 0x15; i++, textpos += 0x8000000)
	{
		// move.l textpos,4(vdp_data_port)
		LevSel_ChgLine(text, 0xE680);
	}

	// Then recolor selected line (v_levselitem) by changing palette for that line

	// move.l	#textpos,d4
	// lsl.w	#7,d0
	// swap	d0
	// add.l	d0,d4
	// lea	(LevelMenuText).l,text
	// lsl.w	#3,d1
	// move.w	d1,d0
	// add.w	d1,d1
	// add.w	d0,d1
	// adda.w	d1,text
	// move.w	#0xC680,d3	; VRAM setting (3rd palette, 0x680th tile)
	// move.l	d4,4(vdp_data_port)
	// bsr.w	LevSel_ChgLine	; recolour selected line
	// move.w	#0xE680,d3
	// cmpi.w	#0x14,(v_levselitem).w
	// bne.s	LevSel_DrawSnd
	// move.w	#0xC680,d3

// LevSel_DrawSnd:

	// Then draw the sound test line
	auto sfx = v_levselsound + 0x80;
	LevSel_DrawDigit(sfx >> 4, 0xEC30);
	LevSel_DrawDigit(sfx, 0xEC30);
}

void LevSel_DrawDigit(ushort digit, ushort vram)
{
	digit &= 0x0F;

	if(digit >= 10)
		digit += 7; // use alpha characters

	// TODO:
	// add.w	vram,digit
	// move.w	digit,(vdp_data_port)
}

void LevSel_ChgLine(const char*& text, ushort vram)
{
	for(int i = 0; i < 0x18; i++)
	{
		auto ch = *text++;

		// TODO:
		if(ch == 0)
		{
			// move.w	#0,(vdp_data_port)		; use blank character
		}
		else
		{
			// add.w	vram,ch		; combine char with VRAM setting
			// move.w	ch,(vdp_data_port)		; send to VRAM
		}
	}
}

// LevelMenuText:	if Revision=0
// 	incbin	"misc\Level Select Text.bin"
// 	else
// 	incbin	"misc\Level Select Text (JP1).bin"
// 	endc
// 	even
