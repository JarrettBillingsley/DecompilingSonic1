// =====================================================================================================================
// Enigma decompression algorithm
// =====================================================================================================================

#define CONSUME_BITS(n)\
	do {\
	bitsInBuffer -= (n);\
	if(bitsInBuffer < 9)\
	{\
		bitsInBuffer += 8;\
		bitBuffer = (bitBuffer << 8) | *src++;\
	}\
	}while(false)

//                  a0            a1              d0
void EniDec(ubyte*& src, ushort* dest, ushort tileOffset)
{
	ubyte inlineCopyBits = *src++;
	ubyte flags = *src++ & 0x1F;
	ushort incCopyWord = ((src[0] << 8) | src[1]) + tileOffset;
	ushort literalCopyWord = ((src[2] << 8) | src[3]) + tileOffset;
	ushort bitBuffer = (src[4] << 8) | src[5];
	src += 6;
	ushort bitsInBuffer = 16;

	while(true)
	{
		auto code = (bitBuffer >> (bitsInBuffer - 7)) & 0x7F;
		auto repeatCount = code;

		if(code >= 0x40)
			CONSUME_BITS(7);
		else
		{
			CONSUME_BITS(6);
			repeatCount >>= 1;
		}

		repeatCount &= 0x0F;

		switch(code >> 4)
		{
			case 0:
			case 1:
				for(; repeatCount >= 0; repeatCount--)
					*dest++ = incCopyWord++;
				break;

			case 2:
			case 3:
				for(; repeatCount >= 0; repeatCount--)
					*dest++ = literalCopyWord;
				break;

			case 4:
				for(auto x = GetInlineValue(src, tileOffset, flags, inlineCopyBits, bitBuffer, bitsInBuffer);
					repeatCount >= 0; repeatCount--)
					*dest++ = x;
				break;

			case 5:
				for(auto x = GetInlineValue(src, tileOffset, flags, inlineCopyBits, bitBuffer, bitsInBuffer);
					repeatCount >= 0; repeatCount--)
					*dest++ = x++;
				break;

			case 6:
				for(auto x = GetInlineValue(src, tileOffset, flags, inlineCopyBits, bitBuffer, bitsInBuffer);
					repeatCount >= 0; repeatCount--)
					*dest++ = x--;
				break;

			case 7:
				if(repeatCount == 15)
				{
					src--;

					if(bitsInBuffer == 16)
						src--;

					if(src & 1)
						src++;

					return;
				}

				for(auto x = GetInlineValue(src, tileOffset, flags, inlineCopyBits, bitBuffer, bitsInBuffer);
					repeatCount >= 0; repeatCount--)
				{
					*dest++ = x;
					x = GetInlineValue(src, tileOffset, flags, inlineCopyBits, bitBuffer, bitsInBuffer);
				}
				break;
		}
	}
}


const ushort MaskTable[] =
{
	0x0001, 0x0003, 0x0007, 0x000F,
	0x001F, 0x003F, 0x007F, 0x00FF,
	0x01FF, 0x03FF, 0x07FF, 0x0FFF,
	0x1FFF, 0x3FFF, 0x7FFF, 0xFFFF,
};

enum
{
	Flags_Prio =   0x10,
	Flags_PalTop = 0x08,
	Flags_PalBot = 0x04,
	Flags_VFlip =  0x02,
	Flags_HFlip =  0x01,
};

#define DEC_AND_TEST_OR(val)\
	do {\
	bitsInBuffer--;\
	if(bitBuffer & (1 << bitsInBuffer))\
		tileOffset |= (val);\
	} while(false);

#define DEC_AND_TEST_ADD(val)\
	do {\
	bitsInBuffer--;\
	if(bitBuffer & (1 << bitsInBuffer))\
		tileOffset += (val);\
	} while(false);

ushort GetInlineValue(ubyte*& src, ushort tileOffset, ubyte flags, ubyte inlineCopyBits, ushort& bitBuffer, ushort& bitsInBuffer)
{
	if(flags & Flags_Prio)   DEC_AND_TEST_OR(0x8000);
	if(flags & Flags_PalTop) DEC_AND_TEST_ADD(0x4000);
	if(flags & Flags_PalBot) DEC_AND_TEST_ADD(0x2000);
	if(flags & Flags_VFlip)  DEC_AND_TEST_OR(0x1000);
	if(flags & Flags_VFlip)  DEC_AND_TEST_OR(0x0800);

	auto ret = bitBuffer;
	auto bitsRemaining = bitsInBuffer - inlineCopyBits;

	// Enough bits in the buffer?
	if(bitsRemaining > 0)
	{
		ret = ((ret >> bitsRemaining) & MaskTable[inlineCopyBits - 1]) + tileOffset;
		CONSUME_BITS(inlineCopyBits);
	}
	else
	{
		// If no bits left in the buffer, it's easy, just refil
		if(bitsRemaining == 0)
			bitsInBuffer = 16;
		else
		{
			// Some bits left in the buffer, so we have to get a new byte and put some of its bits in the return value
			// bitsRemaining is negative here
			bitsInBuffer = bitsRemaining + 16;
			bitsRemaining = -bitsRemaining;
			ret = (ret << bitsRemaining) + (((bitBuffer & 0xFF00) | ROL8(*src >> 8, bitsRemaining)) & MaskTable[bitsRemaining - 1]);
		}

		ret = (ret & MaskTable[inlineCopyBits - 1]) + tileOffset;
		bitBuffer = (src[0] << 8) | src[1]; src += 2;
	}

	return ret;
}