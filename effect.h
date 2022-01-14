#pragma once
#include "gameNode.h"

class animation;

class effect: public gameNode
{
protected:
	int _x;
	int _y;

	image* _effectImage;
	animation* _effectAnimation;
	BOOL _isRunning;
	float _elapsedTime;

public:
	effect();
	~effect() override;

	virtual HRESULT init(image* effectImage, int frameW, int frameH, int fps, float elapsedTime);
	virtual void release() override;
	virtual void update() override;
	virtual void render() override;

	void start(int x, int y);

	virtual void kill();

	BOOL getIsRunning() { return _isRunning; }
};

