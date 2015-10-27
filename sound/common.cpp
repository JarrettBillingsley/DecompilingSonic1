void SoundDriverLoad()
{
	STOP_Z80();
	RESET_Z80();
	KosDec(Kos_Z80, z80_ram);
	RESET_Z80a();
	RESET_Z80();
	START_Z80();
}

//                   d0
void PlaySound(int sound)
{
	v_snddriver_ram+v_playsnd1 = sound;
}

//                          d0
void PlaySound_Special(int sound)
{
	v_snddriver_ram+v_playsnd2 = sound;
}

//                         d0
void PlaySound_Unused(int sound)
{
	v_snddriver_ram+v_playnull = sound;
}

// Subroutine to play music for LZ/SBZ3 after a countdown
void ResumeMusic()
{
	if(v_air <= 12)
	{
		auto bgm = BGM_LZ;

		if(v_zone == Zone_LZ && v_act == 3)
			bgm = BGM_SBZ;

		if(v_invinc)
			bgm = BGM_Invincible;

		if(f_lockscreen)
			bgm = BGM_Boss;

		PlaySound(bgm);
	}

	v_air = 30;
	VAR_B(v_objspace[13], 0x32) = 0; // reset the... sillyTimerB variable of the 0A object. wat.
}