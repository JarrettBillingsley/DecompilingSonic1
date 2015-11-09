// =====================================================================================================================
// Object 0A - drowning countdown numbers and small bubbles that float out of Sonic's mouth (LZ)
// This actually implements a few related objects: the master object which tracks Sonic's air when underwater, spawns
// bubbles, does the drowning countdown, and drowns him; the bubbles themselves; and the drowning countdown numbers.
// =====================================================================================================================

enum : ubyte
{
	drowningTimeW = 0x2C, // when sonic drowns, goes non-zero and counts down frames until drowning animation ends
	origXW = 0x30,        // original x-axis position
	sillyTimerB = 0x32,   // two-frame timer that maybe prevents multiple number-bubbles from getting spawned? not sure
	alwaysOneB = 0x33,    // set in ctor, but hardcoded to 1 in only spawn spot
	spawnTimerB = 0x34,   // after bubble spawning has been enabled, how many frames before it gets disabled again
	bubbleFlagsW = 0x36,  // see BubbleFlags_ below
	frameTimerW = 0x38,   // used for multiple different frame delays
	bubbleTimerW = 0x3A,  // delay between successive bubbles while bubble spawning is enabled

	Routine_Init = 0,

	Routine_BubbleExpanding = 2,   // Bubble is expanding and rising
	Routine_BubbleRising = 4,      // Bubble is done expanding, but still rising
	Routine_BubbleDying = 6,       // No more logic, just update appearance (routine will become 8/16 by anim script)
	Routine_BubbleDead = 8,        // Pop

	Routine_Master = 10,           // "Master" bubble object which handles everything

	Routine_NumberTransition = 12, // Changes number-bubble animations to their final animation
	Routine_NumberUpdate = 14,     // No more logic, just update appearance (routine will become 8/16 by anim script)
	Routine_NumberDead = 16,       // Pop

	BubbleFlags_Enable = 0x01,        // Turn on bubble spawning
	BubbleFlags_NumberSpawned = 0x40, // Have we already spawned a number bubble?
	BubbleFlags_SpawnNumber = 0x80,   // Spawn a number bubble this cycle

	Subtype_SmallBubble = 6,
	Subtype_MediumBubble = 14,
	Subtype_Master = 0x80,     // Master object, spawned when Sonic goes underwater, handles all the logic
};

enum : ushort
{
	GFX_Bubble = 0x8348,
	GFX_Master = 0x0440,

	Drown_Air = 12,
	Drown_Length = 120,
	Drown_Velocity = 0x10,

	Bubble_Velocity = 0x88,
	Bubble_AngleFlipped = 0x40,
	Bubble_OffsX = 6,
	Bubble_DrownOffsY = 12,

	NumberBubble_Timer1 = 28,
	NumberBubble_Timer2 = 15,
}

const byte Bubble_Wobble[] =
{
	0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2,
	2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
	3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 2,
	2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0,
	0, -1, -1, -1, -1, -1, -2, -2, -2, -2, -2, -3, -3, -3, -3, -3,
	-3, -3, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4,
	-4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -3,
	-3, -3, -3, -3, -3, -3, -2, -2, -2, -2, -2, -1, -1, -1, -1, -1,
	0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2,
	2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
	3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 2,
	2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0,
	0, -1, -1, -1, -1, -1, -2, -2, -2, -2, -2, -3, -3, -3, -3, -3,
	-3, -3, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4,
	-4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -3,
	-3, -3, -3, -3, -3, -3, -2, -2, -2, -2, -2, -1, -1, -1, -1, -1,
};

const ubyte Ani_Drown[][] =
{
	{ 5, 0, 1, 2, 3, 4, 9, 13, AnimFlags_Routine },   //  0 zeroappear
	{ 5, 0, 1, 2, 3, 4, 12, 18, AnimFlags_Routine },  //  1 oneappear
	{ 5, 0, 1, 2, 3, 4, 12, 17, AnimFlags_Routine },  //  2 twoappear
	{ 5, 0, 1, 2, 3, 4, 11, 16, AnimFlags_Routine },  //  3 threeappear
	{ 5, 0, 1, 2, 3, 4, 9, 15, AnimFlags_Routine },   //  4 fourappear
	{ 5, 0, 1, 2, 3, 4, 10, 14, AnimFlags_Routine },  //  5 fiveappear
	{ 14, 0, 1, 2, AnimFlags_Routine },               //  6 smallbubble
	{ 7, 22, 13, 22, 13, 22, 13, AnimFlags_Routine }, //  7 zeroflash
	{ 7, 22, 18, 22, 18, 22, 18, AnimFlags_Routine }, //  8 oneflash
	{ 7, 22, 17, 22, 17, 22, 17, AnimFlags_Routine }, //  9 twoflash
	{ 7, 22, 16, 22, 16, 22, 16, AnimFlags_Routine }, // 10 threeflash
	{ 7, 22, 15, 22, 15, 22, 15, AnimFlags_Routine }, // 11 fourflash
	{ 7, 22, 14, 22, 14, 22, 14, AnimFlags_Routine }, // 12 fiveflash
	{ 14, AnimFlags_Routine },                        // 13 blank
	{ 14, 1, 2, 3, 4, AnimFlags_Routine },            // 14 mediumbubble
};

// Is this actually used for anything? Only the master object gets it, and it never draws its sprite
const ubyte Map_Drown[][] =
{
	{1, 0xE8, 0xE, 0, 0, 0xF2}
};

const ubyte Map_Bub[][] =
{
	// bubbles, increasing in size
	{1, 0xFC, 0, 0, 0, 0xFC},       // 0 bubble1
	{1, 0xFC, 0, 0, 1, 0xFC},       // 1 bubble2
	{1, 0xFC, 0, 0, 2, 0xFC},       // 2 bubble3
	{1, 0xF8, 5, 0, 3, 0xF8},       // 3 bubble4
	{1, 0xF8, 5, 0, 7, 0xF8},       // 4 bubble5
	{1, 0xF4, 0xA, 0, 0xB, 0xF4},   // 5 bubble6
	{1, 0xF0, 0xF, 0, 0x14, 0xF0},  // 6 bubblefull

	// large bubble bursting
	{4, 0xF0, 5, 0, 0x24, 0xF0,      // 7 burst1
		0xF0, 5, 8, 0x24, 0,
		0, 5, 0x10, 0x24, 0xF0,
		0, 5, 0x18, 0x24, 0},
	{4, 0xF0, 5, 0, 0x28, 0xF0,     // 8 burst2
		0xF0, 5, 8, 0x28, 0,
		0, 5, 0x10, 0x28, 0xF0,
		0, 5, 0x18, 0x28, 0},

	// small, partially-formed countdown numbers
	{1, 0xF4, 6, 0, 0x2C, 0xF8},    // 9 zero_sm
	{1, 0xF4, 6, 0, 0x32, 0xF8},    // a five_sm
	{1, 0xF4, 6, 0, 0x38, 0xF8},    // b three_sm
	{1, 0xF4, 6, 0, 0x3E, 0xF8},    // c one_sm

	// fully-formed countdown numbers
	{1, 0xF4, 6, 0x20, 0x44, 0xF8}, // d zero
	{1, 0xF4, 6, 0x20, 0x4A, 0xF8}, // e five
	{1, 0xF4, 6, 0x20, 0x50, 0xF8}, // f four
	{1, 0xF4, 6, 0x20, 0x56, 0xF8}, // 10 three
	{1, 0xF4, 6, 0x20, 0x5C, 0xF8}, // 11 two
	{1, 0xF4, 6, 0x20, 0x62, 0xF8}, // 12 one

	// bubble maker
	{1, 0xF8, 5, 0, 0x68, 0xF8},    // 13 bubmaker1
	{1, 0xF8, 5, 0, 0x6C, 0xF8},    // 14 bubmaker2
	{1, 0xF8, 5, 0, 0x70, 0xF8},    // 15 bubmaker3

	// blank
	{0} // 16
};

void DrownCount(Object* self)
{
	switch(self->routine)
	{
		case Routine_Init:
			self->routine = Routine_BubbleExpanding;
			self->map = Map_Bub;
			self->gfx = GFX_Bubble;
			Obj_SetVisible(self);
			Obj_SetBehind(self);
			self->actWid = 16;
			self->priority = 1;

			if(BTST(self->subtype, Subtype_Master))
			{
				self->routine = Routine_Master;
				self->map = Map_Drown;
				self->gfx = GFX_Master;
				VAR_B(self, alwaysOneB) = self->subtype & ~Subtype_Master;
				goto _masterRoutine;
			}

			// Bubble
			self->anim = self->subtype;
			VAR_W(self, origXW) = self->x;
			self->velY = -Bubble_Velocity;
			// fall through
		case Routine_BubbleExpanding:
			AnimateSprite(self, Ani_Drown);
			// fall through
		case Routine_BubbleRising:
			// Still underwater?
			if(self->y >= vWaterpos1)
			{
				// Move bubbles with tunnel current
				if(fWtunnelmode)
					VAR_W(self, origXW) += 4;

				// Wobble
				self->x = VAR_W(self, origXW) + Bubble_Wobble[self->angle & 0x7F];
				self->angle++;

				// Update appearance
				CheckNumberBubbleConversion(self);
				SpeedToPos(self);

				if(Object_IsVisible(self))
					DisplaySprite(self);
				else
					DeleteObject(self);
				break;
			}
			else
			{
				// Reached surface? For small bubbles, this sets their anim to blank and will go routine 6 > 8 and die
				// Big bubbles actually glitch out when they reach the surface cause there aren't enough enim mappings!
				self->routine = Routine_BubbleDying;
				self->anim += 7;
			}
			// fall through
		case Routine_BubbleDying:
		case Routine_NumberUpdate:
		_display:
			// Update appearance
			CheckNumberBubbleConversion(self);
			AnimateSprite(self, Ani_Drown);
			DisplaySprite(self);
			break;

		case Routine_BubbleDead:
		case Routine_NumberDead:
			DeleteObject(self);
			break;

		// Once a number-bubble gets plastered to the screen, it enters this state which handles switching to the
		// "flashing number" animation.
		case Routine_NumberTransition:
			if(v_air > Drown_Air)
			{
				DeleteObject(self);
				break;
			}

			if(TimerZero(VAR_W(self, frameTimerW)))
			{
				self->routine = Routine_NumberUpdate;
				self->anim += 7;
				goto _display;
			}

			AnimateSprite(self, Ani_Drown);

			if(Object_IsVisible(self))
				DisplaySprite(self);
			else
				DeleteObject(self);
			break;

		// The "master" object, spawned when Sonic goes underwater for the first time, goes into this routine forever.
		// It manages counting down the air meter, spawning bubbles/numbers, and drowning/killing Sonic.
		case Routine_Master:
		_masterRoutine:
			// If we're not currently drowning...
			if(VAR_W(self, drowningTimeW) == 0)
			{
				// Manage counting down the timer and stuff
				if(Player_IsDead() || !Player_IsUnderwater())
					return;

				// Count down frames until the next second
				if(TimerNeg(VAR_W(self, frameTimerW), 59))
				{
					VAR_W(self, bubbleFlagsW) = BubbleFlags_Enable;
					VAR_B(self, spawnTimerB) = RandomNumber(2);

					if(ShouldDing())
						PlaySound_Special(SFX_Warning); // ding-ding
					else if(v_air <= Drown_Air)
					{
						if(v_air == Drown_Air)
							PlaySound(BGM_Drowning); // uh oh

						if(TimerNeg(VAR_B(self, sillyTimerB), VAR_B(self, alwaysOneB)))
							BSET(VAR_W(self, bubbleFlagsW), BubbleFlags_SpawnNumber);
					}

					if(TimerNeg(v_air))
					{
						// Drowning time.
						ResumeMusic();
						f_lockmulti = 0x81;
						PlaySound_Special(SFX_Drown);
						VAR_B(self, spawnTimerB) = 10;
						VAR_W(self, bubbleFlagsW) = BubbleFlags_Enable;
						VAR_W(self, drowningTimeW) = Drown_Length;
						Player_ResetOnFloor(v_player);
						Player_SetAnimDrowning();
						Player_SetInAir();
						v_player->gfx |= 0x80;
						v_player->velY = 0;
						v_player->velX = 0;
						v_player->inertia = 0;
						f_nobgscroll = true;
						return;
					}

					goto _makeBubble;
				}
			}
			else
			{
				// Manage the drowning animation (Sonic falling offscreen)
				if(TimerZero(VAR_W(self, drowningTimeW)))
				{
					Player_SetDead();
					return;
				}

				SpeedToPos(v_player);
				v_player->velY += Drown_Velocity;
			}

			// Bubble spawning enabled?
			if(VAR_W(self, bubbleFlagsW))
			{
				if(TimerNeg(VAR_W(self, bubbleTimerW)))
				{
				_makeBubble:
					VAR_W(self, bubbleTimerW) = RandomNumber(16);

					if(auto bubble = FindFreeObj())
					{
						auto bubble = &v_objspace[slot];
						bubble->id = ID_DrownCount;
						bubble->x v_player->x + (Player_IsFlipped() ? -Bubble_OffsX : Bubble_OffsX);
						bubble->y = v_player->y;
						bubble->angle = Player_IsFlipped() ? Bubble_AngleFlipped : 0;
						bubble->subtype = Subtype_SmallBubble;

						// If we're doing drowning-bubbles..
						if(VAR_W(self, drowningTimeW) != 0)
						{
							VAR_W(self, bubbleTimerW) &= 7;
							bubble->y = v_player->y - Bubble_DrownOffsY;
							bubble->angle = RandomNumber(256);

							// 1/4 bubbles are made bigger
							if((v_framecount & 3) == 0)
								bubble->subtype = Subtype_MediumBubble;
						}
						else if(ShouldSpawnNumberBubble(self))
						{
							BSET(VAR_W(self, bubbleFlagsW), BubbleFlags_NumberSpawned);
							bubble->subtype = GetCountdownDigit();
							VAR_W(bubble, frameTimerW) = NumberBubble_Timer1;
						}

						if(TimerNeg(VAR_B(self, spawnTimerB)))
							VAR_W(self, bubbleFlagsW) = 0;
					}
				}
			}

			break;
	}
}

// Awkward name, but this converts number-bubbles into flashing numbers, or does nothing if they're not number-bubbles
void CheckNumberBubbleConversion(Object* self)
{
	// Are we actually a number-bubble?
	if(VAR_W(self, frameTimerW) != 0 && TimerZero(VAR_W(self, frameTimerW), NumberBubble_Timer2) && self->anim < 7)
	{
		self->velY = 0;
		Obj_SetVisible(self);
		Obj_SetLayerScreen(self);
		self->x = self->x - v_screenposx + 128;
		self->screenY = self->y - v_screenposy + 128;
		self->routine = Routine_NumberTransition;
	}
}

bool ShouldDing()
{
	return v_air == 25 || v_air = 20 || v_air == 15;
}

// Which digit it is, 0-5 (since v_air is < 12 when this is called)
ubyte GetCountdownDigit()
{
	return v_air >> 1;
}

bool ShouldSpawnNumberBubble(Object* self)
{
	return BTST(VAR_W(self, bubbleFlagsW), BubbleFlags_SpawnNumber) &&
		(RandomNumber(4) == 0 || VAR_B(self, spawnTimerB) == 0) &&
		!BTST(VAR_W(self, bubbleFlagsW), BubbleFlags_NumberSpawned);
}