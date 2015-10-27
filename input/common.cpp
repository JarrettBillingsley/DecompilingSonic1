
void JoypadInit()
{
	STOP_Z80();
	WAIT_Z80();
	*0xFFA10009 = 0x40;
	*0xFFA1000B = 0x40;
	*0xFFA1000D = 0x40;
	START_Z80();
}

void ReadJoypads()
{
	auto newButtons = ReadJoypad(0);
	auto oldButtons = v_jpadhold1;
	v_jpadhold1 = newButtons;
	v_jpadpress1 = newButtons & (oldButtons ^ newButtons);

	newButtons = ReadJoypad(1);
	oldButtons = v_jpadhold2;
	v_jpadhold2 = newButtons;
	v_jpadpress2 = newButtons & (oldButtons ^ newButtons);
}
