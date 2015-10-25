struct Point16
{
	short x, y;
};

enum
{
	ScreenWidth = 320,
	ScreenHeight = 224,

	HalfScreenWidth = 160,
	HalfScreenHeight = 112,

	OneSecond = 60,
	HalfSecond = 30,

	NumEmeralds = 6,

	MaxSprites = 0x50,
};

enum
{
	GameMode_Sega =     0x00,
	GameMode_Title =    0x04,
	GameMode_Demo =     0x08,
	GameMode_Level =    0x0C,
	GameMode_Special =  0x10,
	GameMode_Continue = 0x14,
	GameMode_Ending =   0x18,
	GameMode_Credits =  0x1C,

	GameMode_PreLevel = 0x80,
};

enum
{
	DemoMode_Off = 0,
	DemoMode_On = 1,
	DemoMode_Credits = 0x8001,
};

enum
{
	Zone_GHZ,
	Zone_LZ,
	Zone_MZ,
	Zone_SLZ,
	Zone_SYZ,
	Zone_SBZ,
	Zone_EndZ,
	Zone_SS,
};

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

