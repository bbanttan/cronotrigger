#include "stdafx.h"
#include "frog.h"

HRESULT frog::init()
{
	_id = 3;

	imgSetting();//이미지
	_chaSet.img = im.idle;//이미지는 맨처음 idle로
	_chaSet.Bimg = im.hit;
	aniSetting();
	_chaSet.ani = an.oneArr_0;
	Cleft = _run = false;
	Cc = _T = 0;

	_chaSet.pt = { WINW / 2 - 100, WINH / 2 - 100 };

	_chaSet.rc = MakeRct(_chaSet.pt.x - (224 / 2), _chaSet.pt.y - 192, (224), (224));

	//개구리 기본 스탯
	_status.Lv = 5;						//개구리 초기 레벨
	_status.hp = _status.maxHp = 128;	//개구리 초기 현재/최대HP
	_status.mp = _status.maxMp = 17;	//개구리 초기 현재/최대MP
	_status.power = 9;					//개구리 초기 힘
	_status.hit = 9;					//개구리 초기 명중
	_status.magic = 8;					//개구리 초기 마력
	_status.speed = 11;					//개구리 초기 속도
	_status.evasion = 9;				//개구리 초기 회피
	_status.stamina = 14;				//개구리 초기 체력
	_status.Mdef = 9;					//개구리 초기 마법방어력
	//개구리 기본 스탯 위까지
	dia = 10;
	return S_OK;
}

void frog::release()
{
	SAFE_DEL(an.oneArr_0); SAFE_DEL(an.oneArr_1); SAFE_DEL(an.oneArr_2);	//1장 애니
	SAFE_DEL(an.twoArrT_0); SAFE_DEL(an.twoArrT_1); SAFE_DEL(an.twoArrT_2);	//2장 애니 - 반복
	SAFE_DEL(an.twoArrF_0); SAFE_DEL(an.twoArrF_1); SAFE_DEL(an.twoArrF_2);	//2장 애니 - 반복X
	SAFE_DEL(an.downs_0); SAFE_DEL(an.downs_1); SAFE_DEL(an.downs_2);		//배틀 _ 쓰러지기 직전
	SAFE_DEL(an.walk_0); SAFE_DEL(an.walk_1); SAFE_DEL(an.walk_2);			//노말 / 배틀 _ 걷기 애니
	SAFE_DEL(an.run_0); SAFE_DEL(an.run_1); SAFE_DEL(an.run_2);				//노말 _ 달리기 애니
	SAFE_DEL(an.Bready_0); SAFE_DEL(an.Bready_1); SAFE_DEL(an.Bready_3); SAFE_DEL(an.Bready_2);	//배틀 _ 준비 애니
	SAFE_DEL(an.SB_0); SAFE_DEL(an.SB_1); SAFE_DEL(an.SB_3); SAFE_DEL(an.SB_2);				//배틀 _ 스탠바이 애니
	SAFE_DEL(an.atk_0); SAFE_DEL(an.atk_1); SAFE_DEL(an.atk_2);				//배틀 _ 공격 애니
	SAFE_DEL(an.cri_0); SAFE_DEL(an.cri_1); SAFE_DEL(an.cri_2);				//배틀 _ 크리티컬 애니
	SAFE_DEL(an.Bwin_0); SAFE_DEL(an.Bwin_1); SAFE_DEL(an.Bwin_2);			//배틀 _ 승리
	SAFE_DEL(an.ladder);													//노말 _ 사다리	
}

void frog::update()
{
	//keySetting();
	imgSwitch();
	battleSwitch();

}

void frog::render()
{
	if (!Cleft)		IMG->animRenderZ(static_cast<int>(_chaSet.pt.y), _chaSet.img, getMemDC(), _chaSet.rc.left, _chaSet.rc.top, _chaSet.ani);
	else if (Cleft)	IMG->animRenderHZ(static_cast<int>(_chaSet.pt.y), _chaSet.img, getMemDC(), _chaSet.rc.left, _chaSet.rc.top, _chaSet.ani);
	if (!Cleft	  &&white())	IMG->animRenderZ(static_cast<int>(_chaSet.pt.y), _chaSet.Bimg, getMemDC(), _chaSet.rc.left, _chaSet.rc.top, _chaSet.ani);
	else if (Cleft&&white())	IMG->animRenderHZ(static_cast<int>(_chaSet.pt.y), _chaSet.Bimg, getMemDC(), _chaSet.rc.left, _chaSet.rc.top, _chaSet.ani);
	_chaSet.Bimg->changeAllColors(RGB(255, 255, 255), RGB(255, 0, 255));
}

void frog::imgSetting()
{
	im.idle = IMG->addF("개구리-보통", "res/images/character/frog/idle.bmp", 224, 672, 1, 3, true, RGB(255, 0, 255));
	im.walk = IMG->addF("개구리-걷기", "res/images/character/frog/walk.bmp", 1344, 672, 6, 3, true, RGB(255, 0, 255));
	im.run = IMG->addF("개구리-달리기", "res/images/character/frog/run.bmp", 896, 672, 4, 3, true, RGB(255, 0, 255));
	im.Bready = IMG->addF("개구리-배틀레디", "res/images/character/frog/Bready.bmp", 1568, 672, 7, 3, true, RGB(255, 0, 255));
	im.rush = IMG->addF("개구리-달려들기", "res/images/character/frog/rush.bmp", 448, 672, 2, 3, true, RGB(255, 0, 255));
	im.atk = IMG->addF("개구리-공격", "res/images/character/frog/atkcri.bmp", 1792, 852, 8, 3, true, RGB(255, 0, 255));//크리티컬까지 있음
	im.Bwin = IMG->addF("개구리-배틀승리", "res/images/character/frog/Bwin.bmp", 672, 896, 3, 4, true, RGB(255, 0, 255));
	im.down = IMG->addF("개구리-쓰러짐", "res/images/character/frog/down.bmp", 224, 672, 1, 3, true, RGB(255, 0, 255));
	im.downs = IMG->addF("개구리-쓰러질때", "res/images/character/frog/downs.bmp", 448, 672, 2, 3, true, RGB(255, 0, 255));
	im.hit = IMG->addF("개구리-피격", "res/images/character/frog/hit.bmp", 224, 672, 1, 3, true, RGB(255, 0, 255));
	im.hit2 = IMG->addF("개구리-피격2", "res/images/character/frog/hit2.bmp", 224, 448, 1, 2, true, RGB(255, 0, 255));
	im.ladder = IMG->addF("개구리-사다리", "res/images/character/frog/labber.bmp", 896, 224, 4, 1, true, RGB(255, 0, 255));
	im.skill = IMG->addF("개구리-스킬", "res/images/character/frog/skill.bmp", 448, 672, 2, 3, true, RGB(255, 0, 255));
	im.win = IMG->addF("개구리-승리", "res/images/character/frog/win.bmp", 448, 224, 2, 1, true, RGB(255, 0, 255));
	im.smile = IMG->addF("개구리-웃음", "res/images/character/frog/smile.bmp", 448, 448, 2, 2, true, RGB(255, 0, 255));
}

void frog::aniSetting()
{
	//1장 애니
	// 노말 - 보통		: 3 (front / back / right)
	// 배틀 - 피격		: 3 (front / back / right)
	// 배틀 - 피격2		: 3 (front / back / right)
	// 배틀 - 쓰러짐		: 3 (front / back / right)
	// 노말 - ?			: 3 (front / back / right)
	{
		an.oneArr_0 = new animation;
		an.oneArr_0->init(224, 672, 224, 224);
		int one_1_arr[] = { 0 };
		an.oneArr_0->setPlaylist(one_1_arr, 1);
		an.oneArr_0->setFPS(1);

		an.oneArr_1 = new animation;
		an.oneArr_1->init(224, 672, 224, 224);
		int one_2_arr[] = { 1 };
		an.oneArr_1->setPlaylist(one_2_arr, 1);
		an.oneArr_1->setFPS(1);

		an.oneArr_2 = new animation;
		an.oneArr_2->init(224, 672, 224, 224);
		int one_3_arr[] = { 2 };
		an.oneArr_2->setPlaylist(one_3_arr, 1);
		an.oneArr_2->setFPS(1);
	}
	//2장 애니 - 반복
	// 노말 - 웃음			: 2 - 2 (front / right)
	// 배틀 - 쓰러지는중		: 3 - 2 (front / back / right)
	// 노말 - 승리 포즈		: 1 - 2 (아이템샾 등)
	{
		an.twoArrT_0 = new animation;
		an.twoArrT_0->init(448, 672, 224, 224);
		int twoT_1_arr[] = { 0,1 };
		an.twoArrT_0->setPlaylist(twoT_1_arr, 2, true);
		an.twoArrT_0->setFPS(1);

		an.twoArrT_1 = new animation;
		an.twoArrT_1->init(448, 672, 224, 224);
		int twoT_2_arr[] = { 2,3 };
		an.twoArrT_1->setPlaylist(twoT_2_arr, 2, true);
		an.twoArrT_1->setFPS(1);

		an.twoArrT_2 = new animation;
		an.twoArrT_2->init(448, 672, 224, 224);
		int twoT_3_arr[] = { 4,5 };
		an.twoArrT_2->setPlaylist(twoT_3_arr, 2, true);
		an.twoArrT_2->setFPS(1);
	}
	//2장 애니 - 반복X
	// 노말 - 승리 포즈		: 1 - 2
	// 패틀 - 스킬
	// 배틀 - 달려들기	: 3 (front / back / right)
	{
		an.twoArrF_0 = new animation;
		an.twoArrF_0->init(448, 672, 224, 224);
		int twoF_1_arr[] = { 0,1 };
		an.twoArrF_0->setPlaylist(twoF_1_arr, 2, false);
		an.twoArrF_0->setFPS(1);

		an.twoArrF_1 = new animation;
		an.twoArrF_1->init(448, 672, 224, 224);
		int twoF_2_arr[] = { 2,3 };
		an.twoArrF_1->setPlaylist(twoF_2_arr, 2, false);
		an.twoArrF_1->setFPS(1);

		an.twoArrF_2 = new animation;
		an.twoArrF_2->init(448, 672, 224, 224);
		int twoF_3_arr[] = { 4,5 };
		an.twoArrF_2->setPlaylist(twoF_3_arr, 2, false);
		an.twoArrF_2->setFPS(1);
	}

	//노말 / 배틀 _ 걷기 애니
	{
		an.walk_0 = new animation;
		an.walk_0->init(1344, 672, 224, 224);
		int Wa_F_arr[] = { 0,1,2,3,4,5 };
		an.walk_0->setPlaylist(Wa_F_arr, 6, true);
		an.walk_0->setFPS(1);

		an.walk_1 = new animation;
		an.walk_1->init(1344, 672, 224, 224);
		int Wa_B_arr[] = { 6,7,8,9,10,11 };
		an.walk_1->setPlaylist(Wa_B_arr, 6, true);
		an.walk_1->setFPS(1);

		an.walk_2 = new animation;
		an.walk_2->init(1344, 672, 224, 224);
		int Wa_R_arr[] = { 12,13,14,15,16,17 };
		an.walk_2->setPlaylist(Wa_R_arr, 6, true);
		an.walk_2->setFPS(1);
	}
	//노말 _ 달리기 애니
	{
		an.run_0 = new animation;
		an.run_0->init(896, 672, 224, 224);
		int Ru_F_arr[] = { 0,1,2,3 };
		an.run_0->setPlaylist(Ru_F_arr, 4, true);
		an.run_0->setFPS(1);

		an.run_1 = new animation;
		an.run_1->init(896, 672, 224, 224);
		int Ru_B_arr[] = { 4,5,6,7 };
		an.run_1->setPlaylist(Ru_B_arr, 4, true);
		an.run_1->setFPS(1);

		an.run_2 = new animation;
		an.run_2->init(896, 672, 224, 224);
		int Ru_R_arr[] = { 8,9,10,11 };
		an.run_2->setPlaylist(Ru_R_arr, 4, true);
		an.run_2->setFPS(1);
	}
	//배틀 _ 준비 애니
	{
		an.Bready_0 = new animation;
		an.Bready_0->init(1568, 672, 224, 224);
		int Br_F_arr[] = { 0,1,2,3 };
		an.Bready_0->setPlaylist(Br_F_arr, 4, false);
		an.Bready_0->setFPS(1);

		an.Bready_1 = new animation;
		an.Bready_1->init(1568, 672, 224, 224);
		int Br_B_arr[] = { 7,8,9,10 };
		an.Bready_1->setPlaylist(Br_B_arr, 4, false);
		an.Bready_1->setFPS(1);

		an.Bready_2 = new animation;
		an.Bready_2->init(1568, 672, 224, 224);
		int Br_R_arr[] = { 14,15,16,17 };
		an.Bready_2->setPlaylist(Br_R_arr, 4, false);
		an.Bready_2->setFPS(1);
	}
	//배틀 _ 스탠바이 애니
	{
		an.SB_0 = new animation;
		an.SB_0->init(1568, 672, 224, 224);
		int Sb_F_arr[] = { 5,4,5,6 };
		an.SB_0->setPlaylist(Sb_F_arr, 4, true);
		an.SB_0->setFPS(1);

		an.SB_1 = new animation;
		an.SB_1->init(1568, 672, 224, 224);
		int Sb_B_arr[] = { 12,11,12,13 };
		an.SB_1->setPlaylist(Sb_B_arr, 4, true);
		an.SB_1->setFPS(1);

		an.SB_2 = new animation;
		an.SB_2->init(1568, 672, 224, 224);
		int Sb_R_arr[] = { 19,18,19,20 };
		an.SB_2->setPlaylist(Sb_R_arr, 4, true);
		an.SB_2->setFPS(1);
	}
	//배틀 _ 공격 애니
	{
		an.atk_0 = new animation;
		an.atk_0->init(1792, 852, 224, 284);
		int At_F_arr[] = { 0,1,2 };
		an.atk_0->setPlaylist(At_F_arr, 3, false);
		an.atk_0->setFPS(1);

		an.atk_1 = new animation;
		an.atk_1->init(1792, 852, 224, 284);
		int At_B_arr[] = { 8,9,10 };
		an.atk_1->setPlaylist(At_B_arr, 3, false);
		an.atk_1->setFPS(1);

		an.atk_2 = new animation;
		an.atk_2->init(1792, 852, 224, 284);
		int At_R_arr[] = { 16,17,18 };
		an.atk_2->setPlaylist(At_R_arr, 3, false);
		an.atk_2->setFPS(1);
	}
	//배틀 _ 크리티컬 애니
	{
		an.cri_0 = new animation;
		an.cri_0->init(1792, 852, 224, 284);
		int Cr_F_arr[] = { 0,1,2,3,4,5,6,7 };
		an.cri_0->setPlaylist(Cr_F_arr, 8, false);
		an.cri_0->setFPS(1);

		an.cri_1 = new animation;
		an.cri_1->init(1792, 852, 224, 284);
		int Cr_B_arr[] = { 8,9,10,11,12,13,14,15 };
		an.cri_1->setPlaylist(Cr_B_arr, 8, false);
		an.cri_1->setFPS(1);

		an.cri_2 = new animation;
		an.cri_2->init(1792, 852, 224, 284);
		int Cr_R_arr[] = { 16,17,18,19,20,21,22,23 };
		an.cri_2->setPlaylist(Cr_R_arr, 8, false);
		an.cri_2->setFPS(1);
	}
	//배틀 _ 승리
	{
		an.Bwin_0 = new animation;
		an.Bwin_0->init(672, 896, 224, 224);
		int Bw_F_arr[] = { 0,1,2,9,10 };
		an.Bwin_0->setPlaylist(Bw_F_arr, 5, false);
		an.Bwin_0->setFPS(1);

		an.Bwin_1 = new animation;
		an.Bwin_1->init(672, 896, 224, 224);
		int Bw_B_arr[] = { 3,4,5,9,10 };
		an.Bwin_1->setPlaylist(Bw_B_arr, 5, false);
		an.Bwin_1->setFPS(1);

		an.Bwin_2 = new animation;
		an.Bwin_2->init(672, 896, 224, 224);
		int Bw_R_arr[] = { 6,7,8,9,10 };
		an.Bwin_2->setPlaylist(Bw_R_arr, 5, false);
		an.Bwin_2->setFPS(1);
	}
	//노말 _ 사다리	
	{
		an.ladder = new animation;
		an.ladder->init(896, 224, 224, 224);
		an.ladder->setDefPlaylist();
		an.ladder->setFPS(1);
	}
}

void frog::imgSwitch()
{
	switch (_chaSet.state)
	{
	case NORMAL_IDLE:

		_chaSet.img = im.idle;

		switch (_T)
		{
		case 0:
			_chaSet.ani = an.oneArr_0;
			break;
		case 1:
			_chaSet.ani = an.oneArr_1;
			break;
		case 2:
			if (Cleft)Cleft = false;
			_chaSet.ani = an.oneArr_2;
			break;
		case 3:
			if (!Cleft)Cleft = true;
			_chaSet.ani = an.oneArr_2;
			break;
		}

		break;

	case NORMAL_WALK:
		_chaSet.img = im.walk;
		if (Cc != 0)Cc = 0;
		switch (_T)
		{
		case 0:
			if (Cleft)Cleft = false;
			_chaSet.ani = an.walk_0;
			break;
		case 1:
			if (Cleft)Cleft = false;
			_chaSet.ani = an.walk_1;
			break;
		case 2:
			if (Cleft)Cleft = false;
			_chaSet.ani = an.walk_2;
			break;
		case 3:
			if (!Cleft)Cleft = true;
			_chaSet.ani = an.walk_2;
			break;
		}
		_chaSet.ani->frameUpdate(TIME->getElapsedTime() * 6);
		_chaSet.ani->resume();
		break;

	case NORMAL_RUN:
		_chaSet.img = im.run;
		if (Cc != 0)Cc = 0;
		switch (_T)
		{
		case 0:
			if (Cleft)Cleft = false;
			_chaSet.ani = an.run_0;
			break;
		case 1:
			if (Cleft)Cleft = false;
			_chaSet.ani = an.run_1;
			break;
		case 2:
			if (Cleft)Cleft = false;
			_chaSet.ani = an.run_2;
			break;
		case 3:
			if (!Cleft)Cleft = true;
			_chaSet.ani = an.run_2;
			break;
		}
		_chaSet.ani->frameUpdate(TIME->getElapsedTime() * 6);
		_chaSet.ani->resume();
		break;

	case GETTING_READY:
		_chaSet.img = im.Bready;
		if (Cc != 0)Cc = 0;
		switch (_T)
		{
		case 0:
			if (Cleft)Cleft = false;
			_chaSet.ani = an.Bready_0;
			break;
		case 1:
			if (Cleft)Cleft = false;
			_chaSet.ani = an.Bready_1;
			break;
		case 2:
			if (!Cleft)Cleft = true;
			_chaSet.ani = an.Bready_2;
			break;
		case 3:
			if (Cleft)Cleft = false;
			_chaSet.ani = an.Bready_2;
			break;
		}
		_chaSet.ani->frameUpdate(TIME->getElapsedTime() * 4);
		break;

	case BATTLE_READY:
		_chaSet.img = im.Bready;
		switch (_T)
		{
		case 0:
			if (Cleft)Cleft = false;
			_chaSet.ani = an.SB_0;
			break;
		case 1:
			if (Cleft)Cleft = false;
			_chaSet.ani = an.SB_1;
			break;
		case 2:
			if (!Cleft)Cleft = false;
			_chaSet.ani = an.SB_2;
			break;
		case 3:
			if (Cleft)Cleft = true;
			_chaSet.ani = an.SB_2;
			break;
		}
		_chaSet.ani->frameUpdate(TIME->getElapsedTime() * 4);

		break;

	case BATTLE_MOVE:
		_chaSet.img = im.walk;
		switch (_T)
		{
		case 0:
			if (Cleft)Cleft = false;
			_chaSet.ani = an.walk_0;
			break;
		case 1:
			if (Cleft)Cleft = false;
			_chaSet.ani = an.walk_1;
			break;
		case 2:
			if (Cleft)Cleft = false;
			_chaSet.ani = an.walk_2;
			break;
		case 3:
			if (!Cleft)Cleft = true;
			_chaSet.ani = an.walk_2;
			break;
		}
		_chaSet.ani->frameUpdate(TIME->getElapsedTime() * 6);
		_chaSet.ani->resume();

		break;
	case BATTLE_RUSH:
		_chaSet.img = im.rush;

		switch (_T)
		{
		case 0:
			if (Cleft)Cleft = false;
			_chaSet.ani = an.twoArrF_0;
			break;
		case 1:
			if (Cleft)Cleft = false;
			_chaSet.ani = an.twoArrF_1;
			break;
		case 2:
			if (Cleft)Cleft = false;
			_chaSet.ani = an.twoArrF_2;
			break;
		case 3:
			if (!Cleft)Cleft = true;
			_chaSet.ani = an.twoArrF_2;
			break;
		}
		_chaSet.ani->frameUpdate(TIME->getElapsedTime() * 6);
		break;

	case BATTLE_ATK:
		_chaSet.img = im.atk;
		if (Cc != 0)Cc = 0;
		switch (_T)
		{
		case 0:
			if (Cleft)Cleft = false;
			_chaSet.ani = an.atk_0;
			break;
		case 1:
			if (Cleft)Cleft = false;
			_chaSet.ani = an.atk_1;
			break;
		case 2:
			if (Cleft)Cleft = false;
			_chaSet.ani = an.atk_2;
			break;
		case 3:
			if (!Cleft)Cleft = true;
			_chaSet.ani = an.atk_2;
			break;
		}
		_chaSet.ani->frameUpdate(TIME->getElapsedTime() * 4);

		break;

	case BATTLE_RETURN:
		_chaSet.img = im.rush;

		if (Cc != 0)Cc = 0;

		switch (_T)
		{
		case 0:
			if (Cleft)Cleft = false;
			_chaSet.ani = an.twoArrF_0;
			break;
		case 1:
			if (Cleft)Cleft = false;
			_chaSet.ani = an.twoArrF_1;
			break;
		case 2:
			if (!Cleft)Cleft = false;
			_chaSet.ani = an.twoArrF_2;
			break;
		case 3:
			if (Cleft)Cleft = true;
			_chaSet.ani = an.twoArrF_2;
			break;
		}
		_chaSet.ani->frameUpdate(TIME->getElapsedTime() * 6);
		break;
	case BATTLE_HIT:
		_chaSet.img = im.hit;
		_chaSet.Bimg = im.hit;
		switch (_T)
		{
		case 0:
			if (Cleft)Cleft = false;
			_chaSet.ani = an.oneArr_0;
			break;
		case 1:
			if (Cleft)Cleft = false;
			_chaSet.ani = an.oneArr_1;
			break;
		case 2:
			if (Cleft)Cleft = false;
			_chaSet.ani = an.oneArr_2;
			break;
		case 3:
			if (!Cleft)Cleft = true;
			_chaSet.ani = an.oneArr_2;
			break;
		}
		break;
	case BATTLE_HELP:
		_chaSet.img = im.downs;
		switch (_T)
		{
		case 0:
			if (Cleft)Cleft = false;
			_chaSet.ani = an.twoArrF_0;
			break;
		case 1:
			if (Cleft)Cleft = false;
			_chaSet.ani = an.twoArrF_1;
			break;
		case 2:
			if (Cleft)Cleft = false;
			_chaSet.ani = an.twoArrF_2;
			break;
		case 3:
			if (!Cleft)Cleft = true;
			_chaSet.ani = an.twoArrF_2;
			break;
		}
		_chaSet.ani->frameUpdate(TIME->getElapsedTime() * 4);
		break;
	case BATTLE_LOSE:
		_chaSet.img = im.down;
		switch (_T)
		{
		case 0:
			if (Cleft)Cleft = false;
			_chaSet.ani = an.oneArr_0;
			break;
		case 1:
			if (Cleft)Cleft = false;
			_chaSet.ani = an.oneArr_1;
			break;
		case 2:
			if (Cleft)Cleft = false;
			_chaSet.ani = an.oneArr_2;
			break;
		case 3:
			if (!Cleft)Cleft = true;
			_chaSet.ani = an.oneArr_2;
			break;
		}
		break;
	case BATTLE_WIN:
		_chaSet.img = im.Bwin;
		switch (_T)
		{
		case 0:
			if (Cleft)Cleft = false;
			_chaSet.ani = an.Bwin_0;
			break;
		case 1:
			if (Cleft)Cleft = false;
			_chaSet.ani = an.Bwin_1;
			break;
		case 2:
			if (Cleft)Cleft = true;
			_chaSet.ani = an.Bwin_2;
			break;
		case 3:
			if (!Cleft)Cleft = false;
			_chaSet.ani = an.Bwin_2;
			break;
		}
		_chaSet.ani->frameUpdate(TIME->getElapsedTime() * 4);
		break;

	}
	if (_chaSet.state == BATTLE_ATK)
	{
		_chaSet.rc = MakeRct(_chaSet.pt.x - ((224) / 2), _chaSet.pt.y - (284 - 64), (224), (284));
	}
	else { _chaSet.rc = MakeRct(_chaSet.pt.x - ((224) / 2), _chaSet.pt.y - 192, (224), (224)); }

}

void frog::keySetting()
{
	if (KEY->press(VK_LEFT) && 0 < _chaSet.pt.x)
	{
		if (_T != 3)_T = 3;
		if (_run)
		{
			if (_chaSet.state != NORMAL_RUN) { _chaSet.state = NORMAL_RUN; an.run_2->stop(); }

			_chaSet.pt.x -= 8;
		}
		if (!_run)
		{
			if (_chaSet.state != NORMAL_WALK) { _chaSet.state = NORMAL_WALK;  an.walk_2->stop(); }
			_chaSet.pt.x -= 5;
		}
	}
	else if (KEY->up(VK_LEFT))
	{
		if (_chaSet.state != NORMAL_IDLE)_chaSet.state = NORMAL_IDLE;
	}
	if (KEY->press(VK_RIGHT) )
	{
		if (_T != 2)_T = 2;
		if (_run)
		{
			if (_chaSet.state != NORMAL_RUN) { _chaSet.state = NORMAL_RUN; an.run_2->stop(); }
			_chaSet.pt.x += 8;
		}
		if (!_run)
		{
			if (_chaSet.state != NORMAL_WALK) { _chaSet.state = NORMAL_WALK;  an.walk_2->stop(); }
			_chaSet.pt.x += 5;
		}

	}
	else if (KEY->up(VK_RIGHT))
	{
		if (_chaSet.state != NORMAL_IDLE)_chaSet.state = NORMAL_IDLE;
	}
	if (KEY->press(VK_UP) && _chaSet.pt.y > 0)
	{
		if (_T != 1)_T = 1;
		if (_run)
		{
			if (_chaSet.state != NORMAL_RUN) { _chaSet.state = NORMAL_RUN; an.run_1->stop(); }
			_chaSet.pt.y -= 8;
		}
		if (!_run)
		{
			if (_chaSet.state != NORMAL_WALK) { _chaSet.state = NORMAL_WALK;  an.walk_1->stop(); }
			_chaSet.pt.y -= 5;
		}
	}
	else if (KEY->up(VK_UP))
	{
		if (_chaSet.state != NORMAL_IDLE)_chaSet.state = NORMAL_IDLE;
	}
	if (KEY->press(VK_DOWN))
	{
		if (_T != 0)_T = 0;
		if (_run)
		{
			if (_chaSet.state != NORMAL_RUN) { _chaSet.state = NORMAL_RUN; an.run_0->stop(); }
			_chaSet.pt.y += 8;
		}
		if (!_run)
		{
			if (_chaSet.state != NORMAL_WALK) { _chaSet.state = NORMAL_WALK; an.walk_0->stop(); }
			_chaSet.pt.y += 5;
		}
	}
	else if (KEY->up(VK_DOWN))
	{
		if (_chaSet.state != NORMAL_IDLE)_chaSet.state = NORMAL_IDLE;
	}
	if (KEY->press(VK_LEFT) && KEY->press(VK_RIGHT)) { if (_chaSet.state != NORMAL_IDLE)_chaSet.state = NORMAL_IDLE; }
	if (KEY->press(VK_UP) && KEY->press(VK_DOWN)) { if (_chaSet.state != NORMAL_IDLE)_chaSet.state = NORMAL_IDLE; }

	if (KEY->press('C'))
	{
		if (_run == false)_run = true;
	}
	else if (KEY->up('C')) { if (_run == true)_run = false; }
	if (KEY->down(VK_SPACE))
	{
		if (_chaSet.state == NORMAL_IDLE ||
			_chaSet.state == NORMAL_WALK ||
			_chaSet.state == NORMAL_RUN)
		{
			_chaSet.state = GETTING_READY;
		}
		else if (_chaSet.state == BATTLE_READY)
		{
			_chaSet.state = BATTLE_WIN;
		}
	}
	if (KEY->down(VK_LBUTTON) && _chaSet.state == BATTLE_READY)
	{
		_chaSet.atk = _mouse;
		_chaSet.atkS = _chaSet.pt;
		float tAngle = Angle(static_cast<float>(_chaSet.atkS.x), static_cast<float>(_chaSet.atkS.y), static_cast<float>(_chaSet.atk.x), static_cast<float>(_chaSet.atk.y));
		float _pi = PI / 9;
		if (_pi * 3 < tAngle && tAngle <= _pi * 6) { _T = 1; }//위쪽보기
		else if (-_pi * 3 < tAngle && tAngle <= _pi * 3) { _T = 2; }//오른쪽보기
		else if (-_pi * 3 >= tAngle && tAngle > -_pi * 6) { _T = 0; }//아래쪽보기
		else { _T = 3; }//왼쪽 보기

		_chaSet.state = BATTLE_RUSH;
	}

}

void frog::battleSwitch()
{
	switch (_chaSet.state)
	{
	case GETTING_READY:
		//시작 시 start / 끝났을 때 STACDBY 상태가 되는 것을 설정한다
		if (_chaSet.ani->getCurrPlaylistIdx() != 0 && _chaSet.ani->isPlay() == FALSE)
		{
			_chaSet.state = BATTLE_READY;
			_chaSet.ani->stop();
		}
		else if (_chaSet.ani->getCurrPlaylistIdx() == 0 && _chaSet.ani->isPlay() == FALSE)_chaSet.ani->start();
		break;
	case BATTLE_READY:
		//배틀 행동 선택 가능할 때 움직일 수 있다
		//임시로 stop 상태일 때 움직이게 해놨음
		if (_chaSet.ani->isPlay() == FALSE)_chaSet.ani->start();

		break;
	case BATTLE_MOVE:
		//적의 방향으로 A*를 이용해서 움직인다

		break;
	case BATTLE_RUSH:
		//적의 방향으로 Angle을 통해 움직인다
		//if (_chaSet.ani->getCurrPlaylistIdx() != 0)
		_chaSet.angle = Angle(static_cast<float> (_chaSet.pt.x), static_cast<float>(_chaSet.pt.y), static_cast<float>(_chaSet.atk.x), static_cast<float>(_chaSet.atk.y));
		_chaSet.Dis = Distance(static_cast<float>(_chaSet.pt.x), static_cast<float>(_chaSet.pt.y), static_cast<float>(_chaSet.atk.x), static_cast<float>(_chaSet.atk.y));

		if (_chaSet.ani->getCurrPlaylistIdx() != 0)
		{
			_chaSet.pt.x += static_cast<int>(cosf(_chaSet.angle) * 15);
			_chaSet.pt.y += -static_cast<int>(sinf(_chaSet.angle) * 15);
		}

		if (_chaSet.Dis <= dia)
		{
			_chaSet.ani->stop();
			_chaSet.state = BATTLE_ATK;
		}
		else if (_chaSet.ani->getCurrPlaylistIdx() == 0 && _chaSet.ani->isPlay() == FALSE) { _chaSet.ani->start(); }

		//스킬과 공격을 결정짓는 인트변수 등을 설정해야 한다


		break;
	case BATTLE_ATK:
		//공격시 이미지에 맞춰 적의 이미지를 설정, 효과음을 설정한다

		//공격 모션이 시작되어야 할 때 시작 / 끝났을 때 돌아가는 것을 설정한다
		if (_chaSet.ani->getCurrPlaylistIdx() != 0 && _chaSet.ani->isPlay() == FALSE)
		{
			_chaSet.ani->stop();
			_chaSet.state = BATTLE_RETURN;
		}
		else if (_chaSet.ani->getCurrPlaylistIdx() == 0 && _chaSet.ani->isPlay() == FALSE) { _chaSet.ani->start(); }
		break;
	case BATTLE_RETURN:
		//시작 위치로 Angle을 통해 움직인다
		_chaSet.angle = Angle(static_cast<float> (_chaSet.atkS.x), static_cast<float>(_chaSet.atkS.y), static_cast<float>(_chaSet.pt.x), static_cast<float>(_chaSet.pt.y));
		_chaSet.Dis = Distance(static_cast<float>(_chaSet.atkS.x), static_cast<float>(_chaSet.atkS.y), static_cast<float>(_chaSet.pt.x), static_cast<float>(_chaSet.pt.y));

		if (_chaSet.ani->getCurrPlaylistIdx() != 0)
		{
			_chaSet.pt.x -= static_cast<int>(cosf(_chaSet.angle) * 15);
			_chaSet.pt.y -= -static_cast<int>(sinf(_chaSet.angle) * 15);
		}

		if (dia >= _chaSet.Dis)
		{
			_chaSet.ani->stop();
			_chaSet.state = BATTLE_READY;
		}

		else if (_chaSet.Dis > dia &&_chaSet.ani->getCurrPlaylistIdx() == 0 && _chaSet.ani->isPlay() == FALSE) { _chaSet.ani->start(); }
		break;
	case BATTLE_HIT:
		++hitCol;
		if (hitCol % 4 == 0)hitCol = 0;
		break;
	case BATTLE_HELP:
		if (Dcheck < 3 && _chaSet.ani->isPlay() == FALSE)
		{
			_chaSet.ani->start();
			Dcheck++;
		}
		else if (Dcheck >= 3 && _chaSet.ani->isPlay() == FALSE)
		{
			_chaSet.state = BATTLE_LOSE;
		}
		break;
	case BATTLE_WIN:
		//노말_레디는 전투 후 크로노(1번째 플레이어)쪽으로 모인다, 임시로 idle로 바로 바뀌게 했다
		if (_chaSet.ani->getCurrPlaylistIdx() != 0 && _chaSet.ani->isPlay() == FALSE)_chaSet.state = NORMAL_IDLE;
		else if (_chaSet.ani->getCurrPlaylistIdx() == 0 && _chaSet.ani->isPlay() == FALSE)_chaSet.ani->start();

		break;
	}
}
