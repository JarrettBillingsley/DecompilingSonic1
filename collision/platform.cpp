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
	Plat_NoXCheck2(self, self->y - 8);
}

// aka Platform3
//                           a0         d0
void Plat_NoXCheck2(Object* self, int platTop)
{
	// perform y-axis range check
	auto diffY = platTop - v_player->height + 4;

	if(diffY > 0 || diffY < -16 || f_lockmulti & 0x80 || Player_IsDead())
		return;

	v_player->y = v_player->y + v_player->height + diffY + 3;
	self->routine += 2;

// loc_74AE: (called by monitor)
	if(Player_IsStanding())
	{
		auto obj = &v_objspace[VAR_B(v_player, Player_StandingObjectB)];
		Obj_SetNotStanding(obj);
		obj->routine2 = 0;

		if(obj->routine == 4)
			obj->routine = 2;
	}

	VAR_B(v_player, Player_StandingObjectB) = Object_PointerToSlot(self);
	v_player->angle = 0;
	v_player->velY = 0;
	v_player->inertia = v_player->velX;

	if(Player_IsInAir())
		Player_ResetOnFloor(v_player);

	Player_SetStanding();
	Obj_SetStanding(self);
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
	if(!Player_IsInAir())
	{
		auto diffX = v_player->x - (self->x - leftOffset);

		if(diffX >= 0 && diffX < (halfWidth * 2))
			return false;
	}

	Player_SetNotStanding();
	self->routine = 2;
	Obj_SetNotStanding(self);
	return true;
}

//                          a0        d2          d3
void MvSonicOnPtfm(Object* self, int prevX, int height)
{
	MvSonicOnPtfmCommon(self, self->y - height, prevX);
}

//                           a0         d2
void MvSonicOnPtfm2(Object* self, int prevX)
{
	MvSonicOnPtfmCommon(self, self->y - 9, prevX);
}

//                                a0        d0       d2
void MvSonicOnPtfmCommon(Object* self, int top, int prevX)
{
	if(!f_lockmulti && !Player_IsDead() && !v_debuguse)
	{
		v_player->y = top - v_player->height;
		v_player->x -= prevX - self->x;
	}
}

// Sloped platform subroutine (GHZ collapsing ledges and SLZ seesaws)
//                        a0            d1                a2
void SlopeObject(Object* self, int halfWidth, ushort* heightTable)
{
	if(v_player->velY >= 0)
	{
		auto diffX = v_player->x - (self->x - halfWidth);

		if(diffX < 0 || diffX >= (halfWidth * 2))
			return;

		if(Obj_IsHorizFlip(self))
			diffX = (halfWidth * 2) - diffX - 1);

		Plat_NoXCheck2(self, self->y - heightTable[diffX]);
	}
}

// Sloped platform subroutine (GHZ collapsing ledges and MZ platforms)
//                         a0           d1           d2              a2
void SlopeObject2(Object* self, int halfwidth, int prevX, ubyte* heightTable)
{
	if(Player_IsStanding())
	{
		auto diffX = (v_player->x - (self->x - halfWidth)) / 2;

		if(Obj_IsHorizFlip(self))
			diffX = halfWidth - diffX - 1;

		v_player->y = self->y - heightTable[diffX] - v_player->height
		v_player->x -= prevX - self->x;
	}
}