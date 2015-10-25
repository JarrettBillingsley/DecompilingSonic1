void GM_Level()
{
_restart:
	v_gamemode |= GameMode::PreLevel; // flag that we're in pre-level sequence

	if(f_demo != DemoMode::Credits)
		PlaySound_Special(BGM::Fade);

	ClearPLC();
	PaletteFadeOut();

	// If we're not doing an ending sequence demo...
	if(f_demo != DemoMode::Credits)
	{
		DISABLE_INTERRUPTS();
			NemDec(Nem_TitleCard, 0xB000);
		ENABLE_INTERRUPTS();

		auto plc1 = LevelHeaders[v_zone].gfx >> 24;

		if(plc1 != 0)
			AddPLC(plc1); // load level patterns

		AddPLC(PLC::Main2); // load standard patterns
	}

	// Clear shit out
	Clear_Objects();
	Clear_F628_F680();
	Clear_F700_F800();
	Clear_FE60_FF80();
	DISABLE_INTERRUPTS();
		ClearScreen();

		// TODO:
		// lea	(vdp_control_port).l,a6
		// move.w	#$8B03,(a6)	; line scroll mode
		// move.w	#$8200+(vram_fg>>10),(a6) ; set foreground nametable address
		// move.w	#$8400+(vram_bg>>13),(a6) ; set background nametable address
		// move.w	#$8500+(vram_sprites>>9),(a6) ; set sprite table address
		// move.w	#$9001,(a6)		; 64-cell hscroll size
		// move.w	#$8004,(a6)		; 8-colour mode
		// move.w	#$8720,(a6)		; set background colour (line 3; colour 0)

		// Set up h-interrupts for Labyrinth's water
		v_hbla_hreg = 0x8A00 + 223; // water palette change pos

		if(v_zone == Zone::LZ)
		{
			ENABLE_H_INT();

			v_waterpos1 = v_waterpos2 = v_waterpos3 = WaterHeight[v_act];
			v_wtr_routine = 0;
			f_wtr_state = 0;
			f_water = true;
		}

		v_air = 30;
	ENABLE_INTERRUPTS();

	// Setup palettes
	PalLoad2(Palette::Sonic);

	if(v_zone == Zone::LZ)
	{
		PalLoad3_Water(v_act == 3 ? Palette::SBZ3SonWat : Palette::LZSonWater);

		if(v_lastLamp)
			f_wtr_state = v_lamp_wtrstat;
	}

	// Start the music and do the title card
	if(f_demo != DemoMode::Credits)
	{
		if(v_zone == Zone::LZ && v_act == 3)
			PlaySound(BGM::SBZ);
		else if(v_zone == Zone::SBZ && v_act == 2)
			PlaySound(BGM::FZ);
		else
			PlaySound(MusicList[v_zone]);

		v_objspace[2].id = ID::TitleCard;

		// Wait for title card to finish
		do
		{
			WaitForVBlank(VBlank_TitleCard);
			ExecuteObjects();
			BuildSprites();
			RunPLC();

		} while(v_objspace[4].x != v_objspace[4].mainX || v_plc_buffer != 0);

		Hud_Base();
	}

	// Load a bunch of crap
	PalLoad1(Palette::Sonic);
	LevelSizeLoad();
	DeformLayers();
	v_bgscroll1 |= (1 << 2);
	LevelDataLoad();
	LoadTilesFromStart();
	FloorLog_Unk();
	ColIndexLoad();
	LZWaterFeatures();

	// Sonic and the HUD
	v_player->id = ID::SonicPlayer;

	if(f_demo != DemoMode::Credits)
		v_objspace[1].id = ID::HUD;

	// Check for debug mode
	if(f_debugcheat && (v_jpadhold1 & Buttons::A))
		f_debugmode = true;

	v_jpadhold2 = 0;
	v_jpadhold1 = 0;

	// Set up water surface objects
	if(v_zone == Zone::LZ)
	{
		v_objspace[30].id = ID::WaterSurface;
		v_objspace[30].x = 96;
		v_objspace[31].id = ID::WaterSurface;
		v_objspace[31].x = 288;
	}

	// Load level objects
	ObjPosLoad();
	ExecuteObjects();
	BuildSprites();

	// Zero out a bunch of stuff
	if(v_lastlamp == 0)
	{
		v_rings = 0;
		v_time = 0;
		v_lifecount = 0;
	}

	f_timeover = 0;
	v_shield = 0;
	v_invinc = 0;
	v_shoes = 0;
	// FFFFFE2F = 0;
	v_debuguse = 0;
	f_restart = 0;
	v_framecount = 0;

	OscillateNumInit();

	// Update HUD
	f_scorecount = true;
	f_ringcount = true;
	f_timecount = true;

	// Demo stuff
	v_btnpushtime1 = 0;

	auto demoData = (f_demo == DemoMode::Credits) ? DemoEndDataPtr[v_creditsnum - 1] : DemoDataPtr[v_zone];

	v_btnpushtime2 = demoData[1] - 1;

	if(f_demo == DemoMode::Credits)
		v_demolength = v_creditsnum == 4 ? 510 : 540;
	else
		v_demoLength = 1800;

	// Water palette
	if(v_zone == Zone::LZ)
		PalLoad4_Water(v_act == 3 ? Palette::SBZ3Water : Palette::LZWater);

	// Delay a few frames
	for(int i = 0; i < 4; i++)
		WaitForVBlank(VBlank_Level);

	v_pfade_start = 0x202F; // fade in 2nd, 3rd & 4th palette lines
	PalFadeIn_Alt();

	if(f_demo == DemoMode::Credits)
	{
		AddPLC(PLC::Explode);
		AddPLC(v_zone + PLC::GHZAnimals);
	}
	else
	{
		// Make the title card fly away
		v_objspace[2].routine += 2;
		v_objspace[3].routine += 4;
		v_objspace[4].routine += 4;
		v_objspace[5].routine += 4;
	}

	v_gamemode &= ~GameMode::PreLevel; // Clear top bit to indicate we're done with pre-level stuff

	// ===========================================================================
	// Main level loop (when all title card and loading sequences are finished)
	// ===========================================================================

Level_MainLoop:
	while(true)
	{
		PauseGame();
		WaitForVBlank(VBlank_Level);
		v_framecount += 1;
		MoveSonicInDemo();
		LZWaterFeatures();
		ExecuteObjects();

		// Rev 1 only
		if(f_restart)
			goto _restart;

		if(v_debuguse || (v_player->routine < 6))
			DeformLayers();

		BuildSprites();
		ObjPosLoad();
		PaletteCycle();
		RunPLC();
		OscillateNumDo();
		SynchroAnimate();
		SignpostArtLoad();

		if(v_gamemode == GameMode::Demo)
		{
			if(f_restart || v_demolength == 0)
			{
				if(v_gamemode == GameMode::Demo)
					v_gamemode = (f_demo == DemoMode::Credits) ? GameMode::Credits : GameMode::Sega

				v_demoLength = 60;
				v_pfade_start = 63;
				v_palchgspeed = 0;

				while(true)
				{
					WaitForVBlank(VBlank_Level);
					MoveSonicInDemo();
					ExecuteObjects();
					BuildSprites();
					ObjPosLoad();

					v_palchgspeed -= 1;

					if(v_palchgspeed < 0)
					{
						v_palchgspeed = 2;
						FadeOut_ToBlack();
					}


					if(v_demolength == 0)
						return;
				}
			}
			else if(v_gamemode != GameMode::Demo)
			{
				v_gamemode = GameMode::Sega;
				return;
			}
		}
		else if(v_gamemode != GameMode::Level)
			return;
	}
}

void PauseGame()
{
	if(v_lives > 0 && f_pause == false && v_jpadpress1 & Buttons::Start)
	{
		f_pause = true;
		f_stopmusic = true;

		do
		{
			WaitForVBlank(VBlank_Paused);

			if(f_slomocheat)
			{
				if(v_jpadpress1 & Buttons::A)
				{
					v_gamemode = GameMode::Title;
					break;
				}

				if(v_jpadhold1 & Buttons::B || v_jpadpress1 & Buttons::C)
				{
					f_pause = true;
					f_stopmusic = 0x80;
					return;
				}
			}
		} while(!(v_jpadpress1 & Buttons::Start));

		f_stopmusic = 0x80;
	}

	f_pause = false;
}

void SignpostArtLoad()
{
	if(v_debuguse || v_act == 2)
		return;

	if(v_screenposx >= v_limitright2 - 256 && f_timecount && v_limitleft2 > 0)
	{
		v_limitleft2 = v_screenposx;
		NewPLC(PLC::Signpost);
	}
}

const ubyte* ColPointers[] =
{
	Col_GHZ,
	Col_LZ,
	Col_MZ,
	Col_SLZ,
	Col_SYZ,
	Col_SBZ,
//	Col_GHZ // Pointer for Ending is missing by default.
};

void ColIndexLoad()
{
	v_collindex = ColPointers[v_zone];
}

// ===========================================================================
// Subroutine to change synchronised animation variables (rings, giant rings)
// ===========================================================================

void SynchroAnimate()
{
	// Used for GHZ spiked log
	if(TimerZero(v_ani0_time, 11))
		v_ani0_frame = (v_ani0_frame - 1) & 7;

	// Used for rings and giant rings
	if(TimerZero(v_ani1_time, 7))
		v_ani1_frame = (v_ani1_frame + 1) & 3;

	// Used for nothing
	if(TimerZero(v_ani2_time, 7))
		IncWrap(v_ani2_frame, 5);

	// Used for bouncing rings
	if(v_ani3_time != 0)
	{
		v_ani3_buf += v_ani3_time;
		v_ani3_frame = ROL16(v_ani3_buf, 7) & 3;
		v_ani3_time--;
	}
}