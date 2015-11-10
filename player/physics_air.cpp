void Sonic_JumpHeight(Object* self)
{
	if(VAR_B(self, 0x3C))
	{
		auto accel = Player_IsUnderwater() ? -0x200 : -0x400;

		if(self->velY < accel && (v_jpadhold2 & Buttons_ABC) == 0)
			self->velY = accel;
	}
	else if(self->velY < -0xFC0)
		self->velY = -0xFC0;
}

void Sonic_JumpDirection(Object* self)
{
	d6 = v_sonspeedmax
	d5 = v_sonspeedacc * 2

	if(!Player_IsJumping())
	{
		if(v_jpadhold2 & Buttons_L)
		{
			Player_SetFlipped();
			self->velX = max(self->velX - (v_sonspeedacc * 2), -v_sonspeedmax);
		}

		if(v_jpadhold2 & Buttons_R)
		{
			Player_SetNotFlipped();
			self->velX = min(self->velX + (v_sonspeedacc * 2), v_sonspeedmax);
		}
	}

	if(v_lookshift < 0x60)
		v_lookshift += 2;
	else if(v_lookshift > 0x60)
		v_lookshift -= 2;

	if(self->velY >= -0x400)
	{
		auto accel = self->velX / 32;

		if(accel > 0)
			self->velX = max(self->velX - accel, 0);
		else if(accel < 0)
			self->velX = min(self->velX - accel, 0);
	}
}

void Sonic_JumpAngle(Object* self)
{
	if(self->angle < 0)
		self->angle = min(self->angle + 2, 0);
	else if(self->angle > 0)
		self->angle = max(self->angle - 2, 0);
}

void Sonic_Floor(Object* self)
{
	d0 = CalcAngle(self->velX, self->velY)
	v_b_FFEC = d0;
	d0 -= 20;
	v_b_FFED = d0;
	d0 &= 0xC0
	v_b_FFEE = d0;

	if(d0 == 0)
		Sonic_FloorDown(self);
	else if(d0 == 0x40)
		Sonic_FloorLeft(self);
	else if(d0 == 0x80)
		Sonic_FloorUp(self);
	else
		Sonic_FloorRight(self);
}

void Sonic_FloorDown(Object* self)
{
	auto dist = Sonic_HitWallLeft(self, nullptr);

	if(dist < 0)
	{
		self->x -= dist; // move *right*
		self->velX = 0;
	}

	dist = Sonic_HitWallRight(self, nullptr);

	if(dist < 0)
	{
		self->x += dist; // move *left*
		self->velX = 0;
	}

	int angle, otherDist;
	dist = Sonic_CheckDown(self, &angle, &otherDist);
	v_b_FFEF = dist;

	if(dist < 0 && (dist >= -self->velY - 8 || otherDist >= -self->velY - 8))
	{
		self->y += dist; // move *up*
		self->angle = angle;
		Sonic_ResetOnFloor(self);
		self->anim = PlayerAnim_Walk;

		if((angle + 0x20) & 0x40)
		{
			self->velX = 0;
			self->velY = min(self->velY, 0xFC0);
			self->inertia = angle < 0 ? -self->velY : self->velY;
		}
		else if((angle + 0x10) & 0x20)
		{
			self->velY /= 2;
			self->inertia = angle < 0 ? -self->velY : self->velY;
		}
		else
		{
			self->velY = 0;
			self->inertia = self->velX;
		}
	}
}

void Sonic_FloorLeft(Object* self)
{
	auto dist = Sonic_HitWallLeft(self, nullptr);

	if(dist < 0)
	{
		self->x -= dist; // move *right*
		self->velX = 0;
		self->inertia = self->velY;
	}
	else
	{
		dist = Sonic_CheckUp(self, nullptr, nullptr);

		if(dist < 0)
		{
			self->y -= dist; // move *down*
			self->velY = clamp0(self->velY);
		}
		else if(self->velY >= 0)
		{
			int angle;
			dist = Sonic_CheckDown(self, &angle, nullptr);

			if(dist < 0)
			{
				self->y += dist; // move *up*
				self->angle = angle;
				Sonic_ResetOnFloor(self);
				self->anim = PlayerAnim_Walk;
				self->velY = 0;
				self->inertia = self->velX;
			}
		}
	}
}

void Sonic_FloorUp(Object* self)
{
	auto dist = Sonic_HitWallLeft(self, nullptr);

	if(dist < 0)
	{
		self->x -= dist; // move *right*
		self->velX = 0;
	}

	dist = Sonic_HitWallRight(self, nullptr);

	if(dist < 0)
	{
		self->x += dist; // move *left*
		self->velX = 0;
	}

	int angle;
	dist = Sonic_CheckUp(self, &angle, nullptr);

	if(dist < 0)
	{
		self->y -= dist; // move *down*

		if(((angle + 0x20) & 0x40) == 0)
			self->velY = 0;
		else
		{
			self->angle = angle;
			Sonic_ResetOnFloor(self);
			self->inertia = (angle < 0) ? -self->velY : self->velY;
		}
	}
}

void Sonic_FloorRight(Object* self)
{
	auto dist = Sonic_HitWallRight(self, nullptr);

	if(dist < 0)
	{
		self->x += dist; // move *left*
		self->velX = 0;
		self->inertia = self->velY;
	}
	else
	{
		dist = Sonic_CheckUp(self, nullptr, nullptr);

		if(dist < 0)
		{
			self->y -= dist; // move *down*
			self->velY = clamp0(self->velY);
		}
		else if(self->velY >= 0)
		{
			int angle;
			dist = Sonic_CheckDown(self, &angle, nullptr);

			if(dist < 0)
			{
				self->y += dist; // move *up*
				self->angle = angle;
				Sonic_ResetOnFloor(self);
				self->anim = PlayerAnim_Walk;
				self->velY = 0;
				self->inertia = self->velX;
			}
		}
	}
}

void Sonic_ResetOnFloor(Object* self)
{
	// dead code
	// if(Player_IsJumping()) {}
	Player_SetNotPushing();
	Player_SetNotInAir();
	Player_SetNotJumping();

	if(Player_IsRolling())
	{
		Player_SetNotRolling();
		self->height = 19;
		self->width = 9;
		self->anim = PlayerAnim_Walk;
		self->y -= 5;
	}

	VAR_B(self, 0x3C) = 0;
	v_itembonus = 0;
}