# Decompiling Sonic 1

I'm having fun by taking the [Sonic 1 disassembly](https://github.com/sonicretro/s1disasm) and decompiling it into C++ to learn how it works! I don't know if it'll ever compile/run, but maybe it'll be useful/fun/interesting to other people.

## Does this compile?

No. It's not even set up to compile. I'm just kinda throwing everything in .cpp files to keep it organized, but there are no headers or makefiles or anything. A lot of the lower-level code (and some of the higher level code) is really dependent on how the Genesis/Mega Drive works, so it would need a lot of work to make a "C++ Sonic 1" a reality. (Not that I really have any interest running aground Sega's legal might.)

## Language, Style etc.

Language is C++ 11, cause I like it. Don't get too scared; the code is really written in a simple structs-and-functions style (it was 68K assembly after all), but I do like having namespacing, ad-hoc overloading, and auto variable declarations, so C++ 11 it is.

Indentation is tabs at 4 spaces, braces on own lines, no spaces before parens on control structures/function calls, CamelCase for names, CamelCase_Underscored for "fake" namespacing. Soft limit of 120 chars/line, hard limit of 140.

Every attempt is made to preserve the exact operating semantics of the original code, so it doesn't use any STL data structures or anything. Just plain old structs, pointers, and C arrays. This also means that the code is kinda messy/global-variable-dependent.

## Notes

Please see [notes.md](/notes.md) for more in-depth info.
