// input:
//	a0 = object to find floor of
//	a3 = direction/distance to look (abusing address regs much?)
//	d2 = y-position of object's bottom edge
//	d3 = x-position of object
//	d5 = bit to test for solidness
//  d6 = 0 if floor, 0x1000 if wall/ceiling?
// output:
//	d1 = distance to the floor
//	a1 = address within 256x256 mappings where object is standing
//	     (refers to a 16x16 tile number)
//	(a4) = floor angle

// a1                      a0           d2          d3          d5              d6             a3               d1                 a4
ushort* FindFloor(Object* self, int objBottom, int objX, int solidityBit, int wallFlag, int direction, int* outDistance, ubyte* outFloorAngle)
{
	auto tileAddr = FindNearestTile(self, objBottom, objX);
	auto tile = *tileAddr;
	auto tileFlags = tile;
	tile &= 0x7FF;

	if(tile == 0 || !BTST(tileFlags, 1 << solidityBit))
	{
	_isBlank:
		int dist;
		tileAddr = FindFloor2(self, objBottom + direction, objX, solidityBit, wallFlag, &dist, outFloorAngle);

		if(outDistance)
			*outDistance = dist + 16;

		return tileAddr;
	}

	auto collIndex = v_collindex[tile] & 0xFF;

	if(collIndex == 0)
		goto _isBlank;

	*outFloorAngle = AngleMap[collIndex];

	auto xpos = objX;

	if(BTST(tileFlags, 0x800))
	{
		xpos = ~xpos;
		*outFloorAngle = -*outFloorAngle;
	}

	if(BTST(tileFlags, 0x1000))
		*outFloorAngle = -(*outFloorAngle + 0x40) - 0x40;

	short height = CollArray1[(xpos & 0xF) + (collIndex << 4)];

	tileFlags ^= wallFlag;

	if(BTST(tileFlags, 0x1000))
		height = -height;

	if(height == 0)
		goto _isBlank;
	else if(height < 0)
	{
		height += objBottom & 0xF;

		if(height >= 0)
			goto _isBlank;
	}
	else if(height != 16)
	{
		if(outDistance)
			*outDistance = 15 - (height + (objBottom & 0xF));

		return tileAddr;
	}

	int dist;
	tileAddr = FindFloor2(self, objBottom - direction, objX, solidityBit, wallFlag, &dist, outFloorAngle);

	if(outDistance)
		*outDistance = dist - 16;

	return tileAddr;
}

// a1                       a0         d2             d3         d5                d6              d1                 a4
ushort* FindFloor2(Object* self, int objBottom, int objX, int solidityBit, int wallFlag, int* outDistance, ubyte* outFloorAngle)
{
	auto tileAddr = FindNearestTile(self, objBottom, objX);
	auto tile = *tileAddr;
	auto tileFlags = tile;

	if(tile == 0 || !(tileFlags & (1 << solidityBit)))
	{
	_isBlank:
		if(outDistance)
			*outDistance = 15 - (objBottom & 0xF);

		return tileAddr;
	}

	auto collIndex = v_collindex[tile] & 0xFF;

	if(collIndex == 0)
		goto _isBlank;

	*outFloorAngle = AngleMap[collIndex];

	auto xpos = objX;

	if(BTST(tileFlags, 0x800))
	{
		xpos = ~xpos
		*outFloorAngle = -*outFloorAngle;
	}

	if(BTST(tileFlags, 0x1000))
		*outFloorAngle = -(*outFloorAngle + 0x40) - 0x40;

	short height = CollArray1[(xpos & 0xF) + (collIndex << 4)];

	tileFlags ^= wallFlag;

	if(BTST(tileFlags, 0x1000))
		height = -height;

	if(height == 0)
		goto _isBlank;

	if(outDistance)
	{
		if(height >= 0)
			*outDistance = 0xF - (height + (objBottom & 0xF))
		else
		{
			*outDistance = objBottom & 0xF;
			height += *outDistance;

			if(height >= 0)
				goto _isBlank;

			*outDistance = ~*outDistance;
		}
	}

	return tileAddr;
}

//a1                      a0         d2             d3         d5                d6            a3             d1                 a4
ushort* FindWall(Object* self, int objBottom, int objX, int solidityBit, int wallFlag, int direction, int* outDistance, ubyte* outFloorAngle)
{
	auto tileAddr = FindNearestTile(self, objBottom, objX);
	auto tile = *tileAddr;
	auto tileFlags = tile;
	tile &= 0x7FF;

	if(tile == 0 || !BTST(tileFlags, 1 << solidityBit))
	{
	_isBlank:
		int dist;
		tileAddr = FindWall2(self, objBottom, objX + direction, solidityBit, wallFlag, &dist, outFloorAngle);

		if(outDistance)
			*outDistance = dist + 16;

		return tileAddr;
	}

	auto collIndex = v_collindex[tile] & 0xFF;

	if(collIndex == 0)
		goto _isBlank;

	*outFloorAngle = AngleMap[collIndex];

	auto ypos = objBottom;

	if(BTST(tileFlags, 0x1000))
	{
		ypos = ~ypos
		*outFloorAngle = -(*outFloorAngle + 0x40) - 0x40;
	}

	if(BTST(tileFlags, 0x800))
		*outFloorAngle = -*outFloorAngle;

	short height = CollArray2[(ypos & 0xF) + (collIndex << 4)]

	tileFlags ^= wallFlag;

	if(BTST(tileFlags, 0x800))
		height = -height

	if(height == 0)
		goto _isBlank;
	else if(height < 0)
	{
		height += objX & 0xF;

		if(height >= 0)
			goto _isBlank;
	}
	else if(height != 16)
	{
		if(outDistance)
			*outDistance = 15 - (height + (objX & 0xF));

		return tileAddr;
	}

	int dist;
	tileAddr = FindWall2(self, objBottom, objX - direction, solidityBit, wallFlag, &dist, outFloorAngle);

	if(outDistance)
		*outDistance = dist - 16;

	return tileAddr;

}

// a1                     a0         d2             d3         d5                d6              d1                 a4
ushort* FindWall2(Object* self, int objBottom, int objX, int solidityBit, int wallFlag, int* outDistance, ubyte* outFloorAngle)
{
	auto tileAddr = FindNearestTile(self, objBottom, objX);
	auto tile = *tileAddr;
	auto tileFlags = tile;

	if(tile == 0 || !(tileFlags & (1 << solidityBit)))
	{
	_isBlank:
		if(outDistance)
			*outDistance = 0xF - (objX & 0xF);

		return tileAddr;
	}

	auto collIndex = v_collindex[tile] & 0xFF;

	if(collIndex == 0)
		goto _isBlank;

	*outFloorAngle = AngleMap[collIndex];

	auto ypos = objBottom;

	if(BTST(tileFlags, 0x1000))
	{
		ypos = ~ypos
		*outFloorAngle = -(*outFloorAngle + 0x40) - 0x40;
	}

	if(BTST(tileFlags, 0x800))
		*outFloorAngle = -*outFloorAngle;

	short height = CollArray2[ypos & 0xF + collIndex << 4];

	tileFlags ^= wallFlag;

	if(BTST(tileFlags, 0x800))
		height = -height;

	if(height == 0)
		goto _isBlank;

	if(outDistance)
	{
		if(height >= 0)
			*outDistance = 0xF - (height + (objX & 0xF))
		else
		{
			*outDistance = objX & 0xF;
			height += *outDistance;

			if(height >= 0)
				goto _isBlank;

			*outDistance = ~*outDistance;
		}
	}

	return tileAddr;
}

// Subroutine to find which tile the object is standing on
// input:
//	d2 = y-position of object's bottom edge
//	d3 = x-position of object
// output:
//	a1 = address within 256x256 mappings where object is standing
//	     (refers to a 16x16 tile number)

ushort* FindNearestTile(Object* self, int objBottom, int objX)
{
	auto chunk = v_lvllayout[(objBottom >> 8) & 7][(objX >> 8) & 0x7F];

	if(chunk == 0)
		return &v_dummytile;

	if(chunk & 0x80) // special chunk?
	{
		chunk &= 0x7F;

		if(Obj_IsBehind(self) && chunk == 0x28)
			chunk = (chunk == 0x28) : 0x51 : chunk + 1; // some weird hardcoded thing
	}

	return &v_256x256[(chunk - 1) & 0x7F][objBottom & 0xF][objX & 0xF];
}