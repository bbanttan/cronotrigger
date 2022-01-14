#pragma once
#include "gameNode.h"

class playground : public gameNode
{
private:
	vector<int> keysToCheck;
	int _fadeCount;

public:
	playground();
	~playground() override;

	HRESULT init() override;
	void release() override;
	void update() override;
	void render() override;
};

