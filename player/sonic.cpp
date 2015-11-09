enum : ushort
{
	GFX_Sonic = 0x0780,
};

void SonicPlayer(Object* self)
{
	if(v_debuguse)
	{
		DebugMode();
		return;
	}

	switch(self->routine)
	{
		case PlayerRoutine_Init:
			self->routine = PlayerRoutine_Normal;
			self->height = 19;
			self->width = 9
			self->map = Map_Sonic
			self->gfx = GFX_Sonic;
			self->priority = 2;
			self->actWid = 24;
			Obj_SetLayerNormal(self);
			v_sonspeedmax = 0xC00;
			v_sonspeedacc = 0x1C;
			v_sonspeeddec = 0x80;
			// fall through
		case PlayerRoutine_Normal:
			if(f_debugmode && (v_jpadpress1 & Button_B))
			{
				v_debuguse = true;
				f_lockctrl = false;
				return;
			}

			if(!f_lockctrl)
				v_jpadhold2 = v_jpadhold1;

			if(!BTST(f_lockmulti, 1))
			{
				if(Player_IsInAir())
				{
					Sonic_JumpHeight(self);
					Sonic_JumpDirection(self);
					Sonic_LevelBound(self);
					ObjectFall(self);

					if(Player_IsUnderwater())
						self->velY -= 0x28;

					Sonic_JumpAngle(self);
					Sonic_Floor(self);
				}
				else if(Player_IsRolling())
				{
					Sonic_Jump(self);
					Sonic_RollRepel(self);
					Sonic_RollSpeed(self);
					Sonic_LevelBound(self);
					SpeedToPos(self);
					Sonic_AnglePos(self);
					Sonic_SlopeRepel(self);
				}
				else
				{
					Sonic_Jump(self);
					Sonic_SlopeResist(self);
					Sonic_Move(self);
					Sonic_Roll(self);
					Sonic_LevelBound(self);
					SpeedToPos(self);
					Sonic_AnglePos(self);
					Sonic_SlopeRepel(self);
				}
			}

			Sonic_Display(self);
			Sonic_RecordPosition(self);
			Sonic_Water(self);
			VAR_B(self, 0x36) = v_anglebuffer;
			VAR_B(self, 0x37) = v_b_F76A;

			if(f_wtunnelmode && self->anim == 0)
				self->anim = self->nextAni;

			Sonic_Animate(self);

			if(!BTST(f_lockmulti, 0x80))
				ReactToItem(self);

			Sonic_Loops(self);
			Sonic_LoadGfx(self);
			break;

		case PlayerRoutine_Hurt:
			SpeedToPos(self);
			self->velY += 0x30;

			if(Player_IsUnderwater)
				self->velY -= 0x20;

			Sonic_HurtStop(self);
			Sonic_LevelBound(self);
			Sonic_RecordPosition(self);
			Sonic_Animate(self);
			Sonic_LoadGfx(self);
			DisplaySprite(self);
			break;

		case PlayerRoutine_Dead:
			GameOver(self);
			ObjectFall(self);
			Sonic_RecordPosition(self);
			Sonic_Animate(self);
			Sonic_LoadGfx(self);
			DisplaySprite(self);
			break;

		case PlayerRoutine_LevelReset:
			if(TimerZero(VAR_W(self, 0x3A)))
				f_restart = true;
			break;
	}
}

void Sonic_HurtStop(Object* self)
{
	if(self->y > v_limitbtm2 + HalfScreenHeight)
		Player_Kill(self, self); // I don't think they actually set the "killer" address to anything proper!
	else
	{
		Sonic_Floor(self);

		if(!Player_IsInAir())
		{
			self->velX = 0;
			self->velY = 0;
			self->inertia = 0;
			self->anim = PlayerAnim_Walk;
			self->routine = PlayerRoutine_Normal;
			VAR_W(self, Player_InvincibilityW) = 2 * OneSecond;
		}
	}
}

void GameOver(Object* self)
{
	if(self->y >= v_limitbtm2 + 256)
	{
		self->velY = -0x38;
		self->routine = PlayerRoutine_LevelReset;
		f_timecount = false;
		f_lifecount = true;
		v_lives--;

		if(v_lives == 0)
		{
			VAR_W(self, 0x3A) = 0;
			v_objspace[2].id = ID_GameOverCard;
			v_objspace[3].id = ID_GameOverCard;
			v_objspace[3].frame = 1;
			f_timeover = false;
		}
		else if(f_timeover)
		{
			VAR_W(self, 0x3A) = 0;
			v_objspace[2].id = ID_GameOverCard;
			v_objspace[3].id = ID_GameOverCard;
			v_objspace[2].frame = 2;
			v_objspace[3].frame = 3;
		}
		else
		{
			VAR_W(self, 0x3A) = OneSecond;
			return;
		}

		PlaySound(BGM_GameOver);
		AddPLC(PLC_GameOver);
	}
}

void Sonic_RecordPosition(Object* self)
{
	v_tracksonic[v_trackpos * 2] = self->x;
	v_tracksonic[v_trackpos * 2 + 1] = self->y;
	v_trackpos = (v_trackpos + 2) & 0xFF;
}

void Sonic_Loops(Object* self)
{
	if(v_zone == Zone_GHZ || v_zone == Zone_SLZ)
	{
		auto chunk = v_lvllayout[HIWORD(self->y) & 7][HIWORD(self->x) & 0x7F];

		if(chunk == v_256roll1 || chunk == v_256roll2)
			Sonic_ChkRoll(self);
		else if(chunk == v_256loop1 || chunk == v_256loop2)
		{
			if(Player_IsInAir() || HIWORD(self->x) < 44)
				Obj_SetNotBehind(self);
			else if(HIWORD(self->x) >= 112)
				Obj_SetBehind(self);
			else if(Obj_IsBehind(self))
			{
				if(self->angle >= 0x80)
					Obj_SetNotBehind(self);
			}
			else if(self->angle > 0 && self->angle <= 0x80)
				Obj_SetBehind(self);
		}
		else
			Obj_SetNotBehind(self);
	}
}

void Sonic_LevelBound(Object* self)
{
	auto projectedX = (self->x + (self->velX << 8)) >> 16;

	if(projectedX < v_limitleft2 + 16)
	{
		self->x = (v_limitleft2 + 16) << 16;
		self->velX = 0;
		self->inertia = 0;
	}
	else
	{
		auto right = v_limitright2 + (f_lockscreen ? 296 : 360);

		if(projectedX >= right)
		{
			self->x = right << 16;
			self->velX = 0;
			self->inertia = 0;
		}
	}

	if(self->y > v_limitbtm2 + HalfScreenHeight)
	{
		if(v_zone == Zone_SBZ && v_act == Act_2 && self->x >= 0x2000)
		{
			v_lastlamp = 0;
			f_restart = true;
			v_zone = Zone_LZ;
			v_act = Act_4;
		}
		else
			Player_Kill(self, self);
	}
}