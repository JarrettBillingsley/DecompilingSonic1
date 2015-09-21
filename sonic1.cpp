namespace GameMode
{
	enum
	{
		Sega =     0x00,
		Title =    0x04,
		Demo =     0x08,
		Level =    0x0C,
		Special =  0x10,
		Continue = 0x14,
		Ending =   0x18,
		Credits =  0x1C,

		PreLevel = 0x80,
	};
};

namespace DemoMode
{
	enum
	{
		Off = 0,
		On = 1,
		Credits = 0x8001,
	};
};

namespace Zone
{
	enum
	{
		GHZ,
		LZ,
		MZ,
		SLZ,
		SYZ,
		SBZ,
		EndZ,
		SS,
	};
};

void GameInit()
{
	// TODO:
	// VDPSetupGame();
	// SoundDriverLoad();
	// JoypadInit();
	v_gamemode = GameMode::Sega;

	while(true)
	{
		switch(v_gamemode & 0x1C)
		{
			case GameMode::Sega:     GM_Sega();     break;
			case GameMode::Title:    GM_Title();    break;
			case GameMode::Demo:
			case GameMode::Level:    GM_Level();    break;
			case GameMode::Special:  GM_Special();  break;
			case GameMode::Continue: GM_Continue(); break;
			case GameMode::Ending:   GM_Ending();   break;
			case GameMode::Credits:  GM_Credits();  break;
		}
	}
}