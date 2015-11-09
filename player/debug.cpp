void DebugMode(Object* self)
{
	switch(v_debuguse)
	{
		case 0: {
			v_debuguse = 2;

			v_limittopdb = v_limittop2;
			v_limitbtmdb = v_limitbtm1;
			v_limittop2 = 0;
			v_limitbtm1 = 0x720;
			v_player->y &= 0x7FF;
			v_screenposy &= 0x7FF;
			v_bg1posy &= 0x3FF;
			self->frame = 0;
			self->anim = PlayerAnim_Walk;

			if(v_gamemode == GameMode_Special)
			{
				v_ssrotate = 0;
				v_ssangle = 0;
			}

			auto debugList = DebugList[v_gamemode == GameMode_Special ? 6 : v_zone];

			if(v_debugitem >= debugList.count)
				v_debugitem = 0;

			Debug_ShowItem(self);
			v_debugmovedelay = 12;
			v_debugmovespeed = 1;
		}
		// fall through
		case 2: {
			auto debugList = DebugList[v_gamemode == GameMode_Special ? 6 : v_zone];
			Debug_Control(self, debugList.count, debugList.objects)
			DisplaySprite(self);
			break;
		}
	}
}

//                          a0             d6                       a2
void Debug_Control(Object* self, uint numObjects, DebugListEntry* objects)
{
	auto directions = v_jpadpress1 & Buttons_Dir;

	if(directions == 0 && (v_jpadhold1 & Buttons_Dir) == 0)
	{
		// Let go of direction buttons?
		v_debugmovedelay = 12;
		v_debugmovespeed = 15;
	}
	else
	{
		// This logic looks funny, but has the effect of moving the object one pixel, then waiting 1/5 of a second
		// before starting to accelerate.
		if(directions)
			directions = v_jpadhold1;
		else if(TimerZero(v_debugmovedelay, 1))
		{
			v_debugmovespeed = clamp(v_debugmovespeed + 1, 0, 255);
			directions = v_jpadhold1;
		}

		auto moveSpeed = (v_debugmovespeed + 1) << 12;
		if(directions & Buttons_Up)    self->y = clamp0(self->y - moveSpeed);
		if(directions & Buttons_Down)  self->y = clamp(self->y + moveSpeed, 0, 0x7FF0000);
		if(directions & Buttons_Left)  self->x = clamp0(self->x - moveSpeed);
		if(directions & Buttons_Right) self->x += moveSpeed;
	}

	if(v_jpadhold1 & Buttons_A)
	{
		if(v_jpadpress1 & Buttons_C)
		{
			v_debugitem = wrap(v_debugitem - 1, 0, numObjects - 1);
			Debug_ShowItem(self);
			return;
		}
		else if(v_jpadpress1 & Buttons_A)
		{
			v_debugitem = wrap(v_debugitem + 1, 0, numObjects - 1);
			Debug_ShowItem(self);
			return;
		}
	}

	if(v_jpadpress1 & Buttons_C)
	{
		if(auto obj = FindFreeObj())
		{
			obj->x = self->x;
			obj->y = self->y;
			obj->id = objects[v_debugitem].id;
			obj->render = self->render;
			obj->status = self->status & 0x7F;
			obj->subtype = objects[v_debugitem].subtype;
			return;
		}
	}

	if(v_jpadpress1 & Buttons_B)
	{
		v_debuguse = false;
		v_player->map = Map_Sonic;
		v_player->gfx = GFX_Sonic;
		v_player->anim = 0;
		LOWORD(self->x = 0);
		LOWORD(self->y = 0);
		v_limittop2 = v_limittopdb;
		v_limitbtm1 = v_limitbtmdb;

		if(v_gamemode == GameMode_Special)
		{
			v_ssangle = 0;
			v_ssrotate = 0x40;
			v_player->anim = PlayerAnim_Roll;
			Player_SetInAir();
			Player_SetRolling();
		}
	}
}

void Debug_ShowItem(Object* self, DebugListEntry* objects)
{
	self->map = objects[v_debugitem].map;
	self->gfx = objects[v_debugitem].vram;
	self->frame = objects[v_debugitem].frame;
}