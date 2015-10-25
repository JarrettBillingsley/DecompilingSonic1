// =====================================================================================================================
// Object 14 - lava balls (MZ, SLZ)
// Those fireballs!
// =====================================================================================================================

enum : ubyte
{
	unknownFlagB = 0x29,
	origYW = 0x30,

	Routine_Init = 0,
	Routine_Main = 2,
	Routine_Delete = 4,

	Anim_Vert = 0,
	Anim_VertCollide = 1,
	Anim_Horiz = 2,
	Anim_HorizCollide = 3,

	Subtype_Gravity = 0,
	Subtype_Gravity1 = 1,
	Subtype_Gravity2 = 2,
	Subtype_Gravity3 = 3,
	Subtype_Up = 4,
	Subtype_Down = 5,
	Subtype_Left = 6,
	Subtype_Right = 7,
	Subtype_Dying = 8,
};

enum : ushort
{
	GFX_LavaBall_MZ = 0x345,
	GFX_LavaBall_SLZ = 0x480,

	LavaBall_Gravity = 24,
};

const short Speeds[] = { -0x400, -0x500, -0x600, -0x700, -0x200, 0x200, -0x200, 0x200, 0 };

const ubyte Ani_Fire[][] =
{
	{ 5, 0, 0x20, 1, 0x21, AnimFlags_End }, // vertical
	{ 5, 2, AnimFlags_Routine },            // vertcollide
	{ 5, 3, 0x43, 4, 0x44, AnimFlags_End }, // horizontal
	{ 5, 5, AnimFlags_Routine },            // horicollide
};

const ubyte Map_Fire[][] =
{
	{ 1, 0xE8, 7,   0, 0,    0xF8 }, // vertical1
	{ 1, 0xE8, 7,   0, 8,    0xF8 }, // vertical2
	{ 1, 0xF0, 6,   0, 0x10, 0xF8 }, // vertcollide
	{ 1, 0xF8, 0xD, 0, 0x16, 0xE8 }, // horizontal1
	{ 1, 0xF8, 0xD, 0, 0x1E, 0xE8 }, // horizontal2
	{ 1, 0xF8, 9,   0, 0x26, 0xF0 }, // horicollide
};

void LavaBall(Object* self)
{
	switch(self->routine)
	{
		case Routine_Init:
			self->routine = Routine_Main;
			self->height = 8;
			self->width = 8;
			self->map = Map_Fire;
			self->gfx = (v_zone == Zone_SLZ) ? GFX_LavaBall_SLZ : GFX_LavaBall_MZ;
			self->render = ObjRender_LayerNormal;
			self->priority = 3;
			self->colType = ColClass_Hurt | ColSize_8x8_A;

			VAR_W(self, origYW) = self->y;

			if(VAR_B(self, unknownFlagB)) // ??? this never seems to be set..
				self->priority += 2;

			self->velY = Speeds[self->subtype];
			self->actWid = 8;

			if(self->subtype >= Subtype_Left) // move horizontally instead of vertically
			{
				self->actWid = 16;
				self->anim = Anim_Horiz;
				self->velX = self->velY;
				self->velY = 0;
			}

			PlaySound_Special(SFX_Fireball);
			// fall through
		case Routine_Main:
			switch(self->subtype)
			{
				case Subtype_Gravity:
				case Subtype_Gravity1:
				case Subtype_Gravity2:
				case Subtype_Gravity3:
					// fly up and fall back down
					self->velY += LavaBall_Gravity;

					if(self->y >= VAR_W(self, origYW))
						self->routine = Routine_Delete;

					if(self->velY < 0)
						BSET(self->status, ObjStatus_Air);
					else
						BCLR(self->status, ObjStatus_Air);
					break;

				case Subtype_Up:
					// moves up until it hits the ceiling
					BSET(self->status, ObjStatus_Air);

					if(ObjHitCeiling(self) < 0)
					{
						self->subtype = Subtype_Dying;
						self->anim = Anim_VertCollide;
						self->velY = 0;
					}
					break;

				case Subtype_Down:
					// moves down until it hits the floor
					BCLR(self->status, ObjStatus_Air);

					if(ObjFloorDist(self, &dist, nullptr) < 0)
					{
						self->subtype = Subtype_Dying;
						self->anim = Anim_VertCollide;
						self->velY = 0;
					}
					break;

				case Subtype_Left:
					// moves left until it hits a wall
					BSET(self->status, ObjStatus_Flip);

					if(ObjHitWallLeft(self, -8) < 0)
					{
						self->subtype = Subtype_Dying;
						self->anim = Anim_HorizCollide;
						self->velX = 0;
					}
					break;

				case Subtype_Right:
					// moves right until it hits a wall
					BCLR(self->status, ObjStatus_Flip);

					if(ObjHitWallRight(self, 8) < 0)
					{
						self->subtype = Subtype_Dying;
						self->anim = Anim_HorizCollide;
						self->velX = 0;
					}
					break;

				case Subtype_Dying:
					// "lavaball hit something and is disintegrating" state
					break;
			}

			SpeedToPos(self);
			AnimateSprite(self, Ani_Fire);

			if(Object_OutOfRange(self))
			{
				DeleteObject(self);
				return;
			}
			break;

		case Routine_Delete:
			DeleteObject(self);
			return;
	}

	DisplaySprite(self);
}