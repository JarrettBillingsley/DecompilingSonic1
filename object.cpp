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
	ID id;     // object id
	ubyte render; // bitfield for x/y flip, display mode
	ushort gfx;   // palette line & VRAM setting (2 bytes)
	uint map;     // mappings address (4 bytes)

	union
	{
		struct
		{
			ushort x16;     // 16-bit x-axis position
			ushort screenY; // y-axis position for screen-fixed items (2 bytes)
		};

		uint x; // 32-bit x-axis position
	};

	uint y;          // y-axis position
	ushort velX;     // x-axis velocity
	ushort velY;     // y-axis velocity (2 bytes)
	ushort inertia;  // potential speed (2 bytes)
	ubyte height;    // height/2
	ubyte width;     // width/2
	ubyte priority;  // sprite stack priority -- 0 is front
	ubyte actWid;    // action width
	ubyte frame;     // current frame displayed
	ubyte aniFrame;  // current frame in animation script
	ubyte anim;      // current animation
	ubyte nextAni;   // next animation
	ubyte timeFrame; // time to next frame
	ubyte delayAni;  // time to delay animation
	ubyte colType;   // collision response type
	ubyte colProp;   // collision extra property
	ubyte status;    // orientation or mode
	ubyte respawnNo; // respawn list index number
	ubyte routine;   // routine number

	union
	{
		ubyte routine2; // secondary routine number
		ubyte solid;    // solid status flag
	};

	ushort angle;  // angle
	ubyte subtype; // object subtype

	ubyte variables[23]; // per-object variables (start at 0x29)

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

void ExecuteObjects()
{
	bool isDead = v_player.routine >= 6;
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

void DisplaySprite(Object* obj)
{
	auto queue = v_spritequeue[obj->priority];

	if((int)queue[0] < 63)
	{
		queue[0] = (Object*)((int)queue[0] + 1);
		queue[(int)queue[0]] = obj;
	}
}

int FindFreeObjSlot()
{
	for(int i = 0; i < 96; i++)
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

int Object_PointerToSlot(Object* obj)
{
	return obj - v_objspace;
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

// output:
// d0 = flag set if object is off screen

bool ChkObjectVisible(Object* self)
{
	auto screenX = self->x - v_screenposx;

	if(screenX < 0 || screenX >= 320)
		return true;

	auto screenY = self->y - v_screenposy;

	if(screenY < 0 || screenY >= 224)
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

	if(screenX + self->actWid < 0 || screenX - self->actWid > 320)
		return true;

	auto screenY = self->y - v_screenposy;

	if(screenY < 0 || screenY >= 224)
		return true;

	return false;
}

void DeleteObject(Object* self)
{
	memset(self, 0, sizeof(Object));
}

// input:
//	a0 = object to find floor of
//	a3 = direction/distance to look (abusing address regs much?)
//	d2 = y-position of object's bottom edge
//	d3 = x-position of object
//	d5 = bit to test for solidness
//  d6 = 0 if floor, 0x1000 if wall/ceiling?
// output:
//	d1 = distance to the floor
//	a1 = address within 256x256 mappings where object is standing
//	     (refers to a 16x16 tile number)
//	(a4) = floor angle

// a1                      a0           d2          d3          d5              d6             a3               d1                 a4
ushort* FindFloor(Object* self, int objBottom, int objX, int solidityBit, int wallFlag, int direction, int* outDistance, ubyte* outFloorAngle)
{
	auto tileAddr = FindNearestTile(self, objBottom, objX);
	auto tile = *tileAddr;
	auto tileFlags = tile;
	tile &= 0x7FF;

	if(tile == 0 || !BTST(tileFlags, 1 << solidityBit))
	{
	_isBlank:
		int dist;
		tileAddr = FindFloor2(self, objBottom + direction, objX, solidityBit, wallFlag, &dist, outFloorAngle);

		if(outDistance)
			*outDistance = dist + 16;

		return tileAddr;
	}

	auto collIndex = v_collindex[tile] & 0xFF;

	if(collIndex == 0)
		goto _isBlank;

	*outFloorAngle = AngleMap[collIndex];

	auto xpos = objX;

	if(BTST(tileFlags, 0x800))
	{
		xpos = ~xpos;
		*outFloorAngle = -*outFloorAngle;
	}

	if(BTST(tileFlags, 0x1000))
		*outFloorAngle = -(*outFloorAngle + 0x40) - 0x40;

	short height = CollArray1[(xpos & 0xF) + (collIndex << 4)];

	tileFlags ^= wallFlag;

	if(BTST(tileFlags, 0x1000))
		height = -height;

	if(height == 0)
		goto _isBlank;
	else if(height < 0)
	{
		height += objBottom & 0xF;

		if(height >= 0)
			goto _isBlank;
	}
	else if(height != 16)
	{
		if(outDistance)
			*outDistance = 15 - (height + (objBottom & 0xF));

		return tileAddr;
	}

	int dist;
	tileAddr = FindFloor2(self, objBottom - direction, objX, solidityBit, wallFlag, &dist, outFloorAngle);

	if(outDistance)
		*outDistance = dist - 16;

	return tileAddr;
}

// a1                       a0         d2             d3         d5                d6              d1                 a4
ushort* FindFloor2(Object* self, int objBottom, int objX, int solidityBit, int wallFlag, int* outDistance, ubyte* outFloorAngle)
{
	auto tileAddr = FindNearestTile(self, objBottom, objX);
	auto tile = *tileAddr;
	auto tileFlags = tile;

	if(tile == 0 || !(tileFlags & (1 << solidityBit)))
	{
	_isBlank:
		if(outDistance)
			*outDistance = 15 - (objBottom & 0xF);

		return tileAddr;
	}

	auto collIndex = v_collindex[tile] & 0xFF;

	if(collIndex == 0)
		goto _isBlank;

	*outFloorAngle = AngleMap[collIndex];

	auto xpos = objX;

	if(BTST(tileFlags, 0x800))
	{
		xpos = ~xpos
		*outFloorAngle = -*outFloorAngle;
	}

	if(BTST(tileFlags, 0x1000))
		*outFloorAngle = -(*outFloorAngle + 0x40) - 0x40;

	short height = CollArray1[(xpos & 0xF) + (collIndex << 4)];

	tileFlags ^= wallFlag;

	if(BTST(tileFlags, 0x1000))
		height = -height;

	if(height == 0)
		goto _isBlank;

	if(outDistance)
	{
		if(height >= 0)
			*outDistance = 0xF - (height + (objBottom & 0xF))
		else
		{
			*outDistance = objBottom & 0xF;
			height += *outDistance;

			if(height >= 0)
				goto _isBlank;

			*outDistance = ~*outDistance;
		}
	}

	return tileAddr;
}

//a1                      a0         d2             d3         d5                d6            a3             d1                 a4
ushort* FindWall(Object* self, int objBottom, int objX, int solidityBit, int wallFlag, int direction, int* outDistance, ubyte* outFloorAngle)
{
	auto tileAddr = FindNearestTile(self, objBottom, objX);
	auto tile = *tileAddr;
	auto tileFlags = tile;
	tile &= 0x7FF;

	if(tile == 0 || !BTST(tileFlags, 1 << solidityBit))
	{
	_isBlank:
		int dist;
		tileAddr = FindWall2(self, objBottom, objX + direction, solidityBit, wallFlag, &dist, outFloorAngle);

		if(outDistance)
			*outDistance = dist + 16;

		return tileAddr;
	}

	auto collIndex = v_collindex[tile] & 0xFF;

	if(collIndex == 0)
		goto _isBlank;

	*outFloorAngle = AngleMap[collIndex];

	auto ypos = objBottom;

	if(BTST(tileFlags, 0x1000))
	{
		ypos = ~ypos
		*outFloorAngle = -(*outFloorAngle + 0x40) - 0x40;
	}

	if(BTST(tileFlags, 0x800))
		*outFloorAngle = -*outFloorAngle;

	short height = CollArray2[(ypos & 0xF) + (collIndex << 4)]

	tileFlags ^= wallFlag;

	if(BTST(tileFlags, 0x800))
		height = -height

	if(height == 0)
		goto _isBlank;
	else if(height < 0)
	{
		height += objX & 0xF;

		if(height >= 0)
			goto _isBlank;
	}
	else if(height != 16)
	{
		if(outDistance)
			*outDistance = 15 - (height + (objX & 0xF));

		return tileAddr;
	}

	int dist;
	tileAddr = FindWall2(self, objBottom, objX - direction, solidityBit, wallFlag, &dist, outFloorAngle);

	if(outDistance)
		*outDistance = dist - 16;

	return tileAddr;

}

// a1                     a0         d2             d3         d5                d6              d1                 a4
ushort* FindWall2(Object* self, int objBottom, int objX, int solidityBit, int wallFlag, int* outDistance, ubyte* outFloorAngle)
{
	auto tileAddr = FindNearestTile(self, objBottom, objX);
	auto tile = *tileAddr;
	auto tileFlags = tile;

	if(tile == 0 || !(tileFlags & (1 << solidityBit)))
	{
	_isBlank:
		if(outDistance)
			*outDistance = 0xF - (objX & 0xF);

		return tileAddr;
	}

	auto collIndex = v_collindex[tile] & 0xFF;

	if(collIndex == 0)
		goto _isBlank;

	*outFloorAngle = AngleMap[collIndex];

	auto ypos = objBottom;

	if(BTST(tileFlags, 0x1000))
	{
		ypos = ~ypos
		*outFloorAngle = -(*outFloorAngle + 0x40) - 0x40;
	}

	if(BTST(tileFlags, 0x800))
		*outFloorAngle = -*outFloorAngle;

	short height = CollArray2[ypos & 0xF + collIndex << 4];

	tileFlags ^= wallFlag;

	if(BTST(tileFlags, 0x800))
		height = -height;

	if(height == 0)
		goto _isBlank;

	if(outDistance)
	{
		if(height >= 0)
			*outDistance = 0xF - (height + (objX & 0xF))
		else
		{
			*outDistance = objX & 0xF;
			height += *outDistance;

			if(height >= 0)
				goto _isBlank;

			*outDistance = ~*outDistance;
		}
	}

	return tileAddr;
}

// Subroutine to find which tile the object is standing on
// input:
//	d2 = y-position of object's bottom edge
//	d3 = x-position of object
// output:
//	a1 = address within 256x256 mappings where object is standing
//	     (refers to a 16x16 tile number)

ushort* FindNearestTile(Object* self, int objBottom, int objX)
{
	auto addr = v_lvllayout[(objBottom >> 1) & 0x380 + (objX >> 8) & 0x7F];

	if(addr == 0)
		return 0xFFFFFF00 | addr;

	if(addr & 0x80) // special tile?
	{
		addr &= 0x7F;

		if(BTST(self->render, ObjRender::Behind))
		{
			addr++;

			if(addr == 0x29)
				addr = 0x51;
		}

		addr--;
	}
	else
	{
		addr--;

		// sign ext lower byte
		if(addr & 0x80)
			addr |= 0xFF00;
	}

	addr = ((addr >> 7) & 0x1FF) | ((addr << 9) & 0xFE00); // ror 7
	addr += ((objBottom * 2) & 0x1E0) + ((objX >> 3) & 0x1E);
	return 0xFFFF0000 | addr;
}

void ObjectFall(Object* self)
{
	self->x += self->velX << 8;
	self->y += self->velY << 8;
	self->velY += 0x28;
}

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

	while(true)
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

		if(TimerNeg(numFrags))
			break;

		newObj = FindFreeObj();

		if(newObj == nullptr)
			break;

		newMapping += 5;
	}

	PlaySound_Special(SFX::WallSmash);
}

void SpeedToPos(Object* self)
{
	self->x += self->velX << 8;
	self->y += self->velY << 8;
}

// Solid	object subroutine (includes spikes, blocks, rocks etc)
// input:
//	d1 = width
//	d2 = height / 2 (when jumping)
//	d3 = height / 2 (when walking) (is this used???? call sites set it to something but it's smashed immediately)
//	d4 = x-axis position
// output:
//	d4 = 0 for no collision, 1 for side collision, -1 for top/bottom collision

//d4                     a0         d1         d2          d3         d4
int SolidObject(Object* self, int width, int jumpHH, int walkHH, int objX)
{
	if(!self->solid)
		return Solid_ChkEnter(self, width, jumpHH, walkHH, objX);

	if(!BTST(v_player->status, ObjStatus::Air))
	{
		auto dist = v_player->x - self->x + width;

		if(dist >= 0 && dist < width * 2)
		{
			MvSonicOnPtfm(objX);
			return 0;
		}
	}

	// clear standing flags
	BCLR(v_player->status, ObjStatus::StandingOn);
	BCLR(self->status, ObjStatus::StandingOn);
	self->solid = 0;
	return 0;
}

//d4                       a0         d1        d4
int SolidObject71(Object* self, int width, int objX)
{
	if(!self->solid)
		return loc_FAD0(self);

	if(!BTST(v_player->status, ObjStatus::Air))
	{
		auto dist = v_player->x - self->x + width;

		if(dist >= 0 && dist < width * 2)
		{
			MvSonicOnPtfm(objX);
			return 0;
		}
	}

	// clear standing flags
	BCLR(v_player->status, ObjStatus::StandingOn);
	BCLR(self->status, ObjStatus::StandingOn);
	self->solid = 0;
	return 0;
}

//d4                       a0         d1          d2            a2
int SolidObject2F(Object* self, int width, int jumpHH, byte* heightArray)
{
	if(!self->render)
		return Solid_Ignore();

	auto dist = v_player->x - self->x + width

	if(dist < 0 || dist > width * 2)
		return Solid_Ignore();

	// flipped horiz?
	if(BTST(self->render, ObjRender::HorizFlip))
		dist = ~dist + width * 2;

	auto diff = self->y - heightArray[dist >> 1] - heightArray[0]
	jumpHH += v_player->height;
	auto distY = v_player->y - diff + 4 + jumpHH

	if(distY < 0 || distY >= jumpHH * 2)
		return Solid_Ignore();
	else
		return Solid_ChkEnter();
}

//d4                        a0         d1          d2
int Solid_ChkEnter(Object* self, int width, int jumpHH)
{
	if(BTST(self->render, ObjRender::Visible))
		return loc_FAD0(self, width, jumpHH)
	else
		return Solid_Ignore(self);
}

// in: d1, d2, a0
// smash: d0, d3, d4, d5, a1
// out: d4

//d4                  a0        d1           d2
int loc_FAD0(Object* self, int width, int jumpHH)
{
	auto distX = v_player->x - self->x + width;

	if(distX < 0 || distX > width * 2)
		return Solid_Ignore(self);

	jumpHH += v_player->height;
	auto distY = v_player->y - self->y + 4 + jumpHH;

	if(distY < 0 || distY >= jumpHH * 2 || f_lockmulti & 0x80)
		return Solid_Ignore(self);

	if(v_player->routine >= 6 || v_debuguse)
		return 0;

	auto realDistX = distX

	if(distX > width) // left of center?
	{
		distX -= width * 2;
		realDistX = -distX;
	}

	auto realDistY = distY

	if(distY > jumpHH) // below center?
	{
		distY = distY - 4 - jumpHH * 2;
		realDistY = -distY;
	}

	if(realDistX > realDistY)
		return Solid_TopBottom(self, distY);

	if(realDistY >= 4)
	{
		// Bonking against the object?
		if(distX != 0 && ((distX > 0 && v_player->velX >= 0) || (distX < 0 && v->player->velX < 0)))
		{
			v_player->inertia = 0;
			v_player->velX = 0;
		}

		v_player->x -= distX; // push out of object

		if(BTST(v_player->status, ObjStatus::Air)) // in air?
		{
			BCLR(self->status, ObjStatus::Pushing);
			BCLR(v_player->status, ObjStatus::Pushing);
		}
		else
		{
			BSET(v_player->status, ObjStatus::Pushing);
			BSET(self->status, ObjStatus::Pushing);
		}
	}
	else
	{
		BCLR(self->status, ObjStatus::Pushing);
		BCLR(v_player->status, ObjStatus::Pushing);
	}

	return 1;
}

//d4                      a0
int Solid_Ignore(Object* self)
{
	if(self->status & ObjStatus::Pushing) // being pushed?
	{
		v_player->anim = SonicAnim::Run;
		BCLR(self->status, ObjStatus::Pushing);
		BCLR(v_player->status, ObjStatus::Pushing);
	}

	return 0;
}

// smash: d1, d2
//d4                          a0        d3
void Solid_TopBottom(Object* self, int distY)
{
	if(distY < 0)
	{
		if(v_player->velY == 0 && !BTST(v_player->status, ObjStatus::Air))
			KillSonic(v_player); // squisho
		else if(v_player->velY < 0 && distY <= 0) // moving down and sonic is above?
		{
			v_player->y -= distY; // push out of object
			v_player->velY = 0;
		}

		return -1; // top/bottom
	}
	else if(distY < 16)
	{
		distY -= 4;

		d1 = self->actWid
		d2 = d1 * 2
		d1 = d1 + v_player->x - self->x

		if(d1 >= 0 && d1 < d2 && v_player->velY >= 0)
		{
			v_player->y -= (distY + 1); // push out of object
			Solid_ResetFloor(self);
			self->solid = 2;
			BSET(self->status, ObjStatus::StandingOn); // stand on it!
			return -1; // top/bottom
		}

		return 0;
	}
	else
		return Solid_Ignore(self);
}

//                             a0
void Solid_ResetFloor(Object* self)
{
	if(BTST(v_player->status, ObjStatus::StandingOn)) // sonic standing on something?
	{
		auto stoodOn = v_objspace[VAR_B(v_player, 0x3D)];
		BCLR(stoodOn->status, ObjStatus::StandingOn);
		stoodOn->solid = 0;
	}

	VAR_B(v_player, 0x3D) = self - v_objspace;
	v_player->angle = 0;
	v_player->velY = 0;
	v_player->inertia = v_player->velX;

	if(BTST(v_player->status, ObjStatus::Air)) // in air?
		Sonic_ResetOnFloor(v_player);

	BSET(v_player->status, ObjStatus::StandingOn);
	BSET(self->status, ObjStatus::StandingOn);
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

//                           a0           d1
void PlatformObject(Object* self, int halfWidth)
{
	if(v_player->velY < 0) // moving up? don't curr
		return;

	// perform x-axis range check
	auto diffX = v_player->x - (self->x - halfWidth);

	if(diffX >= 0 && diffX < (2 * halfWidth));
		Plat_NoXCheck(self);
}

void Plat_NoXCheck(Object* self)
{
	auto diffY = self->y - 8

// Platform3:
	// perform y-axis range check
	diffY -= v_player->height + 4

	if(diffY > 0 || diffY < -16 || f_lockmulti & 0x80 || Player_IsDead())
		return;

	v_player->y = v_player->y + v_player->height + diffY + 3;
	self->routine += 2;

// loc_74AE: (called by monitor)
	if(BTST(v_player->status, ObjStatus::StandingOn))
	{
		auto obj = &v_objspace[VAR_B(v_player, Player_StandingObjectB)];
		BCLR(obj->status, ObjStatus::StandingOn);
		obj->routine2 = 0;

		if(obj->routine == 4)
			obj->routine = 2;
	}

	VAR_B(v_player, Player_StandingObjectB) = Object_PointerToSlot(self);
	v_player->angle = 0;
	v_player->velY = 0;
	v_player->inertia = v_player->velX;

	if(PlayerInAir())
		Sonic_ResetOnFloor(v_player);

	BSET(v_player->status, ObjStatus::StandingOn);
	BSET(self->status, ObjStatus::StandingOn);
}

// Returns true when player leaves platform (cs in original)
//c                        a0             d1            d0
bool ExitPlatform(Object* self, int centerOffset, int& diffX)
{
	return ExitPlatform(self, centerOffset, centerOffset, diffX);
}

//c                        a0            d1              d2             d0
bool ExitPlatform(Object* self, int leftOffset, int halfWidth, int& diffX)
{
	if(!PlayerInAir())
	{
		auto diffX = v_player->x - (self->x - leftOffset);

		if(diffX >= 0 && diffX < (halfWidth * 2))
			return false;
	}

	BCLR(v_player->status, ObjStatus::StandingOn);
	self->routine = 2;
	BCLR(self->status, ObjStatus::StandingOn);
	return true;
}