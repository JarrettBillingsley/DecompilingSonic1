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
	SetHBlankRegister();
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

void VBlankBackupScrollVars()
{
	v_screenposx_dup =    v_screenposx;
	v_screenposy_dup =    v_screenposy;
	v_bg1posx_dup =       v_bg1posx;
	v_bg1posy_dup =       v_bg1posy;
	v_somethingposx_dup = v_somethingposx;
	v_somethingposy_dup = v_somethingposy;
	v_bg2posx_dup =       v_bg2posx;
	v_bg2posy_dup =       v_bg2posy;
	v_bgscroll1_dup =     v_bgscroll1;
	v_bgscroll2_dup =     v_bgscroll2;
	v_bgscroll3_dup =     v_bgscroll3;
	v_bgscroll4_dup =     v_bgscroll4;
}

void VBlankDecreaseDemoLength()
{
	DecToZero(v_demolength);
}

void SetHBlankRegister()
{
	VDP_Control(v_hbla_hreg);
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

void VBlank()
{
	auto routine = v_vbla_routine;

	if(routine != 0)
	{
		VDP_SetAddr(0, VDP_VSRAM_Write);
		VDP_Data(v_scrposy_dup);
		VBlankWaitPAL();
		v_vbla_routine = 0;
		f_hbla_pal = true;
	}

	switch(routine)
	{
		case VBlank_00:
			if(v_gamemode & GameMode_Level && v_zone == Zone_LZ)
			{
				// VBlankWaitPAL();
				f_hbla_pal = true;
				STOP_Z80();
				WAIT_Z80();
				VBlankWriteWaterPalette();
				SetHBlankRegister();
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
			DrawTitleCreditsBGTiles();
			PLC_Decode9Tiles();
			break;

		// Palette fading
		case VBlank_PaletteFade:
			VBlankCommon();
			SetHBlankRegister();
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
			if(v_gamemode == GameMode_Special)
				goto _vblank0A;
			// fall through
		// Level main loop
		case VBlank_Level:
			VBlankCommon2();
			VBlankBackupScrollVars();

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
			VBlankBackupScrollVars();
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
		VDP_SetAddr(0, VDP_CRAM_Write);

		for(int i = 0; i < 64; i++)
			VDP_Data(v_pal_water[i]);

		VDP_RegWrite(0x0A, 223); // reset HBlank register

		if(f_FFFFF64F)
		{
			f_FFFFF64F = false;
			V_H_BlankCommon();
			UpdateMusic();
		}
	}
}