// =====================================================================================================================
// Flags
// =====================================================================================================================

#define SET_FLAG(VAR, FLAG, VALUE)\
	(VAR) = ((VAR) & ~(FLAG)) | ((VALUE) ? (FLAG) : 0)

bool Obj_IsFlipped   (Object* self) { return BTST(self->status, ObjStatus_Flip);       }
bool Obj_IsInAir     (Object* self) { return BTST(self->status, ObjStatus_Air);        }
bool Obj_IsRolling   (Object* self) { return BTST(self->status, ObjStatus_Rolling);    }
bool Obj_IsStanding  (Object* self) { return BTST(self->status, ObjStatus_StandingOn); }
bool Obj_IsJumping   (Object* self) { return BTST(self->status, ObjStatus_Jumping);    }
bool Obj_IsPushing   (Object* self) { return BTST(self->status, ObjStatus_Pushing);    }
bool Obj_IsUnderwater(Object* self) { return BTST(self->status, ObjStatus_Underwater); }
bool Obj_IsDead      (Object* self) { return BTST(self->status, ObjStatus_Dead);       }

void Obj_SetFlipped   (Object* self, bool val = true) { SET_FLAG(self->status, ObjStatus_Flip,       val); }
void Obj_SetInAir     (Object* self, bool val = true) { SET_FLAG(self->status, ObjStatus_Air,        val); }
void Obj_SetRolling   (Object* self, bool val = true) { SET_FLAG(self->status, ObjStatus_Rolling,    val); }
void Obj_SetStanding  (Object* self, bool val = true) { SET_FLAG(self->status, ObjStatus_StandingOn, val); }
void Obj_SetJumping   (Object* self, bool val = true) { SET_FLAG(self->status, ObjStatus_Jumping,    val); }
void Obj_SetPushing   (Object* self, bool val = true) { SET_FLAG(self->status, ObjStatus_Pushing,    val); }
void Obj_SetUnderwater(Object* self, bool val = true) { SET_FLAG(self->status, ObjStatus_Underwater, val); }
void Obj_SetDead      (Object* self, bool val = true) { SET_FLAG(self->status, ObjStatus_Dead,       val); }

void Obj_SetNotFlipped   (Object* self) { Obj_SetFlipped   (self, false); }
void Obj_SetNotInAir     (Object* self) { Obj_SetInAir     (self, false); }
void Obj_SetNotRolling   (Object* self) { Obj_SetRolling   (self, false); }
void Obj_SetNotStanding  (Object* self) { Obj_SetStanding  (self, false); }
void Obj_SetNotJumping   (Object* self) { Obj_SetJumping   (self, false); }
void Obj_SetNotPushing   (Object* self) { Obj_SetPushing   (self, false); }
void Obj_SetNotUnderwater(Object* self) { Obj_SetUnderwater(self, false); }
void Obj_SetNotDead      (Object* self) { Obj_SetDead      (self, false); }

bool Obj_IsHorizFlip    (Object* self) { return BTST(obj->render, ObjRender_HorizFlip);     }
bool Obj_IsVertFlip     (Object* self) { return BTST(obj->render, ObjRender_VertFlip);      }
bool Obj_IsLayerScreen  (Object* self) { return Obj_Layer(self) == ObjRender_LayerScreen;   }
bool Obj_IsLayerNormal  (Object* self) { return Obj_Layer(self) == ObjRender_LayerNormal;   }
bool Obj_IsLayerBG1     (Object* self) { return Obj_Layer(self) == ObjRender_LayerBG1;      }
bool Obj_IsLayerBG2     (Object* self) { return Obj_Layer(self) == ObjRender_LayerBG2;      }
bool Obj_IsSomething    (Object* self) { return BTST(obj->render, ObjRender_Something);     }
bool Obj_IsSmashFragment(Object* self) { return BTST(obj->render, ObjRender_SmashFragment); }
bool Obj_IsBehind       (Object* self) { return BTST(obj->render, ObjRender_Behind);        }
bool Obj_IsVisible      (Object* self) { return BTST(obj->render, ObjRender_Visible);       }

int Obj_Layer(Object* self) { return obj->render & ObjRender_Layer; }

void Obj_SetHorizFlip    (Object* self, bool val = true) { SET_FLAG(self->render, ObjRender_HorizFlip,     val); }
void Obj_SetVertFlip     (Object* self, bool val = true) { SET_FLAG(self->render, ObjRender_VertFlip,      val); }
void Obj_SetLayerScreen  (Object* self, bool val = true) { Obj_SetLayer(self, ObjRender_LayerScreen);            }
void Obj_SetLayerNormal  (Object* self, bool val = true) { Obj_SetLayer(self, ObjRender_LayerNormal);            }
void Obj_SetLayerBG1     (Object* self, bool val = true) { Obj_SetLayer(self, ObjRender_LayerBG1);               }
void Obj_SetLayerBG2     (Object* self, bool val = true) { Obj_SetLayer(self, ObjRender_LayerBG2);               }
void Obj_SetSomething    (Object* self, bool val = true) { SET_FLAG(self->render, ObjRender_Something,     val); }
void Obj_SetSmashFragment(Object* self, bool val = true) { SET_FLAG(self->render, ObjRender_SmashFragment, val); }
void Obj_SetBehind       (Object* self, bool val = true) { SET_FLAG(self->render, ObjRender_Behind,        val); }
void Obj_SetVisible      (Object* self, bool val = true) { SET_FLAG(self->render, ObjRender_Visible,       val); }

void Obj_SetLayer(Object* self, int val) { obj->render = (obj->render & ~ObjRender_Layer) | (val & ObjRender_Layer); };

void Obj_SetNotHorizFlip    (Object* self) { Obj_SetHorizFlip    (self, false)); }
void Obj_SetNotVertFlip     (Object* self) { Obj_SetVertFlip     (self, false)); }
void Obj_SetNotSomething    (Object* self) { Obj_SetSomething    (self, false)); }
void Obj_SetNotSmashFragment(Object* self) { Obj_SetSmashFragment(self, false)); }
void Obj_SetNotBehind       (Object* self) { Obj_SetBehind       (self, false)); }
void Obj_SetNotVisible      (Object* self) { Obj_SetVisible      (self, false)); }

#undef SET_FLAG

// =====================================================================================================================
// Slot management
// =====================================================================================================================

int FindFreeObjSlot()
{
	for(int i = 0; i < NumLevelObjects; i++)
	{
		if(v_lvlobjspace[i].id == 0)
			return i + 32;
	}

	return -1;
}

int FindNextFreeObjSlot(Object* addr)
{
	if(addr == &v_objspace[MaxObjects - 1])
		return MaxObjects - 1;

	for(; addr < (v_objspace + MaxObjects); addr++)
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
	auto objsToUpdate = isDead ? 32 : MaxObjects;

	for(int i = 0; i < objsToUpdate; i++)
	{
		auto id = v_objspace[i].id;

		if(id != 0)
			Obj_Index[id - 1](&v_objspace[i]);
	}

	if(isDead)
	{
		for(int i = 32; i < MaxObjects; i++)
		{
			if(v_objspace[i].id != 0 && Obj_IsVisible(&v_objspace[i]))
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
		case AnimFlags_Routine2:
			self->routine2 += 2;
			break;

		case AnimFlags_Reset:
			self->aniFrame = 0;
			self->routine2 = 0;
			break;

		case AnimFlags_Routine:
			self->routine += 2;
			break;

		case AnimFlags_Change:
			self->anim = script[frame + 2];
			break;

		case AnimFlags_Back:
			self->aniFrame -= script[frame + 2];
			frame -= script[frame + 2];
			command = script[frame + 1];
			goto Anim_Next;

		case AnimFlags_End:
			self->aniFrame = 0;
			command = script[1];
			goto Anim_Next;

		default:
			if(command & AnimFlags_Command) // ignore invalid commands
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
	Obj_SetSmashFragment(self);
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

	PlaySound_Special(SFX_WallSmash);
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
