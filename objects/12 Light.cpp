// =====================================================================================================================
// Object 12 - Lamp (SYZ)
// Extremely simple little decoration object.
// =====================================================================================================================

enum : ubyte
{
	Routine_Init = 0,
	Routine_Main = 2,
};

enum : ushort
{
	GFX_Light = 0,
};

const ubyte Map_Light[] =
{
	{2, 0xF8, 0xC, 0, 0x31, 0xF0,
		0, 0xC, 0x10, 0x31, 0xF0},
	{2, 0xF8, 0xC, 0, 0x35, 0xF0,
		0, 0xC, 0x10, 0x35, 0xF0},
	{2, 0xF8, 0xC, 0, 0x39, 0xF0,
		0, 0xC, 0x10, 0x39, 0xF0},
	{2, 0xF8, 0xC, 0, 0x3D, 0xF0,
		0, 0xC, 0x10, 0x3D, 0xF0},
	{2, 0xF8, 0xC, 0, 0x41, 0xF0,
		0, 0xC, 0x10, 0x41, 0xF0},
	{2, 0xF8, 0xC, 0, 0x45, 0xF0,
		0, 0xC, 0x10, 0x45, 0xF0},
};

void SpinningLight(Object* self)
{
	switch(self->routine)
	{
		case Routine_Init:
			self->routine = Routine_Main;
			self->map = Map_Light;
			self->gfx = GFX_Light;
			Obj_SetLayerNormal(self);
			self->actWid = 16;
			self->priority = 6;
			// fall through
		case Routine_Main:
			if(TimerNeg(self->timeFrame, 7))
				self->frame = (self->frame == 5) ? 0 : self->frame + 1;

			if(Object_OutOfRange(self))
				DeleteObject(self);
			else
				DisplaySprite(self);
			break;
	}
}