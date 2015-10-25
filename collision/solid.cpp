// Solid	object subroutine (includes spikes, blocks, rocks etc)
// input:
//	d1 = width
//	d2 = height / 2 (when jumping)
//	d4 = x-axis position
// output:
//	d4 = 0 for no collision, 1 for side collision, -1 for top/bottom collision

//d4                     a0         d1         d2          d3         d4
int SolidObject(Object* self, int width, int jumpHH, int walkHH, int objX)
{
	if(!self->solid)
		return Solid_ChkEnter(self, width, jumpHH);

	if(!BTST(v_player->status, ObjStatus_Air))
	{
		auto dist = v_player->x - self->x + width;

		if(dist >= 0 && dist < width * 2)
		{
			MvSonicOnPtfm(objX);
			return 0;
		}
	}

	// clear standing flags
	BCLR(v_player->status, ObjStatus_StandingOn);
	BCLR(self->status, ObjStatus_StandingOn);
	self->solid = 0;
	return 0;
}

//d4                       a0         d1        d4
int SolidObject71(Object* self, int width, int objX)
{
	if(!self->solid)
		return Solid_Common(self);

	if(!BTST(v_player->status, ObjStatus_Air))
	{
		auto dist = v_player->x - self->x + width;

		if(dist >= 0 && dist < width * 2)
		{
			MvSonicOnPtfm(objX);
			return 0;
		}
	}

	// clear standing flags
	BCLR(v_player->status, ObjStatus_StandingOn);
	BCLR(self->status, ObjStatus_StandingOn);
	self->solid = 0;
	return 0;
}

//d4                       a0         d1          d2            a2
int SolidObject2F(Object* self, int width, int jumpHH, byte* heightArray)
{
	if(!self->render)
		return Solid_Ignore(self);

	auto dist = v_player->x - self->x + width

	if(dist < 0 || dist > width * 2)
		return Solid_Ignore(self);

	// flipped horiz?
	if(BTST(self->render, ObjRender_HorizFlip))
		dist = ~dist + width * 2;

	auto diff = self->y - heightArray[dist >> 1] - heightArray[0]
	jumpHH += v_player->height;
	auto distY = v_player->y - diff + 4 + jumpHH

	if(distY < 0 || distY >= jumpHH * 2)
		return Solid_Ignore(self);
	else
		return Solid_ChkEnter(self, width, jumpHH);
}

//d4                        a0         d1          d2
int Solid_ChkEnter(Object* self, int width, int jumpHH)
{
	if(BTST(self->render, ObjRender_Visible))
		return Solid_Common(self, width, jumpHH)
	else
		return Solid_Ignore(self);
}

//d4                      a0        d1           d2
int Solid_Common(Object* self, int width, int jumpHH)
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

		if(BTST(v_player->status, ObjStatus_Air)) // in air?
		{
			BCLR(self->status, ObjStatus_Pushing);
			BCLR(v_player->status, ObjStatus_Pushing);
		}
		else
		{
			BSET(v_player->status, ObjStatus_Pushing);
			BSET(self->status, ObjStatus_Pushing);
		}
	}
	else
	{
		BCLR(self->status, ObjStatus_Pushing);
		BCLR(v_player->status, ObjStatus_Pushing);
	}

	return 1;
}

//d4                      a0
int Solid_Ignore(Object* self)
{
	if(self->status & ObjStatus_Pushing) // being pushed?
	{
		v_player->anim = SonicAnim::Run;
		BCLR(self->status, ObjStatus_Pushing);
		BCLR(v_player->status, ObjStatus_Pushing);
	}

	return 0;
}

//d4                          a0        d3
void Solid_TopBottom(Object* self, int distY)
{
	if(distY < 0)
	{
		if(v_player->velY == 0 && !BTST(v_player->status, ObjStatus_Air))
			Player_Kill(v_player); // squisho
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
			BSET(self->status, ObjStatus_StandingOn); // stand on it!
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
	if(BTST(v_player->status, ObjStatus_StandingOn)) // sonic standing on something?
	{
		auto stoodOn = v_objspace[VAR_B(v_player, 0x3D)];
		BCLR(stoodOn->status, ObjStatus_StandingOn);
		stoodOn->solid = 0;
	}

	VAR_B(v_player, 0x3D) = self - v_objspace;
	v_player->angle = 0;
	v_player->velY = 0;
	v_player->inertia = v_player->velX;

	if(BTST(v_player->status, ObjStatus_Air)) // in air?
		Sonic_ResetOnFloor(v_player);

	BSET(v_player->status, ObjStatus_StandingOn);
	BSET(self->status, ObjStatus_StandingOn);
}