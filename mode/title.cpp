void GM_Title()
{
	PlaySound_Special(BGM_Stop);
	ClearPLC();
	PaletteFadeOut();
	DISABLE_INTERRUPTS();
	SoundDriverLoad();
	VDP_RegWrite(0x00, 4);             // 8-colour mode
	VDP_RegWrite(0x02, vram_fg >> 10); // set foreground nametable address
	VDP_RegWrite(0x04, vram_bg >> 13); // set background nametable address
	VDP_RegWrite(0x10, 1);             // 64-cell hscroll size
	VDP_RegWrite(0x12, 0);             // window vertical position
	VDP_RegWrite(0x0B, 3);
	VDP_RegWrite(0x07, 0x20);          // set background colour (palette line 2, entry 0)

	f_wtr_state = 0;
	ClearScreen();
	Clear_Objects();

	NemDec(Nem_JapNames, 0);
	NemDec(Nem_CreditText, 0x14C0);
	EniDec(Eni_JapNames, 0xFF0000, 0);
	CopyTilemap(0xFF0000, 0xC000, 0x27, 0x1B);
	Clear_Palette(v_pal_dry_dup);
	PalLoad1(Palette_Sonic);

	v_objspace[2].id = ID_CreditsText;
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
	DrawChunks(0x6000, &v_bg1posx, v_lvllayout + 0x40);
	EniDec(Eni_Title, 0xFF0000, 0);
	CopyTilemap(0xFF0000, 0xC206, 0x21, 0x15);
	NemDec(Nem_GHZ_1st, 0);
	PalLoad1(Palette_Title);
	PlaySound_Special(BGM_Title);

	f_debugmode = 0;
	v_demolength = 0x178;

	memset(&v_objspace[2], 0, 32);
	v_objspace[1].id = ID_TitleSonic;
	v_objspace[2].id = ID_PSBTM; // "PRESS START BUTTON"

	if(v_megadrive & 0x80)
	{
		// Outside Japan, load the TM object
		v_objspace[3].id = ID_PSBTM;
		v_objspace[3].frame = 3;
	}

	v_objspace[4].id = ID_PSBTM;
	v_objspace[4].frame = 2; // sprite line limiter..?

	ExecuteObjects();
	DeformLayers();
	BuildSprites();
	NewPLC(PLC_Main);
	v_title_dcount = 0;
	v_title_ccount = 0;
	VDP_Control(v_vdp_buffer1 | 0x40);
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
			v_gamemode = GameMode_Sega;
			return;
		}

		auto levSelCode = (v_megadrive & 0x80) ? LevSelCode_US : LevSelCode_J;
		auto buttons = v_jpadpress1 & Buttons_Dir;

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

				PlaySound_Special(SFX_Ring);
			}
		}
		else if(buttons != 0 && v_title_dcount != 9) // seems a little arbitrary...
			v_title_dcount = 0;

		if(v_jpadpress1 & Buttons_C)
			v_title_ccount++;

		if(v_demolength == 0)
		{
			if(GotoDemo()) // returns true if demo fadeout was interrupted by start button
				break;

			return;
		}
	} while(!(v_jpadpress1 & Buttons_Start));

	if(f_levselcheat && v_jpadhold1 & Buttons_A)
	{
		PalLoad2(Palette_LevelSel);

		// This clears the scrolling data (presumably to make all the shit show up in the right place)
		memset(v_hscrolltablebuffer, 0, 0xE0 * sizeof(int));
		v_scrposy_dup = 0;

		DISABLE_INTERRUPTS();
		VDP_SetAddr(0xE000, VDP_VRAM_Write);

		for(int i = 0; i < 0x400; i++)
			VDP_Data(0); //longword

		LevSelDisplayText();

		while(true)
		{
			WaitForVBlank(VBlank_Title);
			LevSelControls();
			RunPLC();

			if(v_plc_buffer == 0 && v_jpadpress1 & (Buttons_ABC | Buttons_Start))
			{
				if(v_levselitem == 0x14) // sound test
				{
					auto sound = v_levselsound + 0x80;

					if(f_creditscheat)
					{
						if(sound == 0x9F)
						{
							v_gamemode = GameMode_Ending;
							v_zone = Zone_EndZ;
							v_act = 0;
							return;
						}
						else if(sound == 0x9E)
						{
							v_gamemode = GameMode_Credits;
							PlaySound_Special(BGM_Credits);
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
						if(ptr.zone == Zone_SS) // special stage?
						{
							v_gamemode = GameMode_Special;
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

	v_gamemode = GameMode_Level;
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
	PlaySound_Special(BGM_Fade);
}

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
			v_gamemode = GameMode_Sega;
			return false;
		}

		if(v_jpadpress1 & Buttons_Start)
			return true;
	} while(v_demolength > 0);

	PlaySound_Special(BGM_Fade);
	auto level = Demo_Levels[v_demonum & 3];
	v_zone = level.zone;
	v_demonum = (v_demonum + 1) & 3;
	f_demo = true;
	v_gamemode = GameMode_Demo;

	if(level.zone == Zone_SS)
	{
		v_gamemode = GameMode_Special;
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
	auto buttonsPressed = v_jpadpress1 & (Buttons_Up | Buttons_Dn);

	if(buttonsPressed || TimerZero(v_levseldelay, 11))
	{
		auto buttonsHeld = v_jpadhold1 & (Buttons_Up | Buttons_Dn);

		if(buttonsHeld)
		{
			if(buttonsHeld & Buttons:Up)
				DecWrap(v_levselitem, 0x14);

			if(buttonsHeld & Buttons_Dn)
				IncWrap(v_levselitem, 0x14);

			LevSelDisplayText();
			return;
		}
	}

	if(v_levselitem == 0x14) // on sound test?
	{
		buttonsPressed = v_jpadpress1 & (Buttons_L | Buttons_R);

		if(buttonsPressed)
		{
			if(buttonsPressed & Buttons_L)
				DecWrap(v_levselsound, 0x4F);

			if(buttonsPressed & Buttons_R)
				IncWrap(v_levselsound, 0x4F);

			LevSelDisplayText();
		}
	}
}


void LevSelDisplayText()
{
	for(int i = 0, textpos = 0xE210; i < 0x15; i++, textpos += 0x80)
	{
		VDP_SetAddr(textpos, VDP_VRAM_Write);
		LevSel_ChgLine(LevelMenuText[i], 0xE680);
	}

	// Then recolor selected line (v_levselitem) by changing palette for that line
	VDP_SetAddr(0xE210 + (0x80 * v_levselitem), VDP_VRAM_Write);
	LevSel_ChgLine(LevelMenuText[v_levselitem], 0xC680);

	// Then draw the sound test line
	VDP_SetAddr(0xEC30, VDP_VRAM_Write);
	auto sfx = v_levselsound + 0x80;
	LevSel_DrawDigit(sfx >> 4, v_levselitem == 0x14 ? 0xC680 : 0xE680);
	LevSel_DrawDigit(sfx, v_levselitem == 0x14 ? 0xC680 : 0xE680);
}

void LevSel_DrawDigit(ushort digit, ushort vram)
{
	digit &= 0x0F;

	if(digit >= 10)
		digit += 7; // use alpha characters

	VDP_Data(vram + digit);
}

void LevSel_ChgLine(const char* text, ushort vram)
{
	for(int i = 0; i < 0x18; i++)
	{
		if(auto ch = *text++)
			VDP_Data(vram + ch);
		else
			VDP_Data(0);
	}
}

// TODO:
// LevelMenuText:	if Revision=0
// 	incbin	"misc\Level Select Text.bin"
// 	else
// 	incbin	"misc\Level Select Text (JP1).bin"
// 	endc
// 	even
