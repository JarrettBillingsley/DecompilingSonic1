ASSEMBLY/PROGRAMMING TECHNIQUE NOTES
========================================================================================================================

Register Usage
------------------------------------------------------------

- d7, d6, d5 often used as for-loop indices
- d0 almost always used as scratch
- d1, d2, d3, d4 often used as local variables and/or function arguments
- a0 usually "this" for object-related routines
- a1 often points to Sonic's object (v_player) in object-related routines
- a2, a3, a4 often used as local variables and/or function arguments
- a6 (and maybe a5??) often holds the VDP control/data port

For Loops
------------------------------------------------------------

The "dbf" (decrement and branch if false) instruction is usually used to implement a for loop. Often the loop will look
like:

		moveq #7, d7
	@loop:
		...
		dbf d7, @loop

But there's a caveat: dbf decrements d7, and then branches to @loop if d7 >= 0, not > 0. So the above loop doesn't run 7
times, it runs 8 times. The C equivalent would be:

	for(int i = 7; i >= 0; i--)

Low-byte Trickery
------------------------------------------------------------

Don't forget to look at the operand size of moves! It's easy to see something like:

	move.w (v_something).w, d0
	; ...code that doesn't touch d0...
	move.b (v_somethingElse).w, d0

and think "weird, why did they smash d0 right after loading v_something into it?" But really what they're doing is
loading the *high byte* of v_something and the *low byte* of v_somethingElse into d0. In other words:

	d0 = (v_something & 0xFF00) | v_somethingElse

Timers
------------------------------------------------------------

By "timers" I mean variables which usually count down frames until something happens. For some reason -- and I presume
this is entirely based on the personal preference of the programmers who were writing the code -- sometimes the timers
count down to 0, and sometimes they count down to -1. That is, sometimes they do:

	timer = 10;
	...
	if(--timer == 0) // 10 frames have elapsed (bne in assembly)

and sometimes they do:

	timer = 9
	...
	if(--timer < 0) // 10 frames have elapsed (bpl in assembly)

There's no real consistency. Sometimes pieces of code in the same function/object script will use both conventions. The
functions TimerZero and TimerNeg have been provided to simplify/abstract these patterns, so you can do:

	timer = 10;
	...
	if(TimerZero(timer)) // 10 frames have elapsed

	// or

	timer = 9;
	...
	if(TimerNeg(timer)) // 10 frames have elapsed

I've even encountered what seem to be (probably harmless) bugs where they do stuff like set the timer to 60 but use the
"less than zero" convention, meaning it really runs for 61 frames.

GAME ENGINE NOTES
========================================================================================================================

Sprite Mappings
------------------------------------------------------------

A sprite mapping is a sort of mini-script that describes how to display a logical sprite. A logical sprite can be
composed of one or more "real" hardware sprites. Each visible object is given a mapping pointer, which is really a
pointer to an array of pointers to actual mappings, one for each "frame". When an object's frame changes, it will then
use a different mapping and get a different appearance. Boom, animation!

Each mapping entry starts with a byte saying how many sub-sprites there are. For each sub-sprite, there are five bytes
as follows:

- mapping[0] = vertical pixel offset
- mapping[1] = horiz/vertical sprite size (according to megadrive measurements)
- mapping[2] = horiz/vert flip flags (according to megadrive specs, though shifted right 8 bits to save space)
- mapping[3] = pattern to use (an offset from the object's gfx field, added to obj->gfx when sprite is displayed)
- mapping[4] = horizontal pixel offset

Animation Scripts
------------------------------------------------------------

Animation scripts are a very simple scripting language that make objects animate and can affect object behavior too.
Similar to mappings, each animation script pointer points to an array of pointers of actual animation scripts. Objects
don't have an animation script member; instead they just pass the script pointer to AnimateSprite. The objects' "anim"
field chooses which animation is used from the list of scripts, and the objects' "aniFrame" member is basically the
program counter into the script.

The first byte of the script is the frame delay, which is how many display frames each script frame lasts. Following
this byte are the frame and command bytes.

Command bytes in the script have the top bit set and their behavior is pretty self-explanatory; they can loop the whole
animation, part of the animation, switch to another animation, increment the object's routine or routine2 members, or
reset the animation state.

Frame bytes have the top bit clear. The next two bits (0x40 and 0x20) are sprite flipping flags, and the lower five bits
(& 0x1F) are the new frame number. Remember that the frame chooses a mapping, which is how the animation actually
changes objects' appearance.

Object Spaces
------------------------------------------------------------

The object space (v_objspace) is an array of 128 slots. Sonic is always in slot 0, and is aliased as v_player. The first
32 slots (31 ignoring Sonic) of v_objspace are reserved for special objects which are often "control" objects that you
don't collide with. The "level object space" (v_lvlobjspace) is really just slots 32 .. 127 of v_objspace, and is where
interactive level objects get spawned as they scroll onscreen/get created by scripts.

Object Script Miscellany
------------------------------------------------------------

Many object scripts take advantage of the fact that object variables are cleared to 0 when the object is created. Just
something to watch out for, since sometimes logic will be confusing until you remember that.

Sometimes timer object variables are used as a flag as well. What I mean by that is when the timer is 0 (as in, right
after the object is created), some event has not yet happened. When the event happens, the timer is set to a nonzero
value, and code will check this ("if(timer != 0) { do stuff since event happpened }").

Sometimes different object subtypes/routines will represent pretty different objects, even though they share a script.
For example, object 0A can, depending on the subtype, be either a "control object" that handles Sonic's breath/drowning
while underwater; an air bubble that escapes Sonic's mouth; or one of the flashing numbers you get during the drowning
countdown. They're all related, sure, but seem like different objects!

The disassembly calls routine 0 "main" which is a bit of a misnomer... maybe "main" in the C entry point sense, but
routine 0 is almost always an initialization routine that immediately changes the routine to something else. My
convention is to call routine 0 "init" and call the object's normal operating routine "main". (It seems that the Sonic 2
and S3K disassemblies use my convention as well.)

Routines are usually numbered such that their main routine is followed by the "delete self" routine, or something like
that. This way animation scripts can delete the object by using the right animation flag to increment the routine
number.

Pattern Load Cues
------------------------------------------------------------

Most of the graphics in Sonic 1 are compressed (mostly with Nemesis compression, and one piece with Kosinski).
Decompression takes time, and in order to offset some of the time it takes to decompress all the graphics, Sonic 1 uses
something called Pattern Load Cues (or PLCs).

A PLC is basically just a list of compressed art assets along with the location in VRAM where they should be copied
after being decompressed. The game enqueues PLCs in a buffer, and the game will dequeue a PLC each VBlank period,
decompress it, and copy it to VRAM.

Level Tile Layout
------------------------------------------------------------

In Sonic 1, the smallest piece of levels is 16x16 pixel tiles. Each 16x16 tile is really made of 4 8x8 pixel tiles, the
graphical unit that the Genesis/Mega Drive actually deals with. Each tile has some properties associated with it, namely
the collision data and the "angle", which is basically the "surface normal" of the tile and is used in doing player
physics.

This probably sounds familiar to you if you know how other 8/16 bit 2D game engines of the era work, but this is where
Sonic 1 gets a little weird. Instead of representing a level as a simple grid of 16x16 tiles, it instead represents it
as a grid of 256x256-pixel "blocks". Each block is composed of 256 16x16-pixel tiles, arranged in a 16x16-tile square.
So if you're keeping track: four 8x8-pixel art tiles make up one 16x16-pixel tile; 256 16x16-pixel tiles make up one
16x16-tile (256x256-pixel) block; and the level itself is a grid of these 256x256-pixel blocks. Both the foreground and
the background are composed the same way.

Loading a level is a matter of loading the "tile mappings" (the descriptions of the 16x16-pixel tiles), the "block
mappings" (the arrangements of 16x16 tiles into blocks), and then the "layout" (the actual list of blocks that make up
the level, which is quite small as a result -- usually only a hundred bytes or less).