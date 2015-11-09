void Sonic_Water(Object* self)
{
	if(v_zone != Zone_LZ)
		return;

	if(self->y > v_waterpos1)
	{
		if(!Player_IsUnderwater())
		{
			Player_SetUnderwater();
			ResumeMusic();

			v_objspace[13].id = ID_DrownCount;
			v_objspace[13].subtype = 0x81; // master
			v_sonspeedmax = 0x300;
			v_sonspeedacc = 6;
			v_sonspeeddec = 0x40;
			self->velX /= 2;
			self->velY /= 4;

			if(self->velY != 0)
			{
				v_objspace[12].id = ID_Splash;
				PlaySound_Special(SFX_Splash);
			}
		}
	}
	else if(Player_IsUnderwater())
	{
		Player_SetAboveWater();
		ResumeMusic();
		v_sonspeedmax = 0x600;
		v_sonspeedacc = 0xC;
		v_sonspeeddec = 0x80;
		self->velY *= 2;

		if(self->velY != 0)
		{
			v_objspace[12].id = ID_Splash;

			if(self->velY < -0x1000)
				self->velY = -0x1000;

			PlaySound_Special(SFX_Splash);
		}
	}
}

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
	if(BTST(self->status, ObjStatus_Rolling))
		return;

	BSET(self->status, ObjStatus_Rolling);
	self->height = 14;
	self->width = 7;
	self->anim = PlayerAnim_Roll;
	self->y += 5;
	PlaySound_Special(SFX_Roll);

	if(self->inertia == 0)
		self->inertia = 0x200;
}

void Sonic_Jump(Object* self)
{
	if(v_jpadpress2 & Buttons_ABC)
	{
		if(Sonic_CheckLevelCollision(self, self->angle + 0x80) >= 6)
		{
			auto jumpVel = Player_IsUnderwater() ? 0x380 : 0x680;
			int cosine;
			auto sine = CalcSine(self->angle - 0x40, &cosine);
			self->velX += (jumpVel * cosine) / 256;
			self->velY += (jumpVel * sine) / 256;
			Player_SetInAir();
			Player_SetNotPushing();
			VAR_B(self, 0x3C) = 1;
			VAR_B(self, 0x38) = 0;
			PlaySound_Special(SFX_Jump);
			self->height = 19;
			self->width = 9;

			if(Player_IsRolling())
				Player_SetJumping();
			else
			{
				self->height = 14;
				self->width = 7;
				self->anim = PlayerAnim_Roll;
				Player_SetRolling();
				player->y += 5;
			}
		}
	}
}

//d1                                             d0
int Sonic_CheckLevelCollision(Object* self, int angle)
{
	v_anglebuffer = angle;
	v_b_F76A = angle;
	angle = (angle + 0x20) & 0xC0;

	if(angle == 0x40)
		return Sonic_CheckLeft(self);
	else if(angle == 0x80)
		return Sonic_CheckUp(self);
	else if(angle == 0xC0)
		return Sonic_CheckRight(self);
	else
		return Sonic_CheckDown(self);
}

int Sonic_CheckLeft(Object* self)
{
	int dist1, dist2;
	FindFloor(self, self->y - self->width, (self->x - self->height) ^ 0xF, 0xE, 0x800, -16, &dist1, &v_anglebuffer)
	FindWall(self, self->y + self->width, (self->x - self->height) ^ 0xF, 0xE, 0x800, -16, &dist2, &v_b_F76A)
	return Sonic_CheckCommon(dist1, dist2, 0x40);
}

// aka "Sonic_DontRunOnWalls" which is the dumbest name
int Sonic_CheckUp(Object* self)
{
	int dist1, dist2;
	FindFloor(self, (self->y - self->height) ^ 0xF, self->x + self->width, 0xE, 0x1000, -16, &dist1, &v_anglebuffer)
	FindWall(self, (self->y - self->height) ^ 0xF, self->x - self->width, 0xE, 0x1000, -16, &dist2, &v_b_F76A)
	return Sonic_CheckCommon(dist1, dist2, -0x80);
}

int Sonic_CheckRight(Object8 self)
{
	int dist1, dist2;
	FindWall(self, self->y - self->width, self->x + self->height, 0xE, 0, 16, &dist1, &v_anglebuffer)
	FindWall(self, self->y + self->width, self->x + self->height, 0xE, 0, 16, &dist2, &v_b_F76A)
	return Sonic_CheckCommon(dist1, dist2, -0x40);
}

// aka "Sonic_HitFloor"
int Sonic_CheckDown(Object* self)
{
	int dist1, dist2;
	FindFloor(self, self->y + self->height, self->x + self->width, 0xD, 0, 16, &dist1, &v_anglebuffer)
	FindFloor(self, self->y + self->height, self->x - self->width, 0xD, 0, 16, &dist2, &v_b_F76A)
	return Sonic_CheckCommon(dist1, dist2, 0);
}

//d1                        d0         d1            d2
int Sonic_CheckCommon(int dist1, int dist2, int defaultAngle)
{
	//    d3... don't think it's used for anything?
	auto angle = v_b_F76A;

	if(dist2 > dist1)
	{
		angle = v_anglebuffer;
		SWAP(dist1, dist2);
	}

	if(angle & 1)
		angle = defaultAngle;

	return dist2;
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

void Sonic_SlopeRepel(Object* self)
{
	if(VAR_B(self, 0x38) == 0 && TimerZero(VAR_W(self, 0x3E)) && (self->angle & 0x20) & 0xC0 != 0 && abs(self->inertia) < 0x280)
	{
		self->inertia = 0;
		Player_SetInAir();
		VAR_W(self, 0x3E) = OneSecond / 2;
	}
}

void Sonic_RollSpeed(Object* self)
{
	// d6 = v_sonspeedmax * 2
	// d4 = v_sonspeeddec / 4

	if(!f_jumponly)
	{
		if(VAR_W(self, 0x3E) == 0)
		{
			if(v_jpadhold2 & Buttons_L)
			{
				if(self->inertia <= 0)
				{
					Player_SetFlipped();
					self->anim = PlayerAnim_Roll;
				}
				else
				{
					self->inertia -= (v_sonspeeddec / 4);

					if(self->inertia < 0)
						self->inertia = -0x80;
				}
			}

			if(v_jpadhold2 & Buttons_R)
			{
				if(self->inertia >= 0)
				{
					Player_SetNotFlipped();
					self->anim = PlayerAnim_Roll;
				}
				else
				{
					self->inertia += (v_sonspeeddec / 4);

					if(self->inertia > 0)
						self->inertia = 0x80;
				}
			}
		}

		// Rolling friction
		if(self->inertia > 0)
			self->inertia = clamp0(self->inertia - (v_sonspeedacc / 2));
		else if(self->inertia < 0)
			self->inertia = -clamp0(-self->inertia - (v_sonspeedacc / 2));

		// Unroll if stopped
		if(self->inertia == 0)
		{
			Player_SetNotRolling();
			self->height = 19;
			self->width = 9;
			self->anim = PlayerAnim_Wait;
			self->y -= 5;
		}
	}

	int cosine;
	auto sine = CalcSine(self->angle, &cosine);
	self->velY = (self->inertia * sine) / 256;
	self->velX = clamp((self->inertia * cosine) / 256, -0x1000, 0x1000);
	loc_1300C(self);
}