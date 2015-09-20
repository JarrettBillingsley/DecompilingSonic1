void GM_Special()
{
	PlaySound_Special(SFX::EnterSS);
	PaletteWhiteOut();
	DISABLE_INTERRUPTS();

	// TODO:
	// lea	(vdp_control_port).l,a6
	// move.w	#0x8B03,(a6)	; line scroll mode
	// move.w	#0x8004,(a6)	; 8-colour mode
	// move.w	#0x8A00+175,(v_hbla_hreg).w
	// move.w	#0x9011,(a6)	; 128-cell hscroll size
	// move.w	(v_vdp_buffer1).w,d0
	// andi.b	#0xBF,d0
	// move.w	d0,(vdp_control_port).l

	ClearScreen();
	ENABLE_INTERRUPTS();
	FillVRAM(0, 0x6FFF, 0x5000);

// SS_WaitForDMA:
// 	move.w	(a5),d1		; read control port (0xC00004)
// 	btst	#1,d1		; is DMA running?
// 	bne.s	SS_WaitForDMA	; if yes, branch

	// move.w	#0x8F02,(a5)	; set VDP increment to 2 bytes
	SS_BGLoad()
	QuickPLC(PLC::SpecialStage);

	Clear_Objects();
	Clear_F700_F800()
	Clear_FE60_FF00();
	Clear_NGFX_Buffer();
	f_wtr_state = false;
	f_restart = 0;
	PalLoad1(Palette::Special);
	SS_Load();
	v_screenposx = 0;
	v_screenposy = 0;
	v_player.id = ID::SonicSpecial;
	PalCycle_SS();
	v_ssangle = 0;
	v_ssrotate = 64;
	PlaySound(BGM::SS);
	v_btnpushtime1 = 0;
	v_btnpushtime2 = DemoDataPtr[6][1] - 1;
	v_rings = 0;
	v_lifecount = 0;
	v_debuguse = 0;
	v_demolength = 1800;

	if(f_debugcheat && v_jpadhold1 & Buttons::A)
		f_debugmode = true;

	// TODO:
	// move.w	(v_vdp_buffer1).w,d0
	// ori.b	#0x40,d0
	// move.w	d0,(vdp_control_port).l
	PaletteWhiteIn();

	// ===========================================================================
	// Main Special Stage loop
	// ===========================================================================

	do
	{
		PauseGame();
		WaitForVBlank(VBlank_Special);
		MoveSonicInDemo();
		v_jpadhold2 = v_jpadhold1;
		ExecuteObjects();
		BuildSprites();
		SS_ShowLayout();
		SS_BGAnimate();

		if(f_demo && v_demolength == 0)
		{
			v_gamemode = GameMode::Sega;
			return;
		}
	} while(v_gamemode == GameMode::Special);

	if(f_demo)
	{
		if(v_gamemode == GameMode::Level)
			v_gamemode = GameMode::Sega;

		return;
	}

	v_gamemode = GameMode::Level;

	if(v_zone >= Zone::SBZ)
	{
		v_zone = 0;
		v_act = 0;
	}


	v_demolength = 60;
	v_pfade_start = 63;
	v_palchgspeed = 0;

	do
	{
		WaitForVBlank(VBlank_SSFade);
		MoveSonicInDemo();
		v_jpadhold2 = v_jpadhold1;
		ExecuteObjects();
		BuildSprites();
		SS_ShowLayout();
		SS_BGAnimate();

		if(TimerZero(v_palchgspeed)))
		{
			v_palchgspeed = 2;
			WhiteOut_ToWhite();
		}
	} while(v_demolength);

	DISABLE_INTERRUPTS();
	// TODO:
	// lea	(vdp_control_port).l,a6
	// move.w	#0x8200+(vram_fg>>10),(a6) ; set foreground nametable address
	// move.w	#0x8400+(vram_bg>>13),(a6) ; set background nametable address
	// move.w	#0x9001,(a6)		; 64-cell hscroll size
	ClearScreen();
	NemDec(Nem_TitleCard, 0xB000);
	Hud_Base();
	ENABLE_INTERRUPTS();
	PalLoad2(Palette::SSResult);
	NewPLC(PLC::Main);
	AddPLC(PLC::SSResult);
	f_scorecount = 1;
	f_endactbonus = 1;
	v_ringbonus = v_rings * 10;
	PlaySound_Special(BGM::GotThrough);

	Clear_Objects();
	v_objspace[23].id = ID::SSResult;

	do
	{
		PauseGame();
		WaitForVBlank(VBlank_SSResults);
		ExecuteObjects();
		BuildSprites();
		RunPLC();
	} while(!f_restart && v_plc_buffer != 0);

	PlaySound_Special(SFX::EnterSS);
	PaletteWhiteOut();
}

void SS_BGLoad()
{
	// Birds and fish
	EniDec(Eni_SSBg1, 0xFF00, 0x4051);

	uint vramLoc = 0x50000001;
	uint gfxLoc = 0xFF0080;

	for(int i = 6; i >= 0; i--)
	{
		auto destLoc = vramLoc;
		auto someFlag = (i >= 3) ? 0 : 1

		for(int j = 3; j >= 0; j--)
		{
			for(int k = 7; k >= 0; k--)
			{
				auto srcLoc = gfxLoc;
				someFlag ^= 1;

				if(someFlag == 0)
				{
					if(i != 6)
						goto loc_48F2;

					srcLoc = 0xFF0000;
				}

				TilemapToVRAM(srcLoc, destLoc, 7, 7);

			loc_48F2:
				destLoc += 0x100000;
			}

			destLoc += 0x3800000;
			someFlag ^= 1;
		}

		vramLoc += 0x10000000;

		if(vramLoc & 0x80000000)
			vramLoc = WSWAP(WSWAP(vramLoc) + 0xC000);

		gfxLoc += 0x80;
	}

	// Clouds
	EniDec(Eni_SSBg2, 0xFF0000, 0x400);
	TilemapToVRAM(0xFF0000, 0x40000003, 63, 31);
	TilemapToVRAM(0xFF0000, 0x50000003, 63, 63);
}

struct PalCycleData
{
	int delay;
	uint planeAIdx;
	uint planeBOffs;
	uint something;
};

const PalCycleData PalCycleStuff[] =
{
	// Delay, idx into PlaneAOffsets, Plane B Nametable offset, palette offset somethingorother
	{  3, 0, 0x8407, 0x92 },
	{  3, 0, 0x8407, 0x90 },
	{  3, 0, 0x8407, 0x8E },
	{  3, 0, 0x8407, 0x8C },
	{  3, 0, 0x8407, 0x8B },
	{  3, 0, 0x8407, 0x80 },
	{  3, 0, 0x8407, 0x82 },
	{  3, 0, 0x8407, 0x84 },
	{  3, 0, 0x8407, 0x86 },
	{  3, 0, 0x8407, 0x88 },
	{  7, 4, 0x8407, 0x00 },
	{  7, 5, 0x8407, 0x0C },
	{ -1, 6, 0x8407, 0x18 },
	{ -1, 6, 0x8407, 0x18 },
	{  7, 5, 0x8407, 0x0C },
	{  7, 4, 0x8407, 0x00 },
	{  3, 0, 0x8406, 0x88 },
	{  3, 0, 0x8406, 0x86 },
	{  3, 0, 0x8406, 0x84 },
	{  3, 0, 0x8406, 0x82 },
	{  3, 0, 0x8406, 0x81 },
	{  3, 0, 0x8406, 0x8A },
	{  3, 0, 0x8406, 0x8C },
	{  3, 0, 0x8406, 0x8E },
	{  3, 0, 0x8406, 0x90 },
	{  3, 0, 0x8406, 0x92 },
	{  7, 1, 0x8406, 0x24 },
	{  7, 2, 0x8406, 0x30 },
	{ -1, 3, 0x8406, 0x3C },
	{ -1, 3, 0x8406, 0x3C },
	{  7, 2, 0x8406, 0x30 },
	{  7, 1, 0x8406, 0x24 },
};

const ushort PlaneAOffsets[][] =
{
	// Plane A Nametable offset, scroll y position
	{ 0x8210, 1 },
	{ 0x8218, 0 },
	{ 0x8218, 1 },
	{ 0x8220, 0 },
	{ 0x8220, 1 },
	{ 0x8228, 0 },
	{ 0x8228, 1 },
};

void PalCycle_SS()
{
	if(f_pause)
		return;

	// Update timer
	if(!TimerZero(v_palss_time))
		return;

	auto cycleData = PalCycleStuff[v_palss_num & 31];
	v_palss_num++;
	v_palss_time = (cycleData.delay < 0) ? 511 : cycleData.delay;
	v_FFFFF7A0 = cycleData.planeAIdx; // this variable has something to do with the BG animation

	// Handle animating the background tiles (bird/fish transformation)
	auto planeAOffset = PlaneAOffsets[cycleData.planeAIdx];
	VDP_COMMAND(planeAOffset[0]);
	v_scrposy_dup = planeAOffset[1];
	VDP_COMMAND(cycleData.planeBOffs);
	VDP_COMMAND(0x40000010);
	VDP_DATA(v_scrposy_dup);

	// Handle animating the palette.... somehow
	auto something = cycleData.something;

	if(something & 0x80)
	{
		memcpy(v_pal_dry + 0x4E, Pal_SSCyc1 + something, 12);
		return;
	}

	auto src = Pal_SSCyc2 + (something < 0x8A ? v_FFFFF79E : v_FFFFF79E + 1) * 0x2A; // NO idea what this variable is
	something &= 0x7E;

	if(something != 0)
		memcpy(v_pal_dry + 0x6E, src, 12);

	src += 12;
	auto dest = v_pal_dry + 0x5A;

	if(something >= 10)
	{
		something -= 10;
		dest = v_pal_dry + 0x7A;
	}

	memcpy(dest, src + (something * 3), 6);
}

// Pal_SSCyc1:	incbin	"palette\Cycle - Special Stage 1.bin"
// 		even
// Pal_SSCyc2:	incbin	"palette\Cycle - Special Stage 2.bin"
// 		even

const ubyte byte_4CB8[] = { 9, 0x28, 0x18, 0x10, 0x28, 0x18, 0x10, 0x30, 0x18, 8, 0x10, 0 };
const ubyte byte_4CC4[] = { 6, 0x30, 0x30, 0x30, 0x28, 0x18, 0x18, 0x18 };
const ubyte byte_4CCC[] = { 8, 2, 4, 0xFF, 2, 3, 8, 0xFF, 4, 2, 2, 3, 8, 0xFD, 4, 2, 2, 3, 2, 0xFF };

// don't even pretend to know what's doing on in this
void SS_BGAnimate()
{
	if(v_FFFFF7A0 == 0) // set in PalCycle_SS from the plane A nametable offset
	{
		// ??? these variables seem to have something to do with BG layer deformation
		v_FFFFF70C = 0;
		v_FFFFF618 = 0;
	}

	if(v_FFFFF7A0 < 8) // always true?
	{
		if(v_FFFFF7A0 == 6)
		{
			v_FFFFF718++;
			v_FFFFF70C++;
			v_FFFFF618 = v_FFFFF70C;
		}

		d0 = WSWAP(-v_FFFFF708) //w
		a1 = byte_4CCC //b
		a3 = v_ngfx_buffer //w

		for(int i = 0; i < 10; i++)
		{
			d0 = CalcSine((d0 & 0xFFFF0000) | a3[1]) //w (idx is w)
			d2 = *a1++ //b
			d0 *= d2
			d0 >>= 8 // arithmetic
			*a3++ = d0 //w
			d2 = *a1++ //b
			*a3 += d2 //w
		}

		a3 = v_ngfx_buffer //w
		a2 = byte_4CB8
	}
	else // never taken?
	{
		if(v_FFFFF7A0 == 12)
		{
			v_FFFFF718--;
			a3 = v_ngfx_buffer + 0x100 //l
			d2 = 0x18000

			for(int i = 0; i < 7; i++)
			{
				*a3 -= d2 //l
				a3++
				d2 -= 0x2000
			}
		}

		a3 = v_ngfx_buffer + 0x100 //w
		a2 = byte_4CC4 //b
	}

	d0 = WSWAP(-v_FFFFF718)
	d3 = *a2++ //b
	d2 = (-v_FFFFF70C & 0xFF) << 2 //w

	for(; d3 >= 0; d3--)
	{
		d0 = *a3++ //w
		a3++
		d1 = *a2++ - 1 //b

		for(; d1 >= 0; d1--)
		{
			v_hscrolltablebuffer[d2] = d0 //l; idx is bytes
			d2 = (d2 + 4) & 0x3FC //w
		}
	}
}

// in: d1, d2, d3, d5, a1, a2, a3
// out: d1, d5, a1, a2
void sub_D762()
{
	for(; d1 >= 0; d1--)
	{
		if(d5 == 0x50)
			return;

		d0 = a1++ //b>w
		d0 += d2
		*a2++ = d0 //w
		*a2++ = *a1++ //b
		d5++
		*a2++ = d5
		d0 = *a1++ << 8 //b
		d0 |= *a1++ //b
		d0 += a3
		*a2++ = d0 //w
		d0 = *a1++ //b>w
		d0 += d3
		d0 &= 0x1FF

		if(d0 == 0)
			d0 = 1

		*a2++ = d0 //w
	}
}

// TODO:
SS_ShowLayout:				; XREF: GM_Special
		bsr.w	SS_AniWallsRings
		bsr.w	SS_AniItems
		move.w	d5,-(sp)
		lea	(0xFFFF8000).w,a1
		move.b	(v_ssangle).w,d0
		andi.b	#0xFC,d0
		jsr	(CalcSine).l
		move.w	d0,d4
		move.w	d1,d5
		muls.w	#0x18,d4
		muls.w	#0x18,d5
		moveq	#0,d2
		move.w	(v_screenposx).w,d2
		divu.w	#0x18,d2
		swap	d2
		neg.w	d2
		addi.w	#-0xB4,d2
		moveq	#0,d3
		move.w	(v_screenposy).w,d3
		divu.w	#0x18,d3
		swap	d3
		neg.w	d3
		addi.w	#-0xB4,d3
		move.w	#0xF,d7

loc_1B19E:
		movem.w	d0-d2,-(sp)
		movem.w	d0-d1,-(sp)
		neg.w	d0
		muls.w	d2,d1
		muls.w	d3,d0
		move.l	d0,d6
		add.l	d1,d6
		movem.w	(sp)+,d0-d1
		muls.w	d2,d0
		muls.w	d3,d1
		add.l	d0,d1
		move.l	d6,d2
		move.w	#0xF,d6

loc_1B1C0:
		move.l	d2,d0
		asr.l	#8,d0
		move.w	d0,(a1)+
		move.l	d1,d0
		asr.l	#8,d0
		move.w	d0,(a1)+
		add.l	d5,d2
		add.l	d4,d1
		dbf	d6,loc_1B1C0

		movem.w	(sp)+,d0-d2
		addi.w	#0x18,d3
		dbf	d7,loc_1B19E

		move.w	(sp)+,d5
		lea	(0xFF0000).l,a0
		moveq	#0,d0
		move.w	(v_screenposy).w,d0
		divu.w	#0x18,d0
		mulu.w	#0x80,d0
		adda.l	d0,a0
		moveq	#0,d0
		move.w	(v_screenposx).w,d0
		divu.w	#0x18,d0
		adda.w	d0,a0
		lea	(0xFFFF8000).w,a4
		move.w	#0xF,d7

loc_1B20C:
		move.w	#0xF,d6

loc_1B210:
		moveq	#0,d0
		move.b	(a0)+,d0
		beq.s	loc_1B268
		cmpi.b	#0x4E,d0
		bhi.s	loc_1B268
		move.w	(a4),d3
		addi.w	#0x120,d3
		cmpi.w	#0x70,d3
		blo.s	loc_1B268
		cmpi.w	#0x1D0,d3
		bhs.s	loc_1B268
		move.w	2(a4),d2
		addi.w	#0xF0,d2
		cmpi.w	#0x70,d2
		blo.s	loc_1B268
		cmpi.w	#0x170,d2
		bhs.s	loc_1B268
		lea	(0xFF4000).l,a5
		lsl.w	#3,d0
		lea	(a5,d0.w),a5
		movea.l	(a5)+,a1
		move.w	(a5)+,d1
		add.w	d1,d1
		adda.w	(a1,d1.w),a1
		movea.w	(a5)+,a3
		moveq	#0,d1
		move.b	(a1)+,d1
		subq.b	#1,d1
		bmi.s	loc_1B268
		jsr	sub_D762

loc_1B268:
		addq.w	#4,a4
		dbf	d6,loc_1B210

		lea	0x70(a0),a0
		dbf	d7,loc_1B20C

		move.b	d5,(v_spritecount).w
		cmpi.b	#0x50,d5
		beq.s	loc_1B288
		move.l	#0,(a2)
		rts
; ===========================================================================

loc_1B288:
		move.b	#0,-5(a2)
		rts
; End of function SS_ShowLayout

; ===========================================================================
; Subroutine to	animate	walls and rings	in the special stage
; ===========================================================================

; ||||||||||||||| S U B	R O U T	I N E |||||||||||||||||||||||||||||||||||||||


SS_AniWallsRings:			; XREF: SS_ShowLayout
		lea	(0xFF400C).l,a1
		moveq	#0,d0
		move.b	(v_ssangle).w,d0
		lsr.b	#2,d0
		andi.w	#0xF,d0
		moveq	#0x23,d1

loc_1B2A4:
		move.w	d0,(a1)
		addq.w	#8,a1
		dbf	d1,loc_1B2A4

		lea	(0xFF4005).l,a1
		subq.b	#1,(v_ani1_time).w
		bpl.s	loc_1B2C8
		move.b	#7,(v_ani1_time).w
		addq.b	#1,(v_ani1_frame).w
		andi.b	#3,(v_ani1_frame).w

loc_1B2C8:
		move.b	(v_ani1_frame).w,0x1D0(a1)
		subq.b	#1,(v_ani2_time).w
		bpl.s	loc_1B2E4
		move.b	#7,(v_ani2_time).w
		addq.b	#1,(v_ani2_frame).w
		andi.b	#1,(v_ani2_frame).w

loc_1B2E4:
		move.b	(v_ani2_frame).w,d0
		move.b	d0,0x138(a1)
		move.b	d0,0x160(a1)
		move.b	d0,0x148(a1)
		move.b	d0,0x150(a1)
		move.b	d0,0x1D8(a1)
		move.b	d0,0x1E0(a1)
		move.b	d0,0x1E8(a1)
		move.b	d0,0x1F0(a1)
		move.b	d0,0x1F8(a1)
		move.b	d0,0x200(a1)
		subq.b	#1,(v_ani3_time).w
		bpl.s	loc_1B326
		move.b	#4,(v_ani3_time).w
		addq.b	#1,(v_ani3_frame).w
		andi.b	#3,(v_ani3_frame).w

loc_1B326:
		move.b	(v_ani3_frame).w,d0
		move.b	d0,0x168(a1)
		move.b	d0,0x170(a1)
		move.b	d0,0x178(a1)
		move.b	d0,0x180(a1)
		subq.b	#1,(v_ani0_time).w
		bpl.s	loc_1B350
		move.b	#7,(v_ani0_time).w
		subq.b	#1,(v_ani0_frame).w
		andi.b	#7,(v_ani0_frame).w

loc_1B350:
		lea	(0xFF4016).l,a1
		lea	(SS_WaRiVramSet).l,a0
		moveq	#0,d0
		move.b	(v_ani0_frame).w,d0
		add.w	d0,d0
		lea	(a0,d0.w),a0
		move.w	(a0),(a1)
		move.w	2(a0),8(a1)
		move.w	4(a0),0x10(a1)
		move.w	6(a0),0x18(a1)
		move.w	8(a0),0x20(a1)
		move.w	0xA(a0),0x28(a1)
		move.w	0xC(a0),0x30(a1)
		move.w	0xE(a0),0x38(a1)
		adda.w	#0x20,a0
		adda.w	#0x48,a1
		move.w	(a0),(a1)
		move.w	2(a0),8(a1)
		move.w	4(a0),0x10(a1)
		move.w	6(a0),0x18(a1)
		move.w	8(a0),0x20(a1)
		move.w	0xA(a0),0x28(a1)
		move.w	0xC(a0),0x30(a1)
		move.w	0xE(a0),0x38(a1)
		adda.w	#0x20,a0
		adda.w	#0x48,a1
		move.w	(a0),(a1)
		move.w	2(a0),8(a1)
		move.w	4(a0),0x10(a1)
		move.w	6(a0),0x18(a1)
		move.w	8(a0),0x20(a1)
		move.w	0xA(a0),0x28(a1)
		move.w	0xC(a0),0x30(a1)
		move.w	0xE(a0),0x38(a1)
		adda.w	#0x20,a0
		adda.w	#0x48,a1
		move.w	(a0),(a1)
		move.w	2(a0),8(a1)
		move.w	4(a0),0x10(a1)
		move.w	6(a0),0x18(a1)
		move.w	8(a0),0x20(a1)
		move.w	0xA(a0),0x28(a1)
		move.w	0xC(a0),0x30(a1)
		move.w	0xE(a0),0x38(a1)
		adda.w	#0x20,a0
		adda.w	#0x48,a1
		rts
; End of function SS_AniWallsRings

; ===========================================================================
SS_WaRiVramSet:	dc.w 0x142, 0x6142, 0x142,	0x142, 0x142, 0x142, 0x142,	0x6142
		dc.w 0x142, 0x6142, 0x142,	0x142, 0x142, 0x142, 0x142,	0x6142
		dc.w 0x2142, 0x142, 0x2142, 0x2142,	0x2142, 0x2142, 0x2142, 0x142
		dc.w 0x2142, 0x142, 0x2142, 0x2142,	0x2142, 0x2142, 0x2142, 0x142
		dc.w 0x4142, 0x2142, 0x4142, 0x4142, 0x4142,	0x4142, 0x4142, 0x2142
		dc.w 0x4142, 0x2142, 0x4142, 0x4142, 0x4142,	0x4142, 0x4142, 0x2142
		dc.w 0x6142, 0x4142, 0x6142, 0x6142, 0x6142,	0x6142, 0x6142, 0x4142
		dc.w 0x6142, 0x4142, 0x6142, 0x6142, 0x6142,	0x6142, 0x6142, 0x4142
; ===========================================================================
; Subroutine to	remove items when you collect them in the special stage
; ===========================================================================

; ||||||||||||||| S U B	R O U T	I N E |||||||||||||||||||||||||||||||||||||||


SS_RemoveCollectedItem:			; XREF: Obj09_ChkItems
		lea	(0xFF4400).l,a2
		move.w	#0x1F,d0

loc_1B4C4:
		tst.b	(a2)
		beq.s	locret_1B4CE
		addq.w	#8,a2
		dbf	d0,loc_1B4C4

locret_1B4CE:
		rts
; End of function SS_RemoveCollectedItem

; ===========================================================================
; Subroutine to	animate	special	stage items when you touch them
; ===========================================================================

; ||||||||||||||| S U B	R O U T	I N E |||||||||||||||||||||||||||||||||||||||


SS_AniItems:				; XREF: SS_ShowLayout
		lea	(0xFF4400).l,a0
		move.w	#0x1F,d7

loc_1B4DA:
		moveq	#0,d0
		move.b	(a0),d0
		beq.s	loc_1B4E8
		lsl.w	#2,d0
		movea.l	SS_AniIndex-4(pc,d0.w),a1
		jsr	(a1)

loc_1B4E8:
		addq.w	#8,a0

loc_1B4EA:
		dbf	d7,loc_1B4DA

		rts
; End of function SS_AniItems

; ===========================================================================
SS_AniIndex:	dc.l SS_AniRingSparks
		dc.l SS_AniBumper
		dc.l SS_Ani1Up
		dc.l SS_AniReverse
		dc.l SS_AniEmeraldSparks
		dc.l SS_AniGlassBlock
; ===========================================================================

SS_AniRingSparks:			; XREF: SS_AniIndex
		subq.b	#1,2(a0)
		bpl.s	locret_1B530
		move.b	#5,2(a0)
		moveq	#0,d0
		move.b	3(a0),d0
		addq.b	#1,3(a0)
		movea.l	4(a0),a1
		move.b	SS_AniRingData(pc,d0.w),d0
		move.b	d0,(a1)
		bne.s	locret_1B530
		clr.l	(a0)
		clr.l	4(a0)

locret_1B530:
		rts
; ===========================================================================
SS_AniRingData:	dc.b 0x42, 0x43, 0x44, 0x45, 0, 0
; ===========================================================================

SS_AniBumper:				; XREF: SS_AniIndex
		subq.b	#1,2(a0)
		bpl.s	locret_1B566
		move.b	#7,2(a0)
		moveq	#0,d0
		move.b	3(a0),d0
		addq.b	#1,3(a0)
		movea.l	4(a0),a1
		move.b	SS_AniBumpData(pc,d0.w),d0
		bne.s	loc_1B564
		clr.l	(a0)
		clr.l	4(a0)
		move.b	#0x25,(a1)
		rts
; ===========================================================================

loc_1B564:
		move.b	d0,(a1)

locret_1B566:
		rts
; ===========================================================================
SS_AniBumpData:	dc.b 0x32, 0x33, 0x32, 0x33, 0, 0
; ===========================================================================

SS_Ani1Up:				; XREF: SS_AniIndex
		subq.b	#1,2(a0)
		bpl.s	locret_1B596
		move.b	#5,2(a0)
		moveq	#0,d0
		move.b	3(a0),d0
		addq.b	#1,3(a0)
		movea.l	4(a0),a1
		move.b	SS_Ani1UpData(pc,d0.w),d0
		move.b	d0,(a1)
		bne.s	locret_1B596
		clr.l	(a0)
		clr.l	4(a0)

locret_1B596:
		rts
; ===========================================================================
SS_Ani1UpData:	dc.b 0x46, 0x47, 0x48, 0x49, 0, 0
; ===========================================================================

SS_AniReverse:				; XREF: SS_AniIndex
		subq.b	#1,2(a0)
		bpl.s	locret_1B5CC
		move.b	#7,2(a0)
		moveq	#0,d0
		move.b	3(a0),d0
		addq.b	#1,3(a0)
		movea.l	4(a0),a1
		move.b	SS_AniRevData(pc,d0.w),d0
		bne.s	loc_1B5CA
		clr.l	(a0)
		clr.l	4(a0)
		move.b	#0x2B,(a1)
		rts
; ===========================================================================

loc_1B5CA:
		move.b	d0,(a1)

locret_1B5CC:
		rts
; ===========================================================================
SS_AniRevData:	dc.b 0x2B, 0x31, 0x2B, 0x31, 0, 0
; ===========================================================================

SS_AniEmeraldSparks:			; XREF: SS_AniIndex
		subq.b	#1,2(a0)
		bpl.s	locret_1B60C
		move.b	#5,2(a0)
		moveq	#0,d0
		move.b	3(a0),d0
		addq.b	#1,3(a0)
		movea.l	4(a0),a1
		move.b	SS_AniEmerData(pc,d0.w),d0
		move.b	d0,(a1)
		bne.s	locret_1B60C
		clr.l	(a0)
		clr.l	4(a0)
		move.b	#4,(0xFFFFD024).w
		sfx	sfx_SSGoal	; play special stage GOAL sound

locret_1B60C:
		rts
; ===========================================================================
SS_AniEmerData:	dc.b 0x46, 0x47, 0x48, 0x49, 0, 0
; ===========================================================================

SS_AniGlassBlock:			; XREF: SS_AniIndex
		subq.b	#1,2(a0)
		bpl.s	locret_1B640
		move.b	#1,2(a0)
		moveq	#0,d0
		move.b	3(a0),d0
		addq.b	#1,3(a0)
		movea.l	4(a0),a1
		move.b	SS_AniGlassData(pc,d0.w),d0
		move.b	d0,(a1)
		bne.s	locret_1B640
		move.b	4(a0),(a1)
		clr.l	(a0)
		clr.l	4(a0)

locret_1B640:
		rts
; ===========================================================================
SS_AniGlassData:dc.b 0x4B, 0x4C, 0x4D, 0x4E, 0x4B, 0x4C, 0x4D,	0x4E, 0,	0

; ===========================================================================
; Special stage	layout pointers
; ===========================================================================
SS_LayoutIndex:
		dc.l SS_1
		dc.l SS_2
		dc.l SS_3
		dc.l SS_4
		dc.l SS_5
		dc.l SS_6
		even

; ===========================================================================
; Special stage start locations
; ===========================================================================
SS_StartLoc:	include	"_inc\Start Location Array - Special Stages.asm"

; ===========================================================================
; Subroutine to	load special stage layout
; ===========================================================================

; ||||||||||||||| S U B	R O U T	I N E |||||||||||||||||||||||||||||||||||||||


SS_Load:				; XREF: GM_Special
		moveq	#0,d0
		move.b	(v_lastspecial).w,d0 ; load number of last special stage entered
		addq.b	#1,(v_lastspecial).w
		cmpi.b	#6,(v_lastspecial).w
		blo.s	SS_ChkEmldNum
		move.b	#0,(v_lastspecial).w ; reset if higher than 6

SS_ChkEmldNum:
		cmpi.b	#6,(v_emeralds).w ; do you have all emeralds?
		beq.s	SS_LoadData	; if yes, branch
		moveq	#0,d1
		move.b	(v_emeralds).w,d1
		subq.b	#1,d1
		blo.s	SS_LoadData
		lea	(v_emldlist).w,a3 ; check which emeralds you have

SS_ChkEmldLoop:
		cmp.b	(a3,d1.w),d0
		bne.s	SS_ChkEmldRepeat
		bra.s	SS_Load
; ===========================================================================

SS_ChkEmldRepeat:
		dbf	d1,SS_ChkEmldLoop

SS_LoadData:
		lsl.w	#2,d0
		lea	SS_StartLoc(pc,d0.w),a1
		move.w	(a1)+,(v_player+obX).w
		move.w	(a1)+,(v_player+obY).w
		movea.l	SS_LayoutIndex(pc,d0.w),a0
		lea	(0xFF4000).l,a1
		move.w	#0,d0
		jsr	(EniDec).l
		lea	(0xFF0000).l,a1
		move.w	#0xFFF,d0

SS_ClrRAM3:
		clr.l	(a1)+
		dbf	d0,SS_ClrRAM3

		lea	(0xFF1020).l,a1
		lea	(0xFF4000).l,a0
		moveq	#0x3F,d1

loc_1B6F6:
		moveq	#0x3F,d2

loc_1B6F8:
		move.b	(a0)+,(a1)+
		dbf	d2,loc_1B6F8

		lea	0x40(a1),a1
		dbf	d1,loc_1B6F6

		lea	(0xFF4008).l,a1
		lea	(SS_MapIndex).l,a0
		moveq	#0x4D,d1

loc_1B714:
		move.l	(a0)+,(a1)+
		move.w	#0,(a1)+
		move.b	-4(a0),-1(a1)
		move.w	(a0)+,(a1)+
		dbf	d1,loc_1B714

		lea	(0xFF4400).l,a1
		move.w	#0x3F,d1

loc_1B730:

		clr.l	(a1)+
		dbf	d1,loc_1B730

		rts
; End of function SS_Load

; ===========================================================================

SS_MapIndex:
		include	"_inc\Special Stage Mappings & VRAM Pointers.asm"

		include	"_maps\SS R Block.asm"
		include	"_maps\SS Glass Block.asm"
		include	"_maps\SS UP Block.asm"
		include	"_maps\SS DOWN Block.asm"
		include	"_maps\SS Chaos Emeralds.asm"

		include	"_incObj\09 Sonic in Special Stage.asm"

		include	"_incObj\10.asm"

		include	"_inc\AnimateLevelGfx.asm"

		include	"_incObj\21 HUD.asm"
		include	"_maps\HUD.asm"
