// =====================================================================================================================
// Enums and structs
// =====================================================================================================================

namespace ObjStatus
{
	enum : ubyte
	{
		Flip = 0x01,       // 0
		Air = 0x02,        // 1
		Rolling = 0x04,    // 2
		StandingOn = 0x08, // 3
		Jumping = 0x10,    // 4
		Pushing = 0x20,    // 5
		Underwater = 0x40, // 6
		Dead = 0x80        // 7
	};
}

namespace ObjRender
{
	enum : ubyte
	{
		HorizFlip =     0x01, // 0
		VertFlip =      0x02, // 1

		LayerNormal =   0x04, // 2
		LayerBG1 =      0x00, // 2
		LayerBG2 =      0x04, // 2
		LayerBG =       0x08, // 3
		Layer =         0x0C, // 2 + 3; 0 = screen-relative; 1 = normal; 2 = BG1-relative; 3 = BG2-relative

		Something =     0x10, // 4; changes Y positioning somehow?
		SmashFragment = 0x20, // 5
		Behind =        0x40, // 6
		Visible =       0x80, // 7
	};
}

namespace AnimFlags
{
	enum
	{
		Command =  0x80, // Top bit set means it's a command
		End =      0xFF, // return to beginning of animation
		Back =     0xFE, // go back (specified number) bytes
		Change =   0xFD, // run specified animation
		Routine =  0xFC, // increment routine counter
		Reset =    0xFB, // reset animation and 2nd object routine counter
		Routine2 = 0xFA, // increment 2nd routine counter
	};
};

namespace ID
{
	enum : ubyte
	{
		SonicPlayer = 1, // 0x01
		Obj02,
		Obj03,
		Obj04,
		Obj05,
		Obj06,
		Obj07,
		Splash,		    // 0x08
		SonicSpecial,
		DrownCount,
		Pole,
		FlapDoor,
		Signpost,
		TitleSonic,
		PSBTM,
		Obj10,		    // 0x10
		Bridge,
		SpinningLight,
		LavaMaker,
		LavaBall,
		SwingingPlatform,
		Harpoon,
		Helix,
		BasicPlatform,	// 0x18
		Obj19,
		CollapseLedge,
		WaterSurface,
		Scenery,
		MagicSwitch,
		BallHog,
		Crabmeat,
		Cannonball,		// 0x20
		HUD,
		BuzzBomber,
		Missile,
		MissileDissolve,
		Rings,
		Monitor,
		ExplosionItem,
		Animals,		// 0x28
		Points,
		AutoDoor,
		Chopper,
		Jaws,
		Burrobot,
		PowerUp,
		LargeGrass,
		GlassBlock,		// 0x30
		ChainStomp,
		Button,
		PushBlock,
		TitleCard,
		GrassFire,
		Spikes,
		RingLoss,
		ShieldItem,		// 0x38
		GameOverCard,
		GotThroughCard,
		PurpleRock,
		SmashWall,
		BossGreenHill,
		Prison,
		ExplosionBomb,
		MotoBug,		// 0x40
		Springs,
		Newtron,
		Roller,
		EdgeWalls,
		SideStomp,
		MarbleBrick,
		Bumper,
		BossBall,		// 0x48
		WaterSound,
		VanishSonic,
		GiantRing,
		GeyserMaker,
		LavaGeyser,
		LavaWall,
		Obj4F,
		Yadrin,		    // 0x50
		SmashBlock,
		MovingBlock,
		CollapseFloor,
		LavaTag,
		Basaran,
		FloatingBlock,
		SpikeBall,
		BigSpikeBall,	// 0x58
		Elevator,
		CirclingPlatform,
		Staircase,
		Pylon,
		Fan,
		Seesaw,
		Bomb,
		Orbinaut,		// 0x60
		LabyrinthBlock,
		Gargoyle,
		LabyrinthConvey,
		Bubble,
		Waterfall,
		Junction,
		RunningDisc,
		Conveyor,		// 0x68
		SpinPlatform,
		Saws,
		ScrapStomp,
		VanishPlatform,
		Flamethrower,
		Electro,
		SpinConvey,
		Girder,		    // 0x70
		Invisibarrier,
		Teleport,
		BossMarble,
		BossFire,
		BossSpringYard,
		BossBlock,
		BossLabyrinth,
		Caterkiller,	// 0x78
		Lamppost,
		BossStarLight,
		BossSpikeball,
		RingFlash,
		HiddenBonus,
		SSResult,
		SSRChaos,
		ContScrItem,	// 0x80
		ContSonic,
		ScrapEggman,
		FalseFloor,
		EggmanCylinder,
		BossFinal,
		BossPlasma,
		EndSonic,
		EndChaos,		// 0x88
		EndSTH,
		CreditsText,
		EndEggman,
		TryChaos
	};
};

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

#define VAR_B (self, name) ((self)->var_b(name))
#define VAR_UB(self, name) ((self)->var_ub(name))
#define VAR_W (self, name) ((self)->var_w(name))
#define VAR_UW(self, name) ((self)->var_uw(name))
#define VAR_L (self, name) ((self)->var_l(name))
#define VAR_UL(self, name) ((self)->var_ul(name))

static_assert(sizeof(Object) == 64);

// =====================================================================================================================
// Slot management
// =====================================================================================================================

int FindFreeObjSlot()
{
	for(int i = 0; i < NUM_LEVEL_OBJECTS; i++)
	{
		if(v_lvlobjspace[i].id == 0)
			return i + 32;
	}

	return -1;
}

int FindNextFreeObjSlot(Object* addr)
{
	if(addr == &v_objspace[MAX_OBJECTS - 1])
		return MAX_OBJECTS - 1;

	for(; addr < (v_objspace + MAX_OBJECTS); addr++)
	{
		if(addr->id == 0)
			return addr - v_objspace;
	}

	return -1;
}

// Original returns slot addr in a1, cs if failed
Object* FindFreeObj()
{
	auto slot = FindFreeObjSlot();

	if(slot == -1)
		return nullptr;
	else
		return &v_objspace[slot];
}

Object* FindNextFreeObj(Object* addr)
{
	auto slot = FindNextFreeObjSlot(addr);

	if(slot == -1)
		return nullptr;
	else
		return &v_objspace[slot];
}

void DeleteObject(Object* self)
{
	memset(self, 0, sizeof(Object));
}

int Object_PointerToSlot(Object* obj)
{
	return obj - v_objspace;
}

// =====================================================================================================================
// Scripts
// =====================================================================================================================

void ExecuteObjects()
{
	bool isDead = v_player->routine >= 6;
	auto objsToUpdate = isDead ? 32 : MAX_OBJECTS;

	for(int i = 0; i < objsToUpdate; i++)
	{
		auto id = v_objspace[i].id;

		if(id != 0)
			Obj_Index[id - 1](&v_objspace[i]);
	}

	if(isDead)
	{
		for(int i = 32; i < MAX_OBJECTS; i++)
		{
			if(v_objspace[i].id != 0 && BTST(v_objspace[i].render, ObjRender::Visible))
				DisplaySprite(&v_objspace[i]);
		}
	}
}

// =====================================================================================================================
// Sprites
// =====================================================================================================================

void DisplaySprite(Object* obj)
{
	auto queue = v_spritequeue[obj->priority];

	if((int)queue[0] < 63)
	{
		queue[0] = (Object*)((int)queue[0] + 1);
		queue[(int)queue[0]] = obj;
	}
}

//                          a0              a1
void AnimateSprite(Object* self, ushort* animScripts)
{
	auto anim = self->anim;

	if(self->anim != 0)
	{
		self->nextAni = anim;
		self->aniFrame = 0;
		self->timeFrame = 0;
	}

	if(!TimerNeg(self->timeFrame))
		return;

	// script[0] is frame duration for entire animation
	// script[1..n] is commands

	auto script = animScripts[anim];
	self->timeFrame = script[0];
	auto frame = self->aniFrame;
	auto command = script[frame + 1];

	switch(command)
	{
		case AnimFlags::Routine2:
			self->routine2 += 2;
			break;

		case AnimFlags::Reset:
			self->aniFrame = 0;
			self->routine2 = 0;
			break;

		case AnimFlags::Routine:
			self->routine += 2;
			break;

		case AnimFlags::Change:
			self->anim = script[frame + 2];
			break;

		case AnimFlags::Back:
			self->aniFrame -= script[frame + 2];
			frame -= script[frame + 2];
			command = script[frame + 1];
			goto Anim_Next;

		case AnimFlags::End:
			self->aniFrame = 0;
			command = script[1];
			goto Anim_Next;

		default:
			if(command & AnimFlags::Command) // ignore invalid commands
				break;
		Anim_Next:
			self->frame = command & 0x1F;
			// preeeetty sure this is doing something with flipping
			self->render = (self->render & 0xFC) | ((command >> 5 | ((command & 31) << 3)) ^ self->status) & 3;
			self->aniFrame++;
			break;
	}
}

// =====================================================================================================================
// Queries
// =====================================================================================================================

// output:
//d0 = flag set if object is off screen
bool ChkObjectVisible(Object* self)
{
	auto screenX = self->x - v_screenposx;

	if(screenX < 0 || screenX >= ScreenWidth)
		return true;

	auto screenY = self->y - v_screenposy;

	if(screenY < 0 || screenY >= ScreenHeight)
		return true;

	return false;
}

// Subroutine to check if an object is off screen
// More precise than above subroutine, taking width into account
// output:
// d0 = flag set if object is off screen

bool ChkPartiallyVisible(Object* self)
{
	auto screenX = self->x - v_screenposx;

	if(screenX + self->actWid < 0 || screenX - self->actWid > ScreenWidth)
		return true;

	auto screenY = self->y - v_screenposy;

	if(screenY < 0 || screenY >= ScreenHeight)
		return true;

	return false;
}

bool Object_IsVisible(Object* self)
{
	return BTST(self->render, ObjRender::Visible);
}

bool Object_IsFlipped(Object* self)
{
	return BTST(self->status, ObjStatus::Flip);
}

bool Object_OutOfRange(Object* self)
{
	return Object_OutOfRange(self, self->x);
}

bool Object_OutOfRange(Object* self, int xpos)
{
	return ((xpos & 0xFF80) - ((v_screenposx - 128) & 0xFF80)) > (128 + 320 + 192);
}

// =====================================================================================================================
// Collision
// =====================================================================================================================

// Subroutine to find the distance of an object to the floor
// input:
//	d3 = x-pos. of object (ObjFloorDist2 only)
// output:
//	d1 = distance to the floor
//	d3 = floor angle
//	a1 = address within 256x256 mappings where object is standing
//	     (refers to a 16x16 tile number)
//	(a4) = floor angle (a4 is always v_anglebuffer)

// a1                         a0

// a1                         a0             d1                d3
ushort* ObjFloorDist(Object* self, int* outDistance, int* outFloorAngle)
{
	return ObjFloorDist2(self, self->x, outDistance, outFloorAngle);
}

// a1                          a0        d3           d1                 d3
ushort* ObjFloorDist2(Object* self, int objX, int* outDistance, int* outFloorAngle)
{
	v_anglebuffer = 0;
	auto ret = FindFloor(self, self->y + self->height, objX, 0xD, 0, 0x10, outDistance, &v_anglebuffer);

	if(outFloorAngle)
		*outFloorAngle = v_anglebuffer & 1 ? 0 : v_anglebuffer;

	return ret;
}

// Returns distance to the ceiling.
// Original returns angle in d3, but nothing seems to use it.
//d1                      a0
int ObjHitCeiling(Object* self)
{
	int dist; // d1
	v_anglebuffer = 0;
	FindFloor(self, (self->y - self->height) ^ 15, self->x, 0xE, 0x1000, -16, &dist, &v_anglebuffer);
	return dist;

	// d3 = v_anglebuffer;
	// if(d3 & 1)
	// 	d3 = 0x80;
}

// Returns distance to the wall.
// Original returns angle in d3, but nothing seems to use it.
//d1                         a0        d3
int ObjHitWallRight(Object* self, int xdir)
{
	int dist;
	v_anglebuffer = 0;
	FindWall(self, self->y, self->x + xdir, 0xE, 0, 16, &dist, &v_anglebuffer);
	return dist;

	// d3 = v_anglebuffer;
	// if(d3 & 1)
	// 	d3 = -0x40;
}

// Returns distance to the wall.
// Original returns angle in d3, but nothing seems to use it.
//d1                        a0        d3
int ObjHitWallLeft(Object* self, int xdir)
{
	int dist;
	v_anglebuffer = 0;
	FindWall(self, self->y, self->x + xdir, 0xE, 0x800, -16, &dist, &v_anglebuffer);
	return dist;

	// d3 = v_anglebuffer;
	// if(d3 & 1)
	// 	d3 = 0x40;
}

// =====================================================================================================================
// Misc
// =====================================================================================================================

const short Smash_FragSpd1[] =
{
	0x400, -0x500,
	0x600, -0x100,
	0x600, 0x100,
	0x400, 0x500,
	0x600, -0x600,
	0x800, -0x200,
	0x800, 0x200,
	0x600, 0x600,
};

const short Smash_FragSpd2[] =
{
	-0x600, -0x600,
	-0x800, -0x200,
	-0x800, 0x200,
	-0x600, 0x600,
	-0x400, -0x500,
	-0x600, -0x100,
	-0x600, 0x100,
	-0x400, 0x500,
};

//                        a0           d4          d1                a4
void SmashObject(Object* self, ID newID, int numFrags, short* velocityArray)
{
	auto newMapping = self->map + self->frame + 1
	BSET(self->render, ObjRender::SmashFragment);
	self->id = newID;
	auto renderBits = self->render;
	auto newObj = self;

	for(auto newObj = self; newObj != nullptr && numFrags > -1; newObj = FindFreeObj(), newMapping += 5, numFrags--)
	{
		newObj->routine = 4;
		newObj->id = newID;
		newObj->map = newMapping;
		newObj->render = renderBits;

		newObj->x = self->x;
		newObj->y = self->y;
		newObj->gfx = self->gfx;
		newObj->priority = self->priority;
		newObj->actWid = self->actWid;
		newObj->velX = *velocityArray++;
		newObj->velY = *velocityArray++;

		if(newObj < self)
		{
			SpeedToPos(newObj)
			newObj->velY += newObj->x;
			DisplaySprite(newObj);
		}
	}

	PlaySound_Special(SFX::WallSmash);
}

// out: a2 = v_objstate
void RememberState(Object* self)
{
	auto approxDist = (self->x & 0xFF80) - ((v_screenposx - 128) & 0xFF80);

	if(approxDist > 128 + 320 + 192)
	{
		if(self->respawnNo != 0)
			BCLR(v_objstate[self->respawnNo + 2], 0x80);

		DeleteObject(self);
	}
	else
		DisplaySprite(self);
}

void SpeedToPos(Object* self)
{
	self->x += self->velX << 8;
	self->y += self->velY << 8;
}

void ObjectFall(Object* self)
{
	self->x += self->velX << 8;
	self->y += self->velY << 8;
	self->velY += 0x28;
}
