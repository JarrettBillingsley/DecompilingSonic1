struct PLCDesc
{
	uint gfx;
	ushort vram;
};

struct PLCList
{
	uint numPLCs;
	PLCDesc plcs[];
};