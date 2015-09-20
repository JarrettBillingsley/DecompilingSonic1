void GM_Continue()
{
	PaletteFadeOut();
	DISABLE_INTERRUPTS();

	// TODO:
	// move.w	(v_vdp_buffer1).w,d0
	// andi.b	#0xBF,d0
	// move.w	d0,(vdp_control_port).l
	// lea	(vdp_control_port).l,a6
	// move.w	#0x8004,(a6)	; 8 colour mode
	// move.w	#0x8700,(a6)	; background colour

	ClearScreen();
	Clear_Objects();

	NemDec(Nem_TitleCard, 0xB000);
	NemDec(Nem_ContSonic, 0xA000);
	NemDec(Nem_MiniSonic, 0xAA20);

	ContScrCounter(10);
	PalLoad1(Palette::Continue);
	PlaySound(BGM::Continue);
	v_demolength = 659;
	v_screenposx = 0;
	v_screenposy = 0x1000000;

	v_objspace[0].id = ID::ContSonic;
	v_objspace[1].id = ID::ContScrItem;
	v_objspace[2].id = ID::ContScrItem;
	v_objspace[2].priority = 3;
	v_objspace[2].frame = 4;
	v_objspace[3].id = ID::ContScrItem;
	v_objspace[3].obRoutine = 4;

	ExecuteObjects();
	BuildSprites();

	// move.w	(v_vdp_buffer1).w,d0
	// ori.b	#0x40,d0
	// move.w	d0,(vdp_control_port).l

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
			v_gamemode = GameMode::Level;
			v_lives = 3;
			v_rings = 0;
			v_time = 0;
			v_score = 0;
			v_lastlamp = 0;
			v_continues = 0;
			return;
		}
	} while(v_objspace[0].routine >= 6 || v_demolength > 0)

	v_gamemode = GameMode::Sega;
}

void ContScrCounter(int d1)
{
	// ?? Original doesn't use d4
	// moveq	#0,d4

	for(int i = 0; i < 2; i++)
	{
		int digit = 0;
		int place = (i == 0) ? 10 : 1;

		while(d1 >= place)
		{
			d1 -= place;
			digit++;
		}

		a3 = Art_Hud[digit << 6];

		// TOOD:
		// locVRAM	$DF80
		// move.l	(a3)+,(vdp_data_port)
		// move.l	(a3)+,(vdp_data_port)
		// move.l	(a3)+,(vdp_data_port)
		// move.l	(a3)+,(vdp_data_port)
		// move.l	(a3)+,(vdp_data_port)
		// move.l	(a3)+,(vdp_data_port)
		// move.l	(a3)+,(vdp_data_port)
		// move.l	(a3)+,(vdp_data_port)
		// move.l	(a3)+,(vdp_data_port)
		// move.l	(a3)+,(vdp_data_port)
		// move.l	(a3)+,(vdp_data_port)
		// move.l	(a3)+,(vdp_data_port)
		// move.l	(a3)+,(vdp_data_port)
		// move.l	(a3)+,(vdp_data_port)
		// move.l	(a3)+,(vdp_data_port)
		// move.l	(a3)+,(vdp_data_port)
	}
}