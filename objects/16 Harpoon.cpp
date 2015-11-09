// =====================================================================================================================
// Object 16 - harpoon (LZ)
// Very simple, just goes in and out. Subtype 0 is horizontal, subtype 2 is vertical.
// =====================================================================================================================

enum : ubyte
{
	harpTimeW = 0x30, // time between stabbing/retracting, in frames

	Routine_Init = 0,
	Routine_Main = 2,
	Routine_Wait = 4,
};

enum : ushort
{
	GFX_Harpoon = 0x3CC,
};

const ubyte Ani_Harp[][] =
{
	{ 3, 1, 2, AnimFlags_Routine }, // horiz_extending
	{ 3, 1, 0, AnimFlags_Routine }, // horiz_retracting
	{ 3, 4, 5, AnimFlags_Routine }, // vert_extending
	{ 3, 4, 3, AnimFlags_Routine }, // vert_retracting
};

const ubyte Map_Harp[][] =
{
	{1, 0xFC, 4, 0, 0, 0xF8 },   // h_retracted
	{1, 0xFC, 0xC, 0, 2, 0xF8 }, // h_middle
	{2, 0xFC, 8, 0, 6, 0xF8,     // h_extended
		0xFC, 8, 0, 3, 0x10 },
	{1, 0xF8, 1, 0, 9, 0xFC },   // v_retracted
	{1, 0xE8, 3, 0, 0xB, 0xFC }, // v_middle
	{2, 0xD8, 2, 0, 0xB, 0xFC,   // v_extended
		0xF0, 2, 0, 0xF, 0xFC },
};

const ubyte ColTypeByFrame = { 0x9B, 0x9C, 0x9D, 0x9E, 0x9F, 0xA0 };

void Harpoon(Object* self)
{
	switch(self->routine)
	{
		case Routine_Init:
			self->routine = Routine_Main;
			self->map = Map_Harp;
			self->gfx = GFX_Harpoon;
			Obj_SetLayerNormal(self);
			self->priority = 4;
			self->anim = self->subtype; // vert/horiz;
			self->actWid = 20;
			// fall through
		case Routine_Main:
			AnimateSprite(self, Ani_Harp);
			self->colType = ColTypeByFrame[self->frame];
			RememberState(self);
			break;

		case Routine_Wait:
			if(TimerNeg(VAR_W(self, harpTimeW), 60))
			{
				self->routine = Routine_Main;
				self->anim ^= 1;
			}

			RememberState(self);
			break;
	}
}