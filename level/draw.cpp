
void LoadTilesAsYouMove()
{
	sub_6954(&v_bgscroll2_dup, &v_bg1posx_dup);
	sub_69F4(&v_bgscroll3_dup, &v_somethingposx_dup);
	sub_6EA4(&v_bgscroll4_dup, &v_bg2posx_dup);

	if(v_bgscroll1_dup)
	{
		if(BTST_AND_CLR(v_bgscroll1_dup, 1)) DrawTiles_LR(0x4000, -16, -16, &v_screenposx_dup, v_lvllayout);
		if(BTST_AND_CLR(v_bgscroll1_dup, 2)) DrawTiles_LR(0x4000, -16, 112, &v_screenposx_dup, v_lvllayout);
		if(BTST_AND_CLR(v_bgscroll1_dup, 4)) DrawTiles_TB(0x4000, -16, -16, &v_screenposx_dup, v_lvllayout);
		if(BTST_AND_CLR(v_bgscroll1_dup, 8)) DrawTiles_TB(0x4000, -16, 320, &v_screenposx_dup, v_lvllayout);
	}
}

void LoadTilesFromStart()
{
	DrawChunks(0x4000, &v_screenposx, v_lvllayout);

	switch(v_zone)
	{
		case Zone_GHZ:
		case Zone_EndZ: Draw_GHz_Bg(); break;
		case Zone_MZ:   Draw_Mz_Bg (); break;
		case Zone_SBZ:  Draw_SBz_Bg(); break;
		default:        DrawChunks (0x6000, &v_bg1posx, v_lvllayout + 0x40); break;
	}
}

// Only called from vblank for title and credits screens
void sub_6886()
{
	sub_6954(&v_bgscroll2, &v_bg1posx);
	sub_69F4(&v_bgscroll3, &v_somethingposx);
}

//                       d2                a3                a4
void DrawChunks(int patternBase, int* layerCoords, ubyte** layout)
{
	for(int i = 0, y = -16; i < 16; i++, y += 16)
	{
		DrawTiles_LR_2(patternBase, 32, 0, y, layerCoords, layout);
		y += 16;
	}
}

// Draw background?
//                  a2             a3
void sub_6954(int* flags, int layerCoords)
{
	if(*flags)
	{
		if(BTST_AND_CLR(*flags, 1))  DrawTiles_LR  (0x6000,    -16, -16, layerCoords, v_lvllayout + 0x40);
		if(BTST_AND_CLR(*flags, 2))  DrawTiles_LR  (0x6000,    -16, 112, layerCoords, v_lvllayout + 0x40);
		if(BTST_AND_CLR(*flags, 4))  DrawTiles_TB  (0x6000,    -16, -16, layerCoords, v_lvllayout + 0x40);
		if(BTST_AND_CLR(*flags, 8))  DrawTiles_TB  (0x6000,    -16, 320, layerCoords, v_lvllayout + 0x40);
		if(BTST_AND_CLR(*flags, 16)) DrawTiles_LR_3(0x6000, 32,  0, -16, layerCoords, v_lvllayout + 0x40);
		if(BTST_AND_CLR(*flags, 32)) DrawTiles_LR_3(0x6000, 32,  0, 112, layerCoords, v_lvllayout + 0x40);
	}
}

// Draw some other piece of the background?
//                  a2             a3
void sub_69F4(int* flags, int layerCoords)
{
	if(*flags)
	{
		if(v_zone == Zone_SBZ)
			Draw_SBz(flags);
		else
		{
			if(BTST_AND_CLR(*flags, 1)) DrawTiles_TB_2(0x6000, 3, -16, 112, layerCoords, v_lvllayout + 0x40);
			if(BTST_AND_CLR(*flags, 2)) DrawTiles_TB_2(0x6000, 3, 112, 320, layerCoords, v_lvllayout + 0x40);
		}
	}
}

// Draw some other piece of the background?
//                  a2             a3
void sub_6EA4(int* flags, int layerCoords)
{
	if(*flags)
	{
		if(v_zone == Zone_MZ)
			Draw_Mz(flags);
		else
		{
			if(BTST_AND_CLR(*flags, 1)) DrawTiles_TB_2(0x6000, 3, -16, 64, layerCoords, v_lvllayout + 0x40);
			if(BTST_AND_CLR(*flags, 2)) DrawTiles_TB_2(0x6000, 3, 320, 64, layerCoords, v_lvllayout + 0x40);
		}
	}
}

//                         d2         d5     d4            a3                a4
void DrawTiles_LR(int patternBase, int x, int y, int* layerCoords, ubyte** layout)
{
	DrawTiles_LR_2(patternBase, 22, x, y, layerCoords, layout);
}

//                           d2             d6        d5     d4            a3                a4
void DrawTiles_LR_2(int patternBase, int numTiles, int x, int y, int* layerCoords, ubyte** layout)
{
	auto addr = Calc_VRAM_Pos(x, y, layerCoords);
	uint* tileAddr;

	for(int i = 0; i < numTiles; i++)
	{
		auto block = GetBlockAddr(x, y, layerCoords, layout, tileAddr);
		DrawTiles(addr, patternBase, tileAddr, block);
		addr = (addr + 4) & 0x7F;
		x += 16;
	}
}

//                           d2             d6        d5     d4            a3                a4
void DrawTiles_LR_3(int patternBase, int numTiles, int x, int y, int* layerCoords, ubyte** layout)
{
	auto addr = Calc_VRAM_Pos(x, y, layerCoords);
	uint* tileAddr;

	for(int i = 0; i < numTiles; i++)
	{
		auto block = GetBlockAddr_2(x, y, layerCoords, layout, tileAddr);
		DrawTiles(addr, patternBase, tileAddr, block);
		addr = (addr + 4) & 0x7F;
		x += 16;
	}
}

//                         d2         d5     d4            a3                a4
void DrawTiles_TB(int patternBase, int x, int y, int* layerCoords, ubyte** layout)
{
	DrawTiles_TB_2(patternBase, 16, x, y, layerCoords, layout);
}

//                           d2             d6        d5     d4            a3                a4
void DrawTiles_TB_2(int patternBase, int numTiles, int x, int y, int* layerCoords, ubyte** layout)
{
	auto addr = Calc_VRAM_Pos(x, y, layerCoords);
	uint* tileAddr;

	for(int i = 0; i < numTiles; i++)
	{
		auto block = GetBlockAddr_2(x, y, layerCoords, layout, tileAddr);
		DrawTiles(addr, patternBase, tileAddr, block);
		addr = (addr + 0x100) & 0xFFF;
		y += 16;
	}
}

//                   d0              d2              a0            a1
void DrawTiles(int vramOffs, int patternBase, uint* tileAddr, int* block)
{
	VDP_SetAddr(vramOffs | patternBase, VDP_VRAM_Write);

	if(BTST(*tileAddr, 1 << 4))
	{
		if(BTST(*tileAddr, 1 << 3))
		{
			// flipped horiz and vert
			VDP_Data(SWAP16(block[1] ^ 0x18001800));
			VDP_SetAddr((vramOffs | patternBase) + 0x80, VDP_VRAM_Write);
			VDP_Data(SWAP16(block[0] ^ 0x18001800));
		}
		else
		{
			// flipped vert
			VDP_Data(block[1] ^ 0x10001000);
			VDP_SetAddr((vramOffs | patternBase) + 0x80, VDP_VRAM_Write);
			VDP_Data(block[0] ^ 0x10001000);
		}
	}
	else if(BTST(*tileAddr, 1 << 3))
	{
		// flipped horiz
		VDP_Data(SWAP16(block[0] ^ 0x08000800))
		VDP_SetAddr((vramOffs | patternBase) + 0x80, VDP_VRAM_Write);
		VDP_Data(SWAP16(block[1] ^ 0x08000800))
	}
	else
	{
		// unflipped
		VDP_Data(block[0]);
		VDP_SetAddr((vramOffs | patternBase) + 0x80, VDP_VRAM_Write);
		VDP_Data(block[1]);
	}
}

//a1                d5     d4          a3                  a4                a0
uint* GetBlockAddr(int x, int y, int* layerCoords, ubyte** layout, uint*& tileAddr)
{
	return GetBlockAddr_2(x + layerCoords[0], y, layerCoords, layout, tileAddr);
}

//a1                  d5     d4          a3                  a4                a0
uint* GetBlockAddr_2(int x, int y, int* layerCoords, ubyte** layout, uint*& tileAddr))
{
	y += layerCoords[1];
	x >>= 3;
	auto chunk = layout[y & 7][x & 0x7F];

	if(chunk)
	{
		tileAddr = &v_256x256[(chunk - 1) & 0x7F][y & 0xF][x & 0xF];
		return v_16x16[*tileAddr & 0x3FF];
	}

	return v_16x16[0];
}

// d0                 d5     d4               a3
uint Calc_VRAM_Pos(int x, int y, uint* layerCoords)
{
	return Calc_VRAM_Pos_2(x + layerCoords[0], y, layerCoords);
}

// d0                   d5     d4             a3
uint Calc_VRAM_Pos_2(int x, int y, uint* layerCoords)
{
	return 0x00030000 | (((y + layerCoords[1]) & 0xF0) << 4) + ((x & 0x1F0) >> 2);
}

const ubyte locj_6DF4[] =
{
	0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0
};

//                   a2
void Draw_SBz(int* flags)
{
	if(BTST_AND_CLR(*flags, 1) || BTST_AND_CLR(*flags, 2))
	{
		auto layerCoords = locj_6FE4[locj_6DF4[(((v_bg1posy + 112) & 0x1F0) >> 4) + 1]];

		if(layerCoords) // ?? always taken?
			DrawTiles_LR(0x6000, -16, 112, layerCoords, v_lvllayout + 0x40);
		else
			DrawTiles_LR_3(0x6000, 32, 0, 112, layerCoords, v_lvllayout + 0x40); // dead code?
	}

	if(*flags)
	{
		if(*flags & 0xA8)
		{
			*flags = (*flags & 0xA8) >> 1;
			locj_6FEC(320, -16, &locj_6DF4[(v_bg1posy & 0x1F0) >> 4], flags, v_lvllayout + 0x40);
		}
		else
			locj_6FEC(-16, -16, &locj_6DF4[(v_bg1posy & 0x1F0) >> 4], flags, v_lvllayout + 0x40);

	}
}

const ubyte locj_6EF2[] =
{
	0, 0, 0, 0, 0, 0, 3, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 0
};

//                  a2
void Draw_Mz(int* flags)
{
	if(BTST_AND_CLR(*flags, 1) || BTST_AND_CLR(*flags, 2))
	{
		auto layerCoords = locj_6FE4[locj_6EF2[(((v_bg1posy - 400) & 0x7F0) >> 4) + 1]];

		if(layerCoords) // ?? always taken?
			DrawTiles_LR(0x6000, -16, 112, layerCoords, v_lvllayout + 0x40);
		else
			DrawTiles_LR_3(0x6000, 32, 0, 112, layerCoords, v_lvllayout + 0x40); // dead code?
	}

	if(*flags)
	{
		if(*flags & 0xA8)
		{
			*flags = (*flags & 0xA8) >> 1;
			locj_6FEC(320, -16, &locj_6EF2[((v_bg1posy - 512) & 0x7F0) >> 4], flags, v_lvllayout + 0x40);
		}
		else
			locj_6FEC(-16, -16, &locj_6EF2[((v_bg1posy - 512) & 0x7F0) >> 4], flags, v_lvllayout + 0x40);
	}
}

const uint* locj_6FE4[] =
{
	&v_bg1posx_dup,
	&v_bg1posx_dup,
	&v_somethingposx_dup,
	&v_bg2posx_dup
};

//                d5     d4           a0              a2             a4
void locj_6FEC(int x, int y, ubyte* something, int* flags, ubyte** layout)
{
	for(int i = 0; i < 16; i++, y += 16)
	{
		auto index = *something++;

		if(BTST(flags, 1 << index))
		{
			auto layerCoords = locj_6FE4[index];
			uint* tile;
			auto block = GetBlockAddr(x, y, layerCoords, layout, tile);
			DrawTiles(Calc_VRAM_Pos(x, y, layerCoords), 0x6000, tile, block);
		}
	}

	*flags = 0;
}

const ubyte locj_724a[] = { 0, 0, 0, 0, 3, 3, 3, 2, 2, 2, 0, 0, 0, 0, 0, 0 };

void Draw_GHz_Bg()
{
	for(int i = 0, y = 0; i < 16; i++, y += 16)
		locj_72Ba((v_bg1posy + y) & 0xF0, y, locj_724a);
}

void Draw_Mz_Bg()
{
	for(int i = 0, y = -16; i < 16; i++, y += 16)
		locj_72Ba(((v_bg1posy - 0x200) + y) & 0x7F0, y, locj_6EF2 + 1);
}

void Draw_SBz_Bg()
{
	for(int i = 0, y = -16; i < 16; i++, y += 16)
		locj_72Ba((v_bg1posy + y) & 0x1F0, y, locj_6DF4 + 1);
}

const uint* locj_72B2[] =
{
	&v_bg1posx,
	&v_bg1posx,
	&v_somethingposx,
	&v_bg2posx,
};

//                     d0             d2        d4             a0               a4
void locj_72Ba(short index, int patternBase, int y, ubyte* someTable, ubyte** layout)
{
	auto layerCoords = locj_72B2[someTable[index >> 4]];

	if(layerCoords) // ?? always taken?
		DrawTiles_LR(0x6000, 32, -16, y, layerCoords, v_lvllayour + 0x40);
	else
		DrawTiles_LR_3(0x6000, 32, 0, y, layerCoords, v_lvllayour + 0x40); // Dead code?
}