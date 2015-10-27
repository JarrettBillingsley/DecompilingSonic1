void GM_Ending()
{
	PlaySound_Special(BGM_Stop);
	PaletteFadeOut();
	Clear_Objects();
	Clear_F628_F680();
	Clear_F700_F800();
	Clear_FE60_FF80();
	DISABLE_INTERRUPTS();
		VDP_Control(v_vdp_buffer1 & 0xFFBF);
		ClearScreen();
		VDP_RegWrite(0x0B, 3);                 // line scroll mode
		VDP_RegWrite(0x02, vram_fg >> 10);     // set foreground nametable address
		VDP_RegWrite(0x04, vram_bg >> 13);     // set background nametable address
		VDP_RegWrite(0x05, vram_sprites >> 9); // set sprite table address
		VDP_RegWrite(0x10, 1);                 // 64-cell hscroll size
		VDP_RegWrite(0x00, 4);                 // 8-colour mode
		VDP_RegWrite(0x07, 0x20);              // set background colour (line 3; colour 0)
		v_hbla_hreg = 0x8A00 + 223;
		VDP_Control(v_hbla_hreg);

		v_air = 30;
		v_zone = Zone_EndZ;
		v_act = (v_emeralds == 6) ? 0 : 1;

		QuickPLC(PLC_Ending);
		Hud_Base();
		LevelSizeLoad();
		DeformLayers();
		v_bgscroll1 |= 4;
		LevelDataLoad();
		LoadTilesFromStart();
		v_collindex = Col::GHZ;
	ENABLE_INTERRUPTS();
	KosDec(Kos_EndFlowers, 0xFFFF9400);
	PalLoad1(Palette_Sonic);
	PlaySound(BGM_Ending);

	if(v_jpadhold1 & Buttons_A)
		f_debugmode = true;

	v_player->id = ID_SonicPlayer;
	v_player->status |= 1; // face left
	f_lockctrl = true;
	v_jpadhold2 |= Buttons_L;
	v_player->inertia = 0xF800;
	v_objspace[1].id = ID_HUD;
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
	VDP_Control(v_vdp_buffer1 | 0x40);
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

		if(v_gamemode != GameMode_Ending)
		{
			v_gamemode = GameMode_Credits;
			PlaySound_Special(BGM_Credits);
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

				if(TimerZero(v_palchgspeed, 3))
					WhiteOut_ToWhite();
			} while(!f_restart);


			f_restart = false;

			// This replaces the un-flowered chunks with the flowered chunks
			v_lvllayout[1][0] = 0x2E;
			v_lvllayout[1][1] = 0x2F;

			DrawChunks(0x4000, &v_screenposx, v_lvllayout);
			PalLoad1(Palette_Ending);
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
				v_jpadhold2 = Buttons_R;
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
			v_player->id = ID_EndSonic;
			v_player->routine = 0;
			break;

		default:
			break;
	}
}