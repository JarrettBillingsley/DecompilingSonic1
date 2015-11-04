struct LevelHeader
{
	uint gfx; // high 8 bits are PLC 1
	uint map16; // high 8 bits are PLC 2
	uint map256;
	ubyte reserved;
	ubyte music;
	ubyte palette;
	ubyte palette_dup;
};

static_assert(sizeof(LevelHeader) == 16);

struct LevelSize
{
	ushort unk;
	ushort left;
	ushort right;
	ushort top;
	ushort bottom;
	ushort lookshift;
};