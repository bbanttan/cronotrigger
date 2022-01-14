#include "stdafx.h"
#include "gato.h"
#include "player.h"
gato::gato()
{
}

gato::~gato()
{


}

HRESULT gato::init()
{
	_id = 0;

	_Status.hp = 76;
	_Status.def = 127;
	_Status.Mdef = 50;
	_Status.exp = 10;
	_Status.atk = 7;
	_Status.magic = 8;
	_Status.speed = 8;
	_Status.techPoint = 1;

	imgSetting();
	_ChaSet.img = _Img.idle;
	aniSetting();
	_ChaSet.ani = _Ani.idle_front;
	_ChaSet.ani->start();
	//_ChaSet.ani->stop();
	_ChaSet.pt = { WINW / 2 +40 , WINH / 2 + 78 };

	_ChaSet.rc = MakeRct(_ChaSet.pt.x - ((348) / 2), _ChaSet.pt.y - 192, (348), (284));

	_ChaSet.Bimg = _Img.hit;
	_ChaSet.Bani = _Ani.hit;
	_ChaSet.triggerTime = 0;


	return S_OK;
}

void gato::release()
{
	SAFE_DEL(_Ani.talk); 
	SAFE_DEL(_Ani.idle_front);
	SAFE_DEL(_Ani.move_front); SAFE_DEL(_Ani.move_back); SAFE_DEL(_Ani.move_right); SAFE_DEL(_Ani.move_left);
	SAFE_DEL(_Ani.atk_front); SAFE_DEL(_Ani.atk_back); SAFE_DEL(_Ani.atk_right); SAFE_DEL(_Ani.atk_left);
	SAFE_DEL(_Ani.hit);
	SAFE_DEL(_Ani.Oth_F); SAFE_DEL(_Ani.Oth_B);  SAFE_DEL(_Ani.Oth_R);  SAFE_DEL(_Ani.Oth_L);
}

void gato::update()
{
	_ChaSet.rc = MakeRct(_ChaSet.pt.x - ((348) / 2), _ChaSet.pt.y - 192, (220), (224));
	
	imgSwitch();

}

void gato::render()
{
	//_ChaSet.img->animRender(getMemDC(), _ChaSet.rc.left, _ChaSet.rc.top, _ChaSet.ani);
	IMG->animRenderZ(_ChaSet.pt.y,_ChaSet.img, getMemDC(), _ChaSet.rc.left, _ChaSet.rc.top, _ChaSet.ani);
	
	//IMG->animRenderZ(_ChaSet.pt.y, _ChaSet.Bimg, getMemDC(), _ChaSet.rc.left, _ChaSet.rc.top, _ChaSet.Bani);
	////_ChaSet.Bimg->frameRender(getMemDC(), _ChaSet.rc.left, _ChaSet.rc.top);
	//_ChaSet.Bimg->changeAllColors(RGB(255,255,255));
}

void gato::imgSetting()
{
	_Img.talk = IMG->addF	("가토-대화", "res/images/enemy/gato/talk.bmp", 1044, 568, 3, 2, true, RGB(255, 0, 255));
	_Img.idle = IMG->addF	("가토-대기", "res/images/enemy/gato/idle.bmp", 696, 284, 2, 1, true, RGB(255, 0, 255));
	_Img.move = IMG->addF	("가토-이동", "res/images/enemy/gato/move.bmp", 1044, 1136, 4, 4, true, RGB(255, 0, 255));
	_Img.atk = IMG->addF	("가토-공격", "res/images/enemy/gato/atk.bmp", 1392, 1136, 4, 4, true, RGB(255, 0, 255));
	_Img.hit = IMG->addF	("가토-피격", "res/images/enemy/gato/hit.bmp", 348, 284, 1, 1, true, RGB(255, 0, 255));
	_Img.other1 = IMG->addF	("가토-허밍1", "res/images/enemy/gato/humming1.bmp", 56, 70, 1, 1, true, RGB(255, 0, 255));
	_Img.other2 = IMG->addF	("가토-허밍2", "res/images/enemy/gato/humming2.bmp", 56, 74, 1, 1, true, RGB(255, 0, 255));
}

void gato::aniSetting()
{
	//대화 애니
	{
		_Ani.talk = new animation;
		_Ani.talk->init(1044, 568, 348, 284);
		int ani_Tk_arr[] = { 3,4,5,4,3 };
		_Ani.talk->setPlaylist(ani_Tk_arr, 5, true);
		_Ani.talk->setFPS(5);
	}
	//대기 애니
	{
		_Ani.idle_front = new animation;
		_Ani.idle_front->init(696, 284, 348, 284);
		int ani_Id_F_arr[] = { 0,1,0,1,0 };
		_Ani.idle_front->setPlaylist(ani_Id_F_arr, 5, true);
		_Ani.idle_front->setFPS(5);
	}
	//전투시 이동 애니
	{
		_Ani.move_front = new animation;
		_Ani.move_front->init(1044, 1036, 348, 284);
		int mv_F_arr[] = { 0,1,2,1,0, };
		_Ani.move_front->setPlaylist(mv_F_arr, 5, true);
		_Ani.move_front->setFPS(5);

		_Ani.move_back = new animation;
		_Ani.move_back->init(1044, 1036, 348, 284);
		int  mv_B_arr[] = { 3,4,5,4,3 };
		_Ani.move_back->setPlaylist(mv_B_arr, 5, true);
		_Ani.move_back->setFPS(5);

		_Ani.move_right = new animation;
		_Ani.move_right->init(1044, 1036, 348, 284);
		int  mv_R_arr[] = { 6,7,8,6,7 };
		_Ani.move_right->setPlaylist(mv_R_arr, 5, true);
		_Ani.move_right->setFPS(5);

		_Ani.move_left = new animation;
		_Ani.move_left->init(1044, 1036, 348, 284);
		int  mv_L_arr[] = { 9,10,11,10,9 };
		_Ani.move_left->setPlaylist(mv_L_arr, 5, true);
		_Ani.move_left->setFPS(5);
	}
	//공격 애니
	{
		_Ani.atk_front = new animation;
		_Ani.atk_front->init(1392, 1136, 348, 284);
		int atk_F_arr[] = { 0,1,2,3,2,1,0 };
		_Ani.atk_front->setPlaylist(atk_F_arr, 7, true);
		_Ani.atk_front->setFPS(5);

		_Ani.atk_back = new animation;
		_Ani.atk_back->init(1392, 1136, 348, 284);
		int  atk_B_arr[] = { 4,5,6,7,6,5,4 };
		_Ani.atk_back->setPlaylist(atk_B_arr, 7, true);
		_Ani.atk_back->setFPS(5);

		_Ani.atk_right = new animation;
		_Ani.atk_right->init(1392, 1136, 348, 284);
		int  atk_R_arr[] = { 8,9,10,11,10,9,8 };
		_Ani.atk_right->setPlaylist(atk_R_arr, 7, true);
		_Ani.atk_right->setFPS(5);

		_Ani.atk_left = new animation;
		_Ani.atk_left->init(1392, 1136, 348, 284);
		int  atk_L_arr[] = { 12,13,14,15,14,13,12 };
		_Ani.atk_left->setPlaylist(atk_L_arr, 7, true);
		_Ani.atk_left->setFPS(5);
	}
	//피격 애니
	{
		_Ani.hit = new animation;
		_Ani.hit->init(348, 284, 348, 284);
		_Ani.hit->setDefPlaylist();
		_Ani.hit->setFPS(5);
	}
	{
		_Ani.Oth_F = new animation;
		_Ani.Oth_F->init(1044, 1036, 348, 284);
		int O_F_arr[] = { 0 };
		_Ani.Oth_F->setPlaylist(O_F_arr, 1);
		_Ani.Oth_F->setFPS(5);

		_Ani.Oth_B = new animation;
		_Ani.Oth_B->init(1044, 1036, 348, 284);
		int  O_B_arr[] = { 3 };
		_Ani.Oth_B->setPlaylist(O_B_arr, 1);
		_Ani.Oth_B->setFPS(5);

		_Ani.Oth_R = new animation;
		_Ani.Oth_R->init(1044, 1036, 348, 284);
		int  O_R_arr[] = { 6 };
		_Ani.Oth_R->setPlaylist(O_R_arr, 1);
		_Ani.Oth_R->setFPS(5);

		_Ani.Oth_L = new animation;
		_Ani.Oth_L->init(1044, 1036, 348, 284);
		int  O_L_arr[] = { 9 };
		_Ani.Oth_L->setPlaylist(O_L_arr, 1);
		_Ani.Oth_L->setFPS(5);
	}
}

void gato::imgSwitch()
{
	switch (_ChaSet.state)
	{
	case NORMAL_IDLE:
		//_ChaSet.img = _Img.idle;

		_ChaSet.img = _Img.move;//보여줄 이미지
		
		//방향 정하기 switch 또는 if
		switch (_T)
		{
			//front
		case 0:
			_ChaSet.ani = _Ani.Oth_F;
			break;

			//back
		case 1:
			_ChaSet.ani = _Ani.Oth_B;
			break;

			//right
		case 2:
			_ChaSet.ani = _Ani.Oth_R;
			break;

			//left
		case 3:
			_ChaSet.ani = _Ani.Oth_L;
			break;
		}
		//2장 이상 있을 경우 프레임 업데이트를 시킨다
		//_ChaSet.ani->frameUpdate(TIME->getElapsedTime() * 5);
		break;
	case BATTLE_ATK:
		_ChaSet.img = _Img.atk;//보여줄 이미지

		//방향 정하기 switch 또는 if
		switch (_T)
		{
			//front
		case 0:
			_ChaSet.ani = _Ani.atk_front;
			break;

			//back
		case 1:
			_ChaSet.ani = _Ani.atk_back;
			break;

			//right
		case 2:
			_ChaSet.ani = _Ani.atk_right;
			break;

			//left
		case 3:
			_ChaSet.ani = _Ani.atk_left;
			break;
		}
		//2장 이상 있을 경우 프레임 업데이트를 시킨다
		//_ChaSet.ani->frameUpdate(TIME->getElapsedTime() * 5);
		break;
	case BATTLE_HIT:
		_ChaSet.img = _Img.hit;//보여줄 이미지
		_ChaSet.ani = _Ani.hit;

		//_ChaSet.ani->frameUpdate(TIME->getElapsedTime() * 5);
		break;
	}
	_ChaSet.ani->frameUpdate(TIME->getElapsedTime());

}

void gato::battleSwitch()
{
	switch (_ChaSet.state)
	{
	case NORMAL_IDLE:
		//_ChaSet.img = _Img.idle;

		_ChaSet.img = _Img.move;//보여줄 이미지

		//방향 정하기 switch 또는 if
		switch (_T)
		{
			//front
		case 0:
			_ChaSet.ani = _Ani.Oth_F;
			break;

			//back
		case 1:
			_ChaSet.ani = _Ani.Oth_B;
			break;

			//right
		case 2:
			_ChaSet.ani = _Ani.Oth_R;
			break;

			//left
		case 3:
			_ChaSet.ani = _Ani.Oth_L;
			break;
		}
		//2장 이상 있을 경우 프레임 업데이트를 시킨다
		//_ChaSet.ani->frameUpdate(TIME->getElapsedTime() * 5);

		break;
	case BATTLE_ATK:
		_ChaSet.img = _Img.atk;//보여줄 이미지

		//방향 정하기 switch 또는 if
		switch (_T)
		{
			//front
		case 0:
			_ChaSet.ani = _Ani.atk_front;
			break;

			//back
		case 1:
			_ChaSet.ani = _Ani.atk_back;
			break;

			//right
		case 2:
			_ChaSet.ani = _Ani.atk_right;
			break;

			//left
		case 3:
			_ChaSet.ani = _Ani.atk_left;
			break;
		}
		//2장 이상 있을 경우 프레임 업데이트를 시킨다
		//_ChaSet.ani->frameUpdate(TIME->getElapsedTime() * 5);
		break;
	case BATTLE_HIT:
		_ChaSet.img = _Img.hit;//보여줄 이미지
		_ChaSet.ani = _Ani.hit;

		//_ChaSet.ani->frameUpdate(TIME->getElapsedTime() * 5);
		break;
	}
}
