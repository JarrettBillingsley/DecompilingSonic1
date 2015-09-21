namespace PLC
{
	enum
	{
		Main =         0x00,
		Main2 =        0x01,
		Explode =      0x02,
		GameOver =     0x03,
		GHZ =          0x04,
		GHZ2 =         0x05,
		LZ =           0x06,
		LZ2 =          0x07,
		MZ =           0x08,
		MZ2 =          0x09,
		SLZ =          0x0A,
		SLZ2 =         0x0B,
		SYZ =          0x0C,
		SYZ2 =         0x0D,
		SBZ =          0x0E,
		SBZ2 =         0x0F,
		TitleCard =    0x10,
		Boss =         0x11,
		Signpost =     0x12,
		Warp =         0x13,
		SpecialStage = 0x14,
		GHZAnimals =   0x15,
		LZAnimals =    0x16,
		MZAnimals =    0x17,
		SLZAnimals =   0x18,
		SYZAnimals =   0x19,
		SBZAnimals =   0x1A,
		SSResult =     0x1B,
		Ending =       0x1C,
		TryAgain =     0x1D,
		EggmanSBZ2 =   0x1E,
		FZBoss =       0x1F,
	};
}

const PLCList ArtLoadCues[][] =
{
	{ // Main
		5,
		{
			{ Nem_Lamp,        0xF400 }, // lamppost
			{ Nem_Hud,         0xD940 }, // HUD
			{ Nem_Lives,       0xFA80 }, // livescounter
			{ Nem_Ring,        0xF640 }, // rings
			{ Nem_Points,      0xF2E0 }, // points from enemy
		}
	},
	{ // Main2
		3,
		{
			{ Nem_Monitors,    0xD000 }, // monitors
			{ Nem_Shield,      0xA820 }, // shield
			{ Nem_Stars,       0xAB80 }, // invincibility stars
		}
	},
	{ // Explode
		1,
		{
			{ Nem_Explode,     0xB400 }, // explosion
		}
	},
	{ // GameOver
		1,
		{
			{ Nem_GameOver,    0xABC0 }, // game/time over
		}
	},
	{ // GHZ
		12,
		{
			{ Nem_GHZ_1st,     0x0000 }, // GHZ main patterns
			{ Nem_GHZ_2nd,     0x39A0 }, // GHZ secondary patterns
			{ Nem_Stalk,       0x6B00 }, // flower stalk
			{ Nem_PplRock,     0x7A00 }, // purple rock
			{ Nem_Crabmeat,    0x8000 }, // crabmeat enemy
			{ Nem_Buzz,        0x8880 }, // buzz bomber enemy
			{ Nem_Chopper,     0x8F60 }, // chopper enemy
			{ Nem_Newtron,     0x9360 }, // newtron enemy
			{ Nem_Motobug,     0x9E00 }, // motobug enemy
			{ Nem_Spikes,      0xA360 }, // spikes
			{ Nem_HSpring,     0xA460 }, // horizontal spring
			{ Nem_VSpring,     0xA660 }, // vertical spring
		}
	},
	{ // GHZ2
		6,
		{
			{ Nem_Swing,       0x7000 }, // swinging platform
			{ Nem_Bridge,      0x71C0 }, // bridge
			{ Nem_SpikePole,   0x7300 }, // spiked pole
			{ Nem_Ball,        0x7540 }, // giant ball
			{ Nem_GhzWall1,    0xA1E0 }, // breakable wall
			{ Nem_GhzWall2,    0x6980 }, // normal wall
		}
	},
	{ // LZ
		12,
		{
			{ Nem_LZ,          0x0000 }, // LZ main patterns
			{ Nem_LzBlock1,    0x3C00 }, // block
			{ Nem_LzBlock2,    0x3E00 }, // blocks
			{ Nem_Splash,      0x4B20 }, // waterfalls and splash
			{ Nem_Water,       0x6000 }, // water surface
			{ Nem_LzSpikeBall, 0x6200 }, // spiked ball
			{ Nem_FlapDoor,    0x6500 }, // flapping door
			{ Nem_Bubbles,     0x6900 }, // bubbles and numbers
			{ Nem_LzBlock3,    0x7780 }, // block
			{ Nem_LzDoor1,     0x7880 }, // vertical door
			{ Nem_Harpoon,     0x7980 }, // harpoon
			{ Nem_Burrobot,    0x94C0 }, // burrobot enemy
		}
	},
	{ // LZ2
		12, // 13 for rev0
		{
			{ Nem_LzPole,      0x7BC0 }, // pole that breaks
			{ Nem_LzDoor2,     0x7CC0 }, // large horizontal door
			{ Nem_LzWheel,     0x7EC0 }, // wheel
			{ Nem_Gargoyle,    0x5D20 }, // gargoyle head
			// { Nem_LzSonic,     0x8800 }, // Sonic holding his breath (rev 0 only)
			{ Nem_LzPlatfm,    0x89E0 }, // rising platform
			{ Nem_Orbinaut,    0x8CE0 }, // orbinaut enemy
			{ Nem_Jaws,        0x90C0 }, // jaws enemy
			{ Nem_LzSwitch,    0xA1E0 }, // //switch
			{ Nem_Cork,        0xA000 }, // cork block
			{ Nem_Spikes,      0xA360 }, // spikes
			{ Nem_HSpring,     0xA460 }, // horizontal spring
			{ Nem_VSpring,     0xA660 }, // vertical spring
		}
	},
	{ // MZ
		10,
		{
			{ Nem_MZ,          0x0000 }, // MZ main patterns
			{ Nem_MzMetal,     0x6000 }, // metal blocks
			{ Nem_MzFire,      0x68A0 }, // fireballs
			{ Nem_Swing,       0x7000 }, // swinging platform
			{ Nem_MzGlass,     0x71C0 }, // green glassy block
			{ Nem_Lava,        0x7500 }, // lava
			{ Nem_Buzz,        0x8880 }, // buzz bomber enemy
			{ Nem_Yadrin,      0x8F60 }, // yadrin enemy
			{ Nem_Basaran,     0x9700 }, // basaran enemy
			{ Nem_Cater,       0x9FE0 }, // caterkiller enemy
		}
	},
	{ // MZ2
		5,
		{
			{ Nem_MzSwitch,    0xA260 }, // switch
			{ Nem_Spikes,      0xA360 }, // spikes
			{ Nem_HSpring,     0xA460 }, // horizontal spring
			{ Nem_VSpring,     0xA660 }, // vertical spring
			{ Nem_MzBlock,     0x5700 }, // green stone block
		}
	},
	{ // SLZ
		9,
		{
			{ Nem_SLZ,         0x0000 }, // SLZ main patterns
			{ Nem_Bomb,        0x8000 }, // bomb enemy
			{ Nem_Orbinaut,    0x8520 }, // orbinaut enemy
			{ Nem_MzFire,      0x9000 }, // fireballs
			{ Nem_SlzBlock,    0x9C00 }, // block
			{ Nem_SlzWall,     0xA260 }, // breakable wall
			{ Nem_Spikes,      0xA360 }, // spikes
			{ Nem_HSpring,     0xA460 }, // horizontal spring
			{ Nem_VSpring,     0xA660 }, // vertical spring
		}
	},
	{ // SLZ2
		6,
		{
			{ Nem_Seesaw,      0x6E80 }, // seesaw
			{ Nem_Fan,         0x7400 }, // fan
			{ Nem_Pylon,       0x7980 }, // foreground pylon
			{ Nem_SlzSwing,    0x7B80 }, // swinging platform
			{ Nem_SlzCannon,   0x9B00 }, // fireball launcher
			{ Nem_SlzSpike,    0x9E00 }, // spikeball
		}
	},
	{ // SYZ
		5,
		{
			{ Nem_SYZ,         0x0000 }, // SYZ main patterns
			{ Nem_Crabmeat,    0x8000 }, // crabmeat enemy
			{ Nem_Buzz,        0x8880 }, // buzz bomber enemy
			{ Nem_Yadrin,      0x8F60 }, // yadrin enemy
			{ Nem_Roller,      0x9700 }, // roller enemy
		}
	},
	{ // SYZ2
		8
		{
			{ Nem_Bumper,      0x7000 }, // bumper
			{ Nem_SyzSpike1,   0x72C0 }, // large spikeball
			{ Nem_SyzSpike2,   0x7740 }, // small spikeball
			{ Nem_Cater,       0x9FE0 }, // caterkiller enemy
			{ Nem_LzSwitch,    0xA1E0 }, // switch
			{ Nem_Spikes,      0xA360 }, // spikes
			{ Nem_HSpring,     0xA460 }, // horizontal spring
			{ Nem_VSpring,     0xA660 }, // vertical spring
		}
	},
	{ // SBZ
		12,
		{
			{ Nem_SBZ,         0x0000 }, // SBZ main patterns
			{ Nem_Stomper,     0x5800 }, // moving platform and stomper
			{ Nem_SbzDoor1,    0x5D00 }, // door
			{ Nem_Girder,      0x5E00 }, // girder
			{ Nem_BallHog,     0x6040 }, // ball hog enemy
			{ Nem_SbzWheel1,   0x6880 }, // spot on large wheel
			{ Nem_SbzWheel2,   0x6900 }, // wheel that grabs Sonic
			{ Nem_SyzSpike1,   0x7220 }, // large spikeball
			{ Nem_Cutter,      0x76A0 }, // pizza cutter
			{ Nem_FlamePipe,   0x7B20 }, // flaming pipe
			{ Nem_SbzFloor,    0x7EA0 }, // collapsing floor
			{ Nem_SbzBlock,    0x9860 }, // vanishing block
		}
	},
	{ // SBZ2
		13,
		{
			{ Nem_Cater,       0x5600 }, // caterkiller enemy
			{ Nem_Bomb,        0x8000 }, // bomb enemy
			{ Nem_Orbinaut,    0x8520 }, // orbinaut enemy
			{ Nem_SlideFloor,  0x8C00 }, // floor that slides away
			{ Nem_SbzDoor2,    0x8DE0 }, // horizontal door
			{ Nem_Electric,    0x8FC0 }, // electric orb
			{ Nem_TrapDoor,    0x9240 }, // trapdoor
			{ Nem_SbzFloor,    0x7F20 }, // collapsing floor
			{ Nem_SpinPform,   0x9BE0 }, // small spinning platform
			{ Nem_LzSwitch,    0xA1E0 }, // switch
			{ Nem_Spikes,      0xA360 }, // spikes
			{ Nem_HSpring,     0xA460 }, // horizontal spring
			{ Nem_VSpring,     0xA660 }, // vertical spring
		}
	},
	{ // TitleCard
		1,
		{
			{ Nem_TitleCard,   0xB000 },
		}
	},
	{ // Boss
		6,
		{
			{ Nem_Eggman,      0x8000 }, // Eggman main patterns
			{ Nem_Weapons,     0x8D80 }, // Eggman's weapons
			{ Nem_Prison,      0x93A0 }, // prison capsule
			{ Nem_Bomb,        0xA300 }, // bomb enemy ((gets overwritten)
			{ Nem_SlzSpike,    0xA300 }, // spikeball ((SLZ boss)
			{ Nem_Exhaust,     0xA540 }, // exhaust flame
		}
	},
	{ // Signpost
		3,
		{
			{ Nem_SignPost,    0xD000 }, // signpost
			{ Nem_Bonus,       0x96C0 }, // hidden bonus points
			{ Nem_BigFlash,    0x8C40 }, // giant ring flash effect
		}
	},
	{ // Warp (beta special stage warp effect)
		0, // 1 for rev 0
		{
			// { Nem_Warp,        0xa820 }, // rev 0 only
		}
	},
	{ // SpecialStage
		20,
		{
			{ Nem_SSBgCloud,   0x0000 }, // bubble and cloud background
			{ Nem_SSBgFish,    0x0A20 }, // bird and fish background
			{ Nem_SSWalls,     0x2840 }, // walls
			{ Nem_Bumper,      0x4760 }, // bumper
			{ Nem_SSGOAL,      0x4A20 }, // GOAL block
			{ Nem_SSUpDown,    0x4C60 }, // UP and DOWN blocks
			{ Nem_SSRBlock,    0x5E00 }, // R block
			{ Nem_SS1UpBlock,  0x6E00 }, // 1UP block
			{ Nem_SSEmStars,   0x7E00 }, // emerald collection stars
			{ Nem_SSRedWhite,  0x8E00 }, // red and white block
			{ Nem_SSGhost,     0x9E00 }, // ghost block
			{ Nem_SSWBlock,    0xAE00 }, // W block
			{ Nem_SSGlass,     0xBE00 }, // glass block
			{ Nem_SSEmerald,   0xEE00 }, // emeralds
			{ Nem_SSZone1,     0xF2E0 }, // ZONE 1 block
			{ Nem_SSZone2,     0xF400 }, // ZONE 2 block
			{ Nem_SSZone3,     0xF520 }, // ZONE 3 block
			{ Nem_SSZone4,     0xF2E0 }, // ZONE 4 block
			{ Nem_SSZone5,     0xF400 }, // ZONE 5 block
			{ Nem_SSZone6,     0xF520 }, // ZONE 6 block
		}
	},
	{ // GHZAnimals
		2,
		{
			{ Nem_Rabbit,      0xB000 }, // rabbit
			{ Nem_Flicky,      0xB240 }, // flicky
		}
	},
	{ // LZAnimals
		2,
		{
			{ Nem_BlackBird,   0xB000 }, // blackbird
			{ Nem_Seal,        0xB240 }, // seal
		}
	},
	{ // MZAnimals
		2,
		{
			{ Nem_Squirrel,    0xB000 }, // squirrel
			{ Nem_Seal,        0xB240 }, // seal
		}
	},
	{ // SLZAnimals
		2,
		{
			{ Nem_Pig,         0xB000 }, // pig
			{ Nem_Flicky,      0xB240 }, // flicky
		}
	},
	{ // SYZAnimals
		2,
		{
			{ Nem_Pig,         0xB000 }, // pig
			{ Nem_Chicken,     0xB240 }, // chicken
		}
	},
	{ // SBZAnimals
		2,
		{
			{ Nem_Rabbit,      0xB000 }, // rabbit
			{ Nem_Chicken,     0xB240 }, // chicken
		}
	},
	{ // SSResult
		2,
		{
			{ Nem_ResultEm,    0xA820 }, // emeralds
			{ Nem_MiniSonic,   0xAA20 }, // mini Sonic
		}
	},
	{ // Ending
		14, // 15 for rev 0
		{
			{ Nem_GHZ_1st,     0x0000 }, // GHZ main patterns
			{ Nem_GHZ_2nd,     0x39A0 }, // GHZ secondary patterns
			{ Nem_Stalk,       0x6B00 }, // flower stalk
			{ Nem_EndFlower,   0x7400 }, // flowers
			{ Nem_EndEm,       0x78A0 }, // emeralds
			{ Nem_EndSonic,    0x7C20 }, // Sonic
			// { Nem_EndEggman,   0xA480 }, // Eggman's death (rev 0 only, unused)
			{ Nem_Rabbit,      0xAA60 }, // rabbit
			{ Nem_Chicken,     0xACA0 }, // chicken
			{ Nem_BlackBird,   0xAE60 }, // blackbird
			{ Nem_Seal,        0xB0A0 }, // seal
			{ Nem_Pig,         0xB260 }, // pig
			{ Nem_Flicky,      0xB4A0 }, // flicky
			{ Nem_Squirrel,    0xB660 }, // squirrel
			{ Nem_EndStH,      0xB8A0 }, // "SONIC THE HEDGEHOG"
		}
	},
	{ // TryAgain
		3,
		{
			{ Nem_EndEm,       0x78A0 }, // emeralds
			{ Nem_TryAgain,    0x7C20 }, // Eggman
			{ Nem_CreditText,  0xB400 }, // credits alphabet
		}
	},
	{ // EggmanSBZ2
		3,
		{
			{ Nem_SbzBlock,    0xA300 }, // block
			{ Nem_Sbz2Eggman,  0x8000 }, // Eggman
			{ Nem_LzSwitch,    0x9400 }, // switch
		}
	},
	{ // FZBoss
		5,
		{
			{ Nem_FzEggman,    0x7400 }, // Eggman after boss
			{ Nem_FzBoss,      0x6000 }, // FZ boss
			{ Nem_Eggman,      0x8000 }, // Eggman main patterns
			{ Nem_Sbz2Eggman,  0x8E00 }, // Eggman without ship
			{ Nem_Exhaust,     0xA540 }, // exhaust flame
		}
	}
};

void AddPLC(uint plc)
{
	auto cues = &ArtLoadCues[plc];
	auto buffer = v_plc_buffer;

	while(buffer->gfx != 0)
		buffer++;

	for(int i = 0; i < cues->numPLCs; i++)
		*buffer++ = cues->plcs[i];
}

void NewPLC(uint plc)
{
	auto cues = &ArtLoadCues[plc];
	ClearPLC();
	auto buffer = v_plc_buffer;

	for(int i = 0; i <= cues->numPLCs; i++)
		*buffer++ = cues->plcs[i];
}

void ClearPLC()
{
	Clear_PLC_Buffer();
	v_ptrnemcode = 0;
	Clear_F6E4_F680();
	f_plc_execute = 0;
}

void RunPLC()
{
	if(v_plc_buffer[0].gfx != 0 && f_plc_execute == 0)
	{
		auto plc = v_plc_buffer[0];

		if(plc.gfx & 0x8000)
			v_ptrnemcode = &loc_150C;
		else
			v_ptrnemcode = &loc_1502;

		f_plc_execute = plc.gfx & 0x7FFF;
		NemDec4(v_plc_buffer, v_ngfx_buffer);
		nem_FFFFF6F0 = BSWAP16(*v_plc_buffer);
		v_plc_buffer++;
		nem_FFFFF6E4 = 0;
		nem_FFFFF6E8 = 0;
		nem_FFFFF6EC = 0;
		nem_FFFFF6F4 = 16;
	}
}

void QuickPLC(ushort plc)
{
	auto cues = &ArtLoadCues[plc];

	for(int i = 0; i <= cues->numPLCs; i++)
		NemDec(cues->plcs[i].gfx, cues->plcs[i].vram);
}

void PLC_sub_1642()
{
	if(f_plc_execute != 0)
	{
		nem_FFFFF6FA = 9;
		nem_FFFFF684 += 288;
		PLC_loc_1676(nem_FFFFF684);
	}
}

void PLC_sub_165E()
{
	if(f_plc_execute != 0)
	{
		nem_FFFFF6FA = 3;
		nem_FFFFF684 += 96;
		PLC_loc_1676(nem_FFFFF684);
	}
}

void PLC_loc_1676(ushort d0)
{
	// TODO:
	// Bunch of weird mysterious shit related to nemesis decompression into VRAM

/*	lea	(vdp_control_port).l,a4

	// This initial bit manipulation is converting the address into a VDP command to set the VRAM write position.
	lsl.l	#2,d0
	lsr.w	#2,d0
	ori.w	#$4000,d0
	swap	d0
	move.l	d0,(a4)

	subq.w	#4,a4
	movea.l	(v_plc_buffer).w,a0
	movea.l	(v_ptrnemcode).w,a3
	move.l	($FFFFF6E4).w,d0
	move.l	($FFFFF6E8).w,d1
	move.l	($FFFFF6EC).w,d2
	move.l	($FFFFF6F0).w,d5
	move.l	($FFFFF6F4).w,d6
	lea	(v_ngfx_buffer).w,a1

loc_16AA:				; XREF: PLC_sub_165E
	movea.w	#8,a5
	bsr.w	NemDec3
	subq.w	#1,(f_plc_execute).w
	beq.s	loc_16DC
	subq.w	#1,($FFFFF6FA).w
	bne.s	loc_16AA
	move.l	a0,(v_plc_buffer).w
	move.l	a3,(v_ptrnemcode).w
	move.l	d0,($FFFFF6E4).w
	move.l	d1,($FFFFF6E8).w
	move.l	d2,($FFFFF6EC).w
	move.l	d5,($FFFFF6F0).w
	move.l	d6,($FFFFF6F4).w
	rts

loc_16DC:
*/

	// Copy PLC buffer entries down
	memmove(v_plc_buffer, v_plc_buffer + 1, sizeof(PLCDesc) * (PLC_BUFFER_SIZE - 1));
}