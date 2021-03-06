bool Player_IsFlipped   () { return Obj_IsFlipped(v_player);    }
bool Player_IsInAir     () { return Obj_IsInAir(v_player);      }
bool Player_IsRolling   () { return Obj_IsRolling(v_player);    }
bool Player_IsStanding  () { return Obj_IsStanding(v_player);   }
bool Player_IsJumping   () { return Obj_IsJumping(v_player);    }
bool Player_IsPushing   () { return Obj_IsPushing(v_player);    }
bool Player_IsUnderwater() { return Obj_IsUnderwater(v_player); }

void Player_SetFlipped   (bool val = true) { Obj_SetFlipped   (v_player, val); }
void Player_SetInAir     (bool val = true) { Obj_SetInAir     (v_player, val); }
void Player_SetRolling   (bool val = true) { Obj_SetRolling   (v_player, val); }
void Player_SetStanding  (bool val = true) { Obj_SetStanding  (v_player, val); }
void Player_SetJumping   (bool val = true) { Obj_SetJumping   (v_player, val); }
void Player_SetPushing   (bool val = true) { Obj_SetPushing   (v_player, val); }
void Player_SetUnderwater(bool val = true) { Obj_SetUnderwater(v_player, val); }

void Player_SetNotFlipped()    { Obj_SetNotFlipped(v_player);    }
void Player_SetNotInAir()      { Obj_SetNotInAir(v_player);      }
void Player_SetNotRolling()    { Obj_SetNotRolling(v_player);    }
void Player_SetNotStanding()   { Obj_SetNotStanding(v_player);   }
void Player_SetNotJumping()    { Obj_SetNotJumping(v_player);    }
void Player_SetNotPushing()    { Obj_SetNotPushing(v_player);    }
void Player_SetNotUnderwater() { Obj_SetNotUnderwater(v_player); }

bool Player_IsDead()         { return v_player->routine >= PlayerRoutine_Dead; }
bool Player_IsControllable() { return v_player->routine < PlayerRoutine_Hurt;  }

void Player_SetHurt() { v_player->routine = PlayerRoutine_Hurt; }
void Player_SetDead() { v_player->routine = PlayerRoutine_Dead; }

void Player_SetAnimFloat()    { v_player->anim = 0xF;  }
void Player_SetAnimDrowning() { v_player->anim = 0x17; }
void Player_SetAnimSlide()    { v_player->anim = 0x1F; }

//                       a0             a2
void Player_Hurt(Object* player, Object* obj)
{
	// If he has no shield..
	if(!v_shield)
	{
		// if he has rings, make the rings spill out
		if(v_rings)
		{
			if(auto ringLoss = FindFreeObj())
			{
				ringLoss->id = ID_RingLoss;
				ringLoss->x = player->x;
				ringLoss->y = player->y;
			}
		}
		else if(!f_debugmode) // otherwise, kill him (if debug mode isn't on)
		{
			Player_Kill(player, obj);
			return;
		}
	}

	// Hurt sonic
	v_shield = 0;
	Player_SetHurt();
	Player_ResetOnFloor(player);
	Player_SetInAir();

	// Bounce him away
	if(Player_IsUnderwater())
	{
		player->velY = -0x200;
		player->velX = -0x100;
	}
	else
	{
		player->velY = -0x400;
		player->velX = -0x200;
	}

	// Make him bounce right if he's to the right of the object
	if(player->x > obj->x)
		player->velX = -player->velX;

	player->inertia = 0;
	player->anim = Anim::Hurt;
	VAR_W(player, Player_InvincibilityW) = 120; // 2 seconds of invincibility;

	// Bwah
	if(obj->id == ID_Spikes || obj->id == ID_Harpoon)
		PlaySound_Special(SFX_HitSpikes);
	else
		PlaySound_Special(SFX_Death);
}

//                       a0             a2
void Player_Kill(Object* player, Object* killer)
{
	if(v_debuguse)
		return;

	// Set him up to do the death bounce
	v_invinc = 0;
	Player_SetDead();
	Player_ResetOnFloor(player);
	Player_SetInAir();
	player->velY = -0x700;
	player->velX = 0;
	player->inertia = 0;
	VAR_W(player, Player_DeathOrigYW) = player->y;
	player->anim = Anim::Death;
	BSET(player->gfx, 0x80);

	// Bwah
	if(killer->id == ID_Spikes)
		PlaySound_Special(SFX_HitSpikes);
	else
		PlaySound_Special(SFX_Death);
}