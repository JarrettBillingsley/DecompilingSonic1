// =====================================================================================================================
// Object 21 - SCORE, TIME, RINGS
// Displays the "SCORE" "TIME" "RINGS" sprites, but doesn't actually display the numbers. There's a little bug, too.
// =====================================================================================================================

enum : ubyte
{
	Routine_Init = 0,
	Routine_Main = 2,
};

enum : ushort
{
	GFX_HUD = 0x06CA,

	HUD_ScreenX = 144,
	HUD_ScreenY = 264,
};

const ubyte Map_HUD[][] =
{
	{0xA, 0x80, 0xD, 0x80, 0, 0, // allyellow
		0x80, 0xD, 0x80, 0x18, 0x20,
		0x80, 0xD, 0x80, 0x20, 0x40,
		0x90, 0xD, 0x80, 0x10, 0,
		0x90, 0xD, 0x80, 0x28, 0x28,
		0xA0, 0xD, 0x80, 8, 0,
		0xA0, 1, 0x80, 0, 0x20,
		0xA0, 9, 0x80, 0x30, 0x30,
		0x40, 5, 0x81, 0xA, 0,
		0x40, 0xD, 0x81, 0xE, 0x10},

	{0xA, 0x80, 0xD, 0x80, 0, 0, // ringred
		0x80, 0xD, 0x80, 0x18, 0x20,
		0x80, 0xD, 0x80, 0x20, 0x40,
		0x90, 0xD, 0x80, 0x10, 0,
		0x90, 0xD, 0x80, 0x28, 0x28,
		0xA0, 0xD, 0xA0, 8, 0,
		0xA0, 1, 0xA0, 0, 0x20,
		0xA0, 9, 0x80, 0x30, 0x30,
		0x40, 5, 0x81, 0xA, 0,
		0x40, 0xD, 0x81, 0xE, 0x10},

	{0xA, 0x80, 0xD, 0x80, 0, 0, // timered
		0x80, 0xD, 0x80, 0x18, 0x20,
		0x80, 0xD, 0x80, 0x20, 0x40,
		0x90, 0xD, 0xA0, 0x10, 0,
		0x90, 0xD, 0x80, 0x28, 0x28,
		0xA0, 0xD, 0x80, 8, 0,
		0xA0, 1, 0x80, 0, 0x20,
		0xA0, 9, 0x80, 0x30, 0x30,
		0x40, 5, 0x81, 0xA, 0,
		0x40, 0xD, 0x81, 0xE, 0x10},

	{0xA, 0x80, 0xD, 0x80, 0, 0, // allred
		0x80, 0xD, 0x80, 0x18, 0x20,
		0x80, 0xD, 0x80, 0x20, 0x40,
		0x90, 0xD, 0xA0, 0x10, 0,
		0x90, 0xD, 0x80, 0x28, 0x28,
		0xA0, 0xD, 0xA0, 8, 0,
		0xA0, 1, 0xA0, 0, 0x20,
		0xA0, 9, 0x80, 0x30, 0x30,
		0x40, 5, 0x81, 0xA, 0,
		0x40, 0xD, 0x81, 0xE, 0x10},
};

void HUD(Object* self)
{
	switch(self->routine)
	{
		case Routine_Init:
			self->routine = Routine_Main;
			self->x = HUD_ScreenX;
			self->screenY = HUD_ScreenY;
			self->map = Map_HUD;
			self->gfx = GFX_HUD;
			Obj_SetLayerScreen(self);
			self->priority = 0;
			// fall through
		case Routine_Main:
			// BUG: this always makes both counters yellow if you have rings, even if the time is running out.
			// Animation frame 2 is never used. This was fixed in Sonic 2!
			if(v_rings == 0 && BTST(v_framecount, 0x08) == 0)
				self->frame = (v_timemin == 9) ? 3 : 1;
			else
				self->frame = 0;

			DisplaySprite(self);
			break;
	}
}

//                   d0
void AddPoints(int points)
{
	f_scorecount = true;

	v_score += d0;

	if(v_score > 999999)
		v_score = 999999;

	if(v_score >= v_scorelife)
	{
		v_scorelife += 5000; // really 50,000

		if(!(v_megadrive & 0x80))
		{
			v_lives++;
			f_lifecount = true;
			PlaySound_Special(BGM_ExtraLife);
		}
	}
}