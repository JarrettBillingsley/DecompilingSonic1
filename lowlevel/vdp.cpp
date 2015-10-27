
enum
{
	VDP_VRAM_Read =   0x00000000,
	VDP_VRAM_Write =  0x40000000,
	VDP_CRAM_Read =   0x00000020,
	VDP_CRAM_Write =  0xC0000000,
	VDP_VSRAM_Read =  0x00000010,
	VDP_VSRAM_Write = 0x40000010,
};

void VDP_Control(uint val)
{
	*vdp_control_port = val;
}

ushort VDP_Control()
{
	return *vdp_control_port;
}

void VDP_Data(uint val)
{
	*vdp_data_port = val;
}

ushort VDP_Data()
{
	return *vdp_data_port;
}

void VDP_RegWrite(ubyte reg, ubyte val)
{
	VDP_Write(0x8000 | reg << 8 | val);
}

void VDP_SetAddr(ushort addr, uint type)
{
	VDP_Control((addr >> 14) | ((addr & 0x3FFF) << 16) | type);
}

void VDP_PSG_Write(ubyte val)
{
	*(vdp_control_port + 0x11) = val;
}

void VDP_ClearVRAM(uint addr = 0, uint length = 0xFFFF)
{
	VDP_RegWrite(0x13, length & 0xFF);
	VDP_RegWrite(0x14, (length >> 8) & 0xFF);
	VDP_RegWrite(0x15, 0);
	VDP_RegWrite(0x16, 0);
	VDP_RegWrite(0x17, 0x80);
	VDP_SetAddr(addr, VDP_VRAM_Write);
	VDP_Data(0);
}

void VDP_ClearCRAM()
{
	VDP_SetAddr(0, VDP_CRAM_Write);

	for(int i = 0; i < 32; i++)
		VDP_Data(0);
}

void VDP_ClearVSRAM()
{
	VDP_SetAddr(0, VDP_VSRAM_Write);

	for(int i = 0; i < 20; i++)
		VDP_Data(0);
}

void VDP_WaitForDMA()
{
	while(VDP_Control() & 1) {}
}

void VDP_SetupGame()
{
	VDP_RegWrite(0x00, 0x04);                        // 8-colour mode
	VDP_RegWrite(0x01, 0x34);                        // enable V.interrupts, enable DMA
	VDP_RegWrite(0x02, 0x00 + (vram_fg >> 10));      // set foreground nametable address
	VDP_RegWrite(0x03, 0x00 + (0xA000 >> 10));       // set window nametable address
	VDP_RegWrite(0x04, 0x00 + (vram_bg >> 13));      // set background nametable address
	VDP_RegWrite(0x05, 0x00 + (vram_sprites >> 9));  // set sprite table address
	VDP_RegWrite(0x06, 0x00);                        // unused
	VDP_RegWrite(0x07, 0x00);                        // set background colour (palette entry 0)
	VDP_RegWrite(0x08, 0x00);                        // unused
	VDP_RegWrite(0x09, 0x00);                        // unused
	VDP_RegWrite(0x0A, 0x00);                        // default H.interrupt register
	VDP_RegWrite(0x0B, 0x00);                        // full-screen vertical scrolling
	VDP_RegWrite(0x0C, 0x81);                        // 40-cell display mode
	VDP_RegWrite(0x0D, 0x00 + (vram_hscroll >> 10)); // set background hscroll address
	VDP_RegWrite(0x0E, 0x00);                        // unused
	VDP_RegWrite(0x0F, 0x02);                        // set VDP increment size
	VDP_RegWrite(0x10, 0x01);                        // 64-cell hscroll size
	VDP_RegWrite(0x11, 0x00);                        // window horizontal position
	VDP_RegWrite(0x12, 0x00);                        // window vertical position
	v_vdp_buffer1 = 0x8134
	*v_hbla_hreg = 0x8A00 | 223;
	VDP_ClearCRAM();
	v_scrposy_dup = 0;
	v_scrposx_dup = 0;
	VDP_ClearVRAM();
	VDP_WaitForDMA();
	VDP_RegWrite(0x0F, 2);
}

void ClearScreen()
{
	VDP_Clear_VRAM(vram_fg, 0x1000);
	VDP_WaitForDMA();
	VDP_RegWrite(0x0F, 2);
	VDP_Clear_VRAM(vram_bg, 0x1000);
	VDP_WaitForDMA();
	VDP_RegWrite(0x0F, 2);
	v_scrposy_dup = 0;
	v_scrposx_dup = 0;
	memset(v_spritetablebuffer, 0, sizeof(v_spritetablebuffer));
	memset(v_hscrolltablebuffer, 0, sizeof(v_hscrolltablebuffer));
}

//                           a1        d0        d1        d2
void TilemapToVRAM(ushort* tiles, int vram, int cols, int rows)
{
	for(int i = 0; i < rows; i++, vram += 0x800000)
	{
		VDP_Control(vram);

		for(int j = 0; j < cols; j++)
			VDP_Data(*tiles++);
	}
}