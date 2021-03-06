void LevelDataLoad()
{
	auto header = LevelHeaders[v_zone];

	EniDec(header.map16 & 0x00FFFFFF, v_16x16, 0);
	KosDec(header.map256, v_256x256);
	LevelLayoutLoad();

	if(v_zone == Zone_LZ && v_act == 3)
		PalLoad1(Palette_SBZ3);
	else if(v_zone == Zone_SBZ && (v_act == 1 || v_act == 2))
		PalLoad1(Palette_SBZ2);
	else
		PalLoad1(header.palette_dup);

	if(header.map16 & 0xFF000000)
		AddPLC(header.map16 >> 24);
}

void LevelLayoutLoad()
{
	// Clears level layout, NGFX buffer, and some other mystery area
	Clear_A400_AC00();
	LevelLayoutLoad2(v_lvllayout, 0);
	LevelLayoutLoad2(v_lvllayout + 0x40, 1);
}

void LevelLayoutLoad2(ubyte* dest, uint layer)
{
	auto levelData = Level_Index[v_zone << 2 | v_act][layer];
	auto levelTiles = levelData.tiles;

	for(int i = 0; i < levelData.height; i++)
	{
		memcpy(dest, tiles, levelData.width);
		dest += 0x80;
		tiles += levelData.width;
	}
}

void ColIndexLoad()
{
	v_collindex = ColPointers[v_zone];
}

void ObjPosLoad()
{
	uint slot = 0;

	switch(v_opl_routine)
	{
		case 0:
			v_opl_routine += 2;
			auto objDataPointers = ObjPos_Index[v_zone << 2 | v_act];
			v_opl_data[0] = objDataPointers[0];
			v_opl_data[1] = objDataPointers[0];
			v_opl_data[2] = objDataPointers[1];
			v_opl_data[3] = objDataPointers[1];
			v_objstate[0] = 1;
			v_objstate[1] = 1;
			memset(v_objstate + 2, 0, sizeof(v_objstate) - 2);

			// This rounds down to next lower multiple of 128, floored at 0
			auto d6 = ((v_screenposx > 128) ? v_screenposx - 128 : 0) & 0xFF80;
			auto addr0 = v_opl_data[0];

			while(addr0 <= d6)
			{
				if(v_opl_data[1] & 0x80000000)
				{
					slot = v_objstate[0];
					v_objstate[0]++;
				}

				addr0 += 3;
			}

			v_opl_data[0] = addr0;
			auto addr1 = v_opl_data[1];

			if(d6 >= 128)
			{
				d6 -= 128;

				while(addr1 <= d6)
				{
					if(v_opl_data[2] & 0x80000000)
						v_objstate[1]++;

					addr1 += 3;
				}
			}

			v_opl_screen = 0xFFFF;
			// fall through
		case 2: {
			auto screen = v_screenposx & 0xFF80;

			if(screen == v_opl_screen)
				return;
			else if(screen < v_opl_screen)
			{
				v_opl_screen = screen;

				auto addr1 = v_opl_data[1];

				screen -= 128;

				if(screen >= 0)
				{
					bool skip = false;

					while(true)
					{
						if(screen >= addr1[-3])
						{
							skip = true;
							break;
						}

						addr1 -= 3;

						if(addr1[2] & 0x8000)
							slot = --v_objstate[1];

						if(Make_Object(addr1, slot))
							break;

						addr1 -= 3;
					}

					if(!skip)
					{
						if(addr1[2] & 0x8000)
							v_objstate[1]++;

						addr1 += 3;
					}
				}

				v_opl_data[1] = addr1;

				auto addr0 = v_opl_data[0];
				screen += 768;

				while(true)
				{
					if(screen > addr0[-3])
						break;

					if(addr0[-1] & 0x8000)
						v_objstate[1]--;

					addr0 -= 3;
				}

				v_opl_data[0] = addr0;
			}
			else // screen > v_opl_screen
			{
				v_opl_screen = screen;
				auto addr0 = v_opl_data[0];
				screen += 640;

				while(screen >= addr0[0])
				{
					if(addr0[2] & 0x8000)
						slot = v_objstate[0]++;

					if(Make_Object(addr0, slot))
						break;
				}

				v_opl_data[0] = addr0;
				auto addr1 = v_opl_data[1];

				if(screen >= 768)
				{
					screen -= 768;

					while(screen >= addr1)
					{
						if(addr1[2] & 0x8000)
							v_objstate[1]++;

						addr1 += 3;
					}
				}

				v_opl_data[1] = addr1;
			}

			break;
		}
	}
}

bool Make_Object(ushort*& objData, ubyte stateSlot) // cc if succeeded, cs if failed (true if failed)
{
	if(objData[2] & 0x8000)
	{
		if(v_objstate[stateSlot + 2] & 0xF0)
		{
			objData += 3;
			return false;
		}
		else
			v_objstate[stateSlot + 2] |= 0xF0;
	}

	if(auto newObj = FindFreeObj())
	{
		newObj->x = objData[0];
		newObj->y = objData[1] & 0xFFF;
		auto flags = objData[1] >> 14;
		newObj->render = flags;
		newObj->status = flags;
		flags = objData[2];
		objData += 3;

		if(flags & 0x8000)
			newObj->respawnNo = (flags >> 8) & 0x7F;

		newObj->id = ID_Zero;
		newObj->subtype = flags & 0xFF;
		return false;
	}

	return true;
}