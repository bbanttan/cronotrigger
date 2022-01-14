#include "stdafx.h"
#include "characterNamingScene.h"

characterNamingScene::characterNamingScene(int playChrIdx):
	_selectArrowPos(0), _nameArrowPos(0), _state(namingSceneState::INFO_MSG_UP),
	_playChrName(_playChrNames[playChrIdx])
{
	switch (playChrIdx)
	{
		case 0:
			_chrFaceImg = IMG->find("Crono");
			_chrIdx = 0;
			break;
		case 1:
			_chrFaceImg = IMG->find("Lucca");
			_chrIdx = 1;
			break;
		case 2:
			_chrFaceImg = IMG->find("Marle");
			_chrIdx = 2;
			break;
		case 3:
			_chrFaceImg = IMG->find("Frog");
			_chrIdx = 3;
			break;
		case 4:
			_chrFaceImg = IMG->find("Robo");
			_chrIdx = 4;
			break;
		case 5:
			_chrFaceImg = IMG->find("Ayla");
			_chrIdx = 5;
			break;
		case 6:
			_chrFaceImg = IMG->find("Magus");
			_chrIdx = 6;
			break;
		default:
			exit(-888);
	}
	_characters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ!?/#abcdefghijklmnopqrstuvwxyz=-+%0123456789`\":&()'., ";
}

characterNamingScene::~characterNamingScene()
{
}

HRESULT characterNamingScene::init()
{
	SC->delScene("전투 모드 선택 화면");
	return S_OK;
}

void characterNamingScene::release()
{
}

void characterNamingScene::update()
{
	if (_state == namingSceneState::INFO_MSG_UP)
	{
		if (KEY->down('V') || KEY->down('C') || KEY->down('X')
			|| KEY->down('A') || KEY->down('S') || KEY->down('D')
			|| KEY->down(VK_RETURN) || KEY->down(VK_SPACE)
			|| KEY->down(VK_LEFT) || KEY->down(VK_RIGHT)
			|| KEY->down(VK_UP) || KEY->down(VK_DOWN))
		{
			_state = namingSceneState::INFO_MSG_DOWN;
			SND->play("효과음 00", _currMasterVolume * _currSFXVolume);
		}
	}
	else
	{
		if (KEY->down('V'))
		{
			if (_nameArrowPos < 4)
			{
				_playChrName[_nameArrowPos] = _characters[_selectArrowPos];
				++_nameArrowPos;
			}
			else
			{
				_playChrName[4] = _characters[_selectArrowPos];
			}
			SND->play("효과음 00", _currMasterVolume * _currSFXVolume);
		}
		else if (KEY->down('C') && _nameArrowPos != 0)
		{
			if (_playChrName[_nameArrowPos] == ' ')
			{
				--_nameArrowPos;
				_playChrName[_nameArrowPos] = ' ';
			}
			else
			{
				_playChrName[_nameArrowPos] = ' ';
			}
			SND->play("효과음 00", _currMasterVolume * _currSFXVolume);
		}
		else if (KEY->down(VK_LEFT) && _selectArrowPos % 10 != 0)
		{
			--_selectArrowPos;
			SND->play("효과음 00", _currMasterVolume * _currSFXVolume);
		}
		else if (KEY->down(VK_RIGHT) && _selectArrowPos % 10 != 9)
		{
			++_selectArrowPos;
			SND->play("효과음 00", _currMasterVolume * _currSFXVolume);
		}
		else if (KEY->down(VK_UP) && _selectArrowPos / 10 != 0)
		{
			_selectArrowPos -= 10;
			SND->play("효과음 00", _currMasterVolume * _currSFXVolume);
		}
		else if (KEY->down(VK_DOWN) && _selectArrowPos / 10 != 7)
		{
			_selectArrowPos += 10;
			SND->play("효과음 00", _currMasterVolume * _currSFXVolume);
		}
	}

}

void characterNamingScene::render()
{
	PatBlt(getMemDC(), 0, 0, WINW, WINH, BLACKNESS);
	_shouldRenderUsingWindowCoords = TRUE;

	IMG->render("이름 입력 창 1 스킨", getMemDC(), 320, 32);
	_chrFaceImg->render(getMemDC(), 320 + 32, 64);
	TXT->render(getMemDC(), _playChrName, 320 + 320, 32 + 128);
	IMG->render("이름 입력 창 2 스킨", getMemDC(), 128, 288);
	TXT->render(getMemDC(),	"A B C D E F G H I J", 128 + 80, 288 + 48);
	TXT->render(getMemDC(), "K L M N O P Q R S T", 128 + 80, 288 + 48 + 64);
	TXT->render(getMemDC(), "U V W X Y Z ! ? / #", 128 + 80, 288 + 48 + 128);
	TXT->render(getMemDC(), "a b c d e f g h i j", 128 + 80, 288 + 48 + 192);
	TXT->render(getMemDC(), "k l m n o p q r s t", 128 + 80, 288 + 48 + 256);
	TXT->render(getMemDC(), "u v w x y z = - + %", 128 + 80, 288 + 48 + 320);
	TXT->render(getMemDC(), "0 1 2 3 4 5 6 7 8 9", 128 + 80, 288 + 48 + 384);
	TXT->render(getMemDC(), "` \" : & ( ) ' . ,  ", 128 + 80, 288 + 48 + 448);

	IMG->frameRender("위치 표시 타일셋", getMemDC(), 620 + 32 * _nameArrowPos, 92, 2, 1);
	//IMG->frameRender("흰색 타일셋0", getMemDC(), 620 + 32 * _nameArrowPos, 92, 4, 2);
	//IMG->frameRender("흰색 타일셋0", getMemDC(), 620 + 32 + 32 * _nameArrowPos, 92, 5, 2);
	//IMG->frameRender("흰색 타일셋0", getMemDC(), 620 + 32 * _nameArrowPos, 92 + 32, 4, 3);
	//IMG->frameRender("흰색 타일셋0", getMemDC(), 620 + 32 + 32 * _nameArrowPos, 92 + 32, 5, 3);

	IMG->frameRender("위치 표시 타일셋", getMemDC(), 620 + 32 * _nameArrowPos, 184, 2, 0);
	//IMG->frameRender("흰색 타일셋0", getMemDC(), 620 + 32 * _nameArrowPos, 184, 4, 0);
	//IMG->frameRender("흰색 타일셋0", getMemDC(), 620 + 32 + 32 * _nameArrowPos, 184, 5, 0);
	//IMG->frameRender("흰색 타일셋0", getMemDC(), 620 + 32 * _nameArrowPos, 184 + 32, 4, 1);
	//IMG->frameRender("흰색 타일셋0", getMemDC(), 620 + 32 + 32 * _nameArrowPos, 184 + 32, 5, 1);

	IMG->frameRender("위치 표시 타일셋", getMemDC(), 144 + 64 * (_selectArrowPos % 10), 332 + 64 * (_selectArrowPos / 10), 0, 0);
	//IMG->frameRender("흰색 타일셋0", getMemDC(), 144 + 64 * (_selectArrowPos % 10), 332 + 64 * (_selectArrowPos / 10), 0, 0);
	//IMG->frameRender("흰색 타일셋0", getMemDC(), 144 + 32 + 64 * (_selectArrowPos % 10), 332 + 64 * (_selectArrowPos / 10), 1, 0);
	//IMG->frameRender("흰색 타일셋0", getMemDC(), 144 + 64 * (_selectArrowPos % 10), 332 + 32 + 64 * (_selectArrowPos / 10), 0, 1);
	//IMG->frameRender("흰색 타일셋0", getMemDC(), 144 + 32 + 64 * (_selectArrowPos % 10), 332 + 32 + 64 * (_selectArrowPos / 10), 1, 1);

	_shouldRenderUsingWindowCoords = FALSE;

	if (_state == namingSceneState::INFO_MSG_UP)
	{
		_shouldRenderUsingWindowCoords = TRUE;
		IMG->render("이름 입력 안내 창 스킨", getMemDC(), 64, 384);
		TXT->render(getMemDC(), "Enter a name, then push the\nStart Button.", 64 + 96, 384 + 44, 0);
		_shouldRenderUsingWindowCoords = FALSE;
	}
	else if (_state == namingSceneState::INFO_MSG_DOWN)
	{
		_state = namingSceneState::ABLE_TO_CONFIRM_NAME;
	}
	else if (_state == namingSceneState::ABLE_TO_CONFIRM_NAME)
	{
		if (KEY->down(VK_RETURN))
		{
			if (_playChrName == "     ")
			{
				// 무명은 허용되지 않으므로 다음 장면으로 넘어가지 않는다.
				SND->play("효과음 01", _currMasterVolume * _currSFXVolume);
			}
			else
			{
				// 왼쪽 트리밍(공백 제거)
				_playChrName.erase(_playChrName.begin(), find_if(_playChrName.begin(), _playChrName.end(), [](int c) {return !isspace(c);})); // 참고: isspace(int c) checks if c is a white-space.

				// 오른쪽 트리밍(공백 제거)
				_playChrName.erase(find_if(_playChrName.rbegin(), _playChrName.rend(), [](int c) {return !isspace(c);}).base(), _playChrName.end()); // 참고: base() converts a reverse iterator into the corresponding forward iterator.

				_isScrBlackingOut = TRUE;
				SC->changeScene("게임 장면");
			}
		}
	}
}
