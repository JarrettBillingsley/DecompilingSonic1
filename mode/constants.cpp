enum
{
	GameMode_Sega =     0x00,
	GameMode_Title =    0x04,
	GameMode_Demo =     0x08,
	GameMode_Level =    0x0C,
	GameMode_Special =  0x10,
	GameMode_Continue = 0x14,
	GameMode_Ending =   0x18,
	GameMode_Credits =  0x1C,

	GameMode_PreLevel = 0x80,
};

enum
{
	DemoMode_Off = 0,
	DemoMode_On = 1,
	DemoMode_Credits = 0x8001,
};

enum
{
	SS_InitialRotate = 64,
	SS_TileSize = 24,

	SS_LayoutRows = 128,
	SS_LayoutColumns = 128,
	SS_LayoutUsedRows = 64,
	SS_LayoutUsedColumns = 64,
	SS_LayoutRowStart = 32,
	SS_NumAnimations = 32,
	SS_PosBufferSize = 16,
	SS_NumWallTileTypes = 36,
};

enum : ubyte
{
	SSObj_None =        0x00,

	SSObj_Bumper =      0x25,

	SSObj_Goal =        0x27,
	SSObj_1Up =         0x28,
	SSObj_Up =          0x29,
	SSObj_Down =        0x2A,
	SSObj_R =           0x2B,
	SSObj_Peppermint =  0x2C,
	SSObj_Glass4Hit =   0x2D,
	SSObj_Glass3Hit =   0x2E,
	SSObj_Glass2Hit =   0x2F,
	SSObj_Glass1Hit =   0x30,
	SSObj_RHit =        0x31,
	SSObj_BumpHit1 =    0x32,
	SSObj_BumpHit2 =    0x33,

	SSObj_Ring =        0x3A,
	SSObj_Emerald1 =    0x3B,
	SSObj_Emerald2 =    0x3C,
	SSObj_Emerald3 =    0x3D,
	SSObj_Emerald4 =    0x3E,
	SSObj_Emerald5 =    0x3F,
	SSObj_Emerald6 =    0x40,
	SSObj_GhostBlock =  0x41,
	SSObj_RingColl1 =   0x42,
	SSObj_RingColl2 =   0x43,
	SSObj_RingColl3 =   0x44,
	SSObj_RingColl4 =   0x45,
	SSObj_1UpColl1 =    0x46,
	SSObj_1UpColl2 =    0x47,
	SSObj_1UpColl3 =    0x48,
	SSObj_1UpColl4 =    0x49,
	SSObj_GhostTurner = 0x4A,
	SSObj_GlassHit1 =   0x4B,
	SSObj_GlassHit2 =   0x4C,
	SSObj_GlassHit3 =   0x4D,
	SSObj_GlassHit4 =   0x4E,

	SSObj_MAX = SSObj_GlassHit4,
	SSObj_COUNT
};

enum : ubyte
{
	SSAni_RingSparks = 1,
	SSAni_Bumper = 2,
	SSAni_1Up = 3,
	SSAni_Reverse = 4,
	SSAni_EmeraldSparks = 5,
	SSAni_GlassBlock = 6,
};

const ubyte* ColPointers[] =
{
	Col_GHZ,
	Col_LZ,
	Col_MZ,
	Col_SLZ,
	Col_SYZ,
	Col_SBZ,
//	Col_GHZ // Pointer for Ending is missing by default.
};

const LevSel_Ptr LevSel_Ptrs[] =
{
	{ Zone_GHZ, 0 },
	{ Zone_GHZ, 1 },
	{ Zone_GHZ, 2 },
	{ Zone_MZ,  0 },
	{ Zone_MZ,  1 },
	{ Zone_MZ,  2 },
	{ Zone_SYZ, 0 },
	{ Zone_SYZ, 1 },
	{ Zone_SYZ, 2 },
	{ Zone_LZ,  0 },
	{ Zone_LZ,  1 },
	{ Zone_LZ,  2 },
	{ Zone_SLZ, 0 },
	{ Zone_SLZ, 1 },
	{ Zone_SLZ, 2 },
	{ Zone_SBZ, 0 },
	{ Zone_SBZ, 1 },
	{ Zone_LZ,  3 },
	{ Zone_SBZ, 2 },
	{ Zone_SS,  0 }, // Special Stage
	{ 0x80,     0 }, // Sound Test
};

LevSel_Ptr Demo_Levels[] =
{
	{ Zone_GHZ,  0 },
	{ Zone_MZ,   0 },
	{ Zone_SYZ,  0 },
	{ Zone_SS,   0 },
};

const ubyte LevSelCode_J[] =  { Buttons_Up, Buttons_Dn, Buttons_Dn, Buttons_Dn, Buttons_L, Buttons_R, 0, 0xFF };
const ubyte LevSelCode_US[] = { Buttons_Up, Buttons_Dn, Buttons_L, Buttons_R, 0, 0xFF };

const ubyte SSAni_RingData[] =  { SSObj_RingColl1, SSObj_RingColl2, SSObj_RingColl3, SSObj_RingColl4, 0, 0 };
const ubyte SSAni_BumpData[] =  { SSObj_BumpHit1, SSObj_BumpHit2, SSObj_BumpHit1, SSObj_BumpHit2, 0, 0 };
const ubyte SSAni_1UpData[] =   { SSObj_1UpColl1, SSObj_1UpColl2, SSObj_1UpColl3, SSObj_1UpColl4, 0, 0 };
const ubyte SSAni_RevData[] =   { SSObj_R, SSObj_RHit, SSObj_R, SSObj_RHit, 0, 0 };
const ubyte SSAni_EmerData[] =  { SSObj_1UpColl1, SSObj_1UpColl2, SSObj_1UpColl3, SSObj_1UpColl4, 0, 0 };
const ubyte SSAni_GlassData[] = { SSObj_GlassHit1, SSObj_GlassHit2, SSObj_GlassHit3, SSObj_GlassHit4,
									SSObj_GlassHit1, SSObj_GlassHit2, SSObj_GlassHit3, SSObj_GlassHit4, 0, 0 };

const Point16 SS_StartLoc[] =
{
	{ 0x03D0, 0x02E0 },
	{ 0x0328, 0x0574 },
	{ 0x04E4, 0x02E0 },
	{ 0x03AD, 0x02E0 },
	{ 0x0340, 0x06B8 },
	{ 0x049B, 0x0358 },
};

const ubyte** SS_LayoutIndex[] =
{
	SS_1,
	SS_2,
	SS_3,
	SS_4,
	SS_5,
	SS_6,
};

// Took a little liberty here; the original code encodes the frame as the top 8 bits of the map, and then "unpacks" this
// data into v_sstileinfo in SS_Load. This is ugly and unneccessary in an HLL representation so I just store it in the
// runtime format and just memcpy it over.
const SSTileInfo SS_MapIndex[] =
{
	{ Map_SSWalls,   0, 0x0142 }, // 01
	{ Map_SSWalls,   0, 0x0142 },
	{ Map_SSWalls,   0, 0x0142 },
	{ Map_SSWalls,   0, 0x0142 },
	{ Map_SSWalls,   0, 0x0142 },
	{ Map_SSWalls,   0, 0x0142 },
	{ Map_SSWalls,   0, 0x0142 },
	{ Map_SSWalls,   0, 0x0142 }, // 08
	{ Map_SSWalls,   0, 0x0142 },
	{ Map_SSWalls,   0, 0x2142 },
	{ Map_SSWalls,   0, 0x2142 },
	{ Map_SSWalls,   0, 0x2142 },
	{ Map_SSWalls,   0, 0x2142 },
	{ Map_SSWalls,   0, 0x2142 },
	{ Map_SSWalls,   0, 0x2142 },
	{ Map_SSWalls,   0, 0x2142 }, // 10
	{ Map_SSWalls,   0, 0x2142 },
	{ Map_SSWalls,   0, 0x2142 },
	{ Map_SSWalls,   0, 0x4142 },
	{ Map_SSWalls,   0, 0x4142 },
	{ Map_SSWalls,   0, 0x4142 },
	{ Map_SSWalls,   0, 0x4142 },
	{ Map_SSWalls,   0, 0x4142 },
	{ Map_SSWalls,   0, 0x4142 }, // 18
	{ Map_SSWalls,   0, 0x4142 },
	{ Map_SSWalls,   0, 0x4142 },
	{ Map_SSWalls,   0, 0x4142 },
	{ Map_SSWalls,   0, 0x6142 },
	{ Map_SSWalls,   0, 0x6142 },
	{ Map_SSWalls,   0, 0x6142 },
	{ Map_SSWalls,   0, 0x6142 },
	{ Map_SSWalls,   0, 0x6142 }, // 20
	{ Map_SSWalls,   0, 0x6142 },
	{ Map_SSWalls,   0, 0x6142 },
	{ Map_SSWalls,   0, 0x6142 },
	{ Map_SSWalls,   0, 0x6142 },
	{ Map_Bump,      0, 0x023B },
	{ Map_SS_R,      0, 0x0570 },
	{ Map_SS_R,      0, 0x0251 },
	{ Map_SS_R,      0, 0x0370 }, // 28
	{ Map_SS_Up,     0, 0x0263 },
	{ Map_SS_Down,   0, 0x0263 },
	{ Map_SS_R,      0, 0x22F0 },
	{ Map_SS_Glass,  0, 0x0470 },
	{ Map_SS_Glass,  0, 0x05F0 },
	{ Map_SS_Glass,  0, 0x65F0 },
	{ Map_SS_Glass,  0, 0x25F0 },
	{ Map_SS_Glass,  0, 0x45F0 }, // 30
	{ Map_SS_R,      0, 0x02F0 },
	{ Map_Bump,      1, 0x023B },
	{ Map_Bump,      2, 0x023B },
	{ Map_SS_R,      0, 0x0797 },
	{ Map_SS_R,      0, 0x07A0 },
	{ Map_SS_R,      0, 0x07A9 },
	{ Map_SS_R,      0, 0x0797 },
	{ Map_SS_R,      0, 0x07A0 }, // 38
	{ Map_SS_R,      0, 0x07A9 },
	{ Map_Ring,      0, 0x27B2 },
	{ Map_SS_Chaos3, 0, 0x0770 },
	{ Map_SS_Chaos3, 0, 0x2770 },
	{ Map_SS_Chaos3, 0, 0x4770 },
	{ Map_SS_Chaos3, 0, 0x6770 },
	{ Map_SS_Chaos1, 0, 0x0770 },
	{ Map_SS_Chaos2, 0, 0x0770 }, // 40
	{ Map_SS_R,      0, 0x04F0 },
	{ Map_Ring,      4, 0x27B2 },
	{ Map_Ring,      5, 0x27B2 },
	{ Map_Ring,      6, 0x27B2 },
	{ Map_Ring,      7, 0x27B2 },
	{ Map_SS_Glass,  0, 0x23F0 },
	{ Map_SS_Glass,  1, 0x23F0 },
	{ Map_SS_Glass,  2, 0x23F0 }, // 48
	{ Map_SS_Glass,  3, 0x23F0 },
	{ Map_SS_R,      2, 0x04F0 },
	{ Map_SS_Glass,  0, 0x05F0 },
	{ Map_SS_Glass,  0, 0x65F0 },
	{ Map_SS_Glass,  0, 0x25F0 },
	{ Map_SS_Glass,  0, 0x45F0 }, // 4E
};

const ubyte* DemoDataPtr[] =
{
	Demo_GHZ,
	Demo_GHZ,
	Demo_MZ,
	Demo_MZ,
	Demo_SYZ,
	Demo_SYZ,
	Demo_SS,
	Demo_SS,
};

const ubyte* DemoEndDataPtr[] =
{
	Demo_EndGHZ1,
	Demo_EndMZ,
	Demo_EndSYZ,
	Demo_EndLZ,
	Demo_EndSLZ,
	Demo_EndSBZ1,
	Demo_EndSBZ2,
	Demo_EndGHZ2,
};

// Random junk?? Looks like demo data but what's it doing here
// dc.b 0,	$8B, 8,	$37, 0,	$42, 8,	$5C, 0,	$6A, 8,	$5F, 0,	$2F, 8,	$2C
// dc.b 0,	$21, 8,	3, $28,	$30, 8,	8, 0, $2E, 8, $15, 0, $F, 8, $46
// dc.b 0,	$1A, 8,	$FF, 8,	$CA, 0,	0, 0, 0, 0, 0, 0, 0, 0,	0
// even

const char LevelMenuText[21][24] =
{
	//  G    R    E    E    N    _    H    I    L    L    _    Z    O    N    E    _    _    S    T    A    G    E    _    1
	{ 0x17,0x22,0x15,0x15,0x1E,0xFF,0x18,0x19,0x1C,0x1C,0xFF,0x10,0x1F,0x1E,0x15,0xFF,0xFF,0x23,0x24,0x11,0x17,0x15,0xFF,0x01 },
	{ 0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x23,0x24,0x11,0x17,0x15,0xFF,0x02 },
	{ 0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x23,0x24,0x11,0x17,0x15,0xFF,0x03 },

	//  M    A    R    B    L    E    _    Z    O    N    E    _    _    _    _    _    _    S    T    A    G    E    _    1
	{ 0x1D,0x11,0x22,0x12,0x1C,0x15,0xFF,0x10,0x1F,0x1E,0x15,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x23,0x24,0x11,0x17,0x15,0xFF,0x01 },
	{ 0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x23,0x24,0x11,0x17,0x15,0xFF,0x02 },
	{ 0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x23,0x24,0x11,0x17,0x15,0xFF,0x03 },

	//  S    P    R    I    N    G    _    Y    A    R    D    _    Z    O    N    E    _    S    T    A    G    E    _    1
	{ 0x23,0x20,0x22,0x19,0x1E,0x17,0xFF,0x0F,0x11,0x22,0x14,0xFF,0x10,0x1F,0x1E,0x15,0xFF,0x23,0x24,0x11,0x17,0x15,0xFF,0x01 },
	{ 0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x23,0x24,0x11,0x17,0x15,0xFF,0x02 },
	{ 0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x23,0x24,0x11,0x17,0x15,0xFF,0x03 },

	//  L    A    B    Y    R    I    N    T    H    _    Z    O    N    E    _    _    _    S    T    A    G    E    _    1
	{ 0x1C,0x11,0x12,0x0F,0x22,0x19,0x1E,0x24,0x18,0xFF,0x10,0x1F,0x1E,0x15,0xFF,0xFF,0xFF,0x23,0x24,0x11,0x17,0x15,0xFF,0x01 },
	{ 0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x23,0x24,0x11,0x17,0x15,0xFF,0x02 },
	{ 0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x23,0x24,0x11,0x17,0x15,0xFF,0x03 },

	//  S    T    A    R    _    L    I    G    H    T    _    Z    O    N    E    _    _    S    T    A    G    E    _    1
	{ 0x23,0x24,0x11,0x22,0xFF,0x1C,0x19,0x17,0x18,0x24,0xFF,0x10,0x1F,0x1E,0x15,0xFF,0xFF,0x23,0x24,0x11,0x17,0x15,0xFF,0x01 },
	{ 0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x23,0x24,0x11,0x17,0x15,0xFF,0x02 },
	{ 0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x23,0x24,0x11,0x17,0x15,0xFF,0x03 },

	//  S    C    R    A    P    _    B    R    A    I    N    _    Z    O    N    E    _    S    T    A    G    E    _    1
	{ 0x23,0x13,0x22,0x11,0x20,0xFF,0x12,0x22,0x11,0x19,0x1E,0xFF,0x10,0x1F,0x1E,0x15,0xFF,0x23,0x24,0x11,0x17,0x15,0xFF,0x01 },
	{ 0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x23,0x24,0x11,0x17,0x15,0xFF,0x02 },
	{ 0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x23,0x24,0x11,0x17,0x15,0xFF,0x03 },

	//  F    I    N    A    L    _    Z    O    N    E
	{ 0x16,0x19,0x1E,0x11,0x1C,0xFF,0x10,0x1F,0x1E,0x15,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF },

	//  S    P    E    C    I    A    L    _    S    T    A    G    E
	{ 0x23,0x20,0x15,0x13,0x19,0x11,0x1C,0xFF,0x23,0x24,0x11,0x17,0x15,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF },

	//  S    O    U    N    D    _    S    E    L    E    C    T
	{ 0x23,0x1F,0x25,0x1E,0x14,0xFF,0x23,0x15,0x1C,0x15,0x13,0x24,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF },
};
