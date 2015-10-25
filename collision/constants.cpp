enum
{
	ColClass_Enemy =   0x00,
	ColClass_Powerup = 0x40,
	ColClass_Hurt =    0x80,
	ColClass_Special = 0xC0,

	ColClass_Mask =    0xC0,
};

enum
{
	ColSize_Invalid = 0x00,
	ColSize_20x20 =   0x01,
	ColSize_12x20 =   0x02,
	ColSize_20x12 =   0x03,
	ColSize_4x16 =    0x04,
	ColSize_12x18 =   0x05,
	ColSize_16x16 =   0x06,
	ColSize_6x6 =     0x07,
	ColSize_24x12 =   0x08,
	ColSize_12x16 =   0x09,
	ColSize_16x12 =   0x0A,
	ColSize_8x8_A =   0x0B,
	ColSize_20x16 =   0x0C,
	ColSize_20x8 =    0x0D,
	ColSize_14x14 =   0x0E,
	ColSize_24x24_A = 0x0F,
	ColSize_40x16 =   0x10,
	ColSize_16x24 =   0x11,
	ColSize_8x16 =    0x12,
	ColSize_32x112 =  0x13,
	ColSize_64x32 =   0x14,
	ColSize_128x32 =  0x15,
	ColSize_32x32 =   0x16,
	ColSize_8x8_B =   0x17,
	ColSize_4x4 =     0x18,
	ColSize_32x8 =    0x19,
	ColSize_12x12 =   0x1A,
	ColSize_8x4 =     0x1B,
	ColSize_24x4 =    0x1C,
	ColSize_40x4 =    0x1D,
	ColSize_4x8 =     0x1E,
	ColSize_4x24 =    0x1F,
	ColSize_4x40 =    0x20,
	ColSize_4x32 =    0x21,
	ColSize_24x24_B = 0x22,
	ColSize_12x24 =   0x23,
	ColSize_72x8 =    0x24,

	ColSize_Mask =    0x3F,
};

const ubyte Sizes[][] =
{
	{   0,   0 },  // 0x00 (unused)
	{  20,  20 },  // 0x01
	{  12,  20 },  // 0x02
	{  20,  12 },  // 0x03
	{   4,  16 },  // 0x04
	{  12,  18 },  // 0x05
	{  16,  16 },  // 0x06
	{   6,   6 },  // 0x07
	{  24,  12 },  // 0x08
	{  12,  16 },  // 0x09
	{  16,  12 },  // 0x0A
	{   8,   8 },  // 0x0B
	{  20,  16 },  // 0x0C
	{  20,   8 },  // 0x0D
	{  14,  14 },  // 0x0E
	{  24,  24 },  // 0x0F
	{  40,  16 },  // 0x10
	{  16,  24 },  // 0x11
	{   8,  16 },  // 0x12
	{  32, 112 },  // 0x13
	{  64,  32 },  // 0x14
	{ 128,  32 },  // 0x15
	{  32,  32 },  // 0x16
	{   8,   8 },  // 0x17
	{   4,   4 },  // 0x18
	{  32,   8 },  // 0x19
	{  12,  12 },  // 0x1A
	{   8,   4 },  // 0x1B
	{  24,   4 },  // 0x1C
	{  40,   4 },  // 0x1D
	{   4,   8 },  // 0x1E
	{   4,  24 },  // 0x1F
	{   4,  40 },  // 0x20
	{   4,  32 },  // 0x21
	{  24,  24 },  // 0x22
	{  12,  24 },  // 0x23
	{  72,   8 },  // 0x24
};
