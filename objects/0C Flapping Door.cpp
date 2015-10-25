// =====================================================================================================================
// Object 0C - flapping door (LZ)
// Pretty simple object, the doors on the entrance to the water tunnels in LZ. Flap open and closed and suck Sonic into
// the tunnels.
// =====================================================================================================================

enum : ubyte
{
	flapTimeW = 0x32, // time between opening/closing
	flapWaitW = 0x30, // time until change

	Routine_Init = 0,
	Routine_Main = 2,
};

enum : ushort
{
	GFX_FlapDoor = 0x4328,
};

const ubyte Ani_Flap[][] =
{
	{ 3, 0, 1, 2, AnimFlags::Back, 1 }, // opening
	{ 3, 2, 1, 0, AnimFlags::Back, 1 }, // closing
};

const ubyte Map_Flap[][] =
{
	{2, 0xE0, 7, 0, 0, 0xF8,   // closed
		0, 7, 0x10, 0, 0xF8},
	{2, 0xDA, 0xF, 0, 8, 0xFB, // halfway
		6, 0xF, 0x10, 8, 0xFB},
	{2, 0xD8, 0xD, 0, 0x18, 0, // open
		0x18, 0xD, 0x10, 0x18, 0}
};

void FlapDoor(Object* self)
{
	switch(self->routine)
	{
		case Routine_Init:
			self->routine = Routine_Main;
			self->map = Map_Flap;
			self->gfx = GFX_FlapDoor;
			self->render |= ObjRender::LayerNormal;
			self->actWid = 40;
			VAR_W(self, flapTimeW) = self->subtype * 60;
			// fall through
		case Routine_Main:
			if(TimerNeg(VAR_W(self, flapWaitW), VAR_W(self, flapTimeW)))
			{
				self->anim ^= 1;

				if(Object_IsVisible(self))
					PlaySound_Special(SFX::Door);
			}

			AnimateSprite(self, Ani_Flap);
			f_wtunnelallow = false;

			if(self->frame == 0 && v_player->x < self->x)
			{
				f_wtunnelallow = true;
				SolidObject(self, 19, 32, 33, self->x);
			}

			RememberState(self);
			break;
	}
}