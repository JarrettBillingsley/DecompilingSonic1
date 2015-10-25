void PaletteFadeIn()
{
	v_pfade_start = 0;
	v_pfade_size = 64;
	PalFadeIn_Alt();
}

void PalFadeIn_Alt()
{
	auto palette = v_pal_dry + v_pfade_start;

	for(int i = 0; i < v_pfade_size; i++)
		palette[i] = Colors_Black;

	for(int i = 0; i < 16; i++)
	{
		WaitForVBlank(VBlank_PaletteFade);
		FadeIn_FromBlack();
		RunPLC();
	}
}

void FadeIn_FromBlack()
{
	auto cur = v_pal_dry + v_pfade_start;
	auto dest = v_pal_dry_dup + v_pfade_start;

	for(int i = 0; i < v_pfade_size; i++)
		FadeIn_AddColour(cur++, dest++);

	if(v_zone == Zone_LZ)
	{
		auto cur = v_pal_water + v_pfade_start;
		auto dest = v_pal_water_dup + v_pfade_start;

		for(int i = 0; i < v_pfade_size; i++)
			FadeIn_AddColour(cur++, dest++);
	}
}

void FadeIn_AddColour(ushort* cur, ushort* dest)
{
	auto destColor = *dest;
	auto curColor = *cur;

	if(curColor != destColor)
	{
		auto tmpColor = curColor + 0x200; // blue

		if(tmpColor <= destColor)
			*cur = tmpColor;
		else
		{
			tmpColor = curColor + 0x20; // green

			if(tmpColor <= destColor)
				*cur = tmpColor;
			else
				*cur += 2; // red
		}
	}
}

void PaletteFadeOut()
{
	v_pfade_start = 0;
	v_pfade_size = 64;

	for(int i = 0; i < 16; i++)
	{
		WaitForVBlank(VBlank_PaletteFade);
		FadeOut_ToBlack();
		RunPLC();
	}
}

void FadeOut_ToBlack()
{
	auto palette = v_pal_dry + v_pfade_start;

	for(int i = 0; i < v_pfade_size; i++)
		FadeOut_DecColour(palette++);

	palette = v_pal_water + v_pfade_start;

	for(int i = 0; i < v_pfade_size; i++)
		FadeOut_DecColour(palette++);
}

void FadeOut_DecColour(ushort* cur)
{
	auto curColor = *cur;

	if(curColor != 0)
	{
		if(curColor & 0x0E)
			*cur -= 2; // red
		else if(curColor & 0xE0)
			*cur -= 0x20; // green
		else if(curColor & 0xE00)
			*cur -= 0x200; // blue
	}
}

void PaletteWhiteIn()
{
	v_pfade_start = 0;
	v_pfade_size = 64;

	auto palette = v_pal_dry + v_pfade_start;

	for(int i = 0; i < v_pfade_size; i++)
		*palette++ = Colors_White;

	for(int i = 0; i < 16; i++)
	{
		WaitForVBlank(VBlank_PaletteFade);
		WhiteIn_FromWhite();
		RunPLC();
	}
}

void WhiteIn_FromWhite()
{
	auto cur = v_pal_dry + v_pfade_start;
	auto dest = v_pal_dry_dup + v_pfade_start;

	for(int i = 0; i < v_pfade_size; i++)
		WhiteIn_DecColour(cur++, dest++);

	if(v_zone == Zone_LZ)
	{
		auto cur = v_pal_water + v_pfade_start;
		auto dest = v_pal_water_dup + v_pfade_start;

		for(int i = 0; i < v_pfade_size; i++)
			WhiteIn_DecColour(cur++, dest++);
	}
}

void WhiteIn_DecColour(ushort* cur, ushort* dest)
{
	auto destColor = *dest;
	auto curColor = *cur;

	if(curColor != destColor)
	{
		if(curColor & 0xE00 != 0)
		{
			auto tmpColor = curColor - 0x200;

			if(tmpColor >= destColor)
			{
				*cur = tmpColor;
				return;
			}
		}

		if(curColor & 0xE0 != 0)
		{
			auto tmpColor = curColor - 0x20;

			if(tmpColor >= destColor)
			{
				*cur = tmpColor;
				return;
			}
		}

		*cur -= 2;
	}
}

void PaletteWhiteOut()
{
	v_pfade_start = 0;
	v_pfade_size = 64;

	for(int i = 0; i < 16; i++)
	{
		WaitForVBlank(VBlank_PaletteFade);
		WhiteOut_ToWhite();
		RunPLC();
	}
}

void WhiteOut_ToWhite()
{
	auto palette = v_pal_dry + v_pfade_start;

	for(int i = 0; i < v_pfade_size; i++)
		WhiteOut_AddColour(palette++);

	palette = v_pal_water + v_pfade_start;

	for(int i = 0; i < v_pfade_size; i++)
		WhiteOut_AddColour(palette++);
}

void WhiteOut_AddColour(ushort* cur)
{
	auto curColor = *cur;

	if(curColor != Colors_White)
	{
		auto tmpColor = curColor & 0x0E; // red

		if(tmpColor != Colors_Red)
			*cur += 2;
		else
		{
			tmpColor = curColor & 0xE0; // green

			if(tmpColor != Colors_Green)
				*cur += 0x20;
			else
			{
				tmpColor = curColor & 0xE00; // blue

				if(tmpColor != Colors_Blue)
					*cur += 0x200;
			}
		}
	}
}