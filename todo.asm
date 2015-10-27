
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

include	"_inc\LZWaterFeatures.asm"
include	"_inc\LevelSizeLoad & BgScrollSpeed (JP1).asm"
include	"_inc\DeformLayers (JP1).asm"
include	"_inc\AnimateLevelGfx.asm"
include	"_inc\HUD_Update.asm"
include	"_inc\HUD (part 2).asm"
include	"_inc\DebugList.asm"

SONIC
Boss*

Level_Index
ObjPos_Index

; ---------------------------------------------------------------------------
; Add points subroutine
; ---------------------------------------------------------------------------

; ||||||||||||||| S U B	R O U T	I N E |||||||||||||||||||||||||||||||||||||||


AddPoints:
		move.b	#1,(f_scorecount).w ; set score counter to update

		if Revision=0
		lea	(v_scorecopy).w,a2
		lea	(v_score).w,a3
		add.l	d0,(a3)		; add d0*10 to the score
		move.l	#999999,d1
		cmp.l	(a3),d1		; is score below 999999?
		bhi.w	@belowmax	; if yes, branch
		move.l	d1,(a3)		; reset	score to 999999
		move.l	d1,(a2)

	@belowmax:
		move.l	(a3),d0
		cmp.l	(a2),d0
		blo.w	@locret_1C6B6
		move.l	d0,(a2)

		else

			lea     (v_score).w,a3
			add.l   d0,(a3)
			move.l  #999999,d1
			cmp.l   (a3),d1 ; is score below 999999?
			bhi.s   @belowmax ; if yes, branch
			move.l  d1,(a3) ; reset score to 999999
		@belowmax:
			move.l  (a3),d0
			cmp.l   (v_scorelife).w,d0 ; has Sonic got 50000+ points?
			blo.s   @noextralife ; if not, branch

			addi.l  #5000,(v_scorelife).w ; increase requirement by 50000
			tst.b   (v_megadrive).w
			bmi.s   @noextralife ; branch if Mega Drive is Japanese
			addq.b  #1,(v_lives).w ; give extra life
			addq.b  #1,(f_lifecount).w
			music	bgm_ExtraLife,1
		endc

@locret_1C6B6:
@noextralife:
		rts
; End of function AddPoints
