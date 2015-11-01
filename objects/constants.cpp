enum
{
	MaxObjects = 128,
	NumLevelObjects = 96,
};

enum : ubyte
{
	ObjStatus_Flip = 0x01,       // 0
	ObjStatus_Air = 0x02,        // 1
	ObjStatus_Rolling = 0x04,    // 2
	ObjStatus_StandingOn = 0x08, // 3
	ObjStatus_Jumping = 0x10,    // 4
	ObjStatus_Pushing = 0x20,    // 5
	ObjStatus_Underwater = 0x40, // 6
	ObjStatus_Dead = 0x80        // 7
};

enum : ubyte
{
	ObjRender_HorizFlip =     0x01, // 0
	ObjRender_VertFlip =      0x02, // 1

	ObjRender_LayerScreen =   0x00, // 2
	ObjRender_LayerNormal =   0x04, // 2
	ObjRender_LayerBG1 =      0x08, // 3
	ObjRender_LayerBG2 =      0x0C, // 3
	ObjRender_Layer =         0x0C, // 2 + 3; 0 = screen-relative; 1 = normal; 2 = BG1-relative; 3 = BG2-relative

	ObjRender_Something =     0x10, // 4; changes Y positioning somehow?
	ObjRender_SmashFragment = 0x20, // 5
	ObjRender_Behind =        0x40, // 6
	ObjRender_Visible =       0x80, // 7
};

enum
{
	AnimFlags_Command =  0x80, // Top bit set means it's a command
	AnimFlags_End =      0xFF, // return to beginning of animation
	AnimFlags_Back =     0xFE, // go back (specified number) bytes
	AnimFlags_Change =   0xFD, // run specified animation
	AnimFlags_Routine =  0xFC, // increment routine counter
	AnimFlags_Reset =    0xFB, // reset animation and 2nd object routine counter
	AnimFlags_Routine2 = 0xFA, // increment 2nd routine counter
};

enum : ubyte
{
	ID_SonicPlayer = 1, // 0x01
	ID_Obj02,
	ID_Obj03,
	ID_Obj04,
	ID_Obj05,
	ID_Obj06,
	ID_Obj07,
	ID_Splash,          // 0x08
	ID_SonicSpecial,
	ID_DrownCount,
	ID_Pole,
	ID_FlapDoor,
	ID_Signpost,
	ID_TitleSonic,
	ID_PSBTM,
	ID_Obj10,           // 0x10
	ID_Bridge,
	ID_SpinningLight,
	ID_LavaMaker,
	ID_LavaBall,
	ID_SwingingPlatform,
	ID_Harpoon,
	ID_Helix,
	ID_BasicPlatform,   // 0x18
	ID_Obj19,
	ID_CollapseLedge,
	ID_WaterSurface,
	ID_Scenery,
	ID_MagicSwitch,
	ID_BallHog,
	ID_Crabmeat,
	ID_Cannonball,      // 0x20
	ID_HUD,
	ID_BuzzBomber,
	ID_Missile,
	ID_MissileDissolve,
	ID_Rings,
	ID_Monitor,
	ID_ExplosionItem,
	ID_Animals,         // 0x28
	ID_Points,
	ID_AutoDoor,
	ID_Chopper,
	ID_Jaws,
	ID_Burrobot,
	ID_PowerUp,
	ID_LargeGrass,
	ID_GlassBlock,      // 0x30
	ID_ChainStomp,
	ID_Button,
	ID_PushBlock,
	ID_TitleCard,
	ID_GrassFire,
	ID_Spikes,
	ID_RingLoss,
	ID_ShieldItem,      // 0x38
	ID_GameOverCard,
	ID_GotThroughCard,
	ID_PurpleRock,
	ID_SmashWall,
	ID_BossGreenHill,
	ID_Prison,
	ID_ExplosionBomb,
	ID_MotoBug,         // 0x40
	ID_Springs,
	ID_Newtron,
	ID_Roller,
	ID_EdgeWalls,
	ID_SideStomp,
	ID_MarbleBrick,
	ID_Bumper,
	ID_BossBall,        // 0x48
	ID_WaterSound,
	ID_VanishSonic,
	ID_GiantRing,
	ID_GeyserMaker,
	ID_LavaGeyser,
	ID_LavaWall,
	ID_Obj4F,
	ID_Yadrin,          // 0x50
	ID_SmashBlock,
	ID_MovingBlock,
	ID_CollapseFloor,
	ID_LavaTag,
	ID_Basaran,
	ID_FloatingBlock,
	ID_SpikeBall,
	ID_BigSpikeBall,    // 0x58
	ID_Elevator,
	ID_CirclingPlatform,
	ID_Staircase,
	ID_Pylon,
	ID_Fan,
	ID_Seesaw,
	ID_Bomb,
	ID_Orbinaut,        // 0x60
	ID_LabyrinthBlock,
	ID_Gargoyle,
	ID_LabyrinthConvey,
	ID_Bubble,
	ID_Waterfall,
	ID_Junction,
	ID_RunningDisc,
	ID_Conveyor,        // 0x68
	ID_SpinPlatform,
	ID_Saws,
	ID_ScrapStomp,
	ID_VanishPlatform,
	ID_Flamethrower,
	ID_Electro,
	ID_SpinConvey,
	ID_Girder,          // 0x70
	ID_Invisibarrier,
	ID_Teleport,
	ID_BossMarble,
	ID_BossFire,
	ID_BossSpringYard,
	ID_BossBlock,
	ID_BossLabyrinth,
	ID_Caterkiller,     // 0x78
	ID_Lamppost,
	ID_BossStarLight,
	ID_BossSpikeball,
	ID_RingFlash,
	ID_HiddenBonus,
	ID_SSResult,
	ID_SSRChaos,
	ID_ContScrItem,     // 0x80
	ID_ContSonic,
	ID_ScrapEggman,
	ID_FalseFloor,
	ID_EggmanCylinder,
	ID_BossFinal,
	ID_BossPlasma,
	ID_EndSonic,
	ID_EndChaos,        // 0x88
	ID_EndSTH,
	ID_CreditsText,
	ID_EndEggman,
	ID_TryChaos
};