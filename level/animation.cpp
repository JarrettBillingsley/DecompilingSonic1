void AnimateLevelGfx()
{
	if(f_pause)
		return;

	AniArt_GiantRing();

	switch(v_zone)
	{
		case Zone_GHZ:  AniArt_GHZ();    break;
		case Zone_MZ:   AniArt_MZ();     break;
		case Zone_SBZ:  AniArt_SBZ();    break;
		case Zone_EndZ: AniArt_Endint(); break;

		default:
			break;
	}
}

void AniArt_GHZ()
{
	if(TimerNeg(v_lani0_time, 5))
	{
		// Waterfall
		auto frame = IncWrap(v_lani0_frame, 1);
		VDP_SetAddr(0x6F00, VDP_VRAM_Write);
		LoadTiles(&Art_GhzWater[frame * 0x100], 8);
	}
	else if(TimerNeg(v_lani1_time, 15))
	{
		// Big Flower
		auto frame = IncWrap(v_lani1_frame, 1);
		VDP_SetAddr(0x6B80, VDP_VRAM_Write);
		LoadTiles(&Art_GhzFlower1[frame * 0x200], 16);
	}
	else if(TimerNeg(v_lani2_time, 7))
	{
		// Small Flower
		auto frame = IncWrap(v_lani2_frame, 3);

		if(frame == 3)
			frame = 1; // 0 1 2 1 0 1 2 1...

		if(frame == 0 || frame == 2)
			v_lani2_time = 0x7F; // longer duration for frames 0 and 2

		VDP_SetAddr(0x6D80, VDP_VRAM_Write);
		LoadTiles(&Art_GhzFlower2[frame * 0x180], 12);
	}
}

void AniArt_MZ()
{
	if(TimerNeg(v_lani0_time, 19))
	{
		// Lava
		auto frame = IncWrap(v_lani0_frame, 2);
		VDP_SetAddr(0x5C40, VDP_VRAM_Write);
		LoadTiles(&Art_MzLava1[frame * 0x100], 8);

	}
	else if(TimerNeg(v_lani1_time, 1))
	{
		// Magma
		VDP_SetAddr(0x5A40, VDP_VRAM_Write);
		ubyte* artBase = &Art_MzLava2[v_lani0_frame * 0x200];

		for(int i = 0, j = v_oscillate[10]; i < 4; i++, j += 4)
		{
			auto art = (uint*)(artBase + (j & 0xF));

			for(int j = 0; j < 32; j++, art += 4)
				VDP_Data(*art);
		}
	}
	else if(TimerNeg(v_lani2_time, 7))
	{
		// Torch
		auto frame = IncWrap(v_lani3_frame, 3);
		VDP_SetAddr(0x5E40, VDP_VRAM_Write);
		LoadTiles(&Art_MzTorch[frame * 0xC00], 6);
	}
}

void AniArt_SBZ()
{
	// v_lani2_frame is being abused as a timer here
	if(v_lani2_frame > 0)
		v_lani2_frame--;
	else if(TimerNeg(v_lani0_time, 7))
	{
		VDP_SetAddr(0x8900, VDP_VRAM_Write);
		auto frame = IncWrap(v_lani0_frame, 7);

		if(frame == 0)
		{
			v_lani2_frame = 180;
			LoadTiles(Art_SbzSmoke, 6);
			LoadTiles(Art_SbzSmoke, 6);
		}
		else
			LoadTiles(&Art_SbzSmoke[(frame - 1) & 0x180], 12);

		return;
	}

	if(v_lani2_time > 0)
		v_lani2_time--;
	else if(TimerNeg(v_lani1_time, 7))
	{
		VDP_SetAddr(0x8A80, VDP_VRAM_Write);
		auto frame = IncWrap(v_lani1_frame, 7);

		if(frame == 0)
		{
			v_lani2_time = 120;
			LoadTiles(Art_SbzSmoke, 6);
			LoadTiles(Art_SbzSmoke, 6);
		}
		else
			LoadTiles(&Art_SbzSmoke[(frame - 1) & 0x180], 12);
	}
}

const ubyte FlowerFrames[] = { 0, 0, 0, 1, 2, 2, 2, 1 };

void AniArt_Ending()
{
	if(TimerNeg(v_lani1_time, 7))
	{
		// Big Flower
		auto frame = IncWrap(v_lani1_frame, 1);
		VDP_SetAddr(0x6B80, VDP_VRAM_Write);
		LoadTiles(&Art_GhzFlower1[frame * 0x200], 16);
		VDP_SetAddr(0x7200, VDP_VRAM_Write);
		LoadTiles(&0xFFFF9400[frame * 0x200], 16);
	}
	else if(TimerNeg(v_lani2_time, 7))
	{
		// Small Flower
		// Interestingly, it uses a much faster animation cycle than the GHZ versions of this flower.. presumably this
		// was the original animation speed, and they slowed it down in GHZ and forgot to here
		auto frame = FlowerFrames[IncWrap(v_lani2_frame, 7)];
		VDP_SetAddr(0x6D80, VDP_VRAM_Write);
		LoadTiles(&Art_GhzFlower2[frame * 0x180], 12);
	}
	else if(TimerNeg(v_lani4_time, 14))
	{
		auto frame = IncWrap(v_lani4_frame, 3);

		if(frame == 3)
			frame = 1;

		VDP_SetAddr(0x7000, VDP_VRAM_Write);
		LoadTiles(&0xFFFF9800[frame * 0x200], 16);
	}
	else if(TimerNeg(v_lani5_time, 11))
	{
		auto frame = IncWrap(v_lani5_frame, 3);

		if(frame == 3)
			frame = 1;

		VDP_SetAddr(0x6800, VDP_VRAM_Write);
		LoadTiles(&0xFFFF9E00[frame * 0x200], 16);
	}
}

//                    a1          d1
void LoadTiles(uint* tiles, uint count)
{
	for(int i = 0; i < count; i++)
	{
		for(int j = 0; j < 8; j++)
			VDP_Data(*tiles++);
	}
}

// Animated pattern routine - giant ring

void AniArt_GiantRing()
{
	if(v_gfxbigring)
	{
		v_gfxbigring -= 0x1C0;
		VDP_SetAddr(0x8000 + v_gfxbigring, VDP_VRAM_Write);
		LoadTiles(&Art_BigRing[v_gfxbigring], 14);
	}
}