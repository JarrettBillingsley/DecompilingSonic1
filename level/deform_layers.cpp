void DeformLayers()
{
	if(f_nobgscroll)
		return;

	v_bgscroll1 = 0;
	v_bgscroll2 = 0;
	v_bgscroll3 = 0;
	v_bgscroll4 = 0;
	ScrollHoriz();
	ScrollVertical();
	DynamicLevelEvents();

#if JP1
		v_scrposy_dup = v_screenposy;
		v_bg1posy_dupx = v_bg1posy;
#else
		v_scrposx_dup = v_screenposx;
		v_scrposy_dup = v_screenposy;
		v_bg1posx_dupx = v_bg1posx;
		v_bg1posy_dupx = v_bg1posy;
		v_bg2posx_dupx = v_bg2posx;
		v_bg2posy_dupx = v_bg2posy;
#endif

	switch(v_zone)
	{
		case Zone_GHZ:
		case Zone_EndZ: Deform_GHZ(); break;
		case Zone_LZ:   Deform_LZ();  break;
		case Zone_MZ:   Deform_MZ();  break;
		case Zone_SLZ:  Deform_SLZ(); break;
		case Zone_SYZ:  Deform_SYZ(); break;
		case Zone_SBZ:  Deform_SBZ(); break;
	}
}

void ScrollHoriz()
{
	auto oldX = v_screenposx;

	auto diffX = v_player->x - v_screenposx;
	int newX;

	if(diffX < 144)
		newX = max(v_player->x - 144, v_limitleft2);
	else if(diffX < 160)
	{
		v_scrshiftx = 0;
		return;
	}
	else
		newX = min(v_limitright2, min(diffX - 160, 16) + v_screenposx);

	auto oldX = v_screenposx;
	v_screenposx = newX;
	v_scrshiftx = (newX - oldX) << 8;

	if(((v_screenposx & 0x10) ^ v_screenscrollx16) == 0)
	{
		v_screenscrollx16 ^= 0x10;

		if(v_screenposx < oldX)
			BSET(v_bgscroll1, Scrolled_L); // back
		else
			BSET(v_bgscroll1, Scrolled_R); // forward
	}
}

void ScrollVertical()
{
	auto diffY = v_player->y - v_screenposy - (Player_IsRolling() ? 5 : 0);

	if(Player_IsInAir())
	{
		diffY += 32 - v_lookshift;

		if(diffY < 0 || diffY >= 64)
		{
			if(diffY >= 64)
				diffY -= 64;

			if(diffY > 16) ScrollVertical_Down1(0x1000);
			else if(diffY < -16) ScrollVertical_Up1(0x1000);
			else ScrollVertical_Down2(0x1000 + v_screenposy);
		}
		else if(diffY >= 64)
			ScrollVertical_InAir(diffY - 64);
		else
			v_scrshifty = 0;
	}
	else
	{
		if(diffY != v_lookshift)
		{
			diffY -= v_lookshift;

			if(v_lookshift != 0x60)
			{
				if(diffY > 2) ScrollVertical_Down1(0x200);
				else if(diffY < -2) ScrollVertical_Up1(0x200);
			}
			else if(abs(v_player->inertia) >= 0x800)
			{
				if(diffY > 16) ScrollVertical_Down1(0x1000);
				else if(diffY < -16) ScrollVertical_Up1(0x1000);
			}
			else
			{
				if(diffY > 6) ScrollVertical_Down1(0x600);
				else if(diffY < -6) ScrollVertical_Up1(0x600);
			}

			if(diffY >= 0) ScrollVertical_Down2(diffY + v_screenposy);
			else ScrollVertical_Up2(diffY + v_screenposy);
		}
		else if(f_bgscrollvert)
		{
			f_bgscrollvert = false;
			ScrollVertical_Down2(v_screenposy);
		}
		else
			v_scrshifty = 0;
	}
}

void ScrollVertical_Up1(int scrollAmount)
{
	ScrollVertical_Up2(SWAP16((-scrollAmount << 8) + v_screenposy));
}

void ScrollVertical_Up2(int newY)
{
	if(LOWORD(newY) <= v_limittop2)
	{
		if(LOWORD(newY) <= -256)
		{
			LOWORD(newY) &= 0x7FF;
			v_player->y &= 0x7FF;
			v_screenposy &= 0x7FF;
			v_bg1posy &= 0x3FF;
		}
		else
			newY = v_limittop2;
	}

	ScrollVertical_Finish(newY);
}

void ScrollVertical_Down1(int scrollAmount)
{
	ScrollVertical_Down2(SWAP16((scrollAmount << 8) + v_screenposy));
}

void ScrollVertical_Down2(int newY)
{
	if(LOWORD(newY) >= v_limitbtm2)
	{
		newY -= 0x800
		if(LOWORD(newY) >= 0)
		{
			v_player->y &= 0x7FF;
			v_screenposy -= 0x800;
			v_bg1posy &= 0x3FF;
		}
		else
			newY = v_limitbtm2;
	}

	ScrollVertical_Finish(newY);
}

void ScrollVertical_Finish(int newY)
{
	auto oldY = v_screenposy;
	newY = SWAP16(newY);
	v_scrshifty = LOWORD(ROR(newY - v_screenposy, 8));
	v_screenposy = LOWORD(newY);

	if(((v_screenposy & 0x10) ^ v_screenscrolly16) == 0)
	{
		v_screenscrolly16 ^= 0x10;

		if(v_screenposy < oldY)
			BSET(v_bgscroll1, Scrolled_U);
		else
			BSET(v_bgscroll1, Scrolled_D);
	}
}

void ScrollCommon(int& scrollVar, int scroll, ubyte& weirdThing, ubyte& scrollFlags, int bit)
{
	auto oldVal = scrollVar;
	auto newVal = oldVal + scroll;
	scrollVar = newVal;

	if((((newVal >> 16) & 0x10) ^ weirdThing) == 0)
	{
		weirdThing ^= 0x10;

		if(newVal < oldVal)
			BSET(scrollFlags, bit);
		else
			BSET(scrollFlags, bit << 1);
	}
}

//                       d4           d5
void ScrollBlock1(int scrollX, int scrollY)
{
	ScrollCommon(v_bg1posx, scrollX, v_bg1scrollx16, v_bgscroll2, Scrolled_L);
	ScrollCommon(v_bg1posy, scrollY, v_bg1scrolly16, v_bgscroll2, Scrolled_U);
}

void WriteScroll(uint*& scrollTable, uint count, short fgScroll, short bgScroll)
{
	auto scrollVal = (fgScroll << 16) | bgScroll;

	for(int i = 0; i < count; i++)
		*scrollTable++ = scrollVal;
}

void Write16Table(ushort*& table, uint count, ushort value)
{
	for(int i = 0; i < count; i++)
		*table++ = value;
}

void Write16TableStep(ushort*& table, uint count, ushort start, uint step, uint stepShift = 12)
{
	uint value = start << 16;

	for(int i = 0; i < count; i++)
	{
		*table++ = HIWORD(value);
		value += step << stepShift;
	}
}

// Helper function that scrolls the screen in 16-row "chunks" instead of having to write every line manually.
// Not actually its own function in the USA version, but the SLZ scrolling code embeds it.
//                        d2             a2
void Bg_Scroll_X(ushort count, ushort* scroll)
{
	auto scrollTable = v_hscrolltablebuffer;
	uint fgScroll = -v_screenposx;

	// this seems wrong, but it's how the original logic works. maybe other code takes this into account and sets count
	// to have the low nybble "reversed"? original code had a duff's device that jumped according to (count & 0xF) but
	// this would mean that a count of 0 would copy 16 bytes and a count of 15 would copy 1...
	WriteScroll(scrollTable, 16 - (count & 0xF), fgScroll, scroll[0]);

	for(int i = 1; i < 16; i++)
		WriteScroll(scrollTable, 16, fgScroll, scroll[i]);
}

#if JP1
	void ScrollBlock1b(int scrollY)
	{
		ScrollCommon(v_bg1posy, scrollY, v_bg1scrolly16, v_bgscroll2, 1);
	}

	//                     d0
	void ScrollBlock2(int newY)
	{
		ScrollCommon(v_bg1posy, newY - v_bg1posy, v_bg1scrolly16, v_bgscroll2, 1);
	}

	//                       d4         d6
	void ScrollBlock3(int scrollX, int bit)
	{
		ScrollCommon(v_bg1posx, scrollX, v_bg1scrollx16, v_bgscroll2, bit);
	}

	//                       d4         d6
	void ScrollBlock4(int scrollX, int bit)
	{
		ScrollCommon(v_somethingposx, scrollX, v_somethingscrollx16, v_bgscroll2, bit);
	}

	//                       d4         d6
	void ScrollBlock5(int scrollX, int bit)
	{
		ScrollCommon(v_bg2posx, scrollX, v_bg2scrollx16, v_bgscroll2, bit);
	}

	//                     d5
	void Bg_Scroll_Y(int scrollY)
	{
		ScrollCommon(v_bg1posy, scrollY, v_bg1scrolly16, v_bgscroll2, Scrolled_L);
	}

	void Deform_GHZ()
	{
		ScrollBlock5(v_scrshiftx * 96, 0);
		ScrollBlock4(v_scrshiftx * 128, 0);

		auto scrollTable = v_hscrolltablebuffer;
		auto fgScroll = v_gamemode == GameMode_Title ? 0 : -v_screenposx;

		// These hold the positions of the cloud layers
		v_scrollsomething[0] += 0x10000;
		v_scrollsomething[1] += 0xC000;
		v_scrollsomething[2] += 0x8000;

		// Variable abuse? Holds number of rows visible at top/bottom of screen because of vertical scrolling
		v_bg1posy_dupx = max(0, (-(v_screenposy & 0x7FF) / 32) + 32);

		// Three rows of clouds, then two rows of mountains
		WriteScroll(scrollTable, 32 - v_bg1posy_dupx, fgScroll, -v_scrollsomething[0] - v_bg2posx);
		WriteScroll(scrollTable, 16,                  fgScroll, -v_scrollsomething[1] - v_bg2posx);
		WriteScroll(scrollTable, 16,                  fgScroll, -v_scrollsomething[2] - v_bg2posx);
		WriteScroll(scrollTable, 48,                  fgScroll, -v_bg2posx);
		WriteScroll(scrollTable, 40,                  fgScroll, -v_somethingposx);

		// Then the water
		auto bgScroll = v_somethingposx;
		auto step = (((v_screenposx - v_somethingposx) * 256) / 104) * 256;
		auto lineScroll = v_somethingposx;

		for(int i = 0; i < v_bg1posy_dupx + 72; i++)
		{
			bgScroll = -LOWORD(lineScroll);
			WriteScroll(scrollTable, 1, fgScroll, bgScroll);
			lineScroll = SWAP16(SWAP16(lineScroll) + step);
		}
	}

	// Water wobble data
	const byte Lz_Scroll_Data[] =
	{
		1,1,2,2,3,3,3,3,2,2,1,1,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		-1,-1,-2,-2,-3,-3,-3,-3,-2,-2,-1,-1,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		1,1,2,2,3,3,3,3,2,2,1,1,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	};

	void Deform_LZ()
	{
		ScrollBlock1(v_scrshiftx * 128, v_scrshifty * 128);
		v_bg1posy_dupx = v_bg1posy;

		auto scrollTable = v_hscrolltablebuffer;
		auto fgScroll = -v_screenposx;
		auto bgScroll = -v_bg1posx;
		auto wobbleIdx = ((v_lzrippleheight >> 16) + v_bg1posy) & 0xFF;
		auto scrollIdx = ((v_lzrippleheight >> 16) + v_screenposy) & 0xFF;
		v_lzrippleheight += 0x80;

		for(int i = 0, lineY = v_screenposy; i < 224; i++, lineY++, IncWrap(wobbleIdx, 256), IncWrap(scrollIdx, 256))
		{
			if(lineY >= v_waterpos1)
				WriteScroll(scrollTable, 1, fgScroll + Lz_Scroll_Data[scrollIdx], bgScroll + Drown_WobbleData[wobbleIdx]);
			else
				WriteScroll(scrollTable, 1, fgScroll, bgScroll);
		}
	}

	void Deform_MZ()
	{
		ScrollBlock3(v_scrshiftx * 192, Scrolled_L);
		ScrollBlock5(v_scrshiftx * 64, Scrolled_L3);
		ScrollBlock4(v_scrshiftx * 128, Scrolled_L2);
		v_somethingposy = 0x200 + (v_screenposy < 456 ? 0 : (3 * (v_screenposy - 456) / 4));
		v_bg2posy = v_somethingposy;
		ScrollBlock2(v_somethingposy);

		v_bg1posy_dupx = v_bg1posy;

		HIBYTE(v_bgscroll4) |= HIBYTE(v_bgscroll2) | HIBYTE(v_bgscroll3);
		HIBYTE(v_bgscroll2) = 0;
		HIBYTE(v_bgscroll3) = 0;

		auto table = v_scrollsomething;
		auto step = (((-v_screenposx / 4) + v_screenposx) * 8) / 5;
		Write16TableStep(table, 5, -v_screenposx / 2, step); // Clouds
		Write16Table(table, 2,  -v_bg2posx);                 // Mountains
		Write16Table(table, 9,  -v_somethingposx);           // Hills/Ruins
		Write16Table(table, 16, -v_bg1posx);                 // Inside (bricks)

		// Offset into the scroll table is [0..16], and there are 32 entries, and Bg_Scroll_X only uses 16, so
		// everything is nice!
		Bg_Scroll_X(v_bg1posy - 0x200, &v_scrollsomething[(min(v_bg1posy - 0x200, 0x100) & 0x1F0) >> 4]);
	}

	void Deform_SLZ()
	{
		Bg_Scroll_Y(v_scrshifty * 128);
		v_bg1posy_dupx = v_bg1posy;

		auto table = v_scrollsomething;
		auto step = (((d2 / 8) + v_screenposx) * 16) / 28;
		Write16TableStep(table, 28, -v_screenposx, step);  // Stars
		Write16Table(table, 5,  (-v_screenposx * 3) / 16); // Distant buildings
		Write16Table(table, 5,  -v_screenposx / 4);        // Nearer buildings
		Write16Table(table, 30, -v_screenposx / 2);        // Very near buildings

		// Offset into scroll table is.. not sure. There are 68 entries, so 52 is the highest valid index, which I'm
		// sure is somehow guaranteed by the scrolling limits. (0x400 would be the highest valid scroll value.)
		Bg_Scroll_X(v_bg1posy, &v_scrollsomething[((v_bg1posy - 0xC0) & 0x3F0) >> 4]);
	}

	void Deform_SYZ
	{
		Bg_Scroll_Y(v_scrshifty * 48);
		v_bg1posy_dupx = v_bg1posy;

		auto table = v_scrollsomething;
		// Yes, * 8 / 8.. I'm sure this stuff was tweaked and it was easier to change the constant than the code
		auto step = (((-v_screenposx / 8) + v_screenposx) * 8) / 8;
		Write16TableStep(table, 8, -v_screenposx / 2, step); // Clouds
		Write16Table(table, 5, -v_screenposx / 8);           // Mountains
		Write16Table(table, 6, -v_screenposx / 4);           // Buildings
		step = ((-v_screenposx / 2) * 16) / 15;
		Write16TableStep(table, 15, -v_screenposx / 2, step); // Bushes/grass

		// Offset into scroll table is gonna have to be [0..18]. That would make highest valid scroll value 0x120.
		Bg_Scroll_X(v_bg1posy, &v_scrollsomething[(v_bg1posy & 0x1F0) >> 4]);
	}

	void Deform_SBZ()
	{
		if(v_act == 0)
		{
			ScrollBlock3(v_scrshiftx * 128, Scrolled_L);
			ScrollBlock5(v_scrshiftx * 64, Scrolled_L3);
			ScrollBlock4(v_scrshiftx * 96, Scrolled_L2);
			ScrollBlock1b(v_scrshifty * 32);
			v_somethingposy = v_bg1posy;
			v_bg2posy = v_bg1posy;
			v_bg1posy_dupx = v_bg1posy;
			HIBYTE(v_bgscroll3) = HIBYTE(v_bgscroll2) | HIBYTE(v_bgscroll4)
			HIBYTE(v_bgscroll2) = 0;
			HIBYTE(v_bgscroll4) = 0;

			auto table = v_scrollsomething;
			auto step = v_screenposx / 4;

			Write16TableStep(table, 4, -v_screenposx / 4, step); // Clouds
			Write16Table(table, 10, -v_bg2posx);                 // Far buildings
			Write16Table(table, 7, -v_somethingposx);            // Mid buildings
			Write16Table(table, 11, -v_bg1posx);                 // Near buildings

			// Offset must be [0..16], means highest valid scroll value is 0x100.
			Bg_Scroll_X(v_bg1posy, &v_scrollsomething[(v_bg1posy & 0x1F0) >> 4]);
		}
		else
		{
			ScrollBlock1(v_scrshiftx * 64, v_scrshifty * 32);
			v_bg1posy_dupx = v_bg1posy;
			auto table = v_hscrolltablebuffer;
			WriteScroll(table, 223, -v_screenposx, -v_bg1posx);
		}
	}
#else
	//                       d4           d5
	void ScrollBlock2(int scrollX, int scrollY)
	{
		auto oldX = v_bg1posx;
		auto newX = oldX + scrollX;
		v_bg1posx = newX;
		ScrollCommon(v_bg1posy, scrollY, v_bg1scrolly16, v_bgscroll2, 1);
	}

	//                     d0
	void ScrollBlock3(int newY)
	{
		ScrollCommon(v_bg1posy, newY - v_bg1posy, v_bg1scrolly16, v_bgscroll2, 1);
	}

	void ScrollBlock4()
	{
		ScrollCommon(v_somethingposx, v_scrshiftx << 7, v_somethingscrollx16, v_bgscroll3, 1 << Scrolled_L);
	}

	void Deform_GHZ()
	{
		ScrollBlock1(v_scrshiftx * 96, 0);
		ScrollBlock4();
		auto count = (-(v_screenposy & 0x7FF) / 32) + 38
		v_somethingposy = count;
		ScrollBlock3(count);
		v_bg1posy_dupx = v_bg1posy;
		auto fgScroll = v_gamemode == GameMode_Title ? 0 : -v_screenposx;
		WriteScroll(table, 112 - count, fgScroll, -v_bg1posx);
		WriteScroll(table, 40, fgScroll, -v_somethingposx);

		auto step = (((v_screenposx - 0x200 - v_somethingposx) * 256) / 104) * 256
		auto xpos = v_somethingposx << 16;

		for(int i = 0; i < count + 72; i++)
		{
			WriteScroll(table, 1, fgScroll, -HIWORD(xpos));
			xpos += step;
		}
	}

	void Deform_LZ()
	{
		ScrollBlock1(v_scrshiftx * 128, v_scrshifty * 128);
		v_bg1posy_dupx = v_bg1posy;
		auto scroll = v_hscrolltablebuffer;
		WriteScroll(scroll, 224, -v_screenposx, -v_bg1posx);

		// Dead code? Maybe they started writing the rippling effect but ran out of time.
		// d0 = v_waterpos1 - v_screenposy
	}

	void Deform_MZ()
	{
		ScrollBlock1(v_scrshiftx * 192, 0);

		if(v_screenposy >= 456)
			HIWORD(v_somethingposy) = 0x200 + ((v_screenposy - 456) * 3) / 4;
		else
			HIWORD(v_somethingposy) = 0x200;

		ScrollBlock3(HIWORD(v_somethingposy));
		v_bg1posy_dupx = v_bg1posy;
		auto scroll = v_hscrolltablebuffer;
		WriteScroll(scroll, 224, -v_screenposx, -v_bg1posx);
	}

	void Deform_SLZ()
	{
		ScrollBlock2(v_scrshiftx * 128, v_scrshifty * 128);
		v_bg1posy_dupx = v_bg1posy;
		Deform_SLZ_2();
		Bg_Scroll_X(v_bg1posy, &v_scrollsomething[((v_bg1posy - 0xC0) & 0x3F0) >> 4]);
	}

	void Deform_SLZ_2()
	{
		auto table = v_scrollsomething;
		auto step = (((d2 / 8) + v_screenposx) * 16) / 28;
		Write16TableStep(table, 28, -v_screenposx, step);  // Stars
		Write16Table(table, 5, -v_screenposx / 8);         // Distant buildings
		Write16Table(table, 5, -v_screenposx / 4);         // Nearer buildings
		Write16Table(table, 30, -v_screenposx / 2);        // Very near buildings
	}

	void Deform_SYZ()
	{
		ScrollBlock1(v_scrshiftx * 64, v_scrshifty * 48);
		v_bg1posy_dupx = v_bg1posy;
		auto scroll = v_hscrolltablebuffer;
		WriteScroll(scroll, 224, -v_screenposx, -v_bg1posx);
	}

	Deform_SBZ()
	{
		ScrollBlock1(v_scrshiftx * 64, v_scrshifty * 48);
		v_bg1posy_dupx = v_bg1posy;
		auto scroll = v_hscrolltablebuffer;
		WriteScroll(scroll, 224, -v_screenposx, -v_bg1posx);
	}
#endif