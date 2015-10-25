// =====================================================================================================================
// Object 0D - signpost at the end of a level
// =====================================================================================================================

enum : ubyte
{
	spinTimeW = 0x30,    // time for signpost to spin
	sparkleTimeW = 0x32, // time between sparkles
	sparkleIdxB = 0x34,  // index into Sign_SparkPos to make sparkles appear at different places

	Routine_Init = 0,
	Routine_Touch = 2,
	Routine_Spin = 4,
	Routine_SonicRun = 6,
	Routine_Exit = 8,
};

enum : ushort
{
	GFX_Signpost = 0x0680,

	Signpost_SpinTime = 60,
	Signpost_SparkleTime = 11,
};

const ubyte Map_Sign[][] =
{
	{3, 0xF0, 0xB, 0, 0, 0xE8,    // eggman
		0xF0, 0xB, 8, 0, 0,
		0x10, 1, 0, 0x38, 0xFC},
	{2, 0xF0, 0xF, 0, 0xC, 0xF0,  // spin1
		0x10, 1, 0, 0x38, 0xFC},
	{2, 0xF0, 3, 0, 0x1C, 0xFC,   // spin2
		0x10, 1, 8, 0x38, 0xFC},
	{2, 0xF0, 0xF, 8, 0xC, 0xF0,  // spin3
		0x10, 1, 8, 0x38, 0xFC},
	{3, 0xF0, 0xB, 0, 0x20, 0xE8, // sonic
		0xF0, 0xB, 0, 0x2C, 0,
		0x10, 1, 0, 0x38, 0xFC},
};

const ubyte Ani_Sign[][] =
{
	{ 15, 0, AnimFlags::End },         // eggman
	{ 1, 0, 1, 2, 3, AnimFlags::End }, // spin1
	{ 1, 4, 1, 2, 3, AnimFlags::End }, // spin2
	{ 15, 4, AnimFlags::End },         // sonic
};

const byte Sign_SparkPos[][] =
{
	{ -24, -16 },
	{   8,   8 },
	{ -16,   0 },
	{  24,  -8 },
	{   0,  -8 },
	{  16,   0 },
	{ -24,   8 },
	{  24,  16 },
};

void Signpost(Object* self)
{
	switch(self->routine)
	{
		case Routine_Init:
			self->routine = Routine_Touch;
			self->map = Map_Sign;
			self->gfx = GFX_Signpost;
			self->render = ObjRender::LayerNormal;
			self->actWid = 24;
			self->priority = 4;
			// fall through
		case Routine_Touch:
			if(v_player->x >= self->x && v_player->x < self->x + 32)
			{
				PlaySound(SFX::Signpost);
				f_timecount = false;
				v_limitleft2 = v_limitright2;
				self->routine = Routine_Spin;
			}
			break;

		case Routine_Spin:
			if(TimerNeg(VAR_W(self, spinTimeW), Signpost_SpinTime) && IncTo(self->anim, 3))
				self->routine = Routine_SonicRun;

			if(TimerNeg(VAR_W(self, sparkleTimeW), Signpost_SparkleTime))
			{
				auto idx = VAR_B(self, sparkleIdxB);
				VAR_B(self, sparkleIdxB) = (VAR_B(self, sparkleIdxB) + 1) & 7;

				if(auto sparkle = FindFreeObj())
				{
					sparkle->id = ID::Rings;
					sparkle->routine = RingRoutine_Sparkle;
					sparkle->x = self->x + Sign_SparkPos[idx][0];
					sparkle->y = self->y + Sign_SparkPos[idx][1];
					sparkle->map = Map_Ring;
					sparkle->gfx = GFX_Ring;
					sparkle->render = ObjRender::LayerNormal;
					sparkle->priority = 2;
					sparkle->actWid = 8;
				}
			}
			break;

		case Routine_SonicRun:
			if(!v_debuguse)
			{
				if(!PlayerInAir())
				{
					f_lockctrl = true;
					v_jpadhold2 = Buttons::R;
				}

				// WEIRD: Wonder why it checks for the player ID being 0..
				if(v_player->id == 0 || v_player->x >= v_limitright2 + 296)
				{
					self->routine = Routine_Exit;
					GotThroughAct();
				}
			}
			break;

		case Routine_Exit:
			break;
	}

	AnimateSprite(self, Ani_Sign);
	DisplaySprite(self);

	if(Object_OutOfRange(self))
		DeleteObject(self);
}

const ushort TimeBonuses[] =
{
	5000, 5000, 1000, 500, 400, 400, 300, 300, 200, 200,
	200, 200, 100, 100, 100, 100, 50, 50, 50, 50, 0
};

// Subroutine to set up bonuses at the end of an act
void GotThroughAct()
{
	if(v_objspace[23].id == 0)
	{
		v_limitleft2 = v_limitright2;
		v_invinc = 0;
		f_timecount = false;

		v_objspace[23].id = ID::GotThroughCard;
		NewPLC(PLC::TitleCard);
		f_endactbonus = true;
		v_timebonus = TimeBonuses[min(((v_timemin * 60) + v_timesec) / 15, 20)];
		v_ringbonus = v_rings * 10;
		PlaySound_Special(BGM::GotThrough);
	}
}