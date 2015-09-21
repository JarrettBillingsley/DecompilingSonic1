// =====================================================================================================================
// Object 0F - "PRESS START BUTTON" and "TM" from title screen
// =====================================================================================================================

enum : ubyte
{
	Routine_Init = 0,
	Routine_PrsStart = 2,
	Routine_Exit = 4,

	Frame_SpriteLineLimiter = 2,
	Frame_TM = 3,
};

enum : ushort
{
	PSB_ScreenX = 128 + 80,
	PSB_ScreenY = 128 + 176,

	TM_ScreenX = 128 + 240,
	TM_ScreenY = 128 + 120,

	GFX_PSB = 0x0200,
	GFX_TM = 0x2510,
};

const ubyte Ani_PSBTM[][] =
{
	{ 31, 0, 1, AnimFlags::End }, // flash
};

const ubyte Map_PSB[][] =
{
	{0},                        // disappear

	{6, 0, 0xC, 0, 0xF0, 0,     // PRESS START BUTTON
		0, 0, 0, 0xF3, 0x20,
		0, 0, 0, 0xF3, 0x30,
		0, 0xC, 0, 0xF4, 0x38,
		0, 8, 0, 0xF8, 0x60,
		0, 8, 0, 0xFB, 0x78},

	{31, 0xB8, 0xF, 0, 0, 0x80, // sprite line limiter
		0xB8, 0xF, 0, 0, 0x80,
		0xB8, 0xF, 0, 0, 0x80,
		0xB8, 0xF, 0, 0, 0x80,
		0xB8, 0xF, 0, 0, 0x80,
		0xB8, 0xF, 0, 0, 0x80,
		0xB8, 0xF, 0, 0, 0x80,
		0xB8, 0xF, 0, 0, 0x80,
		0xB8, 0xF, 0, 0, 0x80,
		0xB8, 0xF, 0, 0, 0x80,
		0xD8, 0xF, 0, 0, 0x80,
		0xD8, 0xF, 0, 0, 0x80,
		0xD8, 0xF, 0, 0, 0x80,
		0xD8, 0xF, 0, 0, 0x80,
		0xD8, 0xF, 0, 0, 0x80,
		0xD8, 0xF, 0, 0, 0x80,
		0xD8, 0xF, 0, 0, 0x80,
		0xD8, 0xF, 0, 0, 0x80,
		0xD8, 0xF, 0, 0, 0x80,
		0xD8, 0xF, 0, 0, 0x80,
		0xF8, 0xF, 0, 0, 0x80,
		0xF8, 0xF, 0, 0, 0x80,
		0xF8, 0xF, 0, 0, 0x80,
		0xF8, 0xF, 0, 0, 0x80,
		0xF8, 0xF, 0, 0, 0x80,
		0xF8, 0xF, 0, 0, 0x80,
		0xF8, 0xF, 0, 0, 0x80,
		0xF8, 0xF, 0, 0, 0x80,
		0xF8, 0xF, 0, 0, 0x80,
		0xF8, 0xF, 0, 0, 0x80},

	{1, 0xFC, 4, 0, 0, 0xF8},   // "TM"
};

void PSBTM(Object* self)
{
	switch(self->routine)
	{
		case Routine_Init:
			self->routine = Routine_PrsStart;
			self->x = PSB_ScreenX;
			self->screenY = PSB_ScreenY;
			self->map = Map_PSB;
			self->gfx = GFX_PSB;

			if(self->frame >= Frame_SpriteLineLimiter)
			{
				self->routine = Routine_Exit;

				if(self->frame == Frame_TM)
				{
					self->gfx = GFX_TM;
					self->x = TM_ScreenX;
					self->screenY = TM_ScreenY;
				}
				break;
			}
			// fall through
		case Routine_PrsStart:
			AnimateSprite(self, Ani_PSBTM);
			break;

		case Routine_Exit:
			break;
	}

	DisplaySprite(self);
}