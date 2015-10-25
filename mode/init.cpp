void GameInit()
{
	// TODO:
	// VDPSetupGame();
	// SoundDriverLoad();
	// JoypadInit();
	v_gamemode = GameMode_Sega;

	while(true)
	{
		switch(v_gamemode & 0x1C)
		{
			case GameMode_Sega:     GM_Sega();     break;
			case GameMode_Title:    GM_Title();    break;
			case GameMode_Demo:
			case GameMode_Level:    GM_Level();    break;
			case GameMode_Special:  GM_Special();  break;
			case GameMode_Continue: GM_Continue(); break;
			case GameMode_Ending:   GM_Ending();   break;
			case GameMode_Credits:  GM_Credits();  break;
		}
	}
}