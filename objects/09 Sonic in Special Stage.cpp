// =====================================================================================================================
// Object 09 - Sonic (special stage)
// =====================================================================================================================

enum : ubyte
{
	Routine_Init = 0,
	Routine_Main = 2,
	Routine_ExitStage = 4,
	Routine_ExitStage2 = 6,

	collidedTypeB = 0x30,   // type of object that was last collided with
	collidedAddrL = 0x32,   // address (into level layout) of object that was last collided with
	upDownDelayB = 0x36,    // when you hit an UP/DOWN, frame delay until the next time you can hit one
	rDelayB = 0x37,         // when you hit an R, frame delay until the next time you can hit one
	exitTimerW = 0x38,      // frame timer used for exiting the stage
	ghostBlockFlagB = 0x3A, // 0 if not yet passed the ghost blocks; 1 if passed but not yet solid; 2 if solid

	SS_UpDownDelay = 30,    // delay in frames between successive hits of UP/DOWN blocks
	SS_RDelay = 30,         // delay in frames between successive hits of R blocks
};

enum : ushort
{
	GFX_SS_Sonic = 0x0780,

	SS_MoveAccel = 12,
	SS_MoveAccelFast = 64,
	SS_MaxInertia = 0x800,
	SS_JumpVel = 0x680,
	SS_Gravity = 42,
	SS_BumperVel = 0x700,

	SS_ContinueRings = 50,

	SS_GhostBlockDone = 0,
	SS_GhostBlockPassed = 1,
	SS_GhostBlockSolid = 2,

	SS_ExitRotateAccel = 0x40,
	SS_ExitRotateMax = 0x3000,
};

void SonicSpecial(Object* self)
{
	if(v_debuguse)
	{
		SS_FixCamera(self);
		DebugMode();
		return;
	}

	switch(self->routine)
	{
		case Routine_Init:
			self->routine = Routine_Main;
			self->height = 14;
			self->width = 7;
			self->map = Map_Sonic;
			self->gfx = GFX_SS_Sonic;
			self->render = ObjRender::LayerNormal;
			self->priority = 0;
			self->anim = PlayerAnim_Roll;
			self->status = ObjStatus::Air | ObjStatus::Rolling;
			// fall through
		case Routine_Main:
			if(f_debugmode && (v_jpadpress1 & Buttons::B))
				v_debuguse = true;

			VAR_B(self, collidedTypeB) = SSObj_None;

			if(!(self->status & ObjStatus::Air))
				SS_Jump(self);

			SS_Move(self);
			SS_Fall(self);
			SS_CheckCollectibleObjects(self);
			SS_CheckStaticObjects(self);
			SpeedToPos(self);
			SS_FixCamera(self);
			v_ssangle += v_ssrotate;
			Sonic_Animate(self);
			Sonic_LoadGfx(self);
			DisplaySprite(self);
			break;

		case Routine_ExitStage:
			v_ssrotate += SS_ExitRotateAccel;

			if(v_ssrotate == 0x1800) // ?? doesn't the gamemode get set in Routine_ExitStage2?
				v_gamemode = GameMode::Level;

			if(v_ssrotate > SS_ExitRotateMax)
			{
				v_ssrotate = 0;
				v_ssangle = 0x4000; // ? not sure why
				self->routine = Routine_ExitStage2;
				VAR_W(self, exitTimerW) = OneSecond;
			}

			v_ssangle += v_ssrotate;

			Sonic_Animate(self);
			Sonic_LoadGfx(self);
			SS_FixCamera(self);
			DisplaySprite(self);
			break;

		case Routine_ExitStage2:
			if(TimerZero(VAR_W(self, exitTimerW)))
				v_gamemode = GameMode::Level;

			Sonic_Animate(self);
			Sonic_LoadGfx(self);
			SS_FixCamera(self);
			DisplaySprite(self);
			break;
	}
}

void SS_Move(Object* self)
{
	if(v_jpadhold2 & Buttons::L)
		SS_MoveLeft(self);
	else if(v_jpadhold2 & Buttons::R)
		SS_MoveRight(self);

	if(!(v_jpadhold2 & (Buttons::L | Buttons::R)) && self->inertia != 0)
	{
		if(self->inertia > 0)
		{
			self->inertia -= SS_MoveAccel;

			if(self->inertia < 0)
				self->inertia = 0;
		}
		else
		{
			self->inertia += SS_MoveAccel;

			if(self->inertia > 0)
				self->inertia = 0;
		}
	}

	int cosine;
	int sine = CalcSine(-((v_ssangle + 0x20) & 0xC0), &cosine);
	auto inertiaX = self->inertia * cosine;
	auto inertiaY *= self->inertia * sine;
	self->x += inertiaX;
	self->y += inertiaY;

	if(SS_CheckCollision(self, self->x, self->y))
	{
		self->x -= inertiaX;
		self->y -= inertiaY;
		self->inertia = 0;
	}
}

void SS_MoveLeft(Object* self)
{
	BSET(self->status, ObjStatus::Flip);

	if(self->inertia > 0)
		self->inertia -= SS_MoveAccelFast;
	else
	{
		self->inertia -= SS_MoveAccel;

		if(self->inertia < -SS_MaxInertia)
			self->inertia = -SS_MaxInertia;
	}
}

void SS_MoveRight(Object* self)
{
	BCLR(self->status, ObjStatus::Flip);

	if(self->inertia < 0)
		self->inertia += SS_MoveAccelFast;
	else
	{
		self->inertia += SS_MoveAccel;

		if(self->inertia > SS_MaxInertia)
			self->inertia = SS_MaxInertia;
	}
}

void SS_Jump(Object* self)
{
	if(v_jpadpress2 & Buttons::ABC)
	{
		int cosine;
		auto sine = CalcSine(-(v_ssangle & 0xFC) - 0x40)

		self->velX = (cosine * SS_JumpVel) >> 8;
		self->velY = (sine * SS_JumpVel) >> 8;
		BSET(self->status, ObjStatus::Air);
		PlaySound_Special(SFX::Jump);
	}
}

// unused subroutine to limit Sonic's upward vertical speed
void Unused_SS_LimitVelY(Object* self)
{
	if(self->velY < 0x400 && !(v_jpadhold2 & Buttons::ABC))
		self->velY = 0x400;
}

void SS_FixCamera(Object* self)
{
	if(self->x >= HalfScreenWidth)
		v_screenposx = self->x - HalfScreenWidth;

	if(self->y >= HalfScreenHeight)
		v_screenposy = self->y - HalfScreenHeight;
}

void SS_Fall(Object* self)
{
	int cosine;
	auto sine = CalcSine(v_ssangle & 0xFC, &cosine);
	auto velX = (sine * SS_Gravity) + self->velX << 8;
	auto velY = (cosine * SS_Gravity) + self->velY << 8;

	if(SS_CheckCollision(self, self->x + velX, self->y))
	{
		velX = 0;
		BCLR(self->status, ObjStatus::Air);

		if(SS_CheckCollision(self, self->x, self->y + velY))
			velY = 0;
	}
	else if(SS_CheckCollision(self, self->x + velX, self->y + velY))
	{
		velY = 0;
		BCLR(self->status, ObjStatus::Air);
	}
	else
		BSET(self->status, ObjStatus::Air);

	self->velX = velX >> 8;
	self->velY = velY >> 8;
}

//c                             a0      d3     d2
bool SS_CheckCollision(Object* self, int x, int y)
{
	auto row = ((y >> 16) + 68) / SS_TileSize;
	auto col = ((x >> 16) + 20) / SS_TileSize;
	bool ret = false;
	ret |= SS_CheckObjCollision(self, &v_sslayout[row][col]);
	ret |= SS_CheckObjCollision(self, &v_sslayout[row + 1][col]);
	ret |= SS_CheckObjCollision(self, &v_sslayout[row + 1][col + 1]);
	return ret;
}

//c                                a0
bool SS_CheckObjCollision(Object* self, ubyte* tile)
{
	if(*tile == SSObj_None || *tile == SSObj_1Up || (*tile >= SSObj_Ring && *tile <= SSObj_GhostTurner))
		return false;

	VAR_B(self, collidedTypeB) = *tile;
	VAR_L(self, collidedAddrL) = tile;
	return true;
}

// Original returns a flag in d4/c but nothing seems to check it
bool SS_CheckCollectibleObjects(Object* self)
{
	auto row = (self->y + 80) / SS_TileSize;
	auto col = (self->x + 32) / SS_TileSize;
	auto objAddr = &v_sslayout[row][col];
	auto objType = *objAddr;

	switch(objType)
	{
		case SSObj_None:
			if(VAR_B(self, ghostBlockFlagB))
			{
				if(VAR_B(self, ghostBlockFlagB) == SS_GhostBlockSolid)
				{
					// Replace all ghost blocks with normal "peppermint" blocks
					for(int i = 0; i < SS_LayoutUsedRows; i++)
					{
						for(int j = 0; j < SS_LayoutUsedColumns; j++)
						{
							if(v_sslayout[i + SS_LayoutRowStart][j] == SSObj_GhostBlock)
								v_sslayout[i + SS_LayoutRowStart][j] = SSObj_Peppermint;
						}
					}
				}

				VAR_B(self, ghostBlockFlagB) = SS_GhostBlockDone;
			}
			return false;

		case SSObj_Ring:
			if(auto slot = SS_FindAnimSlot())
			{
				slot->id = SSAni_RingSparks;
				slot->tile = objAddr
			}

			CollectRing();

			if(v_rings >= SS_ContinueRings)
			{
				if(!(v_lifecount & 1))
				{
					BSET(v_lifecount, 1);
					v_continues++;
					PlaySound(SFX::Continue);
				}
			}
			return false;

		case SSObj_1Up:
			if(auto slot = SS_FindAnimSlot())
			{
				slot->id = SSAni_1Up;
				slot->tile = objAddr
			}

			v_lives++;
			f_lifecount = true;
			PlaySound_Special(BGM::ExtraLife)
			return false;

		case SSObj_GhostBlock:
			VAR_B(self, ghostBlockFlagB) = SS_GhostBlockPassed;
			return true;

		case SSObj_GhostTurner: // thing that turns ghost blocks into regular blocks
			if(VAR_B(self, ghostBlockFlagB) == SS_GhostBlockPassed)
				VAR_B(self, ghostBlockFlagB) = SS_GhostBlockSolid;
			return true;

		default:
			if(objType >= SSObj_Emerald1 && objType <= SSObj_Emerald6) // emerald?
			{
				if(auto slot = SS_FindAnimSlot())
				{
					slot->id = SSAni_EmeraldSparks;
					slot->tile = objAddr
				}

				if(v_emeralds != NumEmeralds)
					v_emldlist[v_emeralds++] = objType - SSObj_Emerald1;

				PlaySound_Special(BGM::Emerald);
				return false;
			}
	}

	return true;
}

void SS_CheckStaticObjects(Object* self)
{
	switch(VAR_B(self, collidedTypeB))
	{
		case SSObj_none:
			if(TimerNeg(VAR_B(self, upDownDelayB)))
				VAR_B(self, upDownDelayB) = 0;

			if(TimerNeg(VAR_B(self, rDelayB)))
				VAR_B(self, rDelayB) = 0;

			return;

		case SSObj_Bumper:
			// Do something sneaky -- extract the X and Y tile coords from the tile addr
			auto addr = VAR_L(self, collidedAddrL) - v_sslayout;
			auto xdiff = ((addr & 0x7F) * SS_TileSize) - 20 - self->x;
			auto ydiff = (((addr >> 7) & 0x7F) * SS_TileSize) - 68 - self->y;
			int cosine;
			sine = CalcSine(CalcAngle(xdiff, ydiff), &cosine);
			self->velX = (cosine * -SS_BumperVel) >> 8;
			self->velY = (sine * -SS_BumperVel) >> 8;
			BSET(self->status, ObjStatus::Air);

			if(auto slot = SS_FindAnimSlot())
			{
				slot->id = SSAni_Bumper;
				slot->tile = VAR_L(self, collidedAddrL);
			}

			PlaySound_Special(SFX::Bumper);
			break;

		case SSObj_Goal:
			self->routine = Routine_ExitStage;
			PlaySound_Special(SFX::SSGoal);
			break;

		case SSObj_Up:
			if(VAR_B(self, upDownDelayB) == 0)
			{
				VAR_B(self, upDownDelayB) = SS_UpDownDelay;

				if(BTST(v_ssrotate, (1 << 6)))
				{
					v_ssrotate <<= 1;
					*VAR_L(self, collidedAddrL) = SSObj_Down;
				}

				PlaySound_Special(SFX::SSItem);
			}
			break;

		case SSObj_Down:
			if(VAR_B(self, upDownDelayB) == 0)
			{
				VAR_B(self, upDownDelayB) = SS_UpDownDelay;

				if(BTST(v_ssrotate, (1 << 6)) == 0)
				{
					v_ssrotate >>= 1;
					*VAR_L(self, collidedAddrL) = SSObj_Up;
				}

				PlaySound_Special(SFX::SSItem);
			}
			break;

		case SSObj_R:
			if(VAR_B(self, rDelayB) == 0)
			{
				VAR_B(self, rDelayB) = SS_RDelay;

				if(auto slot = SS_FindAnimSlot())
				{
					slot->id = SSAni_Reverse;
					slot->tile = VAR_L(self, collidedAddrL);
				}

				v_ssrotate = -v_ssrotate;
				PlaySound_Special(SFX::SSItem);
			}
			break;

		case SSObj_Glass4Hit:
		case SSObj_Glass3Hit:
		case SSObj_Glass2Hit:
		case SSObj_Glass1Hit:
			if(auto slot = SS_FindAnimSlot())
			{
				slot->id = SSAni_GlassBlock;
				slot->tile = VAR_L(self, collidedAddrL);

				if(IncTo(*slot->tile, SSObj_Glass1Hit + 1))
					*slot->tile = SSObj->None
			}

			PlaySound_Special(SFX::SSGlass);
			break;

		default: break;
}