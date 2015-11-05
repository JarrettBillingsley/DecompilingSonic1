// =====================================================================================================================
// Object 79 - Lamppost
// The low 7 bits of the subtype are the lamppost index. When you pass a lamppost, those with lower indices become red
// and can't be passed anymore. This way you can only move the lamppost forward.
// The top bit of the subtype seems to make it so the screen won't scroll left after respawning from that lamppost.
// =====================================================================================================================

enum : ubyte
{
	Routine_Init = 0,
	Routine_Blue = 2,
	Routine_Finish = 4,
	Routine_Twirl = 6,

	origXW = 0x30,     // original x-axis position
	origYW = 0x32,     // original y-axis position
	twirlTimeW = 0x36, // length of time to twirl the lamp

	Frame_Blue = 0,
	Frame_PoleOnly = 1,
	Frame_RedBallOnly = 2,
	Frame_Red = 3,
};

enum : ushort
{
	GFX_Lamp = 0x07A0,

	Lamppost_TwirlTime = 32,
	Lamppost_BallSwingDist = 0xC00,
	Lamppost_AngleStep = 16,
	Lamppost_BallYOffs = 24,
}

const ubyte Map_Lamp[][] =
{
	{6, 0xE4, 1, 0, 0, 0xF8,  // blue
		0xE4, 1, 8, 0, 0,
		0xF4, 3, 0x20, 2, 0xF8,
		0xF4, 3, 0x28, 2, 0,
		0xD4, 1, 0, 6, 0xF8,
		0xD4, 1, 8, 6, 0},
	{4, 0xE4, 1, 0, 0, 0xF8,  // pole only
		0xE4, 1, 8, 0, 0,
		0xF4, 3, 0x20, 2, 0xF8,
		0xF4, 3, 0x28, 2, 0},
	{2, 0xF8, 1, 0, 8, 0xF8,  // red ball only
		0xF8, 1, 8, 8, 0},
	{6, 0xE4, 1, 0, 0, 0xF8,  // red
		0xE4, 1, 8, 0, 0,
		0xF4, 3, 0x20, 2, 0xF8,
		0xF4, 3, 0x28, 2, 0,
		0xD4, 1, 0, 8, 0xF8,
		0xD4, 1, 8, 8, 0},
};

void Lamppost(Object* self)
{
	switch(self->routine)
	{
		case Routine_Init:
			self->routine = Routine_Blue;
			self->map = Map_Lamp;
			self->gfx = GFX_Lamp
			self->render = ObjRender_LayerNormal;
			self->actWid = 8;
			self->priority = 5;

			BCLR(v_objstate[self->respawnNo + 2], 1 << 7);

			if(BTST(v_objstate[self->respawnNo + 2], 1) || (v_lastlamp & 0x7F) >= (self->subtype & 0x7F))
			{
				BSET(v_objstate[self->respawnNo + 2], 1);
				self->routine = Routine_Finish;
				self->frame = Frame_Red;
				break;
			}
			// fall through
		case Routine_Blue:
			if(v_debuguse || f_lockmulti & 0x80)
				break;

			if((v_lastlamp & 0x7F) >= (self->subtype & 0x7F))
			{
				BSET(v_objstate[self->respawnNo + 2], 1);
				self->routine = Routine_Finish;
				self->frame = Frame_Red;
			}
			else if((v_player->x - self->x + 8) < 16 && (v_player->y - self->y + 64) < 104)
			{
				PlaySound(SFX_Lamppost);
				self->routine = Routine_Finish;

				if(auto obj = FindFreeObj())
				{
					obj->id = ID_Lamppost;
					obj->routine = Routine_Twirl;
					VAR_W(obj, origXW) = self->x;
					VAR_W(obj, origYW) = self->y - Lamppost_BallYOffs;
					obj->map = Map_Lamp;
					obj->gfx = GFX_Lamp;
					obj->render = ObjRender_LayerNormal;
					obj->actWid = 8;
					obj->priority = 4;
					obj->frame = Frame_RedBallOnly;
					VAR_W(obj, twirlTimeW) = Lamppost_TwirlTime;
				}

				self->frame = Frame_PoleOnly;
				Lamp_StoreInfo(self);
				BSET(v_objstate[self->respawnNo + 2], 1);
			}
			break;

		case Routine_Finish:
			break;

		case Routine_Twirl:
			if(TimerNeg(VAR_W(self, twirlTimeW)))
				self->routine = Routine_Finish;

			int cosine;
			int sine = CalcSine(self->angle - 64, &cosine)
			self->angle -= Lamppost_AngleStep;
			self->x = VAR_W(self, origXW) + ((cosine * Lamppost_BallSwingDist) >> 16);
			self->y = VAR_W(self, origYW) + ((sine * Lamppost_BallSwingDist) >> 16);
			break;
	}

	RememberState(self);
}


void Lamp_StoreInfo(Object* self)
{
	v_lastlamp = self->subtype;
	v_lamp_lastlamp = v_lastlamp;
	v_lamp_xpos = self->x;
	v_lamp_ypos = self->y;
	v_lamp_rings = v_rings;
	v_lamp_lives = v_lifecount;
	v_lamp_time = v_time;
	v_lamp_dle = v_dle_routine;
	v_lamp_limitbtm = v_limitbtm2;
	v_lamp_scrx = v_screenposx;
	v_lamp_scry = v_screenposy;
	v_lamp_bg1x = v_bg1posx;
	v_lamp_bg1y = v_bg1posy;
	v_lamp_somethingx = v_somethingposx;
	v_lamp_somethingy = v_somethingposy;
	v_lamp_bg2x = v_bg2posx;
	v_lamp_bg2y = v_bg2posy;
	v_lamp_wtrpos = v_waterpos2;
	v_lamp_wtrrout = v_wtr_routine;
	v_lamp_wtrstat = f_wtr_state;
}

void Lamp_LoadInfo()
{
	v_lastlamp = v_lamp_lastlamp;
	v_player->x = v_lamp_xpos;
	v_player->y = v_lamp_ypos;
	v_rings = v_lamp_rings;
	v_lifecount = v_lamp_lives;
	v_rings = 0;
	v_lifecount = 0;
	v_time = v_lamp_time;
	v_timecent = 59;
	v_timesec--;
	v_dle_routine = v_lamp_dle;
	v_wtr_routine = v_lamp_wtrrout;
	v_limitbtm2 = v_lamp_limitbtm;
	v_limitbtm1 = v_lamp_limitbtm;
	v_screenposx = v_lamp_scrx;
	v_screenposy = v_lamp_scry;
	v_bg1posx = v_lamp_bg1x;
	v_bg1posy = v_lamp_bg1y;
	v_somethingposx = v_lamp_somethingx;
	v_somethingposy = v_lamp_somethingy;
	v_bg2posx = v_lamp_bg2x;
	v_bg2posy = v_lamp_bg2y;

	if(v_zone == Zone_LZ)
	{
		v_waterpos2 = v_lamp_wtrpos;
		v_wtr_routine = v_lamp_wtrrout;
		f_wtr_state = v_lamp_wtrstat;
	}

	if(v_lastlamp & 0x80)
		v_limitleft2 = v_lamp_xpos - 160;
}