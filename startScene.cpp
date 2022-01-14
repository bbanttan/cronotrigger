#include "stdafx.h"
#include "startScene.h"
#include "videoPlayer.h"

startScene::startScene(): vP(new videoPlayer)
{
}

startScene::~startScene()
{
	SAFE_DEL(vP); // loadingScene에서 종료할 때를 생각하여 추가한 줄
}

HRESULT startScene::init()
{
	vP->init();
	if (!KEY->press(VK_SPACE))
	{
		vP->playOpening();
	}
	return S_OK;
}

void startScene::release()
{
	vP->release();
	SAFE_DEL(vP);
}

void startScene::update()
{
	vP->update();
}

void startScene::render()
{
	if (KEY->press(VK_SPACE)) // 리마인더: 최종 결과물에서는 C, V 키 입력 시와 동작이 같아야 한다.
	{
		_isScrBlackingOut = TRUE;
		SC->delScene("전투 모드 선택 화면");
		SC->delScene("이름 변경 화면");
		SC->changeScene("게임 장면");
	}
	else if (KEY->down('C') || KEY->down('V'))
	{
		_isScrBlackingOut = TRUE;
		SC->changeScene("전투 모드 선택 화면");
	}
}