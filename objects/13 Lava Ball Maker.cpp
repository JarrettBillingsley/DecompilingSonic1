// =====================================================================================================================
// Object 13 - lava ball maker (MZ, SLZ)
// Spawns lavaballs. The upper nybble of the subtype selects the delay between lavaballs; the lower nybble selects the
// type of lavaball it will make (see 14 Lava Ball.cpp for the types).
// =====================================================================================================================

enum : ubyte
{
	Routine_Init = 0,
	Routine_Main = 2,
};

const ubyte Delays[] = { 30, 60, 90, 120, 150, 180 };

void LavaMaker(Object* self)
{
	switch(self->routine)
	{
		case Routine_Init:
			self->routine = Routine_Main;
			self->delayAni = Delays[(self->subtype >> 4) & 0xF];
			self->subtype &= 0xF;
			// fall through
		case Routine_Main:
			if(TimerZero(self->timeFrame, self->delayAni))
			{
				if(!ChkObjectVisible(self))
				{
					if(auto lavaBall = FindFreeObj())
					{
						lavaBall->id = ID_LavaBall;
						lavaBall->x = self->x;
						lavaBall->y = self->y;
						lavaBall->subtype = self->subtype;
					}
				}
			}

			break;
	}

	if(Object_OutOfRange(self))
		DeleteObject(self);
}