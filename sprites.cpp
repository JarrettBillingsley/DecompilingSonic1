const ushort* BldSpr_ScrPos[] =
{
	nullptr, nullptr,
	&v_screenposx, &v_screenposy,
	&v_bg1posx,    &v_bg1posy,
	&v_bg2posx,    &v_bg2posy
};

ushort* BuildSprites()
{
	auto spriteBuffer = v_spritetablebuffer;
	auto spriteCount = 0;
	auto spriteQueue = v_spritequeue;

	for(int i = 0; i < 8; i++, spriteQueue += 64)
	{
		for(int queueIdx = 1; *spriteQueue != 0; queueIdx++, *spriteQueue -=2)
		{
			Object* self = spriteQueue[queueIdx];

			if(self == nullptr)
				continue;

			BCLR(self->render, ObjRender::Visible);
			int spriteX, spriteY;

			if(self->render & ObjRender::Layer)
			{
				auto offsX = *BldSpr_ScrPos[(self->render & ObjRender::Layer) >> 1];
				auto offsY = *BldSpr_ScrPos[((self->render & ObjRender::Layer) >> 1) + 1];

				spriteX = self->x - offsX;

				if(spriteX + self->actWid < 0 || spriteX - self->actWid >= 320)
					continue;

				spriteX += 128;

				if(BTST(self->render, ObjRender::Something) != 0)
				{
					spriteY = self->y - offsY;

					if(spriteY + self->height < 0 || spriteY - self->height >= 224)
						continue;

					spriteY += 128;
				}
				else
				{
					spriteY = self->y - offsY + 128;

					if(spriteY < 96 || spriteY >= 384)
						continue;
				}
			}
			else
			{
				spriteY = self->screenY;
				spriteX = self->x;
			}

			auto mapping = self->map;
			int numPieces = 0;

			if(BTST(self->render, ObjRender::SmashFragment))
				EnqueueSprite(self->gfx, mapping, numPieces, spriteX, spriteY, self->render, spriteCount, spriteBuffer);
			else
			{
				mapping += mapping[self->frame]
				numPieces = *mapping++ - 1;

				if(numPieces >= 0)
					EnqueueSprite(self->gfx, mapping, numPieces, spriteX, spriteY, self->render, spriteCount, spriteBuffer);
			}

			BSET(self->render, ObjRender::Visible);
		}
	}

	v_spritecount = spriteCount;

	// Terminate sprite link list
	if(spriteCount != MaxSprites)
		spriteBuffer[0] = 0;
	else
		spriteBuffer[-5] = 0;

	return spriteBuffer;
}

// sub_D750
//                          a0            a1             d1             d3           d2            d4               d5                  a2
void EnqueueSprite(ushort gfx, byte* mapping, int numPieces, int spriteX, int spriteY, int renderFlags, int& spriteCount, ushort*& spriteBuffer)
{
	if(BTST(renderFlags, ObjRender::HorizFlip))
	{
		if(BTST(renderFlags, ObjRender::VertFlip))
		{
			// Flipped vertically and horizontally
			for(int i = 0; i <= numPieces; i++, spriteBuffer += 4, mapping += 5)
			{
				if(spriteCount == MaxSprites)
					break;

				spriteCount++;

				spriteBuffer[0] = (-mapping[0] - ((mapping[1] << 3) & 0x18 + 8)) + spriteY;
				spriteBuffer[1] = (mapping[1] << 8) | spriteCount;
				spriteBuffer[2] = ((mapping[2] << 8 | mapping[3]) + gfx) ^ 0x1800;
				spriteBuffer[3] = (-mapping[4] - (((mapping[1] << 1) & 0x18) + 8) + spriteX) & 0x1FF;

				if(spriteBuffer[3] == 0)
					spriteBuffer[3] = 1;
			}
		}
		else
		{
			// Flipped horizontally
			for(int i = 0; i <= numPieces; i++, spriteBuffer += 4, mapping += 5)
			{
				if(spriteCount == MaxSprites)
					break;

				spriteCount++;

				spriteBuffer[0] = mapping[0] + spriteY;
				spriteBuffer[1] = (mapping[1] << 8) | spriteCount;
				spriteBuffer[2] = ((mapping[2] << 8 | mapping[3]) + gfx) ^ 0x0800;
				spriteBuffer[3] = (-mapping[4] - (((mapping[1] << 1) & 0x18) + 8) + spriteX) & 0x1FF;

				if(spriteBuffer[3] == 0)
					spriteBuffer[3] = 1;
			}
		}
	}
	else if(BTST(renderFlags, ObjRender::VertFlip))
	{
		// Flipped vertically
		for(int i = 0; i <= numPieces; i++, spriteBuffer += 4, mapping += 5)
		{
			if(spriteCount == MaxSprites)
				break;

			spriteCount++;

			spriteBuffer[0] = (-mapping[0] - ((mapping[1] << 3) & 0x18 + 8)) + spriteY;
			spriteBuffer[1] = (mapping[1] << 8) | spriteCount;
			spriteBuffer[2] = ((mapping[2] << 8 | mapping[3]) + gfx) ^ 0x1000;
			spriteBuffer[3] = (mapping[4] + spriteX) & 0x1FF;

			if(spriteBuffer[3] == 0)
				spriteBuffer[3] = 1;
		}
	}
	else
		EnqueueSpriteUnflipped(gfx, mapping, numPieces, spriteX, spriteY, spriteCount, spriteBuffer);
}

// sub_D762
//                                   a0           a1             d1           d3           d2               d5                  a2
void EnqueueSpriteUnflipped(ushort gfx, byte* mapping, int numPieces, int spriteX, int spriteY, int& spriteCount, ushort*& spriteBuffer)
{
	for(int i = 0; i <= numPieces; i++, spriteBuffer += 4, mapping += 5)
	{
		if(spriteCount == MaxSprites)
			break;

		spriteCount++;

		spriteBuffer[0] = mapping[0] + spriteY;
		spriteBuffer[1] = (mapping[1] << 8) | spriteCount;
		spriteBuffer[2] = (mapping[2] << 8 | mapping[3]) + gfx;
		spriteBuffer[3] = (mapping[4] + spriteX) & 0x1FF;

		if(spriteBuffer[3] == 0)
			spriteBuffer[3] = 1;
	}
}