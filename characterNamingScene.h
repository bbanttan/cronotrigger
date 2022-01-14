#pragma once
#include "gameNode.h"

enum class namingSceneState
{
	INFO_MSG_UP,
	INFO_MSG_DOWN,
	ABLE_TO_CONFIRM_NAME
};

class characterNamingScene : public gameNode
{
	private:
	image* _chrFaceImg;
	int _chrIdx, _selectArrowPos, _nameArrowPos;
	namingSceneState _state;
	string _characters; // 이름에 사용할 수 있는 문자들
	string& _playChrName;

	public:
	characterNamingScene() = delete;
	characterNamingScene(int playChrIdx);
	~characterNamingScene() override;

	HRESULT init() override;
	void release() override;
	void update() override;
	void render() override;
};


