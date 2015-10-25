struct Object
{
	ID id;        // 00 object id
	ubyte render; // 01 bitfield for x/y flip, display mode
	ushort gfx;   // 02 palette line & VRAM setting (2 bytes)
	uint map;     // 04 mappings address (4 bytes)

	union
	{
		struct
		{
			ushort x16;     // 08 16-bit x-axis position
			ushort screenY; // 0A y-axis position for screen-fixed items (2 bytes)
		};

		uint x; // 08 32-bit x-axis position
	};

	uint y;          // 0C y-axis position
	ushort velX;     // 10 x-axis velocity
	ushort velY;     // 12 y-axis velocity (2 bytes)
	ushort inertia;  // 14 potential speed (2 bytes)
	ubyte height;    // 16 height/2
	ubyte width;     // 17 width/2
	ubyte priority;  // 18 sprite stack priority -- 0 is front
	ubyte actWid;    // 19 action width
	ubyte frame;     // 1A current frame displayed
	ubyte aniFrame;  // 1B current frame in animation script
	ubyte anim;      // 1C current animation
	ubyte nextAni;   // 1D next animation
	ubyte timeFrame; // 1E time to next frame
	ubyte delayAni;  // 1F time to delay animation
	ubyte colType;   // 20 collision response type
	ubyte colProp;   // 21 collision extra property
	ubyte status;    // 22 orientation or mode
	ubyte respawnNo; // 23 respawn list index number
	ubyte routine;   // 24 routine number

	union
	{
		ubyte routine2; // 25 secondary routine number
		ubyte solid;    // 25 solid status flag
	};

	ushort angle;  // 26 angle
	ubyte subtype; // 28 object subtype

	ubyte variables[23]; // 29 per-object variables

	inline byte&   var_b (int offset) { return *(byte*)  &variables[offset - 0x29]; }
	inline ubyte&  var_ub(int offset) { return *(ubyte*) &variables[offset - 0x29]; }
	inline short&  var_w (int offset) { return *(short*) &variables[offset - 0x29]; }
	inline ushort& var_uw(int offset) { return *(ushort*)&variables[offset - 0x29]; }
	inline int&    var_l (int offset) { return *(int*)   &variables[offset - 0x29]; }
	inline uint&   var_ul(int offset) { return *(uint*)  &variables[offset - 0x29]; }
};

static_assert(sizeof(Object) == 64);

#define VAR_B (self, name) ((self)->var_b(name))
#define VAR_UB(self, name) ((self)->var_ub(name))
#define VAR_W (self, name) ((self)->var_w(name))
#define VAR_UW(self, name) ((self)->var_uw(name))
#define VAR_L (self, name) ((self)->var_l(name))
#define VAR_UL(self, name) ((self)->var_ul(name))
