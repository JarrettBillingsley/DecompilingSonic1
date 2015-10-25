// =====================================================================================================================
// Kosinski decompression algorithm
// This has been tested with actual data to ensure correctness.
// =====================================================================================================================

/*
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

typedef unsigned char ubyte;
*/

#define GET_BIT()\
	bit = descField & 1;\
	descField >>= 1;\
	bits--;\
	if(bits == 0)\
	{\
		descField = src[1] << 8 | src[0];\
		src += 2;\
		bits = 16;\
	}

//                  a0          a1
int KosDec(ubyte* src, ubyte* dest)
{
	auto begin = dest;
	auto descField = src[1] << 8 | src[0];
	src += 2;
	int bits = 16;
	ubyte bit;

	while(true)
	{
		GET_BIT();

		if(bit)
		{
			*dest++ = *src++;
			continue;
		}

		int count = 0;
		int offset;
		GET_BIT();

		if(!bit)
		{
			GET_BIT();
			count = (count << 1) | bit;
			GET_BIT();
			count = ((count << 1) | bit) + 1;
			offset = 0xFFFFFF00 | *src++;
		}
		else
		{
			auto desc0 = *src++;
			auto desc1 = *src++;
			offset = ((0xFFFFFF00 | desc1) << 5) | desc0;

			if(!(desc1 & 7))
			{
				desc1 = *src++;

				if(desc1 == 0)
					return dest - begin;
				else if(desc1 == 1)
					continue;
				else
					count = desc1;
			}
			else
				count = (desc1 & 7) + 1;
		}

		for(; count >= 0; count--)
		{
			auto b = dest[offset];
			*dest++ = b;
		}
	}

	return dest - begin;
}

/*
ubyte src[] =
{
	0xFF, 0x3F, 0x54, 0x3B, 0xC4, 0x44, 0x54, 0x33, 0x33, 0x5B, 0x2D, 0x5C, 0x44, 0x5C, 0xC4, 0xC5,
	0xFC, 0x15, 0xFE, 0xC3, 0x44, 0x78, 0x88, 0x98, 0x44, 0x30, 0xFF, 0xFF, 0x00, 0xF8, 0x00
};

int main()
{
	ubyte dest[512];
	memset(dest, 0, 512);

	auto numDecoded = KosDec(src, dest);
	printf("%d bytes\n", numDecoded);

	for(int i = 0; i < numDecoded; i++)
		printf("%02x ", dest[i]);

	printf("\n");

	return 0;
}*/