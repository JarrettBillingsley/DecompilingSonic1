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