#pragma once
#include "singleton.h"
#include <bitset>

constexpr size_t KEYS = 256;

struct KeyState
{
	bitset<KEYS> up;
	bitset<KEYS> down;
	bitset<KEYS> press;
};

class keyManager : public singleton<keyManager>
{
	private:
	KeyState ks;

	public:
	keyManager();
	~keyManager();

	HRESULT init();
	void release();

	void updateKeyState(vector<int>& keyVector);
	bool up(int key) const { return ks.up[key]; }
	bool down(int key) const { return ks.down[key]; }
	bool press(int key) const { return ks.press[key]; }
	bool isToggledOn(int key) const;
};

