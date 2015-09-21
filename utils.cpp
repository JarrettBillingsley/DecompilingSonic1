#define BTST(a, b) (((a) & (b)) != 0)
#define BSET(a, b) ((a) |= (b))
#define BCLR(a, b) ((a) &= ~(b))

// bne
template<typename T>
bool TimerZero(T& timer)
{
	timer--;
	return timer == 0;
}

// bpl
template<typename T>
bool TimerNeg(T& timer)
{
	timer--;
	return timer < 0;
}
