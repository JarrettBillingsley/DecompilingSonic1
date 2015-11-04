
// Interestingly, the "empty" slots in this table are filled with what seem like actual values, instead of just 0s.
const LevelSizeArray[][] =
{
	//       unk    left   right   top    bottom  lookshift
	{
		{ 0x0004, 0x0000, 0x24BF, 0x0000, 0x0300, 0x0060 }, // GHZ 1
		{ 0x0004, 0x0000, 0x1EBF, 0x0000, 0x0300, 0x0060 }, // GHZ 2
		{ 0x0004, 0x0000, 0x2960, 0x0000, 0x0300, 0x0060 }, // GHZ 3
		{ 0x0004, 0x0000, 0x2ABF, 0x0000, 0x0300, 0x0060 }, // um
	},
	{
		{ 0x0004, 0x0000, 0x19BF, 0x0000, 0x0530, 0x0060 }, // LZ 1
		{ 0x0004, 0x0000, 0x10AF, 0x0000, 0x0720, 0x0060 }, // LZ 2
		{ 0x0004, 0x0000, 0x202F, 0xFF00, 0x0800, 0x0060 }, // LZ 3
		{ 0x0004, 0x0000, 0x20BF, 0x0000, 0x0720, 0x0060 }, // SBZ 3
	},
	{
		{ 0x0004, 0x0000, 0x17BF, 0x0000, 0x01D0, 0x0060 }, // MZ 1
		{ 0x0004, 0x0000, 0x17BF, 0x0000, 0x0520, 0x0060 }, // MZ 2
		{ 0x0004, 0x0000, 0x1800, 0x0000, 0x0720, 0x0060 }, // MZ 3
		{ 0x0004, 0x0000, 0x16BF, 0x0000, 0x0720, 0x0060 }, // uh
	},
	{
		{ 0x0004, 0x0000, 0x1FBF, 0x0000, 0x0640, 0x0060 }, // SLZ 1
		{ 0x0004, 0x0000, 0x1FBF, 0x0000, 0x0640, 0x0060 }, // SLZ 2
		{ 0x0004, 0x0000, 0x2000, 0x0000, 0x06C0, 0x0060 }, // SLZ 3
		{ 0x0004, 0x0000, 0x3EC0, 0x0000, 0x0720, 0x0060 }, // hm
	},
	{
		{ 0x0004, 0x0000, 0x22C0, 0x0000, 0x0420, 0x0060 }, // SYZ 1
		{ 0x0004, 0x0000, 0x28C0, 0x0000, 0x0520, 0x0060 }, // SYZ 2
		{ 0x0004, 0x0000, 0x2C00, 0x0000, 0x0620, 0x0060 }, // SYZ 3
		{ 0x0004, 0x0000, 0x2EC0, 0x0000, 0x0620, 0x0060 }, // hrmm
	},
	{
		{ 0x0004, 0x0000, 0x21C0, 0x0000, 0x0720, 0x0060 }, // SBZ 1
		{ 0x0004, 0x0000, 0x1E40, 0xFF00, 0x0800, 0x0060 }, // SBZ 2
		{ 0x0004, 0x2080, 0x2460, 0x0510, 0x0510, 0x0060 }, // FZ
		{ 0x0004, 0x0000, 0x3EC0, 0x0000, 0x0720, 0x0060 }, // umm
	},
	{
		{ 0x0004, 0x0000, 0x0500, 0x0110, 0x0110, 0x0060 }, // End
		{ 0x0004, 0x0000, 0x0DC0, 0x0110, 0x0110, 0x0060 }, // ...
		{ 0x0004, 0x0000, 0x2FFF, 0x0000, 0x0320, 0x0060 }, // ...
		{ 0x0004, 0x0000, 0x2FFF, 0x0000, 0x0320, 0x0060 }, // ...
	}
};

const Point16 EndingStLocArray[] =
{
	{ 0x0050, 0x03B0 },
	{ 0x0EA0, 0x046C },
	{ 0x1750, 0x00BD },
	{ 0x0A00, 0x062C },
	{ 0x0BB0, 0x004C },
	{ 0x1570, 0x016C },
	{ 0x0030, 0x074C },
	{ 0x1400, 0x02AC },
};

const Point16 StartLocArray[][] =
{
	{
		{ 0x0050, 0x03B0 }, // GHZ 1
		{ 0x0050, 0x00FC }, // GHZ 2
		{ 0x0050, 0x03B0 }, // GHZ 3
		{ 0x0080, 0x00A8 }, // blank
	},
	{
		{ 0x0060, 0x006C }, // LZ 1
		{ 0x0050, 0x00EC }, // LZ 2
		{ 0x0050, 0x02EC }, // LZ 3
		{ 0x0B80, 0x0000 }, // SBZ 3
	},
	{
		{ 0x0030, 0x0266 }, // MZ 1
		{ 0x0030, 0x0266 }, // MZ 2
		{ 0x0030, 0x0166 }, // MZ 3
		{ 0x0080, 0x00A8 }, // blank
	},
	{
		{ 0x0040, 0x02CC }, // SLZ 1
		{ 0x0040, 0x014C }, // SLZ 2
		{ 0x0040, 0x014C }, // SLZ 3
		{ 0x0080, 0x00A8 }, // blank
	},
	{
		{ 0x0030, 0x03BD }, // SYZ 1
		{ 0x0030, 0x01BD }, // SYZ 2
		{ 0x0030, 0x00EC }, // SYZ 3
		{ 0x0080, 0x00A8 }, // blank
	},
	{
		{ 0x0030, 0x048C }, // SBZ 1
		{ 0x0030, 0x074C }, // SBZ 2
		{ 0x2140, 0x05AC }, // FZ
		{ 0x0080, 0x00A8 }, // blank
	},
	{
		{ 0x0620, 0x016B }, // End
		{ 0x0EE0, 0x016C }, // End 2??
		{ 0x0080, 0x00A8 }, // blank
		{ 0x0080, 0x00A8 }, // blank
	},
};

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
const ushort Rev0SomethingTable[][] =
{
	{ 0x0070, 0x0100, 0x01000, 0x0100 }, // GHZ
	{ 0x0800, 0x0100, 0x01000, 0x0000 }, // LZ
	{ 0x0800, 0x0100, 0x01000, 0x0000 }, // MZ
	{ 0x0800, 0x0100, 0x01000, 0x0000 }, // SLZ
	{ 0x0800, 0x0100, 0x01000, 0x0000 }, // SYZ
	{ 0x0800, 0x0100, 0x01000, 0x0000 }, // SBZ
	{ 0x0070, 0x0100, 0x01000, 0x0100 }, // End
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
	v_limitright1 = v_limitright2 = sizes.right;
	v_limittop1 = v_limittop2 = sizes.top;
	v_limitbtm1 = v_limitbtm2 = sizes.bottom;
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
	v_rev0something3 = Rev0SomethingTable[v_zone][2];
	v_rev0something4 = Rev0SomethingTable[v_zone][3];
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