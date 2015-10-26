const ubyte Z80_Program[37] =
{
	0xAF,             // xor  a
	0x01, 0xD9, 0x1F, // ld   bc,1fd9h
	0x11, 0x27, 0x00, // ld   de,0027h
	0x21, 0x26, 0x00, // ld   hl,0026h
	0xF9,             // ld   sp,hl
	0x77,             // ld   (hl),a
	0xED, 0xB0,       // ldir
	0xDD, 0xE1,       // pop  ix
	0xFD, 0xE1,       // pop  iy
	0xED, 0x47,       // ld   i,a
	0xED, 0x4F,       // ld   r,a
	0xD1,             // pop  de
	0xE1,             // pop  hl
	0xF1,             // pop  af
	0x08,             // ex   af,af'
	0xD9,             // exx
	0xC1,             // pop  bc
	0xD1,             // pop  de
	0xE1,             // pop  hl
	0xF1,             // pop  af
	0xF9,             // ld   sp,hl
	0xF3,             // di
	0xED, 0x56,       // im1
	0x36, 0xE9,       // ld   (hl),e9h
	0xE9              // jp   (hl)
};

void EntryPoint()
{
	if(z80_port_1_control == 0 && z80_expansion_control == 0)
	{
		if(z80_version & 0xF) // hw version
			tmss_register = FOURCC("SEGA");

		VDP_Control(); // dummy read?
		SET_USP(0); // set user stack pointer to top of memory (since stack grows down)

		VDP_RegWrite(0x00, 4);            // 8-colour mode
		VDP_RegWrite(0x01, 0x14);         // Megadrive mode, DMA enable
		VDP_RegWrite(0x02, (0xC000>>10)); // foreground nametable address
		VDP_RegWrite(0x03, (0xF000>>10)); // window nametable address
		VDP_RegWrite(0x04, (0xE000>>13)); // background nametable address
		VDP_RegWrite(0x05, (0xD800>>9));  // sprite table address
		VDP_RegWrite(0x06, 0);            // unused
		VDP_RegWrite(0x07, 0);            // background colour
		VDP_RegWrite(0x08, 0);            // unused
		VDP_RegWrite(0x09, 0);            // unused
		VDP_RegWrite(0x0A, 255);          // HBlank register
		VDP_RegWrite(0x0B, 0);            // full screen scroll
		VDP_RegWrite(0x0C, 0x81);         // 40 cell display
		VDP_RegWrite(0x0D, (0xDC00>>10)); // hscroll table address
		VDP_RegWrite(0x0E, 0);            // unused
		VDP_RegWrite(0x0F, 1);            // VDP increment
		VDP_RegWrite(0x10, 1);            // 64 cell hscroll size
		VDP_RegWrite(0x11, 0);            // window h position
		VDP_RegWrite(0x12, 0);            // window v position
		VDP_ClearVRAM();
		STOP_Z80();
		RESET_Z80();
		WAIT_Z80();
		memcpy(z80_ram, Z80_Program, sizeof(Z80_Program));
		RESET_Z80a();
		WAIT_Z80();
		RESET_Z80();
		memset(0xFFFF0000, 0, 0x10000); // clear RAM
		VDP_RegWrite(0x01, 4);
		VDP_RegWrite(0x0F, 2);
		VDP_ClearCRAM();
		VDP_ClearVSRAM();
		VDP_PSG_Write(0x9F);
		VDP_PSG_Write(0xBF);
		VDP_PSG_Write(0xDF);
		VDP_PSG_Write(0xFF);
		RESET_Z80a();
		DISABLE_INTERRUPTS();
	}

	VDP_Control(); // dummy read?

	if(!BTST(z80_expansion_control, 1 << 6) || v_init != FOURCC("init"))
	{
		// checksum here, which is not important
		memset(0xFFFFFE00, 0, 0x200);
		v_megadrive = z80_version & 0xC0;
		v_init = FOURCC("init");
	}

	memset(0xFFFF0000, 0, 0xFE00);

	VDP_SetupGame();
	// TODO:
	// SoundDriverLoad();
	// JoypadInit();
	v_gamemode = GameMode_Sega;

	while(true)
	{
		switch(v_gamemode & 0x1C)
		{
			case GameMode_Sega:     GM_Sega();     break;
			case GameMode_Title:    GM_Title();    break;
			case GameMode_Demo:
			case GameMode_Level:    GM_Level();    break;
			case GameMode_Special:  GM_Special();  break;
			case GameMode_Continue: GM_Continue(); break;
			case GameMode_Ending:   GM_Ending();   break;
			case GameMode_Credits:  GM_Credits();  break;
		}
	}
}