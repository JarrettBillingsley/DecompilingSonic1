void GM_Credits()
{
	ClearPLC();
	PaletteFadeOut();
	VDP_RegWrite(0x00, 4); // 8-color
	VDP_RegWrite(0x02, vram_fg >> 10);
	VDP_RegWrite(0x04, vram_bg >> 13);
	VDP_RegWrite(0x10, 1); // 64-cell hscroll
	VDP_RegWrite(0x12, 0); // vertical pos
	VDP_RegWrite(0x0B, 3); // line scroll mode
	VDP_RegWrite(0x07, 0x20); // BG color
	f_wtr_state = false;
	ClearScreen();
	Clear_Objects();
	NemDec(Nem_CreditText, 0xB400);
	Clear_Palette(v_pal_dry_dup);
	PalLoad1(Palette_Sonic);
	v_objspace[2].id = ID_CreditsText;
	ExecuteObjects();
	BuildSprites();
	EndingDemoLoad();

	auto plc = LevelHeaders[v_zone].gfx >> 24;

	if(plc)
		AddPLC(plc);

	AddPLC(PLC_Main2);
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
		VDP_RegWrite(0x00, 4);             // use 8-colour mode
		VDP_RegWrite(0x02, vram_fg >> 10); // set foreground nametable address
		VDP_RegWrite(0x04, vram_bg >> 13); // set background nametable address
		VDP_RegWrite(0x10, 1);             // 64-cell hscroll size
		VDP_RegWrite(0x12, 0);             // window vertical position
		VDP_RegWrite(0x0B, 3);             // line scroll mode
		VDP_RegWrite(0x07, 0x20);          // set background colour (line 3; colour 0)
		f_wtr_state = false;
		ClearScreen();
		Clear_Objects();
		QuickPLC(PLC_TryAgain);
		Clear_Palette(v_pal_dry_dup);
		PalLoad1(Palette_Ending);
		v_pal_dry_dup[32] = 0;
		v_objspace[2].id = ID_EndEggman;
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

			if(v_jpadpress1 & Buttons_Start || v_demolength == 0)
				break;
		} while(v_gamemode == GameMode_Credits);

		v_gamemode = GameMode_Sega;
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
		f_demo = DemoMode_On || DemoMode_Credits;
		v_gamemode = GameMode_Demo;
		v_lives = 3;
		v_rings = 0;
		v_time = 0;
		v_score = 0;
		v_lastlamp = 0;

		if(v_creditsnum == 4) // SLZ demo
		{
			v_lastlamp = 1;
			v_lamp_lastlamp = 1;
			v_lamp_xpos = 0x0A00;
			v_lamp_ypos = 0x62C;
			v_lamp_rings = 13;
			v_lamp_time = 0;
			v_lamp_dle = 0;
			v_FFFFFE3D = 0;
			v_lamp_limitbtm = 0x800;
			v_lamp_scrx = 0x957;
			v_lamp_scry = 0x5CC;
			v_lamp_bg1x = 0x4AB;
			v_lamp_bg1y = 0x3A6;
			v_lamp_somethingx = 0;
			v_lamp_somethingy = 0x28C;
			v_lamp_bg2x = 0;
			v_lamp_bg2y = 0;
			v_lamp_wtrpos = 0x308;
			v_lamp_wtrrout = 1;
			v_lamp_wtrstat = 1;
		}
	}
}