#pragma once
#include "singleton.h"
#include <time.h>

class randomNumber : public singleton<randomNumber>
{
	public:
	randomNumber() { srand(static_cast<unsigned int>(GetTickCount64())); }
	~randomNumber() {}
	inline int getInt(int num) { return rand() % num; }

	inline int getIntFromTo(int fromNum, int toNum) { return rand() % (toNum - fromNum) + fromNum; }

	inline float getFloat(float num) { return (float)rand() / (float)num; }

	inline float getFloatFromTo(float fromNum, float toNum)
	{
		float rnd = (float)rand() / (float)RAND_MAX;
		return (rnd * (toNum - fromNum) + fromNum);
	}
};