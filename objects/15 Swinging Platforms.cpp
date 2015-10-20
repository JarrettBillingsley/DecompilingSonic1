// =====================================================================================================================
// Object 15 - Swinging things on chains
// This object can be one of several different objects depending on its subtype and what zone it spawns in. It can be:
//   - Swinging platforms in GHZ and MZ
//   - Robotnik's boss ball-and-chain in GHZ (if subtype >= 16)
//   - Swinging spike-bottomed platforms in SLZ
//   - Swinging spikey balls in SBZ
//
// This object acts as the "master" object, and although its coordinates are the base of the chain, the master object
// is actually the thing at the end of the chain. The chain links are sub-objects (which use the same ID but a different
// routine).
// =====================================================================================================================

enum : ubyte
{
	baseYW = 0x38,          // y coord of the base of the chain
	baseXW = 0x3A,          // x coord of the base of the chain
	distFromBaseB = 0x3C,   // for each chain link, the distance from the base (used to make them move in an arc)
	swingingLeftB = 0x3D,   // if 1, the swing speed accelerates left; if 0, right
	swingSpeedW = 0x3E,     // the swing speed, which accelerates left and right

	Routine_Init = 0,       // derp.
	Routine_Main = 2,       // Main platform routine (transitions to StoodOn as usual)
	Routine_StoodOn = 4,    // Being stood on by the player (transitions to Main as usual)
	Routine_Delete = 6,     // Deleting
	Routine_Delete2 = 8,    // Deleting again (this convention seems to happen with platforms..)
	Routine_ChainLink = 10, // Chain link sub-objects use this routine to just display themselves
	Routine_Swinging = 12,  // Almost the same as Routine_Main but doesn't act as platform (used for SBZ spike balls)
};

enum : ushort
{
	GFX_Swing_GHZ = 0x4380, // GHZ platform
	GFX_Swing_SLZ = 0x43DC, // SLZ spikey platform
	GFX_BBall = 0x0391,     // SBZ spikeball
	GFX_GBall = 0x43AA,     // Robotnik boss ball
};

const ubyte Map_Swing_GHZ[][] =
{
	{2, 0xF8, 9, 0, 4, 0xE8       // block
		0xF8, 9, 0, 4, 0},
	{1, 0xF8, 5, 0, 0, 0xF8},     // chain
	{1, 0xF8, 5, 0, 0xA, 0xF8},   // anchor
};

const ubyte Map_Swing_SLZ[][] =
{
	{8, 0xF0, 0xF, 0, 4, 0xE0,    // block
		0xF0, 0xF, 8, 4, 0,
		0xF0, 5, 0, 0x14, 0xD0,
		0xF0, 5, 8, 0x14, 0x20,
		0x10, 4, 0, 0x18, 0xE0,
		0x10, 4, 8, 0x18, 0x10,
		0x10, 1, 0, 0x1A, 0xF8,
		0x10, 1, 8, 0x1A, 0},
	{1, 0xF8, 5, 0x40, 0, 0xF8},  // chain
	{1, 0xF8, 5, 0, 0x1C, 0xF8},  // anchor
};

const ubyte Map_BBall[][] =
{
	{5, 0xE8, 4, 0, 0, 0xF8,      // ball
		0xF0, 0xF, 0, 2, 0xF0,
		0xF8, 1, 0, 0x12, 0xE8,
		0xF8, 1, 0, 0x14, 0x10,
		0x10, 4, 0, 0x16, 0xF8},
	{1, 0xF8, 5, 0, 0x20, 0xF8},  // chain
	{2, 0xF8, 0xD, 0, 0x18, 0xF0, // anchor
		0xE8, 0xD, 0x10, 0x18, 0xF0},
};

const ubyte Map_GBall[][] =
{
	{6, 0xF0, 4, 0, 0x24, 0xF0,   // shiny
		0xF8, 4, 0x10, 0x24, 0xF0,
		0xE8, 0xA, 0, 0, 0xE8,
		0xE8, 0xA, 8, 0, 0,
		0, 0xA, 0x10, 0, 0xE8,
		0, 0xA, 0x18, 0, 0},
	{4, 0xE8, 0xA, 0, 9, 0xE8,    // check1
		0xE8, 0xA, 8, 9, 0,
		0, 0xA, 0x10, 9, 0xE8,
		0, 0xA, 0x18, 9, 0},
	{4, 0xE8, 0xA, 0, 0x12, 0xE8, // check2
		0xE8, 0xA, 0, 0x1B, 0,
		0, 0xA, 0x18, 0x1B, 0xE8,
		0, 0xA, 0x18, 0x12, 0},
	{4, 0xE8, 0xA, 8, 0x1B, 0xE8, // check3
		0xE8, 0xA, 8, 0x12, 0,
		0, 0xA, 0x10, 0x12, 0xE8,
		0, 0xA, 0x10, 0x1B, 0},
};

void SwingingPlatform(Object* self)
{
	switch(self->routine)
	{
		case Routine_Init:
			self->routine = Routine_Main;
			self->render = ObjRender::LayerNormal;
			self->priority = 3;
			VAR_W(self, baseYW) = self->y;
			VAR_W(self, baseXW) = self->x;

			// Set up graphics and collision data.
			if(v_zone == Zone::SLZ)
			{
				// Set up SLZ spikey swinging platform
				self->map = Map_Swing_SLZ;
				self->gfx = GFX_Swing_SLZ;
				self->actWid = 32;
				self->height = 16;
				self->colType = 0x99; // hurty, 32x8
			}
			else if(v_zone == Zone::SBZ)
			{
				// Set up SBZ spike ball
				self->map = Map_BBall;
				self->gfx = GFX_BBall;
				self->actWid = 24;
				self->height = 24;
				self->colType = 0x86; // hurty, 16x16
				self->routine = Routine_Swinging;
			}
			else
			{
				// Set up GHZ swinging platform
				self->map = Map_Swing_GHZ;
				self->gfx = GFX_Swing_GHZ;
				self->actWid = 24;
				self->height = 8;
			}

			// Create the chain links
			auto isBossBall = self->subtype & 0x10;
			auto numLinks = self->subtype & 0xF;
			auto distFromBase = numLinks * 16;
			VAR_B(self, distFromBaseB) = distFromBase + 8;

			if(self->frame != 0) // ?? how is this possible?
			{
				distFromBase += 8;
				numLinks--;
			}

			self->subtype = 0; // from now on, subtype stores the count of links actually created
			int chainIdx = 0x29;

			for(; numLinks >= 0; numLinks--)
			{
				auto chain = FindFreeObj();

				if(chain == nullptr)
					break;

				self->subtype++;
				VAR_B(self, chainIdx++) = Object_PointerToSlot(chain);
				chain->routine = Routine_ChainLink;
				chain->id = self->id;
				chain->map = self->map;
				chain->gfx = self->gfx;
				BCLR(chain->gfx, 1 << 6);
				chain->render = ObjRender::LayerNormal;
				chain->priority = 4;
				chain->actWid = 8;
				chain->frame = 1;
				VAR_B(chain, distFromBaseB) = distFromBase
				distFromBase -= 16;

				if(distFromBase < 0) // bcc
				{
					chain->frame = 2;
					chain->priority = 3;
					chain->gfx = self->gfx;
				}
			}

			// Insert self in liast of objects
			VAR_B(self, chainIdx++) = Object_PointerToSlot(self);

			// Set up the swinging (only used for the boss ball)
			self->angle = 0x4080
			VAR_W(self, swingSpeedW) = -0x200;

			// For the boss ball.. feels kinda bodgy to have this down here, don't you think?
			if(isBossBall)
			{
				self->map = Map_GBall;
				self->gfx = GFX_GBall;
				self->frame = 1;
				self->priority = 2;
				self->colType = 0x81; // 20x20, hurty
			}

			if(v_zone == Zone::SBZ)
				goto _main;
			// fall through
		case Routine_Main:
			// Act like a platform
			Swing_Solid(self, self->actWid, self->height);
			// fall through
		case Routine_Swinging:
		_main:
			// Actually do the swinging
			Swing_Move(self);
			DisplaySprite(self);

			if(Swing_ChkDel(self))
				return;
			// fall through
		case Routine_StoodOn: {
			// Check if Sonic is leaving the platform
			int diffX;
			ExitPlatform(self, self->actWid, diffX);
			auto oldX = self->x;
			Swing_Move(self);
			MvSonicOnPtfm(self, oldX, self->height + 1);
			DisplaySprite(self);

			if(Swing_ChkDel(self))
				return;
			break;
		}
		case Routine_Delete:
		case Routine_Delete2:
			DeleteObject(self);
			break;

		case Routine_ChainLink:
			DisplaySprite(self);
			break;
	}
}

// Make platforms behave like platforms.
//                                   d1          d3
void Swing_Solid(Object* self, int hwidth, int height)
{
	if(v_player->velY >= 0)
	{
		auto diffX = v_player->x - self->x + hwidth;

		if(diffX < 0 || diffX >= (hwidth * 2))
			return;

		Plat_NoXCheck2(self, self->y - height);
	}
}

// Make non-boss-ball objects swing
void Swing_Move(Object* self)
{
	// They swing on a global timer.
	auto angle = v_oscillate[26];

	if(BTST(self->status, ObjStatus::Flip))
		angle = -angle + 0x80;

	Swing_PositionLinks(self, angle);
}

// Make the boss ball swing
void BossBall_Move(Object* self)
{
	// Pretty simple logic, just accelerate left and right in alternation

	if(VAR_B(self, swingingLeftB) == 0)
	{
		VAR_W(self, swingSpeedW) += 8;
		self->angle += VAR_W(self, swingSpeedW);

		if(VAR_W(self, swingSpeedW) == 0x200)
			VAR_B(self, swingingLeftB) = 1;
	}
	else
	{
		VAR_W(self, swingSpeedW) -= 8;
		self->angle += VAR_W(self, swingSpeedW)

		if(VAR_W(self, swingSpeedW) == -0x200)
			VAR_B(self, swingingLeftB) = 0;
	}

	Swing_PositionLinks(self, self->angle);
}

// Position the chain links between the base and the object.
void Swing_PositionLinks(Object* self, int angle)
{
	int cosine;
	auto sine = CalcSine(angle, &cosine);
	auto baseY = VAR_W(self, baseYW);
	auto baseX = VAR_W(self, baseXW);

	for(int i = 0; i < self->subtype; i++)
	{
		auto chain = v_objspace[VAR_B(self, i + 0x29)];
		chain->y = ((VAR_B(chain, distFromBaseB) * sine) >> 8) + baseY;
		chain->x = ((VAR_B(chain, distFromBaseB) * cosine) >> 8) + baseX;
	}
}

// Check if the object has moved offscreen and delete it (and its links) if so.
bool Swing_ChkDel(Object* self)
{
	if(Object_OutOfRange(self, VAR_W(self, baseXW)))
	{
		for(int i = 0; i < self->subtype; i++)
			DeleteObject(v_objspace[VAR_B(self, i + 0x29)]);

		return true;
	}

	return false;
}
