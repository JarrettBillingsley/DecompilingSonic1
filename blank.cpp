void WaitForVBlank(int routine)
{
	v_vbla_routine = routine;
	WaitForVBlank();
}

void WaitForVBlank()
{
	ENABLE_INTERRUPTS();
	while(v_vbla_routine != 0) {}
}

void VBlankCommon()
{
	STOP_Z80();
	WAIT_Z80();
	ReadJoypads();
	VBlankWriteWaterPalette();
	VBlankWriteSpriteAndScroll();
	START_Z80();
}

void VBlankCommon2()
{
	STOP_Z80();
	WAIT_Z80();
	ReadJoypads();
	VBlankWriteWaterPalette();
	DoSomethingWithHblaHreg();
	VBlankWriteSpriteAndScroll();
	VBlankWriteSonic();
	START_Z80();
}

void V_H_BlankCommon()
{
	LoadTilesAsYouMove();
	AnimateLevelGfx();
	HUD_Update();
	PLC_Decode3Tiles();
	VBlankDecreaseDemoLength();
}

void VBlankBackupSomeThings()
{
	// copy v_screenposx, v_screenposy, and a bunch of bytes FFFFF700..FFFFF71F to FFFFFF10..FFFFFF2F
	// copy v_bgscroll1, v_bgscroll2, v_bgscroll3, and v_bgscrollvert to FFFFFF30..FFFFFF37
}

void VBlankDecreaseDemoLength()
{
	if(v_demolength > 0)
		v_demolength--;
}

void DoSomethingWithHblaHreg()
{
	// is a5 the VDP control port?? it's never explicitly set but that seems to be the convention
	// *a5 = v_hbla_hreg
}

void VBlankWaitPAL()
{
	// not really needed huh
}

void VBlankWriteSpriteAndScroll()
{
	WriteVRAM(v_spritetablebuffer, 0x280, vram_sprites);
	WriteVRAM(v_hscrolltablebuffer, 0x380, vram_hscroll);
}

void VBlankWriteWaterPalette()
{
	if(f_wtr_state)
		WriteCRAM(v_pal_water, 0x80,0);
	else
		WriteCRAM(v_pal_dry, 0x80, 0);
}

void VBlankWriteSonic()
{
	if(f_sonframechg)
	{
		WriteVRAM(v_sgfx_buffer, 0x2E0, vram_sonic);
		f_sonframechg = false;
	}
}

enum
{
	VBlank_00 = 0x00,
	VBlank_Sega = 0x02,        // Seeeeegaaaaaa!
	VBlank_Title = 0x04,       // Title screen
	VBlank_Credits = 0x04,     // Credits screen
	VBlank_06 = 0x06,
	VBlank_Level = 0x08,       // Level main loop
	VBlank_Special = 0x0A,     // Special stage
	VBlank_TitleCard = 0x0C,   // Waiting for level title card
	VBlank_SSResults = 0x0C,   // Special stage results screen
	VBlank_0E = 0x0E,
	VBlank_Paused = 0x10,      // Game paused
	VBlank_PaletteFade = 0x12, // Palette fading
	VBlank_SegaSwitch = 0x14,  // One frame between SEGA palette cycle and SEEEGAAAA
	VBlank_Continue = 0x16,    // Continue screen
	VBlank_SSFade = 0x16,      // Special stage spin-and-fade-to-white
	VBlank_Ending = 0x18,      // Ending screen
};

void VBlank()
{
	auto routine = v_vbla_routine;

	if(routine != 0)
	{
		// TODO:
		// Set VSRAM write address to 0
		// move.l	#$40000010,(vdp_control_port).l
		// move.l	(v_scrposy_dup).w,(vdp_data_port).l ; send screen y-axis pos. to VSRAM
		// VBlankWaitPAL();
		v_vbla_routine = 0;
		f_hbla_pal = true;
	}

	switch(routine)
	{
		case VBlank_00:
			if(v_gamemode & GameMode::Level && v_zone == Zone::LZ)
			{
				// VBlankWaitPAL();
				f_hbla_pal = true;
				STOP_Z80();
				WAIT_Z80();
				VBlankWriteWaterPalette();
				DoSomethingWithHblaHreg();
				START_Z80();
			}
			break;

		// Seeeeegaaaaaa!
		case VBlank_Sega:
			VBlankCommon();
			// fall through
		// One frame between SEGA palette cycle and SEEEGAAAA
		case VBlank_SegaSwitch:
			VBlankDecreaseDemoLength();
			break;

		// Title screen, Credits screen
		case VBlank_Title:
			VBlankCommon();
			sub_6886(); // something to do with loading tiles
			PLC_Decode9Tiles();
			break;

		// Palette fading
		case VBlank_PaletteFade:
			VBlankCommon();
			DoSomethingWithHblaHreg();
			PLC_Decode9Tiles();
			break;

		case VBlank_06:
			VBlankCommon();
			break;

		case VBlank_0E:
			VBlankCommon();
			v_FFFFF628++;
			v_vbla_routine = 0x0E;
			break;

		// Game paused
		case VBlank_Paused:
			if(v_gamemode == GameMode::Special)
				goto _vblank0A;
			// fall through
		// Level main loop
		case VBlank_Level:
			VBlankCommon2();
			VBlankBackupSomeThings();

			if(v_hbla_line >= 96)
				V_H_BlankCommon();
			else
			{
				f_FFFFF64F = true;
				v_vbla_count++;
				return;
			}
			break;

		// Waiting for level title card, special stage results screen
		case VBlank_Title:
		// Ending screen
		case VBlank_Ending:
			VBlankCommon2();
			VBlankBackupSomeThings();
			LoadTilesAsYouMove();
			AnimateLevelGfx();
			HUD_Update();
			PLC_Decode9Tiles();
			break;

		// Special stage
		case VBlank_Special:
		_vblank0A:
			STOP_Z80();
			WAIT_Z80();
			ReadJoypads();
			WriteCRAM(v_pal_dry, 0x80, 0);
			VBlankWriteSpriteAndScroll();
			START_Z80();
			PalCycle_SS();
			VBlankWriteSonic();
			VBlankDecreaseDemoLength();
			break;

		// Continue screen, special stage spin-and-fade-to-white
		case VBlank_Continue:
			STOP_Z80();
			WAIT_Z80();
			ReadJoypads();
			WriteCRAM(v_pal_dry, 0x80, 0);
			VBlankWriteSpriteAndScroll();
			START_Z80();
			VBlankWriteSonic();
			VBlankDecreaseDemoLength();
			break;

		default: assert(false);
	}

	UpdateMusic();
	v_vbla_count++;
}

void HBlank()
{
	DISABLE_INTERRUPTS();

	if(f_hbla_pal)
	{
		f_hbla_pal = false;

		// TODO:
		// Copy v_pal_water to CRAM here

		// move.w	#$8A00+223,4(a1) ; reset HBlank register

		if(f_FFFFF64F)
		{
			f_FFFFF64F = false;
			V_H_BlankCommon();
			UpdateMusic();
		}
	}
}