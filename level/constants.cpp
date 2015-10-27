enum
{
	Zone_GHZ,
	Zone_LZ,
	Zone_MZ,
	Zone_SLZ,
	Zone_SYZ,
	Zone_SBZ,
	Zone_EndZ,
	Zone_SS,
};

const LevelHeader LevelHeaders[] =
{
	{ (PLC_GHZ << 24) + Nem_GHZ_2nd, (PLC_GHZ2 << 24) + Blk16_GHZ, Blk256_GHZ, BGM_GHZ, Palette_GHZ    }, // Green Hill
	{ (PLC_LZ << 24)  + Nem_LZ,      (PLC_LZ2 << 24)  + Blk16_LZ,  Blk256_LZ,  BGM_LZ,  Palette_LZ     }, // Labyrinth
	{ (PLC_MZ << 24)  + Nem_MZ,      (PLC_MZ2 << 24)  + Blk16_MZ,  Blk256_MZ,  BGM_MZ,  Palette_MZ     }, // Marble
	{ (PLC_SLZ << 24) + Nem_SLZ,     (PLC_SLZ2 << 24) + Blk16_SLZ, Blk256_SLZ, BGM_SLZ, Palette_SLZ    }, // Star Light
	{ (PLC_SYZ << 24) + Nem_SYZ,     (PLC_SYZ2 << 24) + Blk16_SYZ, Blk256_SYZ, BGM_SYZ, Palette_SYZ    }, // Spring Yard
	{ (PLC_SBZ << 24) + Nem_SBZ,     (PLC_SBZ2 << 24) + Blk16_SBZ, Blk256_SBZ, BGM_SBZ, Palette_SBZ1   }, // Scrap Brain
	{                    Nem_GHZ_2nd,                     Blk16_GHZ, Blk256_GHZ, BGM_SBZ, Palette_Ending }, // Ending
};