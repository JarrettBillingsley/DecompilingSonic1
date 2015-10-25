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
	if(v_plc_buffer[0].gfx != 0 && f_plc_execute == 0
	{
		auto src = v_plc_buffer[0].gfx;

		auto numPatterns = (src[0] << 8) | src[1]; src += 2;
		nem_plcState.isXor = numPatterns & 0x8000;
		f_plc_execute = numPatterns & 0x7FFF;
		Nem_Build_Code_Table(src);
		nem_plcState.bitBuffer = (src[0] << 8) | src[1]; src += 2;
		v_plc_buffer[0].gfx = src;
		nem_plcState.bitsInBuffer = 16;
		nem_plcState.repeatCount = 0;
		nem_plcState.palIdx = 0;
		nem_plcState.prevRow = 0;
	}
}

void QuickPLC(ushort plc)
{
	auto cues = &ArtLoadCues[plc];

	for(int i = 0; i <= cues->numPLCs; i++)
		NemDec(cues->plcs[i].gfx, cues->plcs[i].vram);
}

void PLC_Decode9Tiles()
{
	if(f_plc_execute != 0)
	{
		auto dest = v_plc_buffer[0].vram;
		nem_tileDecodeCount = 9;
		v_plc_buffer[0].vram += 32 * 9;
		PLC_DecodeSomeTiles(dest);
	}
}

void PLC_Decode3Tiles()
{
	if(f_plc_execute != 0)
	{
		auto dest = v_plc_buffer[0].vram;
		nem_tileDecodeCount = 3;
		v_plc_buffer[0].vram += 32 * 3;
		PLC_DecodeSomeTiles(dest);
	}
}

void PLC_DecodeSomeTiles(ushort vramAddr)
{
	// Convert vramAddr into a VDP address command
	VDP_COMMAND(WSWAP(((vramAddr << 2) | ((vramAddr & 0xFFFF) >> 2)) | 0x4000));

	for(; nem_tileDecodeCount > 0; nem_tileDecodeCount--)
	{
		NemDec3(nem_plcState, v_plc_buffer[0].gfx, 0, 0, 8, true);

		// Have we decoded all the patterns?
		if(TimerZero(f_plc_execute))
		{
			// Yes, copy PLC buffer entries down and leave
			memmove(v_plc_buffer, v_plc_buffer + 1, sizeof(PLCDesc) * (PLC_BUFFER_SIZE - 1));
			return;
		}
	}
}