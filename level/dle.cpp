
void DynamicLevelEvents()
{
	switch(v_zone)
	{
		case Zone_GHZ:  DLE_GHZ();  break;
		case Zone_LZ:   DLE_LZ();   break;
		case Zone_MZ:   DLE_MZ();   break;
		case Zone_SLZ:  DLE_SLZ();  break;
		case Zone_SYZ:  DLE_SYZ();  break;
		case Zone_SBZ:  DLE_SBZ();  break;
		case Zone_EndZ: DLE_EndZ(); break;
	}

	// Did the bottom screen limit change?
	if(v_limitbtm1 < v_limitbtm2)
	{
		if(v_screenposy >= v_limitbtm1)
			v_limitbtm2 = v_screenposy & 0xFFFE;

		v_limitbtm2 -= 2
		f_bgscrollvert = true;
	}
	else if(v_limitbtm1	> v_limitbtm2)
	{
		v_limitbtm2 += 8;
		f_bgscrollvert = true;
	}
}

enum
{
	DLE_GHZ3_Main = 0,
	DLE_GHZ3_UpperAndBoss = 2,
	DLE_GHZ3_End = 4,
};

void DLE_GHZ()
{
	switch(v_act)
	{
		case Act_1:
			if(v_screenposx < 0x1780) v_limitbtm1 = 0x300;
			else                      v_limitbtm1 = 0x400;
			return;

		case Act_2:
			if     (v_screenposx < 0xED0)  v_limitbtm1 = 0x300;
			else if(v_screenposx < 0x1600) v_limitbtm1 = 0x200;
			else if(v_screenposx < 0x1D60) v_limitbtm1 = 0x400;
			else                           v_limitbtm1 = 0x300;
			return;

		case Act_3:
			break;
	}

	switch(v_dle_routine)
	{
		case DLE_GHZ3_Main:
			if     (v_screenposx < 0x380)  v_limitbtm1 = 0x300;
			else if(v_screenposx < 0x960)  v_limitbtm1 = 0x310;

			if(v_screenposx < 0x1700 && v_screenposy >= 0x280)
			{
				if(v_screenposx < 0x1380)
				{
					v_limitbtm1 = 0x4C0;
					v_limitbtm2 = 0x4C0;
				}
				else
					v_limitbtm1 = 0x400;
			}
			else
			{
				v_limitbtm1 = 0x300;
				v_dle_routine = DLE_GHZ3_UpperAndBoss;
			}
			break;

		case DLE_GHZ3_UpperAndBoss:
			if(v_screenposx < 0x960)
				v_dle_routine = DLE_GHZ3_Main;

			if(v_screenposx >= 0x2960)
			{
				// You can softlock the game if you put enough objects onscreen to prevent the boss from
				// spawning.. but that only ever happens in debug mode so who cares :B
				if(auto obj = FindFreeObj())
				{
					obj->id = ID_BossGreenHill;
					obj->x = 0x2A60;
					obj->y = 0x280;
				}

				PlaySound(BGM_Boss);
				f_lockscreen = true;
				v_dle_routine = DLE_GHZ3_End;
				AddPLC(PLC_Boss);
			}
			break;

		case DLE_GHZ3_End:
			v_limitleft2 = v_screenposx;
			break;

	}
}

void DLE_LZ()
{
	switch(v_act)
	{
		case Act_1:
		case Act_2:
		return;

		case Act_3:
			// Do the fancy switch to change the level layout at the beginning
			if(f_switch[15] && v_lvllayout[1][6] != 7)
			{
				v_lvllayout[1][6] = 7;
				PlaySound_Special(SFX_Rumbling);
			}

			if(v_dle_routine == 0 && v_screenposx >= 0x1CA0 && v_screenposy < 0x600)
			{
				if(auto obj = FindFreeObj())
					obj->id = ID_BossLabyrinth;

				PlaySound(BGM_Boss);
				f_lockscreen = true;
				v_dle_routine = 2;
				AddPLC(PLC_Boss);
			}
			break;

		case 3: // SBZ3
			// Did player go off the top of the level at the exit?
			if(v_screenposx >= 0xD00 && v_player->y <= 24)
			{
				v_lastlamp = 0;
				f_restart = true;
				v_zone = Zone_SBZ;
				v_act = 2;
				f_lockmulti = true;
			}
			break;
	}

}

void DLE_MZ()
{
	switch(v_act)
	{
		case Act_1: DLE_MZ_Act1(); break;

		case Act_2:
			if(v_screenposx < 0x1700)
				v_limitbtm1 = 0x520;
			else
				v_limitbtm1 = 0x200;
			break;

		case Act_3: DLE_MZ_Act3(); break;
	}

}

enum
{
	DLE_MZ1_0 = 0,
	DLE_MZ1_2 = 2,
	DLE_MZ1_4 = 4,
	DLE_MZ1_6 = 6,
};

void DLE_MZ_Act1()
{
	switch(v_dle_routine)
	{
		case DLE_MZ1_0:
			if(v_screenposx < 0x700) v_limitbtm1 = 0x1D0;
			else if(v_screenposx < 0xD00) v_limitbtm1 = 0x220;
			else if(v_screenposy < 0x340) v_limitbtm1 = 0x340;
			else v_dle_routine = DLE_MZ1_2;
			break;

		case DLE_MZ1_2:
			if(v_screenposy < 0x340) v_dle_routine = DLE_MZ1_0;
			else if(v_screenposx >= 0xE00) v_limittop2 = 0;
			else if(v_screenposx >= 0xA90) v_limittop2 = v_limitbtm1 = 0x340;
			else if(v_screenposy < 0x370) v_limitbtm1 = 0x500;
			else v_dle_routine = DLE_MZ1_4;
			break;

		case DLE_MZ1_4:
			if(v_screenposy >= 0x370) v_dle_routine = DLE_MZ1_2;
			else if(v_screenposy >= 0x500 && v_screenposx >= 0xB80)
			{
				v_limittop2 = 0x500;
				v_dle_routine = DLE_MZ1_6;
			}
			break;

		case DLE_MZ1_6:
			if(v_screenposx < 0xB80)
			{
				if(v_limittop2 != 0x340)
					v_limittop2 -= 2;
			}
			else
			{
				if(v_limittop2 != 0x500 && v_screenposy >= 0x500)
					v_limittop2 = 0x500;

				if(v_screenposx >= 0xE70)
				{
					v_limittop2 = 0;
					if(v_screenposx < 0x1430) v_limitbtm1 = 0x500;
					else v_limitbtm1 = 0x210;
				}
			}
			return;
	}
}

enum
{
	DLE_MZ3_Main = 0,
	DLE_MZ3_Boss = 2;
};

void DLE_MZ_Act3()
{
	switch(v_dle_routine)
	{
		case DLE_MZ3_Main:
			if(v_screenposx < 0x1560) v_limitbtm1 = 0x720;
			else if(v_screenposx < 0x17F0) v_limitbtm1 = 0x210;
			else
			{
				if(auto obj = FindFreeObj())
				{
					obj->id = ID_BossMarble;
					obj->x = 0x19F0;
					obj->y = 0x22C;
				}

				PlaySound(BGM_Boss);
				f_lockscreen = true;
				v_dle_routine = DLE_MZ3_Boss;
				AddPLC(PLC_Boss);
			}
			break;

		case DLE_MZ3_Boss:
			v_limitleft2 = v_screenposx;
			break;
	}
}

enum
{
	DLE_SLZ3_Main = 0,
	DLE_SLZ3_PreBoss = 2,
	DLE_SLZ3_Boss = 4,
}

void DLE_SLZ()
{
	switch(v_act)
	{
		case Act_1:
		case Act_2:
			break;

		case Act_3:
			switch(v_dle_routine)
			{
				case DLE_SLZ3_Main:
					if(v_screenposx >= 0x1E70)
					{
						v_limitbtm1 = 0x210;
						v_dle_routine = DLE_SLZ3_PreBoss;
					}
					break;

				case DLE_SLZ3_PreBoss:
					if(v_screenposx >= 0x2000)
					{
						if(auto obj = FindFreeObj())
							obj->id = ID_BossStarLight;

						PlaySound(BGM_Boss);
						f_lockscreen = true;
						v_dle_routine = DLE_SLZ3_Boss;
						AddPLC(PLC_Boss);
					}

				case DLE_SLZ3_Boss:
					v_limitleft2 = v_screenposx;
					break;
			}
			break;
	}
}

enum
{
	DLE_SYZ3_Main = 0,
	DLE_SYZ3_PreBoss = 2,
	DLE_SYZ3_Boss = 4,
};

void DLE_SYZ()
{
	switch(v_act)
	{
		case Act_1: break;

		case Act_2:
			if(v_screenposx < 0x25A0) v_limitbtm1 = 0x520;
			else if(HIWORD(v_player->y) < 0x4D0) v_limitbtm1 = 0x420;
			else v_limitbtm1 = 0x520;
			break;

		case Act_3:
			switch(v_dle_routine)
			{
				case DLE_SYZ3_Main:
					if(v_screenposx >= 0x2AC0)
					{
						if(auto obj = FindFreeObj())
						{
							obj->id = ID_BossBlock;
							v_dle_routine = DLE_SYZ3_PreBoss;
						}
					}
					break;

				case DLE_SYZ3_PreBoss:
					if(v_screenposx >= 0x2C00)
					{
						v_limitbtm1 = 0x4CC;

						if(auto obj = FindFreeObj)
						{
							obj->id = ID_BossSpringYard;
							// Oddly this is the only level that doesn't softlock if the boss can't be spawned
							v_dle_routine = DLE_SYZ3_Boss;
						}

						PlaySound(BGM_Boss);
						f_lockscreen = true;
						AddPLC(PLC_Boss);
					}
					break;

				case DLE_SYZ3_Boss:
					v_limitleft2 = v_screenposx;
					break;
			}
			break;
	}
}

void DLE_SBZ()
{
	switch(v_act)
	{
		case Act_1:
			if(v_screenposx < 0x1880) v_limitbtm1 = 0x720;
			else if(v_screenposx < 0x2000) v_limitbtm1 = 0x620;
			else v_limitbtm1 = 0x2A0;
			break;

		case Act_2: DLE_SBZ2(); break;
		case Act_3: DLE_FZ(); break;
	}
}

enum
{
	DLE_SBZ2_Main = 0,
	DLE_SBZ2_PreBoss = 2,
	DLE_SBZ2_Boss = 4,
	DLE_SBZ2_End = 6,
};

void DLE_SBZ2()
{
	switch(v_dle_routine)
	{
		case DLE_SBZ2_Main:
			if(v_screenposx < 0x1800) v_limitbtm1 = 0x800;
			else if (v_screenposx < 0x1E00) v_limitbtm1 = 0x510;
			else v_dle_routine = DLE_SBZ2_PreBoss;
			break;

		case DLE_SBZ2_PreBoss:
			if(v_screenposx >= 0x1EB0)
			{
				if(auto obj = FindFreeObj())
				{
					obj->id = ID_FalseFloor;
					v_dle_routine = DLE_SBZ2_Boss;
					AddPLC(PLC_EggmanSBZ2);
				}
			}
			break;

		case DLE_SBZ2_Boss:
			if(v_screenposx >= 0x1F60)
			{
				if(auto obj = FindFreeObj())
				{
					obj->id = ID_ScrapEggman;
					v_dle_routine = DLE_SBZ2_End;
				}

				f_lockscreen = true;
			}

			v_limitleft2 = v_screenposx;
			break;

		case DLE_SBZ2_End:
			if(v_screenposx < 0x2050)
				v_limitleft2 = v_screenposx;
			break;
	}
}

enum
{
	DLE_FZ_Main = 0,
	DLE_FZ_PreBoss = 2,
	DLE_FZ_Boss = 4,
	DLE_FZ_End = 6,
};

void DLE_FZ()
{
	switch(v_dle_routine)
	{
		case DLE_FZ_Main:
			if(v_screenposx >= 0x2148)
			{
				v_dle_routine = DLE_FZ_PreBoss;
				AddPLC(PLC_FZBoss);
			}

			v_limitleft2 = v_screenposx;
			break;

		case DLE_FZ_PreBoss:
			if(v_screenposx >= 0x2300)
			{
				if(auto obj = FindFreeObj())
				{
					obj->id = ID_BossFinal;
					v_dle_routine = DLE_FZ_Boss;
					f_lockscreen = true;
				}
			}

			v_limitleft2 = v_screenposx;
			break;

		case DLE_FZ_Boss:
			// Doesn't actually do anything |:V
			if(v_screenposx >= 0x2450)
				v_dle_routine = DLE_FZ_End;

			v_limitleft2 = v_screenposx;
			break;

		case DLE_FZ_End:
			v_limitleft2 = v_screenposx;
			break;
	}
}

void DLE_Ending() {}