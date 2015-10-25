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
