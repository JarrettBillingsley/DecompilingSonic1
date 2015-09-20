#define BTST(a, b) (((a) & (b)) != 0)
#define BSET(a, b) ((a) |= (b))
#define BCLR(a, b) ((a) &= ~(b))

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

namespace Colors
{
	enum
	{
		Black =   0x000,
		White =   0xEEE,
		Blue =    0xE00,
		Green =   0x0E0,
		Red =     0x00E,
		Yellow =  Green + Red,
		Aqua =    Green + Blue,
		Magenta = Blue + Red,
	};
};

namespace Buttons
{
	enum
	{
		Start = 0x80,            // Start button
		A =     0x40,            // A
		C =     0x20,            // C
		B =     0x10,            // B
		R =     0x08,            // Right
		L =     0x04,            // Left
		Dn =    0x02,            // Down
		Up =    0x01,            // Up
		Dir =   L | R | Dn | Up, // Any direction
		ABC =   A | B | C,       // A, B or C
	};
};

namespace AnimFlags
{
	enum
	{
		Command =  0x80, // Top bit set means it's a command
		End =      0xFF, // return to beginning of animation
		Back =     0xFE, // go back (specified number) bytes
		Change =   0xFD, // run specified animation
		Routine =  0xFC, // increment routine counter
		Reset =    0xFB, // reset animation and 2nd object routine counter
		Routine2 = 0xFA, // increment 2nd routine counter
	};
};

namespace BGM
{
	enum
	{
		GHZ,
		LZ,
		MZ,
		SLZ,
		SYZ,
		SBZ,
		Invincible,
		ExtraLife,
		SS,
		Title,
		Ending,
		Boss,
		FZ,
		GotThrough,
		GameOver,
		Continue,
		Credits,
		Drowning,
		Emerald,
		Fade,
		Stop,
	};
};

namespace SFX
{
	enum
	{
		Jump,
		Lamppost,
		A2,
		Death,
		Skid,
		A5,
		HitSpikes,
		Push,
		SSGoal,
		SSItem,
		Splash,
		AB,
		HitBoss,
		Bubble,
		Fireball,
		Shield,
		Saw,
		Electric,
		Drown,
		Flamethrower,
		Bumper,
		Ring,
		SpikesMove,
		Rumbling,
		B8,
		Collapse,
		SSGlass,
		Door,
		Teleport,
		ChainStomp,
		Roll,
		Continue,
		Basaran,
		BreakItem,
		Warning,
		GiantRing,
		Bomb,
		Cash,
		RingLoss,
		ChainRise,
		Burning,
		Bonus,
		EnterSS,
		WallSmash,
		Spring,
		Switch,
		RingLeft,
		Signpost,
		Waterfall,
		Sega,
	};
};

struct LevelHeader
{
	uint gfx; // high 8 bits are PLC 1
	uint map16; // high 8 bits are PLC 2
	uint map256;
	ubyte reserved;
	ubyte music;
	ubyte palette;
	ubyte palette_dup;
};

static_assert(sizeof(LevelHeader) == 16);

struct PLCDesc
{
	uint gfx;
	ushort vram;
};

struct PLCList
{
	uint numPLCs;
	PLCDesc plcs[];
};

void ReadJoypads()
{
	auto newButtons = ReadJoypad(0);
	auto oldButtons = v_jpadhold1;
	v_jpadhold1 = newButtons;
	v_jpadpress1 = newButtons & (oldButtons ^ newButtons);

	newButtons = ReadJoypad(1);
	oldButtons = v_jpadhold2;
	v_jpadhold2 = newButtons;
	v_jpadpress2 = newButtons & (oldButtons ^ newButtons);
}

void GameInit()
{
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

// bne
template<typename T>
bool TimerZero(T& timer)
{
	timer--;
	return timer == 0;
}

// bpl
template<typename T>
bool TimerNeg(T& timer)
{
	timer--;
	return timer < 0;
}
