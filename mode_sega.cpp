void GM_Sega()
{
	PlaySound_Special(BGM_Stop);
	ClearPLC();
	PaletteFadeOut();

	// lea	(vdp_control_port).l,a6
	// move.w	#$8004,(a6)	; use 8-colour mode
	// move.w	#$8200+(vram_fg>>10),(a6) ; set foreground nametable address
	// move.w	#$8400+(vram_bg>>13),(a6) ; set background nametable address
	// move.w	#$8700,(a6)	; set background colour (palette entry 0)
	// move.w	#$8B00,(a6)	; full-screen vertical scrolling

	f_wtr_state = false;
	DISABLE_INTERRUPTS();

	// move.w	(v_vdp_buffer1).w,d0
	// andi.b	#$BF,d0
	// move.w	d0,(vdp_control_port).l

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

	// move.w	(v_vdp_buffer1).w,d0
	// ori.b	#$40,d0
	// move.w	d0,(vdp_control_port).l

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