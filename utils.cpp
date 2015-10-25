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

template<typename T>
bool TimerZero(T& timer, T reload)
{
	timer--;

	if(timer == 0)
	{
		timer = reload;
		return true;
	}

	return false;
}

// bpl
template<typename T>
bool TimerNeg(T& timer)
{
	timer--;
	return timer < 0;
}

template<typename T>
bool TimerNeg(T& timer, T reload)
{
	timer--;

	if(timer < 0)
	{
		timer = reload;
		return true;
	}

	return false;
}

template<typename T>
bool DecToZero(T& val)
{
	if(val > 0)
		val--;

	return val == 0;
}

template<typename T>
bool DecTo(T& val, T limit)
{
	if(val > limit)
		val--;

	return val == limit;
}

template<typename T>
bool IncTo(T& val, T limit)
{
	if(val < limit)
		val++;

	return val == limit;
}

template<typename T>
void DecWrap(T& val, T max)
{
	if(val == 0)
		val = max;
	else
		val--;
}

template<typename T>
void IncWrap(T& val, T max)
{
	if(val == max)
		val = 0;
	else
		val++;
}