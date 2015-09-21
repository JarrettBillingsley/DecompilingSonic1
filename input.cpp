namespace Buttons
{
	enum
	{
		Start = 0x80,            // Start button
		A =     0x40,            // A
		C =     0x20,            // C
		B =     0x10,            // B
		R =     0x08,            // Right
		L =     0x04,            // Left
		Dn =    0x02,            // Down
		Up =    0x01,            // Up
		Dir =   L | R | Dn | Up, // Any direction
		ABC =   A | B | C,       // A, B or C
	};
};

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
