
const LevelSizeArray[][] =
{
	//       unk    left         top      lookshift
	{
		{ 0x0004, 0x000024BF, 0x00000300, 0x0060 }, // GHZ 1
		{ 0x0004, 0x00001EBF, 0x00000300, 0x0060 }, // GHZ 2
		{ 0x0004, 0x00002960, 0x00000300, 0x0060 }, // GHZ 3
		{ 0x0004, 0x00002ABF, 0x00000300, 0x0060 }, // um
	},
	{
		{ 0x0004, 0x000019BF, 0x00000530, 0x0060 }, // LZ 1
		{ 0x0004, 0x000010AF, 0x00000720, 0x0060 }, // LZ 2
		{ 0x0004, 0x0000202F, 0xFF000800, 0x0060 }, // LZ 3
		{ 0x0004, 0x000020BF, 0x00000720, 0x0060 }, // SBZ 3
	},
	{
		{ 0x0004, 0x000017BF, 0x000001D0, 0x0060 }, // MZ 1
		{ 0x0004, 0x000017BF, 0x00000520, 0x0060 }, // MZ 2
		{ 0x0004, 0x00001800, 0x00000720, 0x0060 }, // MZ 3
		{ 0x0004, 0x000016BF, 0x00000720, 0x0060 }, // uh
	},
	{
		{ 0x0004, 0x00001FBF, 0x00000640, 0x0060 }, // SLZ 1
		{ 0x0004, 0x00001FBF, 0x00000640, 0x0060 }, // SLZ 2
		{ 0x0004, 0x00002000, 0x000006C0, 0x0060 }, // SLZ 3
		{ 0x0004, 0x00003EC0, 0x00000720, 0x0060 }, // hm
	},
	{
		{ 0x0004, 0x000022C0, 0x00000420, 0x0060 }, // SYZ 1
		{ 0x0004, 0x000028C0, 0x00000520, 0x0060 }, // SYZ 2
		{ 0x0004, 0x00002C00, 0x00000620, 0x0060 }, // SYZ 3
		{ 0x0004, 0x00002EC0, 0x00000620, 0x0060 }, // hrmm
	},
	{
		{ 0x0004, 0x000021C0, 0x00000720, 0x0060 }, // SBZ 1
		{ 0x0004, 0x00001E40, 0xFF000800, 0x0060 }, // SBZ 2
		{ 0x0004, 0x20802460, 0x05100510, 0x0060 }, // FZ
		{ 0x0004, 0x00003EC0, 0x00000720, 0x0060 }, // umm
	},
	{
		{ 0x0004, 0x00000500, 0x01100110, 0x0060 }, // End
		{ 0x0004, 0x00000DC0, 0x01100110, 0x0060 }, // ...
		{ 0x0004, 0x00002FFF, 0x00000320, 0x0060 }, // ...
		{ 0x0004, 0x00002FFF, 0x00000320, 0x0060 }, // ...
	}
};

// TODO:
// EndingStLocArray:
// 	incbin	"startpos\ghz1 (Credits demo 1).bin"
// 	incbin	"startpos\mz2 (Credits demo).bin"
// 	incbin	"startpos\syz3 (Credits demo).bin"
// 	incbin	"startpos\lz3 (Credits demo).bin"
// 	incbin	"startpos\slz3 (Credits demo).bin"
// 	incbin	"startpos\sbz1 (Credits demo).bin"
// 	incbin	"startpos\sbz2 (Credits demo).bin"
// 	incbin	"startpos\ghz1 (Credits demo 2).bin"

// StartLocArray:	include	"_inc\Start Location Array - Levels.asm"

const ubyte LoopTileNums[][] =
{
	//loop  loop  roll  roll
	{ 0xB5, 0x7F, 0x1F, 0x20 }, // GHZ
	{ 0x7F, 0x7F, 0x7F, 0x7F }, // LZ
	{ 0x7F, 0x7F, 0x7F, 0x7F }, // MZ
	{ 0xAA, 0xB4, 0x7F, 0x7F }, // SLZ
	{ 0x7F, 0x7F, 0x7F, 0x7F }, // SYZ
	{ 0x7F, 0x7F, 0x7F, 0x7F }, // SBZ
	{ 0x7F, 0x7F, 0x7F, 0x7F }, // End
};

#if !JP1
const uint Rev0SomethingTable[][] =
{
	{ 0x00700100, 0x01000100 },
	{ 0x08000100, 0x01000000 },
	{ 0x08000100, 0x01000000 },
	{ 0x08000100, 0x01000000 },
	{ 0x08000100, 0x01000000 },
	{ 0x08000100, 0x01000000 },
	{ 0x00700100, 0x01000100 },
};
#endif

void LevelSizeLoad()
{
	// These variables seem to be set here and never touched anywhere else..
	v_b_F740 = 0;
	v_b_F741 = 0;
	v_b_F746 = 0;
	v_b_F748 = 0;
	v_dle_routine = 0;

	auto& sizes = LevelSizeArray[v_zone][v_act];

	// This is used in REV0 for... something related to level tile drawing
	v_w_F730 = sizes.unk;
	v_limitleft1 = v_limitleft2 = sizes.left;
	v_limittop1 = v_limittop2 = sizes.top;
	v_limitleft3 = v_limitleft2 + 0x240;
	v_screenscrollx16 = 0x1010;
	v_lookshift = sizes.lookshift;

	if(v_lastlamp != 0)
		Lamp_LoadInfo();
	else
	{
		Point16& pos;

		if(f_demo == DemoMode_Credits)
			pos = EndingStLocArray[v_creditsnum - 1];
		else
			pos = StartLocArray[v_zone][v_act];

		v_player->x = pos.x;
		v_player->y = pos.y;
	}

	v_screenposx = min(max(v_player->x - 160, 0), v_limitright2);
	v_screenposy = min(max(v_player->y - 96, 0), v_limitbtm2);

	if(v_lastlamp == 0)
	{
		v_bg1posy = v_screenposy;
		v_somethingposy = v_screenposy;
		v_bg1posx = v_screenposx;
		v_somethingposx = v_screenposx;
		v_bg2posx = v_screenposx;
	}

	switch(v_zone)
	{
		case Zone_GHZ:  BgScroll_GHZ();  break;
		case Zone_LZ:   BgScroll_LZ();   break;
		case Zone_MZ:   BgScroll_MZ();   break;
		case Zone_SLZ:  BgScroll_SLZ();  break;
		case Zone_SYZ:  BgScroll_SYZ();  break;
		case Zone_SBZ:  BgScroll_SBZ();  break;
		case Zone_EndZ: BgScroll_EndZ(); break;
	}

	v_256loop1 = LoopTileNums[v_zone][0];
	v_256loop2 = LoopTileNums[v_zone][1];
	v_256roll1 = LoopTileNums[v_zone][2];
	v_256roll2 = LoopTileNums[v_zone][3];

#if !JP1
	v_rev0something1 = Rev0SomethingTable[v_zone][0];
	v_rev0something2 = Rev0SomethingTable[v_zone][1];
#endif
}

#if JP1
	void BgScroll_GHZ()
	{
		v_bg1posx = 0;
		v_bg1posy = 0;
		v_somethingposy = 0;
		v_bg2posy = 0;
		memset(v_scrollsomething, 0, 12);
	}

	void BgScroll_LZ()
	{
		v_bg1posy = v_screenposy / 2;
	}

	void BgScroll_MZ()
	{
	}

	void BgScroll_SLZ()
	{
		v_bg1posy = (v_screenposy / 2) + 192;
		v_bg1posx = 0;
	}

	void BgScroll_SYZ()
	{
		v_bg1posy = ((v_screenposy * 48) / 256) + 1;
		v_bg1posx = 0;
	}

	void BgScroll_SBZ()
	{
		v_bg1posy = ((v_screenposy & 0x7F8) / 8) + 1;
	}

	void BgScroll_End()
	{
		v_somethingposx = v_bg1posx = v_screenposx / 2;
		v_bg2posx = (v_screenposx * 3) / 8;
		v_bg1posy = 0;
		v_somethingposy = 0;
		v_bg2posy = 0;
		memset(v_scrollsomething, 0, 12);
	}
#else
	void BgScroll_GHZ()
	{
		Deform_GHZ();
	}

	void BgScroll_LZ()
	{
		v_bg1posy = v_screenposy / 2;
	}

	void BgScroll_MZ()
	{
	}

	void BgScroll_SLZ()
	{
		v_bg1posy = (v_screenposy / 2) + 192;
	}

	void BgScroll_SYZ()
	{
		v_somethingposy = v_bg1posy = (v_screenposy * 48) / 256
	}

	void BgScroll_SBZ()
	{
		v_bg1posy = v_screenposy / 8;
	}

	void BgScroll_End()
	{
		v_bg1posy = 30;
		v_somethingposy = 30;
	}
#endif