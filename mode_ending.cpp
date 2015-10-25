void GM_Ending()
{
	PlaySound_Special(BGM::Stop);
	PaletteFadeOut();
	Clear_Objects();
	Clear_F628_F680();
	Clear_F700_F800();
	Clear_FE60_FF80();
	DISABLE_INTERRUPTS();
		// TODO:
		// move.w	(v_vdp_buffer1).w,d0
		// andi.b	#0xBF,d0
		// move.w	d0,(vdp_control_port).l
		ClearScreen();

		// move.w	#0x8B03,(vdp_control_port)	; line scroll mode
		// move.w	#0x8200+(vram_fg>>10),(vdp_control_port) ; set foreground nametable address
		// move.w	#0x8400+(vram_bg>>13),(vdp_control_port) ; set background nametable address
		// move.w	#0x8500+(vram_sprites>>9),(vdp_control_port) ; set sprite table address
		// move.w	#0x9001,(vdp_control_port)		; 64-cell hscroll size
		// move.w	#0x8004,(vdp_control_port)		; 8-colour mode
		// move.w	#0x8720,(vdp_control_port)		; set background colour (line 3; colour 0)
		// move.w	#0x8A00+223,(v_hbla_hreg).w ; set palette change position (for water)
		// move.w	(v_hbla_hreg).w,(vdp_control_port)

		v_air = 30;
		v_zone = Zone::EndZ;
		v_act = (v_emeralds == 6) ? 0 : 1;

		QuickPLC(PLC::Ending);
		Hud_Base();
		LevelSizeLoad();
		DeformLayers();
		v_bgscroll1 |= 4;
		LevelDataLoad();
		LoadTilesFromStart();
		v_collindex = Col::GHZ;
	ENABLE_INTERRUPTS();
	KosDec(Kos_EndFlowers, 0xFFFF9400);
	PalLoad1(Palette::Sonic);
	PlaySound(BGM::Ending);

	if(v_jpadhold1 & Buttons::A)
		f_debugmode = true;

	v_player->id = ID::SonicPlayer;
	v_player->status |= 1; // face left
	f_lockctrl = true;
	v_jpadhold2 |= Buttons::L;
	v_player->inertia = 0xF800;
	v_objspace[1].id = ID::HUD;
	ObjPosLoad();
	ExecuteObjects();
	BuildSprites();
	v_rings = 0;
	v_time = 0;
	v_lifecount = 0;
	v_shield = 0;
	v_invinc = 0;
	v_shoes = 0;
	v_FFFFFE2F = 0;
	v_debuguse = 0;
	f_restart = 0;
	v_framecount = 0;
	OscillateNumInit();
	f_scorecount = true;
	f_ringcount = true;
	f_timecount false;
	v_demolength = 1800;
	WaitForVBlank(VBlank_Ending);

	// TODO:
	// move.w	(v_vdp_buffer1).w,d0
	// ori.b	#0x40,d0
	// move.w	d0,(vdp_control_port).l

	v_pfade_start = 63;
	PaletteFadeIn();

	while(true)
	{
		PauseGame();
		WaitForVBlank(VBlank_Ending);
		v_framecount++;
		End_MoveSonic();
		ExecuteObjects();
		DeformLayers();
		BuildSprites();
		ObjPosLoad();
		PaletteCycle();
		OscillateNumDo();
		SynchroAnimate();

		if(v_gamemode != GameMode::Ending)
		{
			v_gamemode = GameMode::Credits;
			PlaySound_Special(BGM::Credits);
			v_creditsnum = 0;
			return;
		}

		if(f_restart) // has sonic released the emeralds?
		{
			f_restart = false;
			v_pfade_start = 63;
			v_palchgspeed = 0;

			do
			{
				PauseGame();
				WaitForVBlank(VBlank_Ending);
				v_framecount++;
				End_MoveSonic();
				ExecuteObjects();
				DeformLayers();
				BuildSprites();
				ObjPosLoad();
				OscillateNumDo();
				SynchroAnimate();

				if(TimerZero(v_palchgspeed))
				{
					v_palchgspeed = 3;
					WhiteOut_ToWhite();
				}
			} while(!f_restart);


			f_restart = false;

			// TODO:
			// move.w	#0x2E2F,(v_lvllayout+0x80).w ; modify level layout
			// lea	(vdp_control_port).l,a5
			// lea	(vdp_data_port).l,a6
			// lea	(v_screenposx).w,a3
			// lea	(v_lvllayout).w,a4
			// move.w	#0x4000,d2
			// jsr	DrawChunks
			PalLoad1(Palette::Ending);
			PaletteWhiteIn();
		}
	}
}

void End_MoveSonic()
{
	auto sonicEnd = v_sonicend;

	switch(v_sonicEnd)
	{
		case 0:
			if(v_player->x < 144)
			{
				v_sonicend += 2;
				f_lockctrl = true;
				v_jpadhold2 = Buttons::R;
			}
			break;

		case 2:
			if(v_player->x >= 160)
			{
				v_sonicend += 2;
				f_lockctrl = false;
				v_jpadhold2 = 0;
				v_player->inertia = 0;
				f_lockmulti = 0x81;
				v_player->frame = 3;
				v_player->anim = (Anim::Wait << 8) | Anim::Wait;
				v_player->timeFrame = 3;
			}
			break;

		case 4:
			v_sonicend += 2;
			v_player->x = 160;
			v_player->id = ID::EndSonic;
			v_player->routine = 0;
			break;

		default:
			break;
	}
}