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
			self->render = ObjRender_LayerNormal;
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

void Sonic_Display(Object* self)
{
	if(VAR_W(self, Player_InvincibilityW))
	{
		if(VAR_W(self, Player_InvincibilityW) & 4)
			DisplaySprite(self);

		VAR_W(self, Player_InvincibilityW)--;
	}
	else
		DisplaySprite(self);

	if(v_invinc && VAR_W(self, Player_InvTimeW) != 0 && DecToZero(VAR_W(self, Player_InvTimeW)))
	{
		if(!f_lockscreen && v_air >= 12)
		{
			if(v_zone == Zone_LZ && v_act == Act_4)
				PlaySound(BGM_SBZ);
			else
				PlaySound(MusicList2[v_zone]);
		}

		v_invinc = false;
	}

	if(v_shoes && VAR_W(self, Player_ShoeTimeW) != 0 && DecToZero(VAR_W(self, Player_ShoeTimeW)))
	{
		v_sonspeedmax = 0x600;
		v_sonspeedacc = 0xC;
		v_sonspeeddec = 0x80;
		v_shoes = false;
		PlaySound(0xE3);
	}
}

void Sonic_RecordPosition(Object* self)
{
	v_tracksonic[v_trackpos * 2] = self->x;
	v_tracksonic[v_trackpos * 2 + 1] = self->y;
	v_trackpos = (v_trackpos + 2) & 0xFF;
}

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

void Sonic_Animate(Object* self)
{
	if(self->anim != self->nextAni)
	{
		self->nextAni = self->anim;
		self->aniFrame = 0;
		self->timeFrame = 0;
	}

	auto animScript = Ani_Sonic[self->anim];

	if(!BTST(animScript[0], 0x80))
	{
		self->render = (self->render & 0xFC) | (self->status & ObjStatus_Flip);

		if(TimerNeg(self->timeFrame, animScript[0]))
			Sonic_LoadFrame(self, animScript);
	}
	else if(TimerNeg(self->timeFrame))
	{
		switch(animScript[0])
		{
			case 0xFF: { // walk/run
				auto angle = self->angle;

				if(!Player_IsFlipped())
					angle = ~angle;

				angle += 0x10;

				if(angle >= 0x80)
					self->render = (self->render & 0xFC) | (3 ^ (self->status & ObjStatus_Flip))
				else
					self->render = (self->render & 0xFC) | (self->status & ObjStatus_Flip)

				if(Player_IsPushing())
					goto _push;

				angle = (angle / 16) & 6; // make it 0, 2, 4, 6
				auto inertia = abs(self->inertia);

				if(inertia < 0x600)
					angle += (angle / 2); // 0, 3, 5, 7

				self->timeFrame = clamp0(0x800 - inertia) / 256;
				Sonic_LoadFrame(self, Ani_Sonic[inertia >= 0x600 ? PlayerAnim_Run : PlayerAnim_Walk]);
				self->frame += angle * 2;
				break;
			}
			case 0xFE: { // rolling/jumping
				auto inertia = abs(self->inertia);
				self->timeFrame = clamp0(0x400 - inertia) / 0x100;
				self->render = (self->render & 0xFC) | (self->status & ObjStatus_Flip);
				Sonic_LoadFrame(self, Ani_Sonic[inertia >= 0x600 ? PlayerAnim_Roll2 : PlayerAnim_Roll]);
				break;
			}
			case 0xFD: // pushing
			_push:
				self->timeFrame = clamp0(0x800 - abs(self->inertia)) / 0x40;
				self->render = (self->render & 0xFC) | (self->status & ObjStatus_Flip);
				Sonic_LoadFrame(self, Ani_Sonic[PlayerAnim_Push]);
				break;
		}
	}
}

void Sonic_LoadFrame(Object* self, ubyte* animScript)
{
	auto frame = animScript[self->aniFrame + 1];

	switch(frame)
	{
		case AnimFlags_End:
			self->aniFrame = 0;
			frame = animScript[1];
			break;

		case AnimFlags_Back:
			auto offs = animScript[self->aniFrame + 2];
			self->aniFrame -= offs;
			frame = animScript[self->aniFrame + 1];
			break;

		case AnimFlags_Change:
			self->anim = animScript[self->aniFrame + 2];
			return;

		default:
			if(frame & AnimFlag_Command)
				return;

			break;
	}

	self->frame = frame;
	self->aniFrame++;
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
				BCLR(self->render, ObjRender_Behind);
			else if(HIWORD(self->x) >= 112)
				BSET(self->render, ObjRender_Behind);
			else if(BTST(self->render, ObjRender_Behind))
			{
				if(self->angle >= 0x80)
					BCLR(self->render, ObjRender_Behind);
			}
			else if(self->angle > 0 && self->angle <= 0x80)
				BSET(self->render, ObjRender_Behind);
		}
		else
			BCLR(self->render, ObjRender_Behind);
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

void Sonic_LoadGfx(Object* self)
{
	if(self->frame != v_sonframenum)
	{
		v_sonframenum = self->frame;
		f_sonframechg = true;
		auto plc = SonicDynPLC[self->frame];
		auto buffer = v_sgfx_buffer;

		for(int i = 0; i < plc[0]; i++)
		{
			auto frame = (plc[i * 2 + 1] << 8) | plc[i * 2 + 2];
			auto count = ((frame >> 12) + 1) * 32;
			memcpy(buffer, Art_Sonic[frame], count);
			buffer += count;
		}
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