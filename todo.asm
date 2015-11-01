
EndDemo_Levels:	incbin	"misc\Demo Level Order - Ending.bin"

Demo_GHZ:	    incbin	"demodata\Intro - GHZ.bin"
Demo_MZ:	    incbin	"demodata\Intro - MZ.bin"
Demo_SYZ:	    incbin	"demodata\Intro - SYZ.bin"
Demo_SS:	    incbin	"demodata\Intro - Special Stage.bin"
Demo_EndGHZ1:	incbin	"demodata\Ending - GHZ1.bin"
Demo_EndMZ:	    incbin	"demodata\Ending - MZ.bin"
Demo_EndSYZ:	incbin	"demodata\Ending - SYZ.bin"
Demo_EndLZ:	    incbin	"demodata\Ending - LZ.bin"
Demo_EndSLZ:	incbin	"demodata\Ending - SLZ.bin"
Demo_EndSBZ1:	incbin	"demodata\Ending - SBZ1.bin"
Demo_EndSBZ2:	incbin	"demodata\Ending - SBZ2.bin"
Demo_EndGHZ2:	incbin	"demodata\Ending - GHZ2.bin"

AngleMap:	    incbin	"collide\Angle Map.bin"
CollArray1:	    incbin	"collide\Collision Array (Normal).bin"
CollArray2:	    incbin	"collide\Collision Array (Rotated).bin"
Col_GHZ:	    incbin	"collide\GHZ.bin"	; GHZ index
Col_LZ:		    incbin	"collide\LZ.bin"	; LZ index
Col_MZ:		    incbin	"collide\MZ.bin"	; MZ index
Col_SLZ:	    incbin	"collide\SLZ.bin"	; SLZ index
Col_SYZ:	    incbin	"collide\SYZ.bin"	; SYZ index
Col_SBZ:	    incbin	"collide\SBZ.bin"	; SBZ index

include	"_inc\AnimateLevelGfx.asm"
include	"_inc\DebugList.asm"
include	"_inc\DeformLayers (JP1).asm"
include	"_inc\DynamicLevelEvents.asm"
include	"_inc\HUD (part 2).asm"
include	"_inc\HUD_Update.asm"
include	"_inc\LevelSizeLoad & BgScrollSpeed (JP1).asm"
include	"_inc\LZWaterFeatures.asm"

SONIC
Boss*

Level_Index
ObjPos_Index