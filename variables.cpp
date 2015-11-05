
// Special stage variables:
ubyte v_sslayout[SS_LayoutRows][SS_LayoutColumns];         // 0xFFFF0000* ; tile layout data (0x4000 bytes)
SSTileInfo v_sstileinfo[SSObj_COUNT + 1];                  // 0xFFFF4000* ; tile info (78 tile types, plus empty slot 0)
SS_Animation v_ssanimations[SS_NumAnimations];             // 0xFFFF4400* ; animations in progress
Point16 v_ssposbuffer[SS_PosBufferSize][SS_PosBufferSize]; // 0xFFFF8000* ; calculated screen positions of tiles based on rotation

// Vague memory map:
// 0000-A7FF: Level layout stuff
// A800-A9FF: BG scrolling variable array
// AA00-ABFF: Nemesis decompression dictionary buffer
// AC00-AFFF: Object display queue
// B000-C7FF: 16x16 tile mappings
// C800-CAFF: Buffered Sonic graphics
// CB00-CBFF: Sonic position tracking buffer
// CC00-CFFF: Hardware horizontal scrolling table buffer
// D000-EFFF: Object space
// F000-F5FF: Sound driver
// F600-F6FF: Input, V/HBlank-related variables (like water position, palette buffer, PLC stuff)
// F700-F75F: Scrolling, BG, DLE stuff
// F760-F7FF: Sonic/Object-specific variables, object loading stuff, animation timers, other miscellany
// F800-F9FF: Hardware sprite buffer
// FA00-FBFF: Palettes
// FC00-FDFF: Offscreen object memory
// FE00-FE5F: Various gameplay-related counters, timers, flags, lamppost stuff, emeralds etc.
// FE60-FE9F: Oscillating value array
// FEA0-FF7F: Mostly empty with some random shit
// FF80-FFFF: Mostly debug-related things (level select, that kind of thing), some more mode flags, system stuff

// Regular variables:
ushort v_256x256[82][16][16];            // 0xFFFF0000	; 256x256 tile mappings (0xA400 bytes)
ubyte v_lvllayout[8][128];               // 0xFFFFA400	; level and background 256tile layouts (0x400 bytes) (each row is 64 FG chunks and 64 BG chunks)
ushort v_scrollsomething[256];           // 0xFFFFA800  ; scroll array of some sort
ushort v_ngfx_buffer[256];               // 0xFFFFAA00	; Nemesis graphics decompression dictionary buffer (0x200 bytes)
Object* v_spritequeue[8][64];            // 0xFFFFAC00	; sprite display queue, in order of priority (0x400 bytes). (v_spritequeue[n][0] holds next free slot)
uint v_16x16[768][2];                    // 0xFFFFB000	; 16x16 tile mappings (1800 bytes) (768 tiles, each is two longwords, top and bottom)
ubyte v_sgfx_buffer[768];                // 0xFFFFC800	; buffered Sonic graphics (0x18 cells) (0x300 bytes)
ushort v_tracksonic[128];                // 0xFFFFCB00	; position tracking data for Sonic (0x100 bytes)
uint v_hscrolltablebuffer[256];          // 0xFFFFCC00	; scrolling table data (0x400 bytes)
Object v_objspace[MaxObjects];           // 0xFFFFD000	; object variable space (0x40 bytes per object) (0x2000 bytes)
Object* v_player = &v_objspace[0];       // 0xFFFFD000*	; object variable space for Sonic (0x40 bytes)
Object* v_lvlobjspace = &v_objspace[32]; // 0xFFFFD800*	; level object variable space (0x1800 bytes)
ubyte v_snddriver_ram[0x600];            // 0xFFFFF000  ; sound driver RAM
GameMode v_gamemode;                     // 0xFFFFF600	; game mode
ubyte v_jpadhold2;                       // 0xFFFFF602	; joypad input - held, duplicate
ubyte v_jpadpress2;                      // 0xFFFFF603	; joypad input - pressed, duplicate
ubyte v_jpadhold1;                       // 0xFFFFF604	; joypad input - held
ubyte v_jpadpress1;                      // 0xFFFFF605	; joypad input - pressed
                                         // 0xFFFFF606
                                         // 0xFFFFF607
                                         // 0xFFFFF608
                                         // 0xFFFFF609
                                         // 0xFFFFF60A
                                         // 0xFFFFF60B
ushort v_vdp_buffer1;                    // 0xFFFFF60C	; VDP instruction buffer (2 bytes)
                                         // 0xFFFFF60E
                                         // 0xFFFFF60F
                                         // 0xFFFFF610
                                         // 0xFFFFF611
                                         // 0xFFFFF612
                                         // 0xFFFFF613
ushort v_demolength;                     // 0xFFFFF614	; the length of a demo in frames (2 bytes)
short v_scrposy_dup;                     // 0xFFFFF616	; screen position y (duplicate) (2 bytes)
short v_bg1posy_dupx;                    // 0xFFFFF618  ; BG1 pos y duplicate (2 bytes)
short v_scrposx_dup;                     // 0xFFFFF61A	; screen position x (duplicate) (2 bytes)
short v_bg1posx_dupx;                    // 0xFFFFF61C  ;
short v_bg2posy_dupx;                    // 0xFFFFF61E  ;
short v_bg2posx_dupx;                    // 0xFFFFF620  ;
                                         // 0xFFFFF622
                                         // 0xFFFFF623
ushort v_hbla_hreg;                      // 0xFFFFF624	; VDP H.interrupt register buffer (8Axx) (2 bytes)
// ubyte v_hbla_line;                    // 0xFFFFF625*	; screen line where water starts and palette is changed by HBlank
ubyte v_pfade_start;                     // 0xFFFFF626	; palette fading - start position in bytes
ubyte v_pfade_size;                      // 0xFFFFF627	; palette fading - number of colours
                                         // 0xFFFFF628
                                         // 0xFFFFF629
ubyte v_vbla_routine;                    // 0xFFFFF62A	; VBlank - routine counter
                                         // 0xFFFFF62B
ubyte v_spritecount;                     // 0xFFFFF62C	; number of sprites on-screen
                                         // 0xFFFFF62D
                                         // 0xFFFFF62E
                                         // 0xFFFFF62F
                                         // 0xFFFFF630
                                         // 0xFFFFF631
ushort v_pcyc_num;                       // 0xFFFFF632	; palette cycling - current reference number (2 bytes)
ushort v_pcyc_time;                      // 0xFFFFF634	; palette cycling - time until the next change (2 bytes)
uint v_random;                           // 0xFFFFF636	; pseudo random number buffer (4 bytes)
bool f_pause;                            // 0xFFFFF63A	; flag set to pause the game (2 bytes)
                                         // 0xFFFFF63C
                                         // 0xFFFFF63D
                                         // 0xFFFFF63E
                                         // 0xFFFFF63F
ushort v_vdp_buffer2;                    // 0xFFFFF640	; VDP instruction buffer (2 bytes)
                                         // 0xFFFFF642
                                         // 0xFFFFF643
bool f_hbla_pal;                         // 0xFFFFF644	; flag set to change palette during HBlank (0000 = no; 0001 = change) (2 bytes)
ushort v_waterpos1;                      // 0xFFFFF646	; water height, actual (2 bytes)
ushort v_waterpos2;                      // 0xFFFFF648	; water height, ignoring sway (2 bytes)
ushort v_waterpos3;                      // 0xFFFFF64A	; water height, next target (2 bytes)
bool f_water;                            // 0xFFFFF64C	; flag set for water
ubyte v_wtr_routine;                     // 0xFFFFF64D	; water event - routine counter
bool f_wtr_state;                        // 0xFFFFF64E	; water palette state when water is above/below the screen (00 = partly/all dry; 01 = all underwater)
bool f_FFFFF64F;                         // 0xFFFFF64F? ; something to do with v/hblanks
ushort v_pal_buffer[24];                 // 0xFFFFF650	; palette data buffer (used for palette cycling) (0x30 bytes)
PLCDesc v_plc_buffer[16];                // 0xFFFFF680	; pattern load cues buffer (maximum 0x10 PLCs) (0x60 bytes)
NemDec_State nem_plcState;               // 0xFFFFF6E0  ; nemesis backup state for doing a few tiles of decompression per frame (0x18 bytes)
ushort f_plc_execute;                    // 0xFFFFF6F8	; flag set for pattern load cue execution (2 bytes)
ushort nem_tileDecodeCount;              // 0xFFFFF6FA  ; how many tiles to decompress this PLC cycle
                                         // 0xFFFFF6FC
                                         // 0xFFFFF6FD
                                         // 0xFFFFF6FE
                                         // 0xFFFFF6FF
uint v_screenposx;                       // 0xFFFFF700	; screen position x (2 bytes, but little bits of code and the memory alignment suggest 4)
uint v_screenposy;                       // 0xFFFFF704	; screen position y (2 bytes, but little bits of code and the memory alignment suggest 4)
uint v_bg1posx;                          // 0xFFFFF708
uint v_bg1posy;                          // 0xFFFFF70C
uint v_somethingposx;                    // 0xFFFFF710
uint v_somethingposy;                    // 0xFFFFF714
uint v_bg2posx;                          // 0xFFFFF718
uint v_bg2posy;                          // 0xFFFFF71C
ushort v_limitleft1;                     // 0xFFFFF720	; left level boundary (2 bytes)
ushort v_limitright1;                    // 0xFFFFF722	; right level boundary (2 bytes)
ushort v_limittop1;                      // 0xFFFFF724	; top level boundary (2 bytes)
ushort v_limitbtm1;                      // 0xFFFFF726	; bottom level boundary (2 bytes)
ushort v_limitleft2;                     // 0xFFFFF728	; left level boundary (2 bytes)
ushort v_limitright2;                    // 0xFFFFF72A	; right level boundary (2 bytes)
ushort v_limittop2;                      // 0xFFFFF72C	; top level boundary (2 bytes)
ushort v_limitbtm2;                      // 0xFFFFF72E	; bottom level boundary (2 bytes)
                                         // 0xFFFFF730
                                         // 0xFFFFF731
ushort v_limitleft3;                     // 0xFFFFF732	; left level boundary, at the end of an act (2 bytes)
                                         // 0xFFFFF734
                                         // 0xFFFFF735
                                         // 0xFFFFF736
                                         // 0xFFFFF737
                                         // 0xFFFFF738
                                         // 0xFFFFF739
short v_scrshiftx;                       // 0xFFFFF73A	; screen shift as Sonic moves horizontally
short v_scrshifty;                       // 0xFFFFF73C  ; vertical screen shift
short v_lookshift;                       // 0xFFFFF73E	; screen shift when Sonic looks up/down (2 bytes)
                                         // 0xFFFFF740
                                         // 0xFFFFF741
ubyte v_dle_routine;                     // 0xFFFFF742	; dynamic level event - routine counter
                                         // 0xFFFFF743
bool f_nobgscroll;                       // 0xFFFFF744	; flag set to cancel background scrolling
                                         // 0xFFFFF745
                                         // 0xFFFFF746
                                         // 0xFFFFF747
                                         // 0xFFFFF748
                                         // 0xFFFFF749
ubyte v_screenscrollx16;                 // 0xFFFFF74A  ; used to detect if the screen has scrolled 16 pixels left or right
ubyte v_screenscrolly16;                 // 0xFFFFF74B  ; ditto but up or down
ubyte v_bg1scrollx16;                    // 0xFFFFF74C  ; ditto but for BG1
ubyte v_bg1scrolly16;                    // 0xFFFFF74D  ;
ubyte v_somethingscrollx16;              // 0xFFFFF74E  ; ditto but for "something"
                                         // 0xFFFFF74F
ubyte v_bg2scrollx16;                    // 0xFFFFF750  ; ditto but for BG2
                                         // 0xFFFFF751
                                         // 0xFFFFF752
                                         // 0xFFFFF753
ushort v_bgscroll1;                      // 0xFFFFF754	; background scrolling variable 1
ushort v_bgscroll2;                      // 0xFFFFF756	; background scrolling variable 2
ushort v_bgscroll3;                      // 0xFFFFF758	; background scrolling variable 3
ushort v_bgscroll4;                      // 0xFFFFF75A  ; background scrolling variable 4
bool f_bgscrollvert;                     // 0xFFFFF75C	; flag for vertical background scrolling
                                         // 0xFFFFF75D
                                         // 0xFFFFF75E
                                         // 0xFFFFF75F
short v_sonspeedmax;                     // 0xFFFFF760	; Sonic's maximum speed (2 bytes)
short v_sonspeedacc;                     // 0xFFFFF762	; Sonic's acceleration (2 bytes)
short v_sonspeeddec;                     // 0xFFFFF764	; Sonic's deceleration (2 bytes)
ubyte v_sonframenum;                     // 0xFFFFF766	; frame to display for Sonic
bool f_sonframechg;                      // 0xFFFFF767	; flag set to update Sonic's sprite frame
ubyte v_anglebuffer;                     // 0xFFFFF768	; angle of collision block that Sonic or object is standing on
                                         // 0xFFFFF769
                                         // 0xFFFFF76A
                                         // 0xFFFFF76B
ubyte v_opl_routine;                     // 0xFFFFF76C	; ObjPosLoad - routine counter
                                         // 0xFFFFF71D
ushort v_opl_screen;                     // 0xFFFFF76E	; ObjPosLoad - screen variable
uint v_opl_data[4];                      // 0xFFFFF770	; ObjPosLoad - data buffer (0x10 bytes)
ushort v_ssangle;                        // 0xFFFFF780	; Special Stage angle (2 bytes)
ushort v_ssrotate;                       // 0xFFFFF782	; Special Stage rotation speed (2 bytes)
                                         // 0xFFFFF784
                                         // 0xFFFFF785
                                         // 0xFFFFF786
                                         // 0xFFFFF787
                                         // 0xFFFFF788
                                         // 0xFFFFF789
                                         // 0xFFFFF78A
                                         // 0xFFFFF78B
                                         // 0xFFFFF78C
                                         // 0xFFFFF78D
                                         // 0xFFFFF78E
                                         // 0xFFFFF78F
ushort v_btnpushtime1;                   // 0xFFFFF790	; button push duration - in level (2 bytes)
ushort v_btnpushtime2;                   // 0xFFFFF792	; button push duration - in demo (2 bytes)
ushort v_palchgspeed;                    // 0xFFFFF794	; palette fade/transition speed (0 is fastest) (2 bytes)
uint v_collindex;                        // 0xFFFFF796	; ROM address for collision index of current level (4 bytes)
ushort v_palss_num;                      // 0xFFFFF79A	; palette cycling in Special Stage - reference number (2 bytes)
ushort v_palss_time;                     // 0xFFFFF79C	; palette cycling in Special Stage - time until next change (2 bytes)
                                         // 0xFFFFF79E
                                         // 0xFFFFF79F
                                         // 0xFFFFF7A0
                                         // 0xFFFFF7A1
                                         // 0xFFFFF7A2
                                         // 0xFFFFF7A3
ushort v_obj31ypos;                      // 0xFFFFF7A4	; y-position of object 31 (MZ stomper) (2 bytes)
                                         // 0xFFFFF7A6
ubyte v_bossstatus;                      // 0xFFFFF7A7	; status of boss and prison capsule (01 = boss defeated; 02 = prison opened)
ushort v_trackpos;                       // 0xFFFFF7A8	; position tracking reference number (2 bytes)
// v_trackbyte;                          // 0xFFFFF7A9*	; low byte for position tracking
bool f_lockscreen;                       // 0xFFFFF7AA	; flag set to lock screen during bosses
                                         // 0xFFFFF7AB
ubyte v_256loop1;                        // 0xFFFFF7AC	; 256x256 level tile which contains a loop (GHZ/SLZ)
ubyte v_256loop2;                        // 0xFFFFF7AD	; 256x256 level tile which contains a loop (GHZ/SLZ)
ubyte v_256roll1;                        // 0xFFFFF7AE	; 256x256 level tile which contains a roll tunnel (GHZ)
ubyte v_256roll2;                        // 0xFFFFF7AF	; 256x256 level tile which contains a roll tunnel (GHZ)
ubyte v_lani0_frame;                     // 0xFFFFF7B0	; level graphics animation 0 - current frame
ubyte v_lani0_time;                      // 0xFFFFF7B1	; level graphics animation 0 - time until next frame
ubyte v_lani1_frame;                     // 0xFFFFF7B2	; level graphics animation 1 - current frame
ubyte v_lani1_time;                      // 0xFFFFF7B3	; level graphics animation 1 - time until next frame
ubyte v_lani2_frame;                     // 0xFFFFF7B4	; level graphics animation 2 - current frame
ubyte v_lani2_time;                      // 0xFFFFF7B5	; level graphics animation 2 - time until next frame
ubyte v_lani3_frame;                     // 0xFFFFF7B6	; level graphics animation 3 - current frame
ubyte v_lani3_time;                      // 0xFFFFF7B7	; level graphics animation 3 - time until next frame
ubyte v_lani4_frame;                     // 0xFFFFF7B8	; level graphics animation 4 - current frame
ubyte v_lani4_time;                      // 0xFFFFF7B9	; level graphics animation 4 - time until next frame
ubyte v_lani5_frame;                     // 0xFFFFF7BA	; level graphics animation 5 - current frame
ubyte v_lani5_time;                      // 0xFFFFF7BB	; level graphics animation 5 - time until next frame
                                         // 0xFFFFF7BC
                                         // 0xFFFFF7BD
ushort v_gfxbigring;                     // 0xFFFFF7BE	; settings for giant ring graphics loading (2 bytes)
bool f_conveyrev;                        // 0xFFFFF7C0	; flag set to reverse conveyor belts in LZ/SBZ
ushort v_obj63[3];                       // 0xFFFFF7C1	; object 63 (LZ/SBZ platforms) variables (6 bytes)
bool f_wtunnelmode;                      // 0xFFFFF7C7	; LZ water tunnel mode
bool f_lockmulti;                        // 0xFFFFF7C8	; flag set to lock controls, lock Sonic's position & animation
bool f_wtunnelallow;                     // 0xFFFFF7C9	; LZ water tunnels (00 = enabled; 01 = disabled)
bool f_jumponly;                         // 0xFFFFF7CA	; flag set to lock controls apart from jumping
bool v_obj6B;                            // 0xFFFFF7CB	; object 6B (SBZ stomper) variable
bool f_lockctrl;                         // 0xFFFFF7CC	; flag set to lock controls during ending sequence
bool f_bigring;                          // 0xFFFFF7CD	; flag set when Sonic collects the giant ring
                                         // 0xFFFFF7CE
                                         // 0xFFFFF7CF
ushort v_itembonus;                      // 0xFFFFF7D0	; item bonus from broken enemies, blocks etc. (2 bytes)
ushort v_timebonus;                      // 0xFFFFF7D2	; time bonus at the end of an act (2 bytes)
ushort v_ringbonus;                      // 0xFFFFF7D4	; ring bonus at the end of an act (2 bytes)
bool f_endactbonus;                      // 0xFFFFF7D6	; time/ring bonus update flag at the end of an act
ubyte v_sonicend;                        // 0xFFFFF7D7	; routine counter for Sonic in the ending sequence
ushort v_lzrippleheight;                 // 0xFFFFF7D8  ; JP1: vertical position of the ripple in the FG/BG
                                         // 0xFFFFF7DA
                                         // 0xFFFFF7DB
                                         // 0xFFFFF7DC
                                         // 0xFFFFF7DD
                                         // 0xFFFFF7DE
                                         // 0xFFFFF7DF
ubyte f_switch[16];                      // 0xFFFFF7E0	; flags set when Sonic stands on a switch (0x10 bytes)
ushort v_rev0something1;                 // 0xFFFFF7F0? ; something to do with level boundaries or tiles
ushort v_rev0something2;                 // 0xFFFFF7F2? ; ditto
ushort v_rev0something3;                 // 0xFFFFF7F4? ; ditto
ushort v_rev0something4;                 // 0xFFFFF7F6? ; ditto
                                         // 0xFFFFF7F8
                                         // 0xFFFFF7F9
                                         // 0xFFFFF7FA
                                         // 0xFFFFF7FB
                                         // 0xFFFFF7FC
                                         // 0xFFFFF7FD
                                         // 0xFFFFF7FE
                                         // 0xFFFFF7FF
ushort v_spritetablebuffer[256];         // 0xFFFFF800	; sprite table, 64 4-word entries (0x200 bytes)
ushort v_pal_water_dup[64];              // 0xFFFFFA00	; duplicate underwater palette, used for transitions (0x80 bytes)
ushort v_pal_water[64];                  // 0xFFFFFA80	; main underwater palette (0x80 bytes)
ushort v_pal_dry[64];                    // 0xFFFFFB00	; main palette (0x80 bytes)
ushort v_pal_dry_dup[64];                // 0xFFFFFB80	; duplicate palette, used for transitions (0x80 bytes)
ubyte v_objstate[512];                   // 0xFFFFFC00	; object state list (0x200 bytes)
int v_regbuffer[16]                      // 0xFFFFFC00*	; stores registers d0-a7 during an error event (0x40 bytes)
int v_spbuffer;                          // 0xFFFFFC40*	; stores most recent sp address (4 bytes)
int v_errortype;                         // 0xFFFFFC44*	; error type
                                         // 0xFFFFFE00
                                         // 0xFFFFFE01
bool f_restart;                          // 0xFFFFFE02	; restart level flag (2 bytes)
ushort v_framecount;                     // 0xFFFFFE04	; frame counter (adds 1 every frame) (2 bytes)
// ubyte v_framebyte;                    // 0xFFFFFE05*	; low byte for frame counter
ubyte v_debugitem;                       // 0xFFFFFE06	; debug item currently selected (NOT the object number of the item)
                                         // 0xFFFFFE07
ushort v_debuguse;                       // 0xFFFFFE08	; debug mode use & routine counter (when Sonic is a ring/item) (2 bytes)
byte v_debugxspeed;                      // 0xFFFFFE0A	; debug mode - horizontal speed
byte v_debugyspeed;                      // 0xFFFFFE0B	; debug mode - vertical speed
uint v_vbla_count;                       // 0xFFFFFE0C	; vertical interrupt counter (adds 1 every VBlank) (4 bytes)
// ushort v_vbla_word;                   // 0xFFFFFE0E*	; low word for vertical interrupt counter (2 bytes)
// ubyte v_vbla_byte;                    // 0xFFFFFE0F*	; low byte for vertical interrupt counter
ubyte v_zone;                            // 0xFFFFFE10	; current zone number (often accessed as a word to read/write zone and act simultaneously)
ubyte v_act;                             // 0xFFFFFE11	; current act number
ubyte v_lives;                           // 0xFFFFFE12	; number of lives
                                         // 0xFFFFFE13
ushort v_air;                            // 0xFFFFFE14	; air remaining while underwater (2 bytes)
// ubyte v_airbyte;                      // 0xFFFFFE15*	; low byte for air
ubyte v_lastspecial;                     // 0xFFFFFE16	; last special stage number
                                         // 0xFFFFFE17
ubyte v_continues;                       // 0xFFFFFE18	; number of continues
                                         // 0xFFFFFE19
bool f_timeover;                         // 0xFFFFFE1A	; time over flag
ubyte v_lifecount;                       // 0xFFFFFE1B	; lives counter value (for actual number, see "v_lives")
bool f_lifecount;                        // 0xFFFFFE1C	; lives counter update flag
bool f_ringcount;                        // 0xFFFFFE1D	; ring counter update flag
bool f_timecount;                        // 0xFFFFFE1E	; time counter update flag
bool f_scorecount;                       // 0xFFFFFE1F	; score counter update flag
ushort v_rings;                          // 0xFFFFFE20	; rings (2 bytes)
// ubyte v_ringbyte;                     // 0xFFFFFE21*	; low byte for rings
uint v_time;                             // 0xFFFFFE22	; time (4 bytes)
// ubyte v_timemin;                      // 0xFFFFFE23*	; time - minutes
// ubyte v_timesec;                      // 0xFFFFFE24*	; time - seconds
// ubyte v_timecent;                     // 0xFFFFFE25*	; time - centiseconds
uint v_score;                            // 0xFFFFFE26	; score (4 bytes)
                                         // 0xFFFFFE2A
                                         // 0xFFFFFE2B
bool v_shield;                           // 0xFFFFFE2C	; shield status (00 = no; 01 = yes)
bool v_invinc;                           // 0xFFFFFE2D	; invinciblity status (00 = no; 01 = yes)
bool v_shoes;                            // 0xFFFFFE2E	; speed shoes status (00 = no; 01 = yes)
ubyte v_FFFFFE2F;                        // 0xFFFFFE2F?	; specifically zeroed in a few places, not sure if read
ubyte v_lastlamp;                        // 0xFFFFFE30	; number of the last lamppost you hit
ubyte v_lamp_lastlamp;                   // 0xFFFFFE31	; technically same as v_lastlamp, not sure why saved/restored
ushort v_lamp_xpos;                      // 0xFFFFFE32	; x-axis for Sonic to respawn at lamppost (2 bytes)
ushort v_lamp_ypos;                      // 0xFFFFFE34	; y-axis for Sonic to respawn at lamppost (2 bytes)
ushort v_lamp_rings;                     // 0xFFFFFE36	; rings stored at lamppost (2 bytes)
uint v_lamp_time;                        // 0xFFFFFE38	; time stored at lamppost (4 bytes)
ubyte v_lamp_dle;                        // 0xFFFFFE3C	; dynamic level event routine counter at lamppost
ubyte v_FFFFFE3D;                        // 0xFFFFFE3D?	; set in the credits sequence...
ushort v_lamp_limitbtm;                  // 0xFFFFFE3E	; level bottom boundary at lamppost (2 bytes)
ushort v_lamp_scrx;                      // 0xFFFFFE40	; x-axis screen at lamppost (2 bytes)
ushort v_lamp_scry;                      // 0xFFFFFE42	; y-axis screen at lamppost (2 bytes)
ushort v_lamp_bg1x;                      // 0xFFFFFE44	; scrolling info
ushort v_lamp_bg1y;                      // 0xFFFFFE46	; scrolling info
ushort v_lamp_somethingx;                // 0xFFFFFE48	; scrolling info
ushort v_lamp_somethingy;                // 0xFFFFFE4A	; scrolling info
ushort v_lamp_bg2x;                      // 0xFFFFFE4C	; scrolling info
ushort v_lamp_bg2y;                      // 0xFFFFFE4E	; scrolling info
ushort v_lamp_wtrpos;                    // 0xFFFFFE50	; water position at lamppost (2 bytes)
ubyte v_lamp_wtrrout;                    // 0xFFFFFE52	; water routine at lamppost
ubyte v_lamp_wtrstat;                    // 0xFFFFFE53	; water state at lamppost
ubyte v_lamp_lives;                      // 0xFFFFFE54	; lives counter at lamppost
                                         // 0xFFFFFE55
                                         // 0xFFFFFE56
ubyte v_emeralds;                        // 0xFFFFFE57	; number of chaos emeralds
ubyte v_emldlist[6];                     // 0xFFFFFE58	; which individual emeralds you have (00 = no; 01 = yes) (6 bytes)
ushort v_oscillate[33];                  // 0xFFFFFE5E	; values which oscillate - for swinging platforms, et al (0x42 bytes)
                                         // 0xFFFFFEA0
                                         // 0xFFFFFEA1
                                         // 0xFFFFFEA2
                                         // 0xFFFFFEA3
                                         // 0xFFFFFEA4
                                         // 0xFFFFFEA5
                                         // 0xFFFFFEA6
                                         // 0xFFFFFEA7
                                         // 0xFFFFFEA8
                                         // 0xFFFFFEA9
                                         // 0xFFFFFEAA
                                         // 0xFFFFFEAB
                                         // 0xFFFFFEAC
                                         // 0xFFFFFEAD
                                         // 0xFFFFFEAE
                                         // 0xFFFFFEAF
                                         // 0xFFFFFEB0
                                         // 0xFFFFFEB1
                                         // 0xFFFFFEB2
                                         // 0xFFFFFEB3
                                         // 0xFFFFFEB4
                                         // 0xFFFFFEB5
                                         // 0xFFFFFEB6
                                         // 0xFFFFFEB7
                                         // 0xFFFFFEB8
                                         // 0xFFFFFEB9
                                         // 0xFFFFFEBA
                                         // 0xFFFFFEBB
                                         // 0xFFFFFEBC
                                         // 0xFFFFFEBD
                                         // 0xFFFFFEBE
                                         // 0xFFFFFEBF
ubyte v_ani0_time;                       // 0xFFFFFEC0	; synchronised sprite animation 0 - time until next frame (used for synchronised animations)
ubyte v_ani0_frame;                      // 0xFFFFFEC1	; synchronised sprite animation 0 - current frame
ubyte v_ani1_time;                       // 0xFFFFFEC2	; synchronised sprite animation 1 - time until next frame
ubyte v_ani1_frame;                      // 0xFFFFFEC3	; synchronised sprite animation 1 - current frame
ubyte v_ani2_time;                       // 0xFFFFFEC4	; synchronised sprite animation 2 - time until next frame
ubyte v_ani2_frame;                      // 0xFFFFFEC5	; synchronised sprite animation 2 - current frame
ubyte v_ani3_time;                       // 0xFFFFFEC6	; synchronised sprite animation 3 - time until next frame
ubyte v_ani3_frame;                      // 0xFFFFFEC7	; synchronised sprite animation 3 - current frame
ushort v_ani3_buf;                       // 0xFFFFFEC8	; synchronised sprite animation 3 - info buffer (2 bytes)
                                         // 0xFFFFFECA
                                         // 0xFFFFFECB
                                         // 0xFFFFFECC
                                         // 0xFFFFFECD
                                         // 0xFFFFFECE
                                         // 0xFFFFFECF
                                         // 0xFFFFFED0
                                         // 0xFFFFFED1
                                         // 0xFFFFFED2
                                         // 0xFFFFFED3
                                         // 0xFFFFFED4
                                         // 0xFFFFFED5
                                         // 0xFFFFFED6
                                         // 0xFFFFFED7
                                         // 0xFFFFFED8
                                         // 0xFFFFFED9
                                         // 0xFFFFFEDA
                                         // 0xFFFFFEDB
                                         // 0xFFFFFEDC
                                         // 0xFFFFFEDD
                                         // 0xFFFFFEDE
                                         // 0xFFFFFEDF
                                         // 0xFFFFFEE0
                                         // 0xFFFFFEE1
                                         // 0xFFFFFEE2
                                         // 0xFFFFFEE3
                                         // 0xFFFFFEE4
                                         // 0xFFFFFEE5
                                         // 0xFFFFFEE6
                                         // 0xFFFFFEE7
                                         // 0xFFFFFEE8
                                         // 0xFFFFFEE9
                                         // 0xFFFFFEEA
                                         // 0xFFFFFEEB
                                         // 0xFFFFFEEC
                                         // 0xFFFFFEED
                                         // 0xFFFFFEEE
                                         // 0xFFFFFEEF
ushort v_limittopdb;                     // 0xFFFFFEF0	; level upper boundary, buffered for debug mode (2 bytes)
ushort v_limitbtmdb;                     // 0xFFFFFEF2	; level bottom boundary, buffered for debug mode (2 bytes)
                                         // 0xFFFFFEE4
                                         // ... (28 bytes unaccounted for)
                                         // 0xFFFFFEFF
ushort v_dummytile;                      // 0xFFFFFF00  ; maybe? FindNearestTile seems to return this address if there is no tile
                                         // 0xFFFFFF02
                                         // 0xFFFFFF03
                                         // 0xFFFFFF04
                                         // 0xFFFFFF05
                                         // 0xFFFFFF06
                                         // 0xFFFFFF07
                                         // 0xFFFFFF08
                                         // 0xFFFFFF09
                                         // 0xFFFFFF0A
                                         // 0xFFFFFF0B
                                         // 0xFFFFFF0C
                                         // 0xFFFFFF0D
                                         // 0xFFFFFF0E
                                         // 0xFFFFFF0F
uint v_screenposx_dup;                   // 0xFFFFFF10
uint v_screenposy_dup;                   // 0xFFFFFF14
uint v_bg1posx_dup;                      // 0xFFFFFF18
uint v_bg1posy_dup;                      // 0xFFFFFF1C
uint v_somethingposx_dup;                // 0xFFFFFF20
uint v_somethingposy_dup;                // 0xFFFFFF24
uint v_bg2posx_dup;                      // 0xFFFFFF28
uint v_bg2posy_dup;                      // 0xFFFFFF2C
ushort v_bgscroll1_dup;                  // 0xFFFFFF30
ushort v_bgscroll2_dup;                  // 0xFFFFFF32
ushort v_bgscroll3_dup;                  // 0xFFFFFF34
ushort v_bgscroll4_dup;                  // 0xFFFFFF36
                                         // 0xFFFFFF38
                                         // ... (72 bytes unaccounted for)
                                         // 0xFFFFFF7F
ushort v_levseldelay;                    // 0xFFFFFF80	; level select - time until change when up/down is held (2 bytes)
ushort v_levselitem;                     // 0xFFFFFF82	; level select - item selected (2 bytes)
ushort v_levselsound;                    // 0xFFFFFF84	; level select - sound selected (2 bytes)
                                         // 0xFFFFFF86
                                         // ... (314 bytes unaccounted for)
                                         // 0xFFFFFFBF
uint v_scorecopy;                        // 0xFFFFFFC0	; score, duplicate (4 bytes)
// uint v_scorelife;                     // 0xFFFFFFC0*	; points required for an extra life (4 bytes) (JP1 only)
                                         // 0xFFFFFFC4
                                         // 0xFFFFFFC5
                                         // 0xFFFFFFC6
                                         // 0xFFFFFFC7
                                         // 0xFFFFFFC8
                                         // 0xFFFFFFC9
                                         // 0xFFFFFFCA
                                         // 0xFFFFFFCB
                                         // 0xFFFFFFCC
                                         // 0xFFFFFFCD
                                         // 0xFFFFFFCE
                                         // 0xFFFFFFCF
                                         // 0xFFFFFFD0
                                         // 0xFFFFFFD1
                                         // 0xFFFFFFD2
                                         // 0xFFFFFFD3
                                         // 0xFFFFFFD4
                                         // 0xFFFFFFD5
                                         // 0xFFFFFFD6
                                         // 0xFFFFFFD7
                                         // 0xFFFFFFD8
                                         // 0xFFFFFFD9
                                         // 0xFFFFFFDA
                                         // 0xFFFFFFDB
                                         // 0xFFFFFFDC
                                         // 0xFFFFFFDD
                                         // 0xFFFFFFDE
                                         // 0xFFFFFFDF
bool f_levselcheat;                      // 0xFFFFFFE0	; level select cheat flag
bool f_slomocheat;                       // 0xFFFFFFE1	; slow motion & frame advance cheat flag
bool f_debugcheat;                       // 0xFFFFFFE2	; debug mode cheat flag
bool f_creditscheat;                     // 0xFFFFFFE3	; hidden credits & press start cheat flag
ushort v_title_dcount;                   // 0xFFFFFFE4	; number of times the d-pad is pressed on title screen (2 bytes)
ushort v_title_ccount;                   // 0xFFFFFFE6	; number of times C is pressed on title screen (2 bytes)
                                         // 0xFFFFFFE8
                                         // 0xFFFFFFE9
ushort v_FFFFFFEA;                       // 0xFFFFFFEA  ; unused; set to 0 in one spot and never touched
                                         // 0xFFFFFFEC
                                         // 0xFFFFFFED
                                         // 0xFFFFFFEE
                                         // 0xFFFFFFEF
ushort f_demo;                           // 0xFFFFFFF0	; demo mode flag (0 = no; 1 = yes; 0x8001 = ending) (2 bytes)
ushort v_demonum;                        // 0xFFFFFFF2	; demo level number (not the same as the level number) (2 bytes)
ushort v_creditsnum;                     // 0xFFFFFFF4	; credits index number (2 bytes)
                                         // 0xFFFFFFF6
                                         // 0xFFFFFFF7
ubyte v_megadrive;                       // 0xFFFFFFF8	; Megadrive machine type
                                         // 0xFFFFFFF9
bool f_debugmode;                        // 0xFFFFFFFA	; debug mode flag (sometimes 2 bytes)
uint v_init;                             // 0xFFFFFFFC	; 'init' text string (4 bytes)

/*
// =================================================================================
// From here on, until otherwise stated, all offsets are relative to v_snddriver_ram
// =================================================================================
v_sndprio; // 0x000	; sound priority (priority of new music/SFX must be higher or equal to this value or it won't play; bit 7 of priority being set prevents this value from changing)
v_main_tempo_timeout; // 0x001	; Counts down to zero; when zero, resets to next value and delays song by 1 frame
v_main_tempo; // 0x002	; Used for music only
f_stopmusic; // 0x003	; flag set to stop music when paused
v_fadeout_counter; // 0x004

v_fadeout_delay; // 0x006

f_updating_dac; // 0x008	; 0x80 if updating DAC, 0x00 otherwise
v_playsnd0; // 0x009	; sound or music copied from below
v_playsnd1; // 0x00A	; sound or music to play
v_playsnd2; // 0x00B	; special sound to play
v_playnull; // 0x00C	; unused sound to play

f_voice_selector; // 0x00E	; 0x00 = use music voice pointer; 0x40 = use special voice pointer; 0x80 = use track voice pointer

v_voice_ptr; // 0x018	; voice data pointer (4 bytes)

v_special_voice_ptr; // 0x020	; voice data pointer for special SFX (0xD0-0xDF) (4 bytes)

f_fadein_flag; // 0x024	; Flag for fade in
v_fadein_delay; // 0x025
v_fadein_counter; // 0x026	; Timer for fade in/out
f_1up_playing; // 0x027	; flag indicating 1-up song is playing
v_tempo_mod; // 0x028	; music - tempo modifier
v_speeduptempo; // 0x029	; music - tempo modifier with speed shoes
f_speedup; // 0x02A	; flag indicating whether speed shoes tempo is on (0x80) or off (0x00)
v_ring_speaker; // 0x02B	; which speaker the "ring" sound is played in (00 = right; 01 = left)
f_push_playing; // 0x02C	; if set, prevents further push sounds from playing

v_track_ram; // 0x040	; Start of music RAM

v_dac_track; // 0x040
v_dac_playback_control; // 0x040	; Playback control bits for DAC channel
v_dac_voice_control; // 0x041	; Voice control bits for DAC channel
v_dac_tempo_time; // 0x042	; music - tempo dividing timing
v_dac_ptr; // 0x044	; DAC channel pointer (4 bytes)
v_dac_amsfmspan; // 0x04A
v_dac_stack_ptr; // 0x04D
v_dac_note_timeout; // 0x04E	; Counts down to zero; when zero, a new DAC sample is needed
v_dac_note_duration; // 0x04F
v_dac_curr_sample; // 0x050
v_dac_loop_index; // 0x064	; Several bytes, may overlap with gosub/return stack

; Note: using the channel assignment bits to determine FM channel #. Thus, there is no FM 3.

v_fm1_track; // 0x070
v_fm1_playback_control; // 0x070	; Playback control bits for FM1
v_fm1_voice_control; // 0x071	; Voice control bits
v_fm1_tempo_time; // 0x072	; music - tempo dividing timing
v_fm1_ptr; // 0x074	; FM channel 0 pointer (4 bytes)
v_fm1_key; // 0x078	; FM channel 0 key displacement
v_fm1_volume; // 0x079	; FM channel 0 volume attenuation
v_fm1_amsfmspan; // 0x07A
v_fm1_voice; // 0x07B
v_fm1_stack_ptr; // 0x07D
v_fm1_note_timeout; // 0x07E	; Counts down to zero; when zero, a new note is needed
v_fm1_note_duration; // 0x07F
v_fm1_curr_note; // 0x080
v_fm1_note_fill; // 0x082
v_fm1_note_fill_master; // 0x083
v_fm1_modulation_ptr; // 0x084	; 4 bytes
v_fm1_modulation_wait; // 0x088
v_fm1_modulation_speed; // 0x089
v_fm1_modulation_delta; // 0x08A
v_fm1_modulation_steps; // 0x08B
v_fm1_modulation_freq; // 0x08C	; 2 bytes
v_fm1_freq_adjust; // 0x08E
v_fm1_feedbackalgo; // 0x08F
v_fm1_loop_index; // 0x094	; Several bytes, may overlap with gosub/return stack

v_fm2_track; // 0x0A0
v_fm2_playback_control; // 0x0A0	; Playback control bits for FM2
v_fm2_voice_control; // 0x0A1	; Voice control bits
v_fm2_tempo_time; // 0x0A2	; music - tempo dividing timing
v_fm2_ptr; // 0x0A4	; FM channel 1 pointer (4 bytes)
v_fm2_key; // 0x0A8	; FM channel 1 key displacement
v_fm2_volume; // 0x0A9	; FM channel 1 volume attenuation
v_fm2_amsfmspan; // 0x0AA
v_fm2_voice; // 0x0AB
v_fm2_stack_ptr; // 0x0AD
v_fm2_note_timeout; // 0x0AE	; Counts down to zero; when zero, a new note is needed
v_fm2_note_duration; // 0x0AF
v_fm2_curr_note; // 0x0B0
v_fm2_note_fill; // 0x0B2
v_fm2_note_fill_master; // 0x0B3
v_fm2_modulation_ptr; // 0x0B4	; 4 bytes
v_fm2_modulation_wait; // 0x0B8
v_fm2_modulation_speed; // 0x0B9
v_fm2_modulation_delta; // 0x0BA
v_fm2_modulation_steps; // 0x0BB
v_fm2_modulation_freq; // 0x0BC	; 2 bytes
v_fm2_freq_adjust; // 0x0BE
v_fm2_feedbackalgo; // 0x0BF
v_fm2_loop_index; // 0x0C4	; Several bytes, may overlap with gosub/return stack

v_fm3_track; // 0x0D0
v_fm3_playback_control; // 0x0D0	; Playback control bits for FM3
v_fm3_voice_control; // 0x0D1	; Voice control bits
v_fm3_tempo_time; // 0x0D2	; music - tempo dividing timing
v_fm3_ptr; // 0x0D4	; FM channel 2 pointer (4 bytes)
v_fm3_key; // 0x0D8	; FM channel 2 key displacement
v_fm3_volume; // 0x0D9	; FM channel 2 volume attenuation
v_fm3_amsfmspan; // 0x0DA
v_fm3_voice; // 0x0DB
v_fm3_stack_ptr; // 0x0DD
v_fm3_note_timeout; // 0x0DE	; Counts down to zero; when zero, a new note is needed
v_fm3_note_duration; // 0x0DF
v_fm3_curr_note; // 0x0E0
v_fm3_note_fill; // 0x0E2
v_fm3_note_fill_master; // 0x0E3
v_fm3_modulation_ptr; // 0x0E4	; 4 bytes
v_fm3_modulation_wait; // 0x0E8
v_fm3_modulation_speed; // 0x0E9
v_fm3_modulation_delta; // 0x0EA
v_fm3_modulation_steps; // 0x0EB
v_fm3_modulation_freq; // 0x0EC	; 2 bytes
v_fm3_freq_adjust; // 0x0EE
v_fm3_feedbackalgo; // 0x0EF
v_fm3_loop_index; // 0x0F4	; Several bytes, may overlap with gosub/return stack

v_fm4_track; // 0x100
v_fm4_playback_control; // 0x100	; Playback control bits for FM4
v_fm4_voice_control; // 0x101	; Voice control bits
v_fm4_tempo_time; // 0x102	; music - tempo dividing timing
v_fm4_ptr; // 0x104	; FM channel 4 pointer (4 bytes)
v_fm4_key; // 0x108	; FM channel 4 key displacement
v_fm4_volume; // 0x109	; FM channel 4 volume attenuation
v_fm4_amsfmspan; // 0x10A
v_fm4_voice; // 0x10B
v_fm4_stack_ptr; // 0x10D
v_fm4_note_timeout; // 0x10E	; Counts down to zero; when zero, a new note is needed
v_fm4_note_duration; // 0x10F
v_fm4_curr_note; // 0x110
v_fm4_note_fill; // 0x112
v_fm4_note_fill_master; // 0x113
v_fm4_modulation_ptr; // 0x114	; 4 bytes
v_fm4_modulation_wait; // 0x118
v_fm4_modulation_speed; // 0x119
v_fm4_modulation_delta; // 0x11A
v_fm4_modulation_steps; // 0x11B
v_fm4_modulation_freq; // 0x11C	; 2 bytes
v_fm4_freq_adjust; // 0x11E
v_fm4_feedbackalgo; // 0x11F
v_fm4_loop_index; // 0x124	; Several bytes, may overlap with gosub/return stack

v_fm5_track; // 0x130
v_fm5_playback_control; // 0x130	; Playback control bits for FM5
v_fm5_voice_control; // 0x131	; Voice control bits
v_fm5_tempo_time; // 0x132	; music - tempo dividing timing
v_fm5_ptr; // 0x134	; FM channel 5 pointer (4 bytes)
v_fm5_key; // 0x138	; FM channel 5 key displacement
v_fm5_volume; // 0x139	; FM channel 5 volume attenuation
v_fm5_amsfmspan; // 0x13A
v_fm5_voice; // 0x13B
v_fm5_stack_ptr; // 0x13D
v_fm5_note_timeout; // 0x13E	; Counts down to zero; when zero, a new note is needed
v_fm5_note_duration; // 0x13F
v_fm5_curr_note; // 0x140
v_fm5_note_fill; // 0x142
v_fm5_note_fill_master; // 0x143
v_fm5_modulation_ptr; // 0x144	; 4 bytes
v_fm5_modulation_wait; // 0x148
v_fm5_modulation_speed; // 0x149
v_fm5_modulation_delta; // 0x14A
v_fm5_modulation_steps; // 0x14B
v_fm5_modulation_freq; // 0x14C	; 2 bytes
v_fm5_freq_adjust; // 0x14E
v_fm5_feedbackalgo; // 0x14F
v_fm5_loop_index; // 0x154	; Several bytes, may overlap with gosub/return stack

v_fm6_track; // 0x160
v_fm6_playback_control; // 0x160	; Playback control bits for FM6
v_fm6_voice_control; // 0x161	; Voice control bits
v_fm6_tempo_time; // 0x162	; music - tempo dividing timing
v_fm6_ptr; // 0x164	; FM channel 6 pointer (4 bytes)
v_fm6_key; // 0x168	; FM channel 6 key displacement
v_fm6_volume; // 0x169	; FM channel 6 volume attenuation
v_fm6_amsfmspan; // 0x16A
v_fm6_voice; // 0x16B
v_fm6_stack_ptr; // 0x16D
v_fm6_note_timeout; // 0x16E	; Counts down to zero; when zero, a new note is needed
v_fm6_note_duration; // 0x16F
v_fm6_curr_note; // 0x170
v_fm6_note_fill; // 0x172
v_fm6_note_fill_master; // 0x173
v_fm6_modulation_ptr; // 0x174	; 4 bytes
v_fm6_modulation_wait; // 0x178
v_fm6_modulation_speed; // 0x179
v_fm6_modulation_delta; // 0x17A
v_fm6_modulation_steps; // 0x17B
v_fm6_modulation_freq; // 0x17C	; 2 bytes
v_fm6_freq_adjust; // 0x17E
v_fm6_feedbackalgo; // 0x17F
v_fm6_loop_index; // 0x184	; Several bytes, may overlap with gosub/return stack

v_psg1_track; // 0x190
v_psg1_playback_control; // 0x190	; Playback control bits for PSG1
v_psg1_voice_control; // 0x191	; Voice control bits
v_psg1_tempo_time; // 0x192	; music - tempo dividing timing
v_psg1_ptr; // 0x194	; PSG channel 1 pointer (4 bytes)
v_psg1_key; // 0x198	; PSG channel 1 key displacement
v_psg1_volume; // 0x199	; PSG channel 1 volume attenuation
v_psg1_amsfmspan; // 0x19A
v_psg1_tone; // 0x19B
v_psg1_flutter_index; // 0x19C
v_psg1_stack_ptr; // 0x19D
v_psg1_note_timeout; // 0x19E	; Counts down to zero; when zero, a new note is needed
v_psg1_note_duration; // 0x19F
v_psg1_curr_note; // 0x1A0
v_psg1_note_fill; // 0x1A2
v_psg1_note_fill_master; // 0x1A3
v_psg1_modulation_ptr; // 0x1A4	; 4 bytes
v_psg1_modulation_wait; // 0x1A8
v_psg1_modulation_speed; // 0x1A9
v_psg1_modulation_delta; // 0x1AA
v_psg1_modulation_steps; // 0x1AB
v_psg1_modulation_freq; // 0x1AC	; 2 bytes
v_psg1_freq_adjust; // 0x1AE
v_psg1_noise; // 0x1AF
v_psg1_loop_index; // 0x1B4	; Several bytes, may overlap with gosub/return stack

v_psg2_track; // 0x1C0
v_psg2_playback_control; // 0x1C0	; Playback control bits for PSG2
v_psg2_voice_control; // 0x1C1	; Voice control bits
v_psg2_tempo_time; // 0x1C2	; music - tempo dividing timing
v_psg2_ptr; // 0x1C4	; PSG channel 2 pointer (4 bytes)
v_psg2_key; // 0x1C8	; PSG channel 2 key displacement
v_psg2_volume; // 0x1C9	; PSG channel 2 volume attenuation
v_psg2_amsfmspan; // 0x1CA
v_psg2_tone; // 0x1CB
v_psg2_flutter_index; // 0x1CC
v_psg2_stack_ptr; // 0x1CD
v_psg2_note_timeout; // 0x1CE	; Counts down to zero; when zero, a new note is needed
v_psg2_note_duration; // 0x1CF
v_psg2_curr_note; // 0x1D0
v_psg2_note_fill; // 0x1D2
v_psg2_note_fill_master; // 0x1D3
v_psg2_modulation_ptr; // 0x1D4	; 4 bytes
v_psg2_modulation_wait; // 0x1D8
v_psg2_modulation_speed; // 0x1D9
v_psg2_modulation_delta; // 0x1DA
v_psg2_modulation_steps; // 0x1DB
v_psg2_modulation_freq; // 0x1DC	; 2 bytes
v_psg2_freq_adjust; // 0x1DE
v_psg2_noise; // 0x1DF
v_psg2_loop_index; // 0x1E4	; Several bytes, may overlap with gosub/return stack

v_psg3_track; // 0x1F0
v_psg3_playback_control; // 0x1F0	; Playback control bits for PSG3
v_psg3_voice_control; // 0x1F1	; Voice control bits
v_psg3_tempo_time; // 0x1F2	; music - tempo dividing timing
v_psg3_ptr; // 0x1F4	; PSG channel 3 pointer (4 bytes)
v_psg3_key; // 0x1F8	; PSG channel 3 key displacement
v_psg3_volume; // 0x1F9	; PSG channel 3 volume attenuation
v_psg3_amsfmspan; // 0x1FA
v_psg3_tone; // 0x1FB
v_psg3_flutter_index; // 0x1FC
v_psg3_stack_ptr; // 0x1FD
v_psg3_note_timeout; // 0x1FE	; Counts down to zero; when zero, a new note is needed
v_psg3_note_duration; // 0x1FF
v_psg3_curr_note; // 0x200
v_psg3_note_fill; // 0x202
v_psg3_note_fill_master; // 0x203
v_psg3_modulation_ptr; // 0x204	; 4 bytes
v_psg3_modulation_wait; // 0x208
v_psg3_modulation_speed; // 0x209
v_psg3_modulation_delta; // 0x20A
v_psg3_modulation_steps; // 0x20B
v_psg3_modulation_freq; // 0x20C	; 2 bytes
v_psg3_freq_adjust; // 0x20E
v_psg3_noise; // 0x20F
v_psg3_loop_index; // 0x214	; Several bytes, may overlap with gosub/return stack

v_sfx_track_ram; // 0x220	; Start of sfx RAM

v_sfx_fm3_track; // 0x220
v_sfx_fm3_playback_control; // 0x220	; Playback control bits for sfx FM3
v_sfx_fm3_voice_control; // 0x221	; Voice control bits
v_sfx_fm3_tempo_time; // 0x222	; sfx - tempo dividing timing
v_sfx_fm3_ptr; // 0x224	; FM channel 2 pointer (4 bytes)
v_sfx_fm3_key; // 0x228	; FM channel 2 key displacement
v_sfx_fm3_volume; // 0x229	; FM channel 2 volume attenuation
v_sfx_fm3_amsfmspan; // 0x22A
v_sfx_fm3_voice; // 0x22B
v_sfx_fm3_stack_ptr; // 0x22D
v_sfx_fm3_note_timeout; // 0x22E	; Counts down to zero; when zero, a new note is needed
v_sfx_fm3_note_duration; // 0x22F
v_sfx_fm3_curr_note; // 0x230
v_sfx_fm3_note_fill; // 0x232
v_sfx_fm3_note_fill_master; // 0x233
v_sfx_fm3_modulation_ptr; // 0x234	; 4 bytes
v_sfx_fm3_modulation_wait; // 0x238
v_sfx_fm3_modulation_speed; // 0x239
v_sfx_fm3_modulation_delta; // 0x23A
v_sfx_fm3_modulation_steps; // 0x23B
v_sfx_fm3_modulation_freq; // 0x23C	; 2 bytes
v_sfx_fm3_freq_adjust; // 0x23E
v_sfx_fm3_feedbackalgo; // 0x23F
v_sfx_fm3_voice_ptr; // 0x240
v_sfx_fm3_loop_index; // 0x244	; Several bytes, may overlap with gosub/return stack

v_sfx_fm4_track; // 0x250
v_sfx_fm4_playback_control; // 0x250	; Playback control bits for sfx FM4
v_sfx_fm4_voice_control; // 0x251	; Voice control bits
v_sfx_fm4_tempo_time; // 0x252	; sfx - tempo dividing timing
v_sfx_fm4_ptr; // 0x254	; FM channel 4 pointer (4 bytes)
v_sfx_fm4_key; // 0x258	; FM channel 4 key displacement
v_sfx_fm4_volume; // 0x259	; FM channel 4 volume attenuation
v_sfx_fm4_amsfmspan; // 0x25A
v_sfx_fm4_voice; // 0x25B
v_sfx_fm4_stack_ptr; // 0x25D
v_sfx_fm4_note_timeout; // 0x25E	; Counts down to zero; when zero, a new note is needed
v_sfx_fm4_note_duration; // 0x25F
v_sfx_fm4_curr_note; // 0x260
v_sfx_fm4_note_fill; // 0x262
v_sfx_fm4_note_fill_master; // 0x263
v_sfx_fm4_modulation_ptr; // 0x264	; 4 bytes
v_sfx_fm4_modulation_wait; // 0x268
v_sfx_fm4_modulation_speed; // 0x269
v_sfx_fm4_modulation_delta; // 0x26A
v_sfx_fm4_modulation_steps; // 0x26B
v_sfx_fm4_modulation_freq; // 0x26C	; 2 bytes
v_sfx_fm4_freq_adjust; // 0x26E
v_sfx_fm4_feedbackalgo; // 0x26F
v_sfx_fm4_voice_ptr; // 0x270
v_sfx_fm4_loop_index; // 0x274	; Several bytes, may overlap with gosub/return stack

v_sfx_fm5_track; // 0x280
v_sfx_fm5_playback_control; // 0x280	; Playback control bits for sfx FM5
v_sfx_fm5_voice_control; // 0x281	; Voice control bits
v_sfx_fm5_tempo_time; // 0x282	; sfx - tempo dividing timing
v_sfx_fm5_ptr; // 0x284	; FM channel 5 pointer (4 bytes)
v_sfx_fm5_key; // 0x288	; FM channel 5 key displacement
v_sfx_fm5_volume; // 0x289	; FM channel 5 volume attenuation
v_sfx_fm5_amsfmspan; // 0x28A
v_sfx_fm5_voice; // 0x28B
v_sfx_fm5_stack_ptr; // 0x28D
v_sfx_fm5_note_timeout; // 0x28E	; Counts down to zero; when zero, a new note is needed
v_sfx_fm5_note_duration; // 0x28F
v_sfx_fm5_curr_note; // 0x290
v_sfx_fm5_note_fill; // 0x292
v_sfx_fm5_note_fill_master; // 0x293
v_sfx_fm5_modulation_ptr; // 0x294	; 4 bytes
v_sfx_fm5_modulation_wait; // 0x298
v_sfx_fm5_modulation_speed; // 0x299
v_sfx_fm5_modulation_delta; // 0x29A
v_sfx_fm5_modulation_steps; // 0x29B
v_sfx_fm5_modulation_freq; // 0x29C	; 2 bytes
v_sfx_fm5_freq_adjust; // 0x29E
v_sfx_fm5_feedbackalgo; // 0x29F
v_sfx_fm5_voice_ptr; // 0x2A0
v_sfx_fm5_loop_index; // 0x2A4	; Several bytes, may overlap with gosub/return stack

v_sfx_psg1_track; // 0x2B0
v_sfx_psg1_playback_control; // 0x2B0	; Playback control bits for sfx PSG1
v_sfx_psg1_voice_control; // 0x2B1	; Voice control bits
v_sfx_psg1_tempo_time; // 0x2B2	; sfx - tempo dividing timing
v_sfx_psg1_ptr; // 0x2B4	; PSG channel 1 pointer (4 bytes)
v_sfx_psg1_key; // 0x2B8	; PSG channel 1 key displacement
v_sfx_psg1_volume; // 0x2B9	; PSG channel 1 volume attenuation
v_sfx_psg1_amsfmspan; // 0x2BA
v_sfx_psg1_tone; // 0x2BB
v_sfx_psg1_flutter_index; // 0x2BC
v_sfx_psg1_stack_ptr; // 0x2BD
v_sfx_psg1_note_timeout; // 0x2BE	; Counts down to zero; when zero, a new note is needed
v_sfx_psg1_note_duration; // 0x2BF
v_sfx_psg1_curr_note; // 0x2C0
v_sfx_psg1_note_fill; // 0x2C2
v_sfx_psg1_note_fill_master; // 0x2C3
v_sfx_psg1_modulation_ptr; // 0x2C4	; 4 bytes
v_sfx_psg1_modulation_wait; // 0x2C8
v_sfx_psg1_modulation_speed; // 0x2C9
v_sfx_psg1_modulation_delta; // 0x2CA
v_sfx_psg1_modulation_steps; // 0x2CB
v_sfx_psg1_modulation_freq; // 0x2CC	; 2 bytes
v_sfx_psg1_freq_adjust; // 0x2CE
v_sfx_psg1_noise; // 0x2CF
v_sfx_psg1_loop_index; // 0x2D4	; Several bytes, may overlap with gosub/return stack

v_sfx_psg2_track; // 0x2E0
v_sfx_psg2_playback_control; // 0x2E0	; Playback control bits for sfx PSG2
v_sfx_psg2_voice_control; // 0x2E1	; Voice control bits
v_sfx_psg2_tempo_time; // 0x2E2	; sfx - tempo dividing timing
v_sfx_psg2_ptr; // 0x2E4	; PSG channel 2 pointer (4 bytes)
v_sfx_psg2_key; // 0x2E8	; PSG channel 2 key displacement
v_sfx_psg2_volume; // 0x2E9	; PSG channel 2 volume attenuation
v_sfx_psg2_amsfmspan; // 0x2EA
v_sfx_psg2_tone; // 0x2EB
v_sfx_psg2_flutter_index; // 0x2EC
v_sfx_psg2_stack_ptr; // 0x2ED
v_sfx_psg2_note_timeout; // 0x2EE	; Counts down to zero; when zero, a new note is needed
v_sfx_psg2_note_duration; // 0x2EF
v_sfx_psg2_curr_note; // 0x2F0
v_sfx_psg2_note_fill; // 0x2F2
v_sfx_psg2_note_fill_master; // 0x2F3
v_sfx_psg2_modulation_ptr; // 0x2F4	; 4 bytes
v_sfx_psg2_modulation_wait; // 0x2F8
v_sfx_psg2_modulation_speed; // 0x2F9
v_sfx_psg2_modulation_delta; // 0x2FA
v_sfx_psg2_modulation_steps; // 0x2FB
v_sfx_psg2_modulation_freq; // 0x2FC	; 2 bytes
v_sfx_psg2_freq_adjust; // 0x2FE
v_sfx_psg2_noise; // 0x2FF
v_sfx_psg2_loop_index; // 0x304	; Several bytes, may overlap with gosub/return stack

v_sfx_psg3_track; // 0x310
v_sfx_psg3_playback_control; // 0x310	; Playback control bits for sfx PSG3
v_sfx_psg3_voice_control; // 0x311	; Voice control bits
v_sfx_psg3_tempo_time; // 0x312	; sfx - tempo dividing timing
v_sfx_psg3_ptr; // 0x314	; PSG channel 3 pointer (4 bytes)
v_sfx_psg3_key; // 0x318	; PSG channel 3 key displacement
v_sfx_psg3_volume; // 0x319	; PSG channel 3 volume attenuation
v_sfx_psg3_amsfmspan; // 0x31A
v_sfx_psg3_tone; // 0x31B
v_sfx_psg3_flutter_index; // 0x31C
v_sfx_psg3_stack_ptr; // 0x31D
v_sfx_psg3_note_timeout; // 0x31E	; Counts down to zero; when zero, a new note is needed
v_sfx_psg3_note_duration; // 0x31F
v_sfx_psg3_curr_note; // 0x320
v_sfx_psg3_note_fill; // 0x322
v_sfx_psg3_note_fill_master; // 0x323
v_sfx_psg3_modulation_ptr; // 0x324	; 4 bytes
v_sfx_psg3_modulation_wait; // 0x328
v_sfx_psg3_modulation_speed; // 0x329
v_sfx_psg3_modulation_delta; // 0x32A
v_sfx_psg3_modulation_steps; // 0x32B
v_sfx_psg3_modulation_freq; // 0x32C	; 2 bytes
v_sfx_psg3_freq_adjust; // 0x32E
v_sfx_psg3_noise; // 0x32F
v_sfx_psg3_loop_index; // 0x334	; Several bytes, may overlap with gosub/return stack

v_sfx2_track_ram; // 0x340	; Start of special sfx RAM

v_sfx2_fm4_track; // 0x340
v_sfx2_fm4_playback_control; // 0x340	; Playback control bits for sfx FM4
v_sfx2_fm4_voice_control; // 0x341	; Voice control bits
v_sfx2_fm4_tempo_time; // 0x342	; sfx - tempo dividing timing
v_sfx2_fm4_ptr; // 0x344	; FM channel 4 pointer (4 bytes)
v_sfx2_fm4_key; // 0x348	; FM channel 4 key displacement
v_sfx2_fm4_volume; // 0x349	; FM channel 4 volume attenuation
v_sfx2_fm4_amsfmspan; // 0x34A
v_sfx2_fm4_voice; // 0x34B
v_sfx2_fm4_stack_ptr; // 0x34D
v_sfx2_fm4_note_timeout; // 0x34E	; Counts down to zero; when zero, a new note is needed
v_sfx2_fm4_note_duration; // 0x34F
v_sfx2_fm4_curr_note; // 0x350
v_sfx2_fm4_note_fill; // 0x352
v_sfx2_fm4_note_fill_master; // 0x353
v_sfx2_fm4_modulation_ptr; // 0x354	; 4 bytes
v_sfx2_fm4_modulation_wait; // 0x358
v_sfx2_fm4_modulation_speed; // 0x359
v_sfx2_fm4_modulation_delta; // 0x35A
v_sfx2_fm4_modulation_steps; // 0x35B
v_sfx2_fm4_modulation_freq; // 0x35C	; 2 bytes
v_sfx2_fm4_freq_adjust; // 0x35E
v_sfx2_fm4_feedbackalgo; // 0x35F
v_sfx2_fm4_voice_ptr; // 0x360
v_sfx2_fm4_loop_index; // 0x364	; Several bytes, may overlap with gosub/return stack

v_sfx2_psg3_track; // 0x370
v_sfx2_psg3_playback_control; // 0x370	; Playback control bits for sfx PSG3
v_sfx2_psg3_voice_control; // 0x371	; Voice control bits
v_sfx2_psg3_tempo_time; // 0x372	; sfx - tempo dividing timing
v_sfx2_psg3_ptr; // 0x374	; PSG channel 3 pointer (4 bytes)
v_sfx2_psg3_key; // 0x378	; PSG channel 3 key displacement
v_sfx2_psg3_volume; // 0x379	; PSG channel 3 volume attenuation
v_sfx2_psg3_amsfmspan; // 0x37A
v_sfx2_psg3_tone; // 0x37B
v_sfx2_psg3_flutter_index; // 0x37C
v_sfx2_psg3_stack_ptr; // 0x37D
v_sfx2_psg3_note_timeout; // 0x37E	; Counts down to zero; when zero, a new note is needed
v_sfx2_psg3_note_duration; // 0x37F
v_sfx2_psg3_curr_note; // 0x380
v_sfx2_psg3_note_fill; // 0x382
v_sfx2_psg3_note_fill_master; // 0x383
v_sfx2_psg3_modulation_ptr; // 0x384	; 4 bytes
v_sfx2_psg3_modulation_wait; // 0x388
v_sfx2_psg3_modulation_speed; // 0x389
v_sfx2_psg3_modulation_delta; // 0x38A
v_sfx2_psg3_modulation_steps; // 0x38B
v_sfx2_psg3_modulation_freq; // 0x38C	; 2 bytes
v_sfx2_psg3_freq_adjust; // 0x38E
v_sfx2_psg3_noise; // 0x38F
v_sfx2_psg3_loop_index; // 0x394	; Several bytes, may overlap with gosub/return stack

v_1up_ram_copy; // 0x3A0

f_fastmusic; // 0x3CA	; flag set to speed up the music (00 = normal; 80 = fast)
*/