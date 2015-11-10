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

void Sonic_RollRepel(Object* self)
{
	if((self->angle + 0x60) < 0xC0)
	{
		auto inertiaChange = (CalcSine(self->angle) * 0x50) / 256;

		if(self->inertia >= 0)
			self->inertia += (inertiaChange < 0) ? inertiaChange / 4 : inertiaChange;
		else
			self->inertia += (inertiaChange >= 0) ? inertiaChange / 4 : inertiaChange;
	}
}