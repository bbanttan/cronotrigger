#include "stdafx.h"
#include "lucca.h"

HRESULT lucca::init()
{
	_id = 1;

	imgSetting();//이미지
	_chaSet.img = im.idle;//이미지는 맨처음 idle로
	_chaSet.Bimg = im.hit;
	aniSetting();
	_chaSet.ani = an.oneArr_0;
	Cleft = _run = false;
	Cc = _T = 0;

	_chaSet.pt = { WINW / 2 + 100, WINH / 2 + 100 };

	_chaSet.rc = MakeRct(_chaSet.pt.x - ((220) / 2), _chaSet.pt.y - 192, (220), (224));

	//루카 기본 스탯
	_status.Lv = 2;					//루카 초기 레벨
	_status.hp = _status.maxHp = 75;	//루카 초기 현재/최대HP
	_status.mp = _status.maxMp = 14;	//루카 초기 현재/최대MP
	_status.power = 2;				//루카 초기 힘
	_status.hit = 8;					//루카 초기 명중
	_status.magic = 8;				//루카 초기 마력
	_status.speed = 6;				//루카 초기 속도
	_status.evasion = 8;				//루카 초기 회피
	_status.stamina = 6;				//루카 초기 체력
	_status.Mdef = 10;				//루카 초기 마법방어력
	//루카 기본 스탯 위까지
	dia = 10;
	return S_OK;
}

void lucca::release()
{
	SAFE_DEL(an.oneArr_0); SAFE_DEL(an.oneArr_1); SAFE_DEL(an.oneArr_2);	//1장 애니
	SAFE_DEL(an.twoArrT_0); SAFE_DEL(an.twoArrT_1); SAFE_DEL(an.twoArrT_2);	//2장 애니 : 반복
	SAFE_DEL(an.twoArrF_0); SAFE_DEL(an.twoArrF_1); SAFE_DEL(an.twoArrF_2);	//2장 애니 : 반복X
	SAFE_DEL(an.walk_0); SAFE_DEL(an.walk_1); SAFE_DEL(an.walk_2);			//노말 / 배틀 _ 걷기 애니
	SAFE_DEL(an.run_0); SAFE_DEL(an.run_1); SAFE_DEL(an.run_2);				//노말 _ 달리기 애니
	SAFE_DEL(an.Bready_0); SAFE_DEL(an.Bready_1); SAFE_DEL(an.Bready_2);	//배틀 _ 준비 애니
	SAFE_DEL(an.SB_0); SAFE_DEL(an.SB_1); SAFE_DEL(an.SB_2);				//배틀 _ 스탠바이 애니
	SAFE_DEL(an.atk_0); SAFE_DEL(an.atk_1); SAFE_DEL(an.atk_2);				//배틀 _ 근거리 공격 애니
	SAFE_DEL(an.atk2_0); SAFE_DEL(an.atk2_1); SAFE_DEL(an.atk2_2);			//배틀 _ 원거리 공격 애니
	SAFE_DEL(an.Bwin);														//배틀 _ 승리
	SAFE_DEL(an.cantRun_0); SAFE_DEL(an.cantRun_1); SAFE_DEL(an.cantRun_2);	//노말 _ 못달린다
	SAFE_DEL(an.ladder);

}

void lucca::update()
{
	//keySetting();
	imgSwitch();
	battleSwitch();
}

void lucca::render()
{
	if (!Cleft)		IMG->animRenderZ(static_cast<int>(_chaSet.pt.y), _chaSet.img, getMemDC(), _chaSet.rc.left, _chaSet.rc.top, _chaSet.ani);
	else if (Cleft)	IMG->animRenderHZ(static_cast<int>(_chaSet.pt.y), _chaSet.img, getMemDC(), _chaSet.rc.left, _chaSet.rc.top, _chaSet.ani);
	if (!Cleft	  &&white())	IMG->animRenderZ(static_cast<int>(_chaSet.pt.y), _chaSet.Bimg, getMemDC(), _chaSet.rc.left, _chaSet.rc.top, _chaSet.ani);
	else if (Cleft&&white())	IMG->animRenderHZ(static_cast<int>(_chaSet.pt.y), _chaSet.Bimg, getMemDC(), _chaSet.rc.left, _chaSet.rc.top, _chaSet.ani);
	_chaSet.Bimg->changeAllColors(RGB(255, 255, 255), RGB(255, 0, 255));

}

void lucca::imgSetting()
{
	im.idle = IMG->addF("루카-보통", "res/images/character/lucca/idle.bmp", 220, 672, 1, 3, true, RGB(255, 0, 255));
	im.walk = IMG->addF("루카-걷기", "res/images/character/lucca/walk.bmp", 1320, 672, 6, 3, true, RGB(255, 0, 255));
	im.run = IMG->addF("루카-달리기", "res/images/character/lucca/run.bmp", 1320, 672, 6, 3, true, RGB(255, 0, 255));
	im.Bready = IMG->addF("루카-배틀레디", "res/images/character/lucca/Bready.bmp", 1320, 672, 6, 3, true, RGB(255, 0, 255));
	im.atk = IMG->addF("루카-근거리공격", "res/images/character/lucca/Satk.bmp", 660, 672, 3, 3, true, RGB(255, 0, 255));
	im.atk2 = IMG->addF("루카-원거리공격", "res/images/character/lucca/Latk.bmp", 880, 672, 4, 3, true, RGB(255, 0, 255));
	im.Bwin = IMG->addF("루카-배틀승리", "res/images/character/lucca/Bwin.bmp", 1540, 224, 7, 1, true, RGB(255, 0, 255));
	im.cantRun = IMG->addF("루카-못달림", "res/images/character/lucca/cantRun.bmp", 660, 672, 3, 3, true, RGB(255, 0, 255));
	im.down = IMG->addF("루카-쓰러짐", "res/images/character/lucca/down.bmp", 220, 224, 1, 1, true, RGB(255, 0, 255));
	im.downs = IMG->addF("루카-쓰러질때", "res/images/character/lucca/downs.bmp", 440, 672, 2, 3, true, RGB(255, 0, 255));
	im.hit = IMG->addF("루카-피격", "res/images/character/lucca/hit.bmp", 220, 672, 1, 3, true, RGB(255, 0, 255));
	im.hit2 = IMG->addF("루카-피격2", "res/images/character/lucca/hit2.bmp", 220, 672, 1, 3, true, RGB(255, 0, 255));
	im.ladder = IMG->addF("루카-사다리", "res/images/character/lucca/labber.bmp", 880, 224, 4, 1, true, RGB(255, 0, 255));
	im.smile = IMG->addF("루카-스킬", "res/images/character/lucca/smile.bmp", 440, 448, 2, 2, true, RGB(255, 0, 255));
	im.what = IMG->addF("루카-궁금", "res/images/character/lucca/what.bmp", 220, 672, 1, 3, true, RGB(255, 0, 255));
	im.win = IMG->addF("루카-승리", "res/images/character/lucca/win.bmp", 440, 224, 2, 1, true, RGB(255, 0, 255));
}

void lucca::aniSetting()
{
	//1장 애니
	// 노말 - 보통		: 3 (front / back / right)
	// 배틀 - 피격		: 3 (front / back / right)
	// 배틀 - 피격2		: 3 (front / back / right)
	// 배틀 - 쓰러짐		: 1 
	// 노말 - ?			: 3 (front / back / right)
	{
		an.oneArr_0 = new animation;
		an.oneArr_0->init(220, 672, 220, 224);
		int one_1_arr[] = { 0 };
		an.oneArr_0->setPlaylist(one_1_arr, 1, false);
		an.oneArr_0->setFPS(1);

		an.oneArr_1 = new animation;
		an.oneArr_1->init(220, 672, 220, 224);
		int one_2_arr[] = { 1 };
		an.oneArr_1->setPlaylist(one_2_arr, 1, false);
		an.oneArr_1->setFPS(1);

		an.oneArr_2 = new animation;
		an.oneArr_2->init(220, 672, 220, 224);
		int one_3_arr[] = { 2 };
		an.oneArr_2->setPlaylist(one_3_arr, 1, false);
		an.oneArr_2->setFPS(1);
	}
	//2장 애니 - 반복
	// 노말 - 웃음			: 2 - 2 (front / right)
	// 배틀 - 쓰러지는중		: 3 - 2 (front / back / right)
	// 노말 - 안경			: 2 - 2 (front / right)
	// 노말 - 승리 포즈		: 1 - 2 (아이템샾 등)
	{
		an.twoArrT_0 = new animation;
		an.twoArrT_0->init(440, 672, 220, 224);
		int twoT_1_arr[] = { 0,1 };
		an.twoArrT_0->setPlaylist(twoT_1_arr, 2, true);
		an.twoArrT_0->setFPS(1);

		an.twoArrT_1 = new animation;
		an.twoArrT_1->init(440, 672, 220, 224);
		int twoT_2_arr[] = { 2,3 };
		an.twoArrT_1->setPlaylist(twoT_2_arr, 2, true);
		an.twoArrT_1->setFPS(1);

		an.twoArrT_2 = new animation;
		an.twoArrT_2->init(440, 672, 220, 224);
		int twoT_3_arr[] = { 4,5 };
		an.twoArrT_2->setPlaylist(twoT_3_arr, 2, true);
		an.twoArrT_2->setFPS(1);
	}
	//2장 애니 - 반복X
	// 노말 - 승리 포즈		: 1 - 2 
	{
		an.twoArrF_0 = new animation;
		an.twoArrF_0->init(440, 672, 220, 224);
		int twoF_1_arr[] = { 0,1 };
		an.twoArrF_0->setPlaylist(twoF_1_arr, 2, false);
		an.twoArrF_0->setFPS(1);

		an.twoArrF_1 = new animation;
		an.twoArrF_1->init(440, 672, 220, 224);
		int twoF_2_arr[] = { 2,3 };
		an.twoArrF_1->setPlaylist(twoF_2_arr, 2, false);
		an.twoArrF_1->setFPS(1);

		an.twoArrF_2 = new animation;
		an.twoArrF_2->init(440, 672, 220, 224);
		int twoF_3_arr[] = { 4,5 };
		an.twoArrF_2->setPlaylist(twoF_3_arr, 2, false);
		an.twoArrF_2->setFPS(1);
	}

	//노말 / 배틀 _ 걷기 애니
	{
		an.walk_0 = new animation;
		an.walk_0->init(1320, 672, 220, 224);
		int Wa_F_arr[] = { 0,1,2,3,4,5 };
		an.walk_0->setPlaylist(Wa_F_arr, 6, true);
		an.walk_0->setFPS(1);

		an.walk_1 = new animation;
		an.walk_1->init(1320, 672, 220, 224);
		int Wa_B_arr[] = { 6,7,8,9,10,11 };
		an.walk_1->setPlaylist(Wa_B_arr, 6, true);
		an.walk_1->setFPS(1);

		an.walk_2 = new animation;
		an.walk_2->init(1320, 672, 220, 224);
		int Wa_R_arr[] = { 12,13,14,15,16,17 };
		an.walk_2->setPlaylist(Wa_R_arr, 6, true);
		an.walk_2->setFPS(1);
	}

	//노말 _ 달리기 애니
	{
		an.run_0 = new animation;
		an.run_0->init(1320, 672, 220, 224);
		int Ru_F_arr[] = { 0,1,2,3,4,5 };
		an.run_0->setPlaylist(Ru_F_arr, 6, true);
		an.run_0->setFPS(1);

		an.run_1 = new animation;
		an.run_1->init(1320, 672, 220, 224);
		int Ru_B_arr[] = { 6,7,8,9,10,11 };
		an.run_1->setPlaylist(Ru_B_arr, 6, true);
		an.run_1->setFPS(1);

		an.run_2 = new animation;
		an.run_2->init(1320, 672, 220, 224);
		int Ru_R_arr[] = { 12,13,14,15,16,17 };
		an.run_2->setPlaylist(Ru_R_arr, 6, true);
		an.run_2->setFPS(1);
	}
	//배틀 _ 준비 애니
	{
		an.Bready_0 = new animation;
		an.Bready_0->init(1320, 672, 220, 224);
		int Br_F_arr[] = { 0,1,2,3 };
		an.Bready_0->setPlaylist(Br_F_arr, 4, false);
		an.Bready_0->setFPS(1);

		an.Bready_1 = new animation;
		an.Bready_1->init(1320, 672, 220, 224);
		int Br_B_arr[] = { 6,7,8,9 };
		an.Bready_1->setPlaylist(Br_B_arr, 4, false);
		an.Bready_1->setFPS(1);

		an.Bready_2 = new animation;
		an.Bready_2->init(1320, 672, 220, 224);
		int Br_R_arr[] = { 12,13,14,15 };
		an.Bready_2->setPlaylist(Br_R_arr, 4, false);
		an.Bready_2->setFPS(1);
	}
	//배틀 _ 스탠바이 애니
	{
		an.SB_0 = new animation;
		an.SB_0->init(1320, 672, 220, 224);
		int Sb_F_arr[] = { 3,4,3,5 };
		an.SB_0->setPlaylist(Sb_F_arr, 4, true);
		an.SB_0->setFPS(1);

		an.SB_1 = new animation;
		an.SB_1->init(1320, 672, 220, 224);
		int Sb_B_arr[] = { 9,10,9,11 };
		an.SB_1->setPlaylist(Sb_B_arr, 4, true);
		an.SB_1->setFPS(1);

		an.SB_2 = new animation;
		an.SB_2->init(1320, 672, 220, 224);
		int Sb_R_arr[] = { 15,16,15,17 };
		an.SB_2->setPlaylist(Sb_R_arr, 4, true);
		an.SB_2->setFPS(1);
	}
	//배틀 _ 근거리 공격 애니
	{
		an.atk_0 = new animation;
		an.atk_0->init(660, 672, 220, 224);
		int At_F_arr[] = { 0,1,2 };
		an.atk_0->setPlaylist(At_F_arr, 3, false);
		an.atk_0->setFPS(1);

		an.atk_1 = new animation;
		an.atk_1->init(660, 672, 220, 224);
		int At_B_arr[] = { 3,4,5 };
		an.atk_1->setPlaylist(At_B_arr, 3, false);
		an.atk_1->setFPS(1);

		an.atk_2 = new animation;
		an.atk_2->init(660, 672, 220, 224);
		int At_R_arr[] = { 6,7,8 };
		an.atk_2->setPlaylist(At_R_arr, 3, false);
		an.atk_2->setFPS(1);
	}
	//배틀 _ 원거리 공격 애니
	{
		an.atk2_0 = new animation;
		an.atk2_0->init(880, 672, 220, 224);
		int At2_F_arr[] = { 0,1,2,3 };
		an.atk2_0->setPlaylist(At2_F_arr, 4, false);
		an.atk2_0->setFPS(1);

		an.atk2_1 = new animation;
		an.atk2_1->init(880, 672, 220, 224);
		int At2_B_arr[] = { 4,5,6,7 };
		an.atk2_1->setPlaylist(At2_B_arr, 4, false);
		an.atk2_1->setFPS(1);

		an.atk2_2 = new animation;
		an.atk2_2->init(880, 672, 220, 224);
		int At2_R_arr[] = { 8,9,10,11 };
		an.atk2_2->setPlaylist(At2_R_arr, 4, false);
		an.atk2_2->setFPS(1);
	}
	//배틀 _ 승리
	{
		an.Bwin = new animation;
		an.Bwin->init(1540, 224, 220, 224);
		int Bw_arr[] = { 0,1,2,3,4,4,5,6 };
		an.Bwin->setPlaylist(Bw_arr, 8);
		an.Bwin->setFPS(1);
	}
	//노말 _ 못달린다
	{
		an.cantRun_0 = new animation;
		an.cantRun_0->init(660, 672, 220, 224);
		int Ca_F_arr[] = { 0,1,2,1 };
		an.cantRun_0->setPlaylist(Ca_F_arr, 4, true);
		an.cantRun_0->setFPS(1);

		an.cantRun_1 = new animation;
		an.cantRun_1->init(660, 672, 220, 224);
		int Ca_B_arr[] = { 3,4,5,4 };
		an.cantRun_1->setPlaylist(Ca_B_arr, 4, true);
		an.cantRun_1->setFPS(1);

		an.cantRun_2 = new animation;
		an.cantRun_2->init(660, 672, 220, 224);
		int Ca_R_arr[] = { 6,7,8,7 };
		an.cantRun_2->setPlaylist(Ca_R_arr, 4, true);
		an.cantRun_2->setFPS(1);
	}
	//노말 _ 사다리	
	{
		an.ladder = new animation;
		an.ladder->init(880, 224, 220, 224);
		an.ladder->setDefPlaylist();
		an.ladder->setFPS(1);
	}
}

void lucca::imgSwitch()
{
	if (_chaSet.img != im.Bready)
	{
		an.Bready_0->stop();
		an.Bready_1->stop();
		an.Bready_2->stop();
		an.SB_0->stop();
		an.SB_1->stop();
		an.SB_2->stop();
	}
	if (_chaSet.img != im.atk)
	{
		an.atk_0->stop();
		an.atk_1->stop();
		an.atk_2->stop();
	}
	if (_chaSet.img != im.Bwin)
	{
		an.Bwin->stop();
	}
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
			if (!Cleft)Cleft = false;
			_chaSet.ani = an.Bready_2;
			break;
		case 3:
			if (Cleft)Cleft = true;
			_chaSet.ani = an.Bready_2;
			break;
		}
		_chaSet.ani->frameUpdate(TIME->getElapsedTime() * 4);
		break;

	case BATTLE_READY:
		_chaSet.img = im.Bready;
		if (Cc != 0)Cc = 0;
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

	//근거리 공격시 움직이는 모션이 있긴 한데, 너무 짧은 거리라 확실하지 않아보임, 의논 후 결정

	/*case BATTLE_MOVE:
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
	case BATTLE_RUSH:
		//_chaSet.img = im.rush;

		if (Cc != 0)Cc = 0;
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
	case BATTLE_RETURN:
		//_chaSet.img = im.rush;

		if (Cc != 0)Cc = 0;

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
			if (!Cleft)Cleft = false;
			_chaSet.ani = an.oneArr_2;
			break;
		case 3:
			if (Cleft)Cleft = true;
			_chaSet.ani = an.oneArr_2;
			break;
		}
		break;*/
		
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
		_chaSet.ani = an.oneArr_0;
		break;

	case BATTLE_WIN:

		_chaSet.img = im.Bwin;
		_chaSet.ani = an.Bwin;

		_chaSet.ani->frameUpdate(TIME->getElapsedTime() * 8);
		break;

	}
	_chaSet.rc = MakeRct(_chaSet.pt.x - ((220) / 2), _chaSet.pt.y - 192, (220), (224));

}

void lucca::keySetting()
{
	if (KEY->press(VK_LEFT) && 0 < _chaSet.pt.x)
	{
		if (_T != 3)_T = 3;
		if (_run)
		{
			_chaSet.state = NORMAL_RUN;
			_chaSet.pt.x -= 8;
		}
		if (!_run)
		{
			_chaSet.state = NORMAL_WALK;
			_chaSet.pt.x -= 5;
		}
	}
	else if (KEY->up(VK_LEFT))
	{
		if (_chaSet.state != NORMAL_IDLE)_chaSet.state = NORMAL_IDLE;
	}
	if (KEY->press(VK_RIGHT))
	{
		if (_T != 2)_T = 2;
		if (_run)
		{
			_chaSet.state = NORMAL_RUN;
			_chaSet.pt.x += 8;
		}
		if (!_run)
		{
			_chaSet.state = NORMAL_WALK;
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
			_chaSet.state = NORMAL_RUN;
			_chaSet.pt.y -= 8;
		}
		if (!_run)
		{
			_chaSet.pt.y -= 5;
			_chaSet.state = NORMAL_WALK;
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
			_chaSet.pt.y += 8;
			_chaSet.state = NORMAL_RUN;
		}
		if (!_run)
		{
			_chaSet.pt.y += 5;
			_chaSet.state = NORMAL_WALK;
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

		_chaSet.state = BATTLE_ATK;
	}

}

void lucca::battleSwitch()
{
	switch (_chaSet.state)
	{
	case GETTING_READY:

		if (_chaSet.ani->getCurrPlaylistIdx() != 0 && _chaSet.ani->isPlay() == FALSE)_chaSet.state = BATTLE_READY;
		else if (_chaSet.ani->getCurrPlaylistIdx() == 0 && _chaSet.ani->isPlay() == FALSE)_chaSet.ani->start();
		break;
	case BATTLE_READY:

		if (_chaSet.ani->isPlay() == FALSE)_chaSet.ani->start();

		break;
		// 근거리 공격을 위한 이동은 의논 후 결정
	/*	case BATTLE_MOVE:

		break;

	case BATTLE_RUSH:
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
		break;

	case BATTLE_RETURN:
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

		break;*/

	case BATTLE_ATK:
		//근 / 원거리 공격 구분 필요
		//공격시 이미지에 맞춰 적의 이미지를 설정, 효과음을 설정한다

		//공격 모션이 시작되어야 할 때 시작 / 끝났을 때 돌아가는 것을 설정한다
		if (_chaSet.ani->getCurrPlaylistIdx() != 0 && _chaSet.ani->isPlay() == FALSE)
		{
			_chaSet.ani->stop();
			_chaSet.state = BATTLE_READY;
		}
		else if (_chaSet.ani->getCurrPlaylistIdx() == 0 && _chaSet.ani->isPlay() == FALSE) { _chaSet.ani->start(); }

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
		//if (_chaSet.ani->getCurrPlaylistIdx() != 0 && _chaSet.ani->isPlay() == FALSE)_chaSet.state = NORMAL_READY;
		if (_chaSet.ani->getCurrPlaylistIdx() != 0 && _chaSet.ani->isPlay() == FALSE)_chaSet.state = NORMAL_IDLE;
		else if (_chaSet.ani->getCurrPlaylistIdx() == 0 && _chaSet.ani->isPlay() == FALSE)_chaSet.ani->start();

		break;
	}
}
