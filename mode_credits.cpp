void GM_Credits()
{
	ClearPLC();
	PaletteFadeOut();

	// TODO:
	// lea	(vdp_control_port).l,a6
	// move.w	#0x8004,(a6)		; 8-colour mode
	// move.w	#0x8200+(vram_fg>>10),(a6) ; set foreground nametable address
	// move.w	#0x8400+(vram_bg>>13),(a6) ; set background nametable address
	// move.w	#0x9001,(a6)		; 64-cell hscroll size
	// move.w	#0x9200,(a6)		; window vertical position
	// move.w	#0x8B03,(a6)		; line scroll mode
	// move.w	#0x8720,(a6)		; set background colour (line 3; colour 0)

	f_wtr_state = false;
	ClearScreen();
	Clear_Objects();
	NemDec(Nem_CreditText, 0xB400);
	Clear_Palette(v_pal_dry_dup);
	PalLoad1(Palette::Sonic);
	v_objspace[2].id = ID::CreditsText;
	ExecuteObjects();
	BuildSprites();
	EndingDemoLoad();

	auto plc = LevelHeaders[v_zone].gfx >> 24;

	if(plc)
		AddPLC(plc);

	AddPLC(PLC::Main2);
	v_demolength = 120;
	PaletteFadeIn();

	do
	{
		WaitForVBlank(VBlank_Credits);
		RunPLC();
	} while(v_demolength || v_plc_buffer);

	if(v_creditsnum == 9)
	{
		ClearPLC();
		PaletteFadeOut();

		// TODO:
		// lea	(vdp_control_port).l,a6
		// move.w	#0x8004,(a6)	; use 8-colour mode
		// move.w	#0x8200+(vram_fg>>10),(a6) ; set foreground nametable address
		// move.w	#0x8400+(vram_bg>>13),(a6) ; set background nametable address
		// move.w	#0x9001,(a6)	; 64-cell hscroll size
		// move.w	#0x9200,(a6)	; window vertical position
		// move.w	#0x8B03,(a6)	; line scroll mode
		// move.w	#0x8720,(a6)	; set background colour (line 3; colour 0)

		f_wtr_state = false;
		ClearScreen();
		Clear_Objects();
		QuickPLC(PLC::TryAgain);
		Clear_Palette(v_pal_dry_dup);
		PalLoad1(Palette::Ending);
		v_pal_dry_dup[32] = 0;
		v_objspace[2].id = ID::EndEggman;
		ExecuteObjects();
		BuildSprites();
		v_demolength = 1800;
		PaletteFadeIn();

		do
		{
			PauseGame();
			WaitForVBlank(VBlank_Credits);
			ExecuteObjects();
			BuildSprites();

			if(v_jpadpress1 & Buttons::Start || v_demolength == 0)
				break;
		} while(v_gamemode == GameMode::Credits);

		v_gamemode = GameMode::Sega;
		return;
	}
}

void EndingDemoLoad()
{
	auto place = EndDemo_Levels[v_creditsnum & 0xF];
	v_zone = place.zone;
	v_act = place.act;

	v_creditsnum++;

	if(v_creditsnum < 9)
	{
		f_demo = DemoMode::On || DemoMode::Credits;
		v_gamemode = GameMode::Demo;
		v_lives = 3;
		v_rings = 0;
		v_time = 0;
		v_score = 0;
		v_lastlamp = 0;

		if(v_creditsnum == 4) // SLZ demo
		{
			v_lastlamp = 1;
			v_FFFFFE31 = 1;
			v_lamp_xpos = 0x0A00;
			v_lamp_ypos = 0x62C;
			v_lamp_rings = 13;
			v_lamp_time = 0;
			v_lamp_dle = 0;
			v_FFFFFE3D = 0;
			v_lamp_limitbtm = 0x800;
			v_lamp_scrx = 0x957;
			v_lamp_scry = 0x5CC;
			v_FFFFFE44 = 0x4AB;
			v_FFFFFE46 = 0x3A6;
			v_FFFFFE48 = 0;
			v_FFFFFE4A = 0x28C;
			v_FFFFFE4C = 0;
			v_FFFFFE4E = 0;
			v_lamp_wtrpos = 0x308;
			v_lamp_wtrrout = 1;
			v_lamp_wtrstat = 1;
		}
	}
}