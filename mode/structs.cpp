struct LevSel_Ptr
{
	int zone;
	int act;
};

struct SS_Animation
{
	ubyte id;
	ubyte _unused;
	ubyte timer;
	ubyte frame;
	uint* tile;
};

struct SSTileInfo
{
	ubyte* map;
	ushort frame;
	ushort vram;
};
