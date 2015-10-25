// Load into duplicate palette
void PalLoad1(ushort palIdx) // d0
{
	auto pal = &PalPointers[palIdx];
	memcpy(0xFFFF0000 | pal->ram + 0x80, pal->palette, pal->numColors * sizeof(uint));
}

// Load into main palette
void PalLoad2(ushort palIdx) // d0
{
	auto pal = &PalPointers[palIdx];
	memcpy(0xFFFF0000 | pal->ram, pal->palette, pal->numColors * sizeof(uint));
}

// Load into water palette
void PalLoad3_Water(ushort palIdx) // d0
{
	auto pal = &PalPointers[palIdx];
	memcpy(0xFFFF0000 | pal->ram - 0x80, pal->palette, pal->numColors * sizeof(uint));
}

// Load into water duplicate palette
void PalLoad4_Water(ushort palIdx) // d0
{
	auto pal = &PalPointers[palIdx];
	memcpy(0xFFFF0000 | pal->ram - 0x100, pal->palette, pal->numColors * sizeof(uint));
}