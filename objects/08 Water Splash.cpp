// =====================================================================================================================
// Object 08 - water splash (LZ)
// The splash at the surface when you enter/leave the water. Just displays itself and then dies.
// =====================================================================================================================

enum : ubyte
{
	Routine_Init = 0,
	Routine_Main = 2,
	Routine_Delete = 4,
};

enum : ushort
{
	GFX_Splash = 0x4259,
};

const ubyte Map_Splash[][] =
{
	{2, 0xF2, 4, 0, 0x6D, 0xF8,   // splash1
		0xFA, 0xC, 0, 0x6F, 0xF0},
	{2, 0xE2, 0, 0, 0x73, 0xF8,   // splash2
		0xEA, 0xE, 0, 0x74, 0xF0},
	{1, 0xE2, 0xF, 0, 0x80, 0xF0}, // splash3
};

const ubyte Ani_Splash[][] =
{
	{ 4, 0, 1, 2, AnimFlags_Routine },
};

void Splash(Object* self)
{
	switch(self->routine)
	{
		case Routine_Init:
			self->routine = Routine_Main;
			self->map = Map_Splash;
			Obj_SetSomething(self);
			self->priority = 1;
			self->obActWid = 16;
			self->gfx = GFX_Splash;
			self->x = v_player->x;
			// fall through
		case Routine_Main:
			self->y = v_waterpos1;
			AnimateSprite(self, Ani_Splash)
			DisplaySprite(self);
			break;

		case Routine_Delete:
			DeleteObject(self);
			break;
	}
}