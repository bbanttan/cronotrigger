#include "stdafx.h"
#include "keyManager.h"

keyManager::keyManager()
{
}

keyManager::~keyManager()
{
}

HRESULT keyManager::init()
{
	for (int i = 0; i < KEYS; ++i)
	{
		this->ks.down.set(i, false);
		this->ks.up.set(i, false);
		this->ks.press.set(i, false);
	}
	return S_OK;
}

void keyManager::release()
{
}

void keyManager::updateKeyState(vector<int>& keyVector)
{
	for (const int& key : keyVector)
	{
		if (GetAsyncKeyState(key) & 0x8000)
		{
			if (ks.press[key] == false) ks.down.set(key, true);
			else ks.down.set(key, false);
			ks.press.set(key, true);
		}
		else
		{
			if (ks.press[key] == true) ks.up.set(key, true);
			else ks.up.set(key, false);
			ks.press.set(key, false);
			ks.down.set(key, false);
		}
	}
}

bool keyManager::isToggledOn(int key) const
{
	if (GetKeyState(key) & 0x0001) return true;
	return false;
}