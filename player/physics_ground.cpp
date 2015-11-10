// This should be called Sonic_ChkRoll..
void Sonic_Roll(Object* self)
{
	// Pressing ONLY down and not L/R?
	if(!f_jumponly && abs(self->inertia) >= 0x80 && (v_jpadhold2 & (Button_L | Button_R | Button_Down)) == Button_Down)
		Sonic_ChkRoll(self);
}

// This should be called Sonic_Roll..
void Sonic_ChkRoll(Object* self)
{
	if(Player_IsRolling())
		return;

	Player_SetRolling();
	self->height = 14;
	self->width = 7;
	self->anim = PlayerAnim_Roll;
	self->y += 5;
	PlaySound_Special(SFX_Roll);

	if(self->inertia == 0)
		self->inertia = 0x200;
}

void Sonic_SlopeResist(Object* self)
{
	if((self->angle + 0x60) < 0xC0)
	{
		auto slopeVel = CalcSine(self->angle) * 0x20 / 256;

		if(self->inertia != 0)
			self->inertia += slopeVel;
	}
}

void Sonic_Move(Object* self)
{
	if(!f_jumponly)
	{
		bool isLooking = false;

		if(VAR_W(self, Player_SomeTimerW) == 0)
		{
			if(v_jpadhold2 & Buttons_L)
				Sonic_MoveLeft(self);

			if(v_jpadhold2 & Buttons_R)
				Sonic_MoveRight(self);

			if(((self->angle + 0x20) & 0xC0) == 0 && self->inertia == 0)
			{
				Player_SetNotPushing();
				self->anim = PlayerAnim->Wait;

				if(!Sonic_CheckBalancing(self))
					isLooking = Sonic_CheckLooking(self);
			}
		}

		if(!isLooking)
			Sonic_ResetLookShift(self);

		Sonic_SlowDown(self);
	}

	int cosine;
	auto sine = CalcSine(self->angle, &cosine);
	self->velX = (self->inertia >> 8) * cosine;
	self->velY = (self->inertia >> 8) * sine;
	Sonic_StickToGround(self);
}

void Sonic_ResetLookShift(Object* self)
{
	if(v_lookshift > 0x60)
		v_lookshift -= 2;
	else if(v_lookshift < 0x60)
		v_lookshift += 2;
}

bool Sonic_CheckBalancing(Object* self)
{
	if(Player_IsStanding())
	{
		if(!Obj_IsDead(v_objspace[VAR_B(self, Player_StandingObjectB)]))
		{
			// Is player balancing on edge of platform?
			auto diffX = obj->actWid + self->x - obj->x;

			if(diffX < 4)
			{
				Player_SetFlipped();
				self->anim = PlayerAnim_Balance;
				return true;
			}
			else if(diffX >= (obj->actWid * 2) - 4)
			{
				Player_SetNotFlipped();
				self->anim = PlayerAnim_Balance;
				return true;
			}
		}
	}
	else
	{
		int dist;
		ObjFloorDist(self, &dist, nullptr);

		if(d1 >= 12)
		{
			if(VAR_B(self, Player_LastAngle1B) == 3)
			{
				Player_SetNotFlipped();
				self->anim = PlayerAnim_Balance;
				return true;
			}
			else if(VAR_B(self, Player_LastAngle2B) == 3)
			{
				Player_SetFlipped();
				self->anim = PlayerAnim_Balance;
				return true;
			}
		}
	}

	return false;
}

bool Sonic_CheckLooking(Object* self)
{
	if(v_jpadhold2 & Buttons_Up)
	{
		self->anim = PlayerAnim_LookUp;
		v_lookshift = min(v_lookshift + 2, 0xC8);
		return true;
	}

	if(v_jpadhold2 & Buttons_Down)
	{
		self->anim = PlayerAnim_Duck;
		v_lookshift = max(v_lookshift - 2, 8);
		return true;
	}

	return false;
}

void Sonic_SlowDown(Object* self)
{
	if((v_jpadhold2 & (Buttons_L | Buttons_R)) == 0)
	{
		if(self->inertia > 0)
			self->inertia = max(self->inertia - v_sonspeedacc, 0);
		else if(self->inertia < 0)
			self->inertia = min(self->inertia + v_sonspeedacc, 0);
	}
}

void Sonic_MoveLeft(Object* self)
{
	if(self->inertia <= 0)
	{
		if(!Player_IsFlipped())
		{
			Player_SetFlipped();
			Player_SetNotPushing();
			self->nextAni = 1;
		}

		self->inertia = max(self->inertia - v_sonspeedacc, -v_sonspeedmax);
		self->anim = PlayerAnim_Walk;
	}
	else
	{
		self->inertia -= v_sonspeeddec;

		if(self->inertia < 0)
			self->inertia = -0x80;

		if(((self->angle + 0x20) & 0xC0) == 0 && self->inertia >= 0x400)
		{
			self->anim = PlayerAnim_Stop;
			Player_SetNotFlipped();
			PlaySound_Special(SFX_Skid);
		}
	}
}

void Sonic_MoveRight(Object* self)
{
	if(self->inertia >= 0)
	{
		if(Player_IsFlipped())
		{
			Player_SetNotFlipped();
			Player_SetNotPushing();
			self->nextAni = 1;
		}

		self->inertia = min(self->inertia + v_sonspeedacc, v_sonspeedmax);
		self->anim = PlayerAnim_Walk
	}
	else
	{
		self->inertia += v_sonspeeddec;

		if(self->inertia >= 0)
			self->inertia = 0x80;

		if(((self->angle + 0x20) & 0xC0) == 0 && self->inertia < -0x400)
		{
			self->anim = PlayerAnim_Stop;
			Player_SetFlipped();
			PlaySound_Special(SFX_Skid);
		}
	}
}