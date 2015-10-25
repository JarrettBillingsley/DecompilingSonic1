enum
{
	Buttons_Start = 0x80,            // Start button
	Buttons_A =     0x40,            // A
	Buttons_C =     0x20,            // C
	Buttons_B =     0x10,            // B
	Buttons_R =     0x08,            // Right
	Buttons_L =     0x04,            // Left
	Buttons_Dn =    0x02,            // Down
	Buttons_Up =    0x01,            // Up
	Buttons_Dir =   Buttons_L | Buttons_R | Buttons_Dn | Buttons_ Up, // Any direction
	Buttons_ABC =   Buttons_A | Buttons_B | Buttons_C,                // Any button
};
