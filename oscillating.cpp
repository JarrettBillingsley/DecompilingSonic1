const ushort OscillatingBaselines[] =
{
	0x007C,
	0x0080, 0x0000, 0x0080, 0x0000, 0x0080, 0x0000, 0x0080, 0x0000,
	0x0080, 0x0000, 0x0080, 0x0000, 0x0080, 0x0000, 0x0080, 0x0000,
	0x0080, 0x0000, 0x50f0, 0x011e, 0x2080, 0x00b4, 0x3080, 0x010e,
	0x5080, 0x01c2, 0x7080, 0x0276, 0x0080, 0x0000, 0x0080, 0x0000,
};

const ushort OscillatingSettings[] =
{
	// frequency, amplitude
	2,	0x10,
	2,	0x18,
	2,	0x20,
	2,	0x30,
	4,	0x20,
	8,	0x08,
	8,	0x40,
	4,	0x40,
	2,	0x50,
	2,	0x50,
	2,	0x20,
	3,	0x30,
	5,	0x50,
	7,	0x70,
	2,	0x10,
	2,	0x10,
};

void OscillateNumInit()
{
	for(int i = 0; i < 33; i++)
		v_oscillate[i] = OscillatingBaselines[i]
}

void OscillateNumDo()
{
	if(v_player.routine >= 6)
		return;

	auto oscDirs = v_oscillate[0];
	auto osc = &v_oscillate[1];
	auto settings = OscillatingSettings;

	for(int i = 15; i >= 0; i--)
	{
		auto freq = settings[0];
		auto amp = settings[1];
		settings += 2;

		if(oscDirs & (1 << i))
		{
			// down
			osc[1] -= freq;
			osc[0] += osc[1];

			if(osc[0] < amp)
				oscDirs &= ~(1 << i);
		}
		else
		{
			// up
			osc[1] += freq;
			osc[0] += osc[1];

			if(osc[0] >= amp)
				oscDirs |= (1 << i);
		}

		osc += 2;
	}

	v_oscillate[0] = oscDirs;
}