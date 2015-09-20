// =====================================================================================================================
// Palette Types
// =====================================================================================================================

namespace Palette
{
	enum
	{
		SegaBG,     // 00
		Title,      // 01
		LevelSel,   // 02
		Sonic,      // 03
		GHZ,        // 04
		LZ,         // 05
		MZ,         // 06
		SLZ,        // 07
		SYZ,        // 08
		SBZ1,       // 09
		Special,    // 0A
		LZWater,    // 0B
		SBZ3,       // 0C
		SBZ3Water,  // 0D
		SBZ2,       // 0E
		LZSonWater, // 0F
		SBZ3SonWat, // 10
		SSResult,   // 11
		Continue,   // 12
		Ending,     // 13
	};
};

// =====================================================================================================================
// Palette Loading
// =====================================================================================================================

// Load into duplicate palette
void PalLoad1(ushort palIdx) // d0
{
	auto pal = &PalPointers[palIdx];
	memcpy(0xFFFF0000 | pal->ram + 0x80, pal->palette, pal->numColors * sizeof(uint));
}

// Load into main palette
void PalLoad2(ushort palIdx) // d0
{
	auto pal = &PalPointers[palIdx];
	memcpy(0xFFFF0000 | pal->ram, pal->palette, pal->numColors * sizeof(uint));
}

// Load into water palette
void PalLoad3_Water(ushort palIdx) // d0
{
	auto pal = &PalPointers[palIdx];
	memcpy(0xFFFF0000 | pal->ram - 0x80, pal->palette, pal->numColors * sizeof(uint));
}

// Load into water duplicate palette
void PalLoad4_Water(ushort palIdx) // d0
{
	auto pal = &PalPointers[palIdx];
	memcpy(0xFFFF0000 | pal->ram - 0x100, pal->palette, pal->numColors * sizeof(uint));
}

// =====================================================================================================================
// Palette Cycling
// =====================================================================================================================

void PaletteCycle()
{
	switch(v_zone)
	{
		case Zone::GHZ:
		case Zone::EndZ: PCycle_GHZ(); break;
		case Zone::LZ:   PCycle_LZ();  break;
		case Zone::MZ:   PCycle_MZ();  break;
		case Zone::SLZ:  PCycle_SLZ(); break;
		case Zone::SYZ:  PCycle_SYZ(); break;
		case Zone::SBZ:  PCycle_SBZ(); break;
		default: assert(false);
	}
}

void PCycle_Title()
{
	PCycle_GHZ_Common(Pal_TitleCyc);
}

void PCycle_GHZ()
{
	PCycle_GHZ_Common(Pal_GHZCyc);
}

void PCycle_GHZ_Common(uint* palette)
{
	if(TimerZero(v_pcyc_time))
	{
		v_pcyc_time = 5;
		auto cycle = v_pcyc_num;
		v_pcyc_num = (v_pcyc_num + 1) & 3;
		memcpy(v_pal_dry + 0x50, palette + (cycle * 8), 8);
	}
}

const ubyte PCycLZ_Seq[8] = { 1, 0, 0, 1, 0, 0, 1, 0 };

void PCycle_LZ()
{
	if(TimerZero(v_pcyc_time))
	{
		v_pcyc_time = 2;
		auto cycle = (v_pcyc_num++) & 3;
		cycle *= 8;
		uint* palette = (v_act == 3) ? Pal_SBZ3Cyc1 : Pal_LZCyc1;
		memcpy(v_pal_dry + 0x56, palette + cycle, 8);
		memcpy(v_pal_water + 0x56, palette + cycle, 8);
	}

	// Conveyor belts
	if(PCycLZ_Seq[v_framecount & 7] != 0)
	{
		auto frame = v_pal_buffer & 3;

		if(f_conveyrev)
		{
			if(TimerNeg(frame))
				frame = 2;
		}
		else
		{
			frame++;

			if(frame == 3)
				frame = 0;
		}

		v_pal_buffer = frame;
		frame *= 6;
		memcpy(v_pal_dry + 0x76, Pal_LZCyc2 + frame, 6);
		memcpy(v_pal_water + 0x76, Pal_LZCyc2 + frame, 6);
	}
}

void PCycle_MZ()
{
}

void PalCycle_SLZ()
{
	if(TimerZero(v_pcyc_time))
	{
		v_pcyc_time = 7;
		auto cycle = v_pcyc_num + 1;

		if(cycle >= 6)
			cycle = 0;

		v_pcyc_num = cycle;
		cycle *= 6;
		memcpy(v_pal_dry + 0x56, Pal_SLZCyc + cycle, 6);
	}
}

void PalCycle_SYZ()
{
	if(TimerZero(v_pcyc_time))
	{
		v_pcyc_time = 5;
		auto cycle = ((v_pcyc_num++) & 3) * 4;
		memcpy(v_pal_dry + 0x6E, Pal_SYZCyc1 + (cycle * 2), 8);
		memcpy(v_pal_dry + 0x76, Pal_SYZCyc2 + cycle, 2);
		memcpy(v_pal_dry + 0x7A, Pal_SYZCyc2 + cycle + 2, 2);
	}
}

void PalCycle_SBZ()
{
	// TODO:
		/*lea	(Pal_SBZCycList1).l,a2
		tst.b	(v_act).w
		beq.s	loc_1ADA
		lea	(Pal_SBZCycList2).l,a2

loc_1ADA:
		lea	(v_pal_buffer).w,a1
		move.w	(a2)+,d1

loc_1AE0:
		subq.b	#1,(a1)
		bmi.s	loc_1AEA
		addq.l	#2,a1
		addq.l	#6,a2
		bra.s	loc_1B06
; ===========================================================================

loc_1AEA:				; XREF: PalCycle_SBZ
		move.b	(a2)+,(a1)+
		move.b	(a1),d0
		addq.b	#1,d0
		cmp.b	(a2)+,d0
		bcs.s	loc_1AF6
		moveq	#0,d0

loc_1AF6:
		move.b	d0,(a1)+
		andi.w	#$F,d0
		add.w	d0,d0
		movea.w	(a2)+,a0
		movea.w	(a2)+,a3
		move.w	(a0,d0.w),(a3)

loc_1B06:				; XREF: PalCycle_SBZ
		dbf	d1,loc_1AE0
		subq.w	#1,(v_pcyc_time).w
		bpl.s	locret_1B64
		lea	(Pal_SBZCyc4).l,a0
		move.w	#1,(v_pcyc_time).w
		tst.b	(v_act).w
		beq.s	loc_1B2E
		lea	(Pal_SBZCyc10).l,a0
		move.w	#0,(v_pcyc_time).w

loc_1B2E:
		moveq	#-1,d1
		tst.b	(f_conveyrev).w
		beq.s	loc_1B38
		neg.w	d1

loc_1B38:
		move.w	(v_pcyc_num).w,d0
		andi.w	#3,d0
		add.w	d1,d0
		cmpi.w	#3,d0
		bcs.s	loc_1B52
		move.w	d0,d1
		moveq	#0,d0
		tst.w	d1
		bpl.s	loc_1B52
		moveq	#2,d0

loc_1B52:
		move.w	d0,(v_pcyc_num).w
		add.w	d0,d0
		lea	(v_pal_dry+$58).w,a1
		move.l	(a0,d0.w),(a1)+
		move.w	4(a0,d0.w),(a1)

locret_1B64:
		rts	*/
}

// Returns 1 while still cycling
int PalCycle_Sega()
{
	return 0;

	// TODO:
		/*tst.b	(v_pcyc_time+1).w
		bne.s	loc_206A
		lea	(v_pal_dry+$20).w,a1
		lea	(Pal_Sega1).l,a0
		moveq	#5,d1
		move.w	(v_pcyc_num).w,d0

loc_2020:
		bpl.s	loc_202A
		addq.w	#2,a0
		subq.w	#1,d1
		addq.w	#2,d0
		bra.s	loc_2020
; ===========================================================================

loc_202A:				; XREF: PalCycle_Sega
		move.w	d0,d2
		andi.w	#$1E,d2
		bne.s	loc_2034
		addq.w	#2,d0

loc_2034:
		cmpi.w	#$60,d0
		bhs.s	loc_203E
		move.w	(a0)+,(a1,d0.w)

loc_203E:
		addq.w	#2,d0
		dbf	d1,loc_202A

		move.w	(v_pcyc_num).w,d0
		addq.w	#2,d0
		move.w	d0,d2
		andi.w	#$1E,d2
		bne.s	loc_2054
		addq.w	#2,d0

loc_2054:
		cmpi.w	#$64,d0
		blt.s	loc_2062
		move.w	#$401,(v_pcyc_time).w
		moveq	#-$C,d0

loc_2062:
		move.w	d0,(v_pcyc_num).w
		moveq	#1,d0
		rts
; ===========================================================================

loc_206A:				; XREF: loc_202A
		subq.b	#1,(v_pcyc_time).w
		bpl.s	loc_20BC
		move.b	#4,(v_pcyc_time).w
		move.w	(v_pcyc_num).w,d0
		addi.w	#$C,d0
		cmpi.w	#$30,d0
		blo.s	loc_2088
		moveq	#0,d0
		rts
; ===========================================================================

loc_2088:				; XREF: loc_206A
		move.w	d0,(v_pcyc_num).w
		lea	(Pal_Sega2).l,a0
		lea	(a0,d0.w),a0
		lea	(v_pal_dry+$04).w,a1
		move.l	(a0)+,(a1)+
		move.l	(a0)+,(a1)+
		move.w	(a0)+,(a1)
		lea	(v_pal_dry+$20).w,a1
		moveq	#0,d0
		moveq	#$2C,d1

loc_20A8:
		move.w	d0,d2
		andi.w	#$1E,d2
		bne.s	loc_20B2
		addq.w	#2,d0

loc_20B2:
		move.w	(a0),(a1,d0.w)
		addq.w	#2,d0
		dbf	d1,loc_20A8

loc_20BC:
		moveq	#1,d0*/
}

// =====================================================================================================================
// Palette Fading (to/from black/white)
// =====================================================================================================================

void PaletteFadeIn()
{
	v_pfade_start = 0;
	v_pfade_size = 64;
	PalFadeIn_Alt();
}

void PalFadeIn_Alt()
{
	auto palette = v_pal_dry + v_pfade_start;

	for(int i = 0; i < v_pfade_size; i++)
		palette[i] = Colors::Black;

	for(int i = 0; i < 16; i++)
	{
		WaitForVBlank(VBlank_PaletteFade);
		FadeIn_FromBlack();
		RunPLC();
	}
}

void FadeIn_FromBlack()
{
	auto cur = v_pal_dry + v_pfade_start;
	auto dest = v_pal_dry_dup + v_pfade_start;

	for(int i = 0; i < v_pfade_size; i++)
		FadeIn_AddColour(cur++, dest++);

	if(v_zone == Zone::LZ)
	{
		auto cur = v_pal_water + v_pfade_start;
		auto dest = v_pal_water_dup + v_pfade_start;

		for(int i = 0; i < v_pfade_size; i++)
			FadeIn_AddColour(cur++, dest++);
	}
}

void FadeIn_AddColour(ushort* cur, ushort* dest)
{
	auto destColor = *dest;
	auto curColor = *cur;

	if(curColor != destColor)
	{
		auto tmpColor = curColor + 0x200; // blue

		if(tmpColor <= destColor)
			*cur = tmpColor;
		else
		{
			tmpColor = curColor + 0x20; // green

			if(tmpColor <= destColor)
				*cur = tmpColor;
			else
				*cur += 2; // red
		}
	}
}

void PaletteFadeOut()
{
	v_pfade_start = 0;
	v_pfade_size = 64;

	for(int i = 0; i < 16; i++)
	{
		WaitForVBlank(VBlank_PaletteFade);
		FadeOut_ToBlack();
		RunPLC();
	}
}

void FadeOut_ToBlack()
{
	auto palette = v_pal_dry + v_pfade_start;

	for(int i = 0; i < v_pfade_size; i++)
		FadeOut_DecColour(palette++);

	palette = v_pal_water + v_pfade_start;

	for(int i = 0; i < v_pfade_size; i++)
		FadeOut_DecColour(palette++);
}

void FadeOut_DecColour(ushort* cur)
{
	auto curColor = *cur;

	if(curColor != 0)
	{
		if(curColor & 0x0E)
			*cur -= 2; // red
		else if(curColor & 0xE0)
			*cur -= 0x20; // green
		else if(curColor & 0xE00)
			*cur -= 0x200; // blue
	}
}

void PaletteWhiteIn()
{
	v_pfade_start = 0;
	v_pfade_size = 64;

	auto palette = v_pal_dry + v_pfade_start;

	for(int i = 0; i < v_pfade_size; i++)
		*palette++ = Colors::White;

	for(int i = 0; i < 16; i++)
	{
		WaitForVBlank(VBlank_PaletteFade);
		WhiteIn_FromWhite();
		RunPLC();
	}
}

void WhiteIn_FromWhite()
{
	auto cur = v_pal_dry + v_pfade_start;
	auto dest = v_pal_dry_dup + v_pfade_start;

	for(int i = 0; i < v_pfade_size; i++)
		WhiteIn_DecColour(cur++, dest++);

	if(v_zone == Zone::LZ)
	{
		auto cur = v_pal_water + v_pfade_start;
		auto dest = v_pal_water_dup + v_pfade_start;

		for(int i = 0; i < v_pfade_size; i++)
			WhiteIn_DecColour(cur++, dest++);
	}
}

void WhiteIn_DecColour(ushort* cur, ushort* dest)
{
	auto destColor = *dest;
	auto curColor = *cur;

	if(curColor != destColor)
	{
		if(curColor & 0xE00 != 0)
		{
			auto tmpColor = curColor - 0x200;

			if(tmpColor >= destColor)
			{
				*cur = tmpColor;
				return;
			}
		}

		if(curColor & 0xE0 != 0)
		{
			auto tmpColor = curColor - 0x20;

			if(tmpColor >= destColor)
			{
				*cur = tmpColor;
				return;
			}
		}

		*cur -= 2;
	}
}

void PaletteWhiteOut()
{
	v_pfade_start = 0;
	v_pfade_size = 64;

	for(int i = 0; i < 16; i++)
	{
		WaitForVBlank(VBlank_PaletteFade);
		WhiteOut_ToWhite();
		RunPLC();
	}
}

void WhiteOut_ToWhite()
{
	auto palette = v_pal_dry + v_pfade_start;

	for(int i = 0; i < v_pfade_size; i++)
		WhiteOut_AddColour(palette++);

	palette = v_pal_water + v_pfade_start;

	for(int i = 0; i < v_pfade_size; i++)
		WhiteOut_AddColour(palette++);
}

void WhiteOut_AddColour(ushort* cur)
{
	auto curColor = *cur;

	if(curColor != Colors::White)
	{
		auto tmpColor = curColor & 0x0E; // red

		if(tmpColor != Colors::Red)
			*cur += 2;
		else
		{
			tmpColor = curColor & 0xE0; // green

			if(tmpColor != Colors::Green)
				*cur += 0x20;
			else
			{
				tmpColor = curColor & 0xE00; // blue

				if(tmpColor != Colors::Blue)
					*cur += 0x200;
			}
		}
	}
}