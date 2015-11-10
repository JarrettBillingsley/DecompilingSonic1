//d1                                             d0
int Sonic_CheckLevelCollision(Object* self, int angle)
{
	v_anglebuffer = angle;
	v_b_F76A = angle;
	angle = (angle + 0x20) & 0xC0;

	if(angle == 0x40)
		return Sonic_CheckLeft(self, nullptr, nullptr);
	else if(angle == 0x80)
		return Sonic_CheckUp(self, nullptr, nullptr);
	else if(angle == 0xC0)
		return Sonic_CheckRight(self, nullptr, nullptr);
	else
		return Sonic_CheckDown(self, nullptr, nullptr);
}

int Sonic_CheckLeft(Object* self, int* outAngle, int* otherDist)
{
	int dist1, dist2;
	FindFloor(self, self->y - self->width, (self->x - self->height) ^ 0xF, 0xE, 0x800, -16, &dist1, &v_anglebuffer)
	FindWall(self, self->y + self->width, (self->x - self->height) ^ 0xF, 0xE, 0x800, -16, &dist2, &v_b_F76A)
	return Sonic_CheckCommon(dist1, dist2, 0x40, outAngle, otherDist);
}

// aka "Sonic_DontRunOnWalls" which is the dumbest name
int Sonic_CheckUp(Object* self, int* outAngle, int* otherDist)
{
	int dist1, dist2;
	FindFloor(self, (self->y - self->height) ^ 0xF, self->x + self->width, 0xE, 0x1000, -16, &dist1, &v_anglebuffer)
	FindWall(self, (self->y - self->height) ^ 0xF, self->x - self->width, 0xE, 0x1000, -16, &dist2, &v_b_F76A)
	return Sonic_CheckCommon(dist1, dist2, -0x80, outAngle, otherDist);
}

int Sonic_CheckRight(Object* self, int* outAngle, int* otherDist)
{
	int dist1, dist2;
	FindWall(self, self->y - self->width, self->x + self->height, 0xE, 0, 16, &dist1, &v_anglebuffer)
	FindWall(self, self->y + self->width, self->x + self->height, 0xE, 0, 16, &dist2, &v_b_F76A)
	return Sonic_CheckCommon(dist1, dist2, -0x40, outAngle, otherDist);
}

// aka "Sonic_HitFloor"
int Sonic_CheckDown(Object* self, int* outAngle, int* otherDist)
{
	int dist1, dist2;
	FindFloor(self, self->y + self->height, self->x + self->width, 0xD, 0, 16, &dist1, &v_anglebuffer)
	FindFloor(self, self->y + self->height, self->x - self->width, 0xD, 0, 16, &dist2, &v_b_F76A)
	return Sonic_CheckCommon(dist1, dist2, 0, outAngle, otherDist);
}

//d1                        d0         d1            d2              d3              d0
int Sonic_CheckCommon(int dist1, int dist2, int defaultAngle, int* outAngle, int* otherDist)
{
	if(outAngle)
		*outAngle = v_b_F76A;

	if(dist2 > dist1)
	{
		if(outAngle)
			*outAngle = v_anglebuffer;

		SWAP(dist1, dist2);
	}

	if(outAngle && *outAngle & 1)
		*outAngle = defaultAngle;

	if(otherDist)
		*otherDist = dist1;

	return dist2;
}

//d1                           a0          d3
int Sonic_HitWallLeft(Object* self, int* outAngle)
{
	return Sonic_HitWallLeft2(self, self->x, self->y, outAngle);
}

// loc_1504A:
int Sonic_HitWallLeft2(Object* self, int x, int y, int* outAngle)
{
	int dist;
	FindWall(self, y, (x - 10) ^ 0xF, 0xE, 0x800, -16, &dist, &v_anglebuffer);

	if(outAngle)
		*outAngle = (v_anglebuffer & 1) ? 0x40 : v_anglebuffer;

	return dist;
}

//d1                            a0           d3
int Sonic_HitWallRight(Object* self, int* outAngle)
{
	return Sonic_HitWallRight2(self, self->x, self->y, outAngle);
}

// loc_14EBC:
int Sonic_HitWallRight2(Object* self, int x, int y, int* outAngle)
{
	int dist;
	FindWall(self, y, x + 10, 0xE, 0, 16, &dist, &v_anglebuffer);

	if(outAngle)
		*outAngle = (v_anglebuffer & 1) ? -0x40 : v_anglebuffer;

	return dist;
}

//d1                   a0      d3     d2          d3
int loc_14DF0(Object* self, int x, int y, int* outAngle)
{
	int dist;
	FindFloor(self, y + 10, x, 0xE, 0, 16, &dist, &v_anglebuffer);

	if(outAngle)
		*outAngle = (v_anglebuffer & 1) ? 0 : v_anglebuffer;

	return dist;
}

//d1                   a0      d3     d2          d3
int loc_14F7C(Object* self, int x, int y, int* outAngle)
{
	int dist;
	FindFloor(self, (y - 10) ^ 0xF, x, 0xE, 0x1000, -16, &dist, &v_anglebuffer);

	if(outAngle)
		*outAngle = (v_anglebuffer & 1) ? 0x80 : v_anglebuffer;

	return dist;
}

void Sonic_SlopeRepel(Object* self)
{
	if(VAR_B(self, 0x38) == 0 && TimerZero(VAR_W(self, 0x3E)) && (self->angle & 0x20) & 0xC0 != 0 && abs(self->inertia) < 0x280)
	{
		self->inertia = 0;
		Player_SetInAir();
		VAR_W(self, 0x3E) = OneSecond / 2;
	}
}