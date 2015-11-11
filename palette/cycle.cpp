void PaletteCycle()
{
	switch(v_zone)
	{
		case Zone_GHZ:
		case Zone_EndZ: PCycle_GHZ(); break;
		case Zone_LZ:   PCycle_LZ();  break;
		case Zone_MZ:   PCycle_MZ();  break;
		case Zone_SLZ:  PCycle_SLZ(); break;
		case Zone_SYZ:  PCycle_SYZ(); break;
		case Zone_SBZ:  PCycle_SBZ(); break;
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
	if(TimerZero(v_pcyc_time, 5))
	{
		auto cycle = v_pcyc_num;
		v_pcyc_num = (v_pcyc_num + 1) & 3;
		memcpy(v_pal_dry + 0x50, palette + (cycle * 8), 8);
	}
}

const ubyte PCycLZ_Seq[8] = { 1, 0, 0, 1, 0, 0, 1, 0 };

void PCycle_LZ()
{
	if(TimerZero(v_pcyc_time, 2))
	{
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
			DecWrap(frame, 2);
		else
			IncWrap(frame, 2);

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
	if(TimerZero(v_pcyc_time, 7))
	{
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
	if(TimerZero(v_pcyc_time, 5))
	{
		auto cycle = ((v_pcyc_num++) & 3) * 4;
		memcpy(v_pal_dry + 0x6E, Pal_SYZCyc1 + (cycle * 2), 8);
		memcpy(v_pal_dry + 0x76, Pal_SYZCyc2 + cycle, 2);
		memcpy(v_pal_dry + 0x7A, Pal_SYZCyc2 + cycle + 2, 2);
	}
}

// include	"_inc\SBZ Palette Scripts.asm"

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
		andi.w	#0xF,d0
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
		lea	(v_pal_dry+0x58).w,a1
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
		lea	(v_pal_dry+0x20).w,a1
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
		andi.w	#0x1E,d2
		bne.s	loc_2034
		addq.w	#2,d0

loc_2034:
		cmpi.w	#0x60,d0
		bhs.s	loc_203E
		move.w	(a0)+,(a1,d0.w)

loc_203E:
		addq.w	#2,d0
		dbf	d1,loc_202A

		move.w	(v_pcyc_num).w,d0
		addq.w	#2,d0
		move.w	d0,d2
		andi.w	#0x1E,d2
		bne.s	loc_2054
		addq.w	#2,d0

loc_2054:
		cmpi.w	#0x64,d0
		blt.s	loc_2062
		move.w	#0x401,(v_pcyc_time).w
		moveq	#-0xC,d0

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
		addi.w	#0xC,d0
		cmpi.w	#0x30,d0
		blo.s	loc_2088
		moveq	#0,d0
		rts
; ===========================================================================

loc_2088:				; XREF: loc_206A
		move.w	d0,(v_pcyc_num).w
		lea	(Pal_Sega2).l,a0
		lea	(a0,d0.w),a0
		lea	(v_pal_dry+0x04).w,a1
		move.l	(a0)+,(a1)+
		move.l	(a0)+,(a1)+
		move.w	(a0)+,(a1)
		lea	(v_pal_dry+0x20).w,a1
		moveq	#0,d0
		moveq	#0x2C,d1

loc_20A8:
		move.w	d0,d2
		andi.w	#0x1E,d2
		bne.s	loc_20B2
		addq.w	#2,d0

loc_20B2:
		move.w	(a0),(a1,d0.w)
		addq.w	#2,d0
		dbf	d1,loc_20A8

loc_20BC:
		moveq	#1,d0*/
}

struct SSPalCycleData
{
	byte delay;
	ubyte planeAIdx;
	ubyte planeBOffs;
	ubyte palOffs;
	bool copyTo4E; // ~0x80
	bool copyTo6E; // 0x01
	bool isSecondary; // +10
};

const SSPalCycleData SSPalCycleStuff[] =
{
	{  3, 0, 0xE000 >> 13, 8, false, false, true  },
	{  3, 0, 0xE000 >> 13, 6, false, false, true  },
	{  3, 0, 0xE000 >> 13, 4, false, false, true  },
	{  3, 0, 0xE000 >> 13, 2, false, false, true  },
	{  3, 0, 0xE000 >> 13, 0, false, true,  true  },
	{  3, 0, 0xE000 >> 13, 0, false, false, false },
	{  3, 0, 0xE000 >> 13, 2, false, false, false },
	{  3, 0, 0xE000 >> 13, 4, false, false, false },
	{  3, 0, 0xE000 >> 13, 6, false, false, false },
	{  3, 0, 0xE000 >> 13, 8, false, false, false },
	{  7, 4, 0xE000 >> 13, 0, true,  false, false },
	{  7, 5, 0xE000 >> 13, 1, true,  false, false },
	{ -1, 6, 0xE000 >> 13, 2, true,  false, false },
	{ -1, 6, 0xE000 >> 13, 2, true,  false, false },
	{  7, 5, 0xE000 >> 13, 1, true,  false, false },
	{  7, 4, 0xE000 >> 13, 0, true,  false, false },
	{  3, 0, 0xC000 >> 13, 8, false, false, false },
	{  3, 0, 0xC000 >> 13, 6, false, false, false },
	{  3, 0, 0xC000 >> 13, 4, false, false, false },
	{  3, 0, 0xC000 >> 13, 2, false, false, false },
	{  3, 0, 0xC000 >> 13, 0, false, true,  false },
	{  3, 0, 0xC000 >> 13, 0, false, false, true  },
	{  3, 0, 0xC000 >> 13, 2, false, false, true  },
	{  3, 0, 0xC000 >> 13, 4, false, false, true  },
	{  3, 0, 0xC000 >> 13, 6, false, false, true  },
	{  3, 0, 0xC000 >> 13, 8, false, false, true  },
	{  7, 1, 0xC000 >> 13, 3, true,  false, false },
	{  7, 2, 0xC000 >> 13, 4, true,  false, false },
	{ -1, 3, 0xC000 >> 13, 5, true,  false, false },
	{ -1, 3, 0xC000 >> 13, 5, true,  false, false },
	{  7, 2, 0xC000 >> 13, 4, true,  false, false },
	{  7, 1, 0xC000 >> 13, 3, true,  false, false },
};

const ubyte SSPlaneAOffsets[][] =
{
	// Plane A Nametable offset, scroll y position
	{ 0x4000 >> 10, 0x100 >> 8 },
	{ 0x6000 >> 10, 0x000 >> 8 },
	{ 0x6000 >> 10, 0x100 >> 8 },
	{ 0x8000 >> 10, 0x000 >> 8 },
	{ 0x8000 >> 10, 0x100 >> 8 },
	{ 0xA000 >> 10, 0x000 >> 8 },
	{ 0xA000 >> 10, 0x100 >> 8 },
};

// This routine not only cycles/changes the SS BG palette, but also orchestrates all the complex BG changes (swapping
// between birds/clouds and fish/bubbles). SS_BGAnimate does different stuff depending on what this routine tells it to.
void PalCycle_SS()
{
	if(!f_pause && TimerZero(v_palss_time))
	{
		auto cycleData = SSPalCycleStuff[v_palss_num & 31];
		v_palss_num++;
		v_palss_time = (cycleData.delay < 0) ? 511 : cycleData.delay;
		v_ssbgstate = cycleData.planeAIdx;

		// Animate the background tiles (bird/fish transformation) by swapping plane positions
		auto planeAOffset = SSPlaneAOffsets[cycleData.planeAIdx];
		VDP_RegWrite(0x2, planeAOffset[0]);
		v_scrposy_dup = planeAOffset[1] << 8;
		VDP_RegWrite(0x4, cycleData.planeBOffs);
		VDP_SetAddr(0, VDP_VSRAM_Write);
		VDP_Data(v_scrposy_dup);
		VDP_Data(v_bg1posy_dupx);

		// Handle animating the palette
		if(cycleData.copyTo4E)
			memcpy(&v_pal_dry[0x4E], Pal_SSCyc1[cycleData.palOffs], 12);
		else
		{
			auto palette = Pal_SSCyc2[v_palss_bgpalselect + (cycleData.isSecondary ? 1 : 0)];

			if(cycleData.copyTo6E)
				memcpy(&v_pal_dry[0x6E], palette[0], 12);

			memcpy(&v_pal_dry[cycleData.isSecondary ? 0x7A : 0x5A], &palette[cycleData.palOffs + 1], 6);
		}
	}
}