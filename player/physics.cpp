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

void Sonic_AnglePos(Object* self)
{
	if(Player_IsStanding())
	{
		v_anglebuffer = 0;
		v_b_F76A = 0;
		return;
	}

	v_anglebuffer = 3;
	v_b_F76A = 3;
	auto angle = self->angle;

	if((angle + 0x20) >= 0x80)
		angle = ((angle >= 0x80) ? angle + 0x1F : angle + 0x20) & 0xC0;
	else
		angle = ((angle >= 0x80) ? angle + 0x20 : angle + 0x1F) & 0xC0;

	if(angle == 0x40)
		Sonic_WalkVertL(self);
	else if(angle == 0x80)
		Sonic_WalkCeiling(self);
	else if(angle == 0xC0)
		Sonic_WalkVertR(self);
	else
		Sonic_WalkFloor(self);
}

void Sonic_WalkFloor(Object* self)
{
	int dist1, dist2;
	FindFloor(self, self->y + self->height, self->x + self->width, 0xD, 0, 16, &dist1, &v_anglebuffer);
	FindFloor(self, self->y + self->height, self->x - self->width, 0xD, 0, 16, &dist2, &v_b_F76A);
	dist1 = Sonic_Angle(self, dist1, dist2);

	if(dist1 < 0)
	{
		if(dist1 >= -14)
			self->y += dist1; // move up to push out of ground
	}
	else if(dist1 > 0)
	{
		if(dist1 <= 14 || VAR_B(self, 0x38) != 0)
			self->y += dist1; // move down to stick to ground
		else
		{
			Player_SetAir();
			Player_SetNotPushing();
			self->nextAni = 1;
		}
	}
}

void Sonic_WalkVertR(Object* self)
{
	int dist1, dist2;
	FindWall(self, self->y - self->width, self->x + self->height, 0xD, 0, 16, &dist1, &v_anglebuffer);
	FindWall(self, self->y + self->width, self->x - self->height, 0xD, 0, 16, &dist2, &v_b_F76A);
	dist1 = Sonic_Angle(self, dist1, dist2);

	if(dist1 < 0)
	{
		if(dist1 >= -14)
			self->x += dist1; // move left to push out of wall
	}
	else if(dist1 > 0)
	{
		if(dist1 <= 14 || VAR_B(self, 0x38) != 0)
			self->x += dist1; // move right to stick to wall
		else
		{
			Player_SetAir();
			Player_SetNotPushing();
			self->nextAni = 1;
		}
	}
}

void Sonic_WalkCeiling(Object* self)
{
	int dist1, dist2;
	FindFloor(self, (self->y - self->height) ^ 0xF, self->x + self->width, 0xD, 0x1000, -16, &dist1, &v_anglebuffer);
	FindFloor(self, (self->y - self->height) ^ 0xF, self->x - self->width, 0xD, 0x1000, -16, &dist2, &v_b_F76A);
	dist1 = Sonic_Angle(self, dist1, dist2);

	if(dist1 < 0)
	{
		if(dist1 >= -14)
			self->y -= dist1; // move down to push out of ceiling
	}
	else if(dist1 > 0)
	{
		if(dist1 <= 14 || VAR_B(self, 0x38) != 0)
			self->y -= dist1; // move up to stick to ceiling
		else
		{
			Player_SetAir();
			Player_SetNotPushing();
			self->nextAni = 1;
		}
	}
}

void Sonic_WalkVertL(Object* self)
{
	int dist1, dist2;
	FindWall(self, self->y - self->width, (self->x - self->height) ^ 0xF, 0xD, 0x800, -16, &dist1, &v_anglebuffer);
	FindWall(self, self->y + self->width, (self->x - self->height) ^ 0xF, 0xD, 0x800, -16, &dist2, &v_b_F76A);
	dist1 = Sonic_Angle(self, dist1, dist2);

	if(dist1 < 0)
	{
		if(dist1 >= -14)
			self->x -= dist1; // move right to push out of wall
	}
	else if(dist1 > 0)
	{
		if(dist1 <= 14 || VAR_B(self, 0x38) != 0)
			self->x -= dist1; // move left to stick to wall
		else
		{
			Player_SetAir();
			Player_SetNotPushing();
			self->nextAni = 1;
		}
	}
}

//d1                      a0         d1         d0
int Sonic_Angle(Object* self, int dist1, int dist2)
{
	auto angle = v_b_F76A;

	if(dist1 > dist2)
	{
		angle = v_anglebuffer;
		dist1 = dist2;
	}

	if(angle & 1)
		self->angle = (self->angle + 0x20) & 0xC0;
	else
		self->angle = angle;

	return dist1;
}


void loc_1300C(Object* self)
{
	if((self->angle + 0x40) >= 0 && self->inertia != 0)
	{
		auto walkAngle = self->angle + (self->inertia < 0) ? 0x40 : -0x40;
		auto dist = Sonic_WalkSpeed(self, walkAngle);

		if(dist < 0)
		{
			walkAngle = (walkAngle + 0x20) & 0xC0;

			if(walkAngle == 0)
				self->velY += dist << 8;
			else if(walkAngle == 0x40)
			{
				self->velX -= dist << 8;
				Player_SetPushing();
				self->inertia = 0;
			}
			else if(walkAngle == 0x80)
				self->velY -= dist << 8;
			else
			{
				self->velX += dist << 8;
				Player_SetPushing();
				self->inertia = 0;
			}
		}
	}
}

//d1                         a0         d0
int Sonic_WalkSpeed(Object* self, int angle)
{
	auto x = HIWORD(self->x + (self->velX << 8));
	auto y = HIWORD(self->y + (self->velY << 8));
	v_anglebuffer = angle;
	v_b_F76A = angle;

	auto angle2 = angle + 0x20;

	if(angle2 < 0)
		angle2 += (angle < 0) ? 0x19 : 0x20;
	else
		angle2 += (angle < 0) ? 0x20 : 0x19;

	angle2 &= 0xC0;

	if(angle2 == 0)
		return loc_14DF0(self, x, y, nullptr);
	else if(angle2 == 0x80)
		return loc_14F7C(self, x, y, nullptr);
	else
	{
		if((angle & 0x38) == 0)
			y += 8;

		if(angle2 == 0x40)
			return Sonic_HitWallLeft2(self, x, y, nullptr);
		else
			return Sonic_HitWallRight2(self, x, y, nullptr);
	}
}