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
