// =====================================================================================================================
// Nemesis decompression algorithm
// Not all of the original source has been translated, as there are unused bits and pieces of the algorithm for
// decompressing to RAM instead of VRAM. This stuff is included in every game that uses Nemesis decompression but
// this game doesn't use it, so I didn't bother decompiling it!
// I've also used the naming conventions from the wiki article (http://segaretro.org/Nemesis_compression) instead of the
// much more confusing names in the Sonic 1 disassembly.
// =====================================================================================================================

struct NemDec_State
{
	uint repeatCount;  // d0
	uint palIdx;       // d1
	uint prevRow;      // d2
	uint bitBuffer;    // d5
	uint bitsInBuffer; // d6
	bool isXor;        // a3
};

//                  a0       (implicit)
void NemDec(ubyte* src, ubyte* vram)
{
	auto numPatterns = src[0] << 8 | src[1]; src += 2;
	NemDec_State s;
	s.isXor = numPatterns & 0x8000;
	numPatterns &= 0x7FFF;
	Nem_Build_Code_Table(src);
	s.prevRow = 0;
	s.bitBuffer = src[0] << 8 | src[1]; src += 2; // get first two bytes of compressed data
	s.bitsInBuffer = 16;
	Nem_Process_Compressed_Data(s, src, 8, 0, numPatterns << 3, false);
}

#define CONSUME_BITS(n)\
	s.bitsInBuffer -= (n);\
	if(s.bitsInBuffer < 9)\
	{\
		s.bitsInBuffer += 8;\
		s.bitBuffer = (s.bitBuffer << 8) | *src++;\
	}

//                                   <multiple>            a0          d3              d4            a5
void Nem_Process_Compressed_Data(NemDec_State& s, ubyte*& src, int pixelsLeft, int pixelData, int rowsLeft, bool resume) // alias NemDec2
{
	if(resume)
		goto _resume;

	while(true)
	{
		s.repeatCount = 0;
		s.palIdx = 0;

		auto code = s.bitBuffer >> (s.bitsInBuffer - 8) // shift so high bit of code is in bit 7

		if(code >= 0xFC) // are high 6 bits set?
		{
			// Inline data
			CONSUME_BITS(6);
			auto data = s.bitBuffer >> (s.bitsInBuffer - 7); // shift so that low bit of the data is in bit position 0
			CONSUME_BITS(7);
			s.palIdx = data & 0x0F;
			s.repeatCount = (data & 0x70) >> 4;
		}
		else
		{
			auto dictEntry = v_ngfx_buffer[code & 0xFF];
			CONSUME_BITS(dictEntry >> 8); // bit length
			s.palIdx = dictEntry & 0x0F;
			s.repeatCount = (dictEntry & 0xF0) >> 4;
		}

		for(auto i = s.repeatCount; i >= 0; i--)
		{
			pixelData = (pixelData << 4) | s.palIdx; // calculate the pixel value
			pixelsLeft--;

			if(pixelsLeft != 0)
				continue;
			else
			{
				if(s.isXor)
				{
					s.prevRow ^= pixelData;
					*vdp_data_port = s.prevRow;
				}
				else
					*vdp_data_port = pixelData;

				rowsLeft--;

				if(rowsLeft == 0)
					return;
			}

		_resume: // alias NemDec3
			pixelData = 0;
			pixelsLeft = 8;
		}
	}
}

void Nem_Build_Code_Table(ubyte*& src) // alias NemDec4
{
	auto b = *src++;

_restart:
	if(b == -1) // has end-of-table been reached?
		return;

	auto paletteIndex = b & 0x0F;

	while(true)
	{
		b = *src++

		if(b & 0x80) // new palette index?
			goto _restart;

		auto repeatCount = b & 0x70;
		auto codeBitLength = b & 0xF;
		auto codeEntry = paletteIndex | repeatCount | codeBitLength << 8;
		auto shift = 8 - codeBitLength;

		if(shift == 0) // is the code exactly 8 bits long?
			v_ngfx_buffer[*src++] = codeEntry;//w store code entry in table
		else
		{
			auto idx = *src++ << shift; // get code and convert to table index

			for(int i = (1 << shift) - 1; i >= 0; i--)
				v_ngfx_buffer[idx++] = codeEntry;//w store entry
		}
	}
}