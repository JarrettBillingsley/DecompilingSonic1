const PLCList ArtLoadCues[][] =
{
	// ...
	{
		0,
		{}
	}
};

void AddPLC(uint plc)
{
	auto cues = &ArtLoadCues[plc];
	auto buffer = v_plc_buffer;

	while(buffer->gfx != 0)
		buffer++;

	// <= is right
	for(int i = 0; i <= cues->numPLCs; i++)
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
	// also clears FFFFF6E4..FFFFF6F7
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