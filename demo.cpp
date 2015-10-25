const ubyte* DemoDataPtr[] =
{
	Demo_GHZ,
	Demo_GHZ,
	Demo_MZ,
	Demo_MZ,
	Demo_SYZ,
	Demo_SYZ,
	Demo_SS,
	Demo_SS,
};

const ubyte* DemoEndDataPtr[] =
{
	Demo_EndGHZ1,
	Demo_EndMZ,
	Demo_EndSYZ,
	Demo_EndLZ,
	Demo_EndSLZ,
	Demo_EndSBZ1,
	Demo_EndSBZ2,
	Demo_EndGHZ2,
};

// Random junk?? Looks like demo data but what's it doing here
// dc.b 0,	$8B, 8,	$37, 0,	$42, 8,	$5C, 0,	$6A, 8,	$5F, 0,	$2F, 8,	$2C
// dc.b 0,	$21, 8,	3, $28,	$30, 8,	8, 0, $2E, 8, $15, 0, $F, 8, $46
// dc.b 0,	$1A, 8,	$FF, 8,	$CA, 0,	0, 0, 0, 0, 0, 0, 0, 0,	0
// even

void MoveSonicInDemo()
{
	if(f_demo == DemoMode_Off)
		return;

	if(v_jpadhold1 & Buttons_Start && f_demo != DemoMode_Credits)
		v_gamemode = GameMode_Title;

	ubyte* demoData;

	if(f_demo == DemoMode_Credits)
		demoData = DemoEndDataPtr[v_creditsnum - 1];
	else
		demoData = DemoDataPtr[(v_gamemode == GameMode_Special) ? 6 : v_zone];

	auto newButton = demoData[v_btnpushtime1];

	// Rev 0
	// d2 = v_jpadhold1
	// Rev 1
	// d2 = 0

	// d0 ^= d2

	v_jpadhold1 = newButton;
	v_jpadpress1 = newButton;
	v_btnpushtime2 -= 1;

	if(v_btnpushtime2 < 0)
	{
		v_btnpushtime2 = demoData[3];
		v_btnpushtime1 += 2;
	}
}