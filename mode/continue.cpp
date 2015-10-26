void GM_Continue()
{
	PaletteFadeOut();
	DISABLE_INTERRUPTS();

	VDP_Control(v_vdp_buffer1 & 0xFFBF);
	VDP_RegWrite(0x00, 4); // 8 color mode
	VDP_RegWrite(0x07, 0); // BG color

	ClearScreen();
	Clear_Objects();

	NemDec(Nem_TitleCard, 0xB000);
	NemDec(Nem_ContSonic, 0xA000);
	NemDec(Nem_MiniSonic, 0xAA20);

	ContScrCounter(10);
	PalLoad1(Palette_Continue);
	PlaySound(BGM_Continue);
	v_demolength = 659;
	v_screenposx = 0;
	v_screenposy = 0x1000000;

	v_objspace[0].id = ID_ContSonic;
	v_objspace[1].id = ID_ContScrItem;
	v_objspace[2].id = ID_ContScrItem;
	v_objspace[2].priority = 3;
	v_objspace[2].frame = 4;
	v_objspace[3].id = ID_ContScrItem;
	v_objspace[3].obRoutine = 4;

	ExecuteObjects();
	BuildSprites();

	VDP_Control(v_vdp_buffer1 | 0x40);

	PaletteFadeIn();

	do
	{
		WaitForVBlank(VBlank_Continue);

		if(v_objspace[0].routine < 6)
		{
			DISABLE_INTERRUPTS();
			ContScrCounter((v_demolength / 60) & 0xF);
			ENABLE_INTERRUPTS();
		}

		ExecuteObjects();
		BuildSprites();

		// Has Sonic run off screen?
		if(v_objspace[0].x) >= 384)
		{
			v_gamemode = GameMode_Level;
			v_lives = 3;
			v_rings = 0;
			v_time = 0;
			v_score = 0;
			v_lastlamp = 0;
			v_continues = 0;
			return;
		}
	} while(v_objspace[0].routine >= 6 || v_demolength > 0)

	v_gamemode = GameMode_Sega;
}

void ContScrCounter(int d1)
{
	for(int i = 0; i < 2; i++)
	{
		int digit = 0;
		int place = (i == 0) ? 10 : 1;

		while(d1 >= place)
		{
			d1 -= place;
			digit++;
		}

		auto art = Art_Hud[digit << 6];

		VDP_SetAddr(0xDF80, VDP_VRAM_Write);

		for(int i = 0; i < 16; i++)
			VDP_Data(art[i]); // longword
	}
}