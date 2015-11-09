void GM_Sega()
{
	PlaySound_Special(BGM_Stop);
	ClearPLC();
	PaletteFadeOut();
	VDP_RegWrite(0x0, 4);
	VDP_RegWrite(0x2, vram_fg >> 10);
	VDP_RegWrite(0x4, vram_bg >> 13);
	VDP_RegWrite(0x7, 0);
	VDP_RegWrite(0xB, 0);
	f_wtr_state = false;
	DISABLE_INTERRUPTS();
	VDP_Control(v_vdp_buffer1 & 0xBF);
	ClearScreen();
	NemDec(Nem_SegaLogo, 0);
	EniDec(Eni_SegaLogo, 0xFF0000, 0);

	CopyTilemap(0xFF0000, 0xE510, 0x17, 7);
	CopyTilemap(0xFF0180, 0xC000, 0x27, 0x1B);

	// In Japan, hide the TM
	if(!(v_megadrive & 0x80))
		CopyTilemap(0xFF0A40, 0xC53A, 2, 1);

	PalLoad2(Palette_SegaBG);

	v_pcyc_num = -10;
	v_pcyc_time = 0;
	v_pal_buffer[0x12] = 0; // word, byte offset
	v_pal_buffer[0x10] = 0; // word, byte offset
	VDP_Control(v_vdp_buffer1 | 0x40);

	do
		WaitForVBlank(VBlank_Sega);
	while(PalCycle_Sega());

	PlaySound_Special(SFX_Sega);
	WaitForVBlank(VBlank_SegaSwitch);
	v_demolength = 31;

	do
		WaitForVBlank(VBlank_Sega);
	while(v_demolength > 0 && !(v_jpadpress1 & Buttons_Start));

	v_gamemode = GameMode_Title;
}