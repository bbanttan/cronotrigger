#include "stdafx.h"
#include "crono.h"

HRESULT crono::init()
{
	_id = 0;

	imgSetting();//이미지
	_chaSet.img = im.idle;//이미지는 맨처음 idle로
	_chaSet.Bimg = im.hit;
	aniSetting();
	_chaSet.ani = an.idle_0;
	_chaSet.ani->start();
	Cleft = _run = false;
	Cc = _T = 0;

	_chaSet.pt = { WINW / 2, WINH / 2 };

	_rcW = 220;
	_rcH = 224;
	_chaSet.rc = MakeRct(_chaSet.pt.x - (_rcW / 2), _chaSet.pt.y - (_rcH - 32), _rcW, _rcH);

	_status.Lv = 1;					//크로노 초기 레벨
	_status.hp = _status.maxHp = 70;	//크로노 초기 현재/최대HP
	_status.mp = _status.maxMp = 8;	//크로노 초기 현재/최대HP
	_status.power = 5;				//크로노 초기 힘
	_status.hit = 12;				//크로노 초기 명중
	_status.magic = 8;				//크로노 초기 마력
	_status.speed = 12;				//크로노 초기 속도
	_status.evasion = 5;				//크로노 초기 회피
	_status.stamina = 8;				//크로노 초기 체력
	_status.Mdef = 2;				//크로노 초기 마법방어력


	dia = 20;
	return S_OK;
}

void crono::release()
{
	SAFE_DEL(an.oneArr_0);  SAFE_DEL(an.oneArr_1);  SAFE_DEL(an.oneArr_2);	//1장 애니
	SAFE_DEL(an.twoArrT_0); SAFE_DEL(an.twoArrT_1); SAFE_DEL(an.twoArrT_2);	//2장 반복 애니
	SAFE_DEL(an.twoArrF_0); SAFE_DEL(an.twoArrF_1); SAFE_DEL(an.twoArrF_2);	//2장 애니
	SAFE_DEL(an.idle_0); SAFE_DEL(an.idle_1); SAFE_DEL(an.idle_2);			//노말 _ 보통 애니
	SAFE_DEL(an.walk_0); SAFE_DEL(an.walk_1); SAFE_DEL(an.walk_2);			//노말 / 배틀 _ 걷기 애니
	SAFE_DEL(an.run_0); SAFE_DEL(an.run_1); SAFE_DEL(an.run_2);				//노말 _ 달리기 애니
	SAFE_DEL(an.Bready_0); SAFE_DEL(an.Bready_1); SAFE_DEL(an.Bready_3);	//배틀 _ 준비 애니
	SAFE_DEL(an.SB_0); SAFE_DEL(an.SB_1); SAFE_DEL(an.SB_3);				//배틀 _ 스탠바이 애니
	SAFE_DEL(an.atk_0); SAFE_DEL(an.atk_1); SAFE_DEL(an.atk_2);				//배틀 _ 공격 애니
	SAFE_DEL(an.cri_0); SAFE_DEL(an.cri_1); SAFE_DEL(an.cri_2);				//배틀 _ 크리티컬 애니
	SAFE_DEL(an.spin);														//배틀 _ 스핀 애니
	SAFE_DEL(an.Bwin_0); SAFE_DEL(an.Bwin_1); SAFE_DEL(an.Bwin_3);			//배틀 _ 승리
	SAFE_DEL(an.cantRun_0); SAFE_DEL(an.cantRun_1); SAFE_DEL(an.cantRun_2);	//노말 _ 못달린다
	SAFE_DEL(an.ladder);													//노말 _ 사다리	


}

void crono::update()
{
	//keySetting();
	if (KEY->down(VK_RETURN) && !_isChrUnmovable)
	{
		Astar* astar;
		astar = new Astar;

		Path = astar->pathFinder({ _chaSet.pt.x / TILESIZE, _chaSet.pt.y / TILESIZE }, { 8, 12 });
		astar->release();
		delete astar;
		a = b = rich = 0;
	}

	if (Path.size() != 0)
	{

		if (rich <= 5.0f)
		{

			a = (float)Path[Path.size() - 1].x;
			b = (float)Path[Path.size() - 1].y;
			Path.pop_back();

			a = (a * TILESIZE) + TILESIZE / 2;
			b = (b * TILESIZE) + TILESIZE / 2;

		}

		int muonX = (int)(cosf(atan2f(b - (float)_chaSet.pt.y, a - (float)_chaSet.pt.x)) * 5);
		int muonY = (int)(-sinf(-atan2f(b - (float)_chaSet.pt.y, a - (float)_chaSet.pt.x)) * 5);

		_chaSet.pt.x += muonX;
		_chaSet.pt.y += muonY;

		_chaSet.state = NORMAL_WALK;
		if (muonX < 0) _T = 3;
		if (muonX > 0) _T = 2;
		if (muonY < 0) _T = 1;
		if (muonY > 0) _T = 0;

		rich = Distance((float)_chaSet.pt.x, (float)_chaSet.pt.y, a, b);

		if (Path.size() == 0)
		{
			_chaSet.state = NORMAL_IDLE;
			_chaSet.ani->stop();
		}

	}
	imgSwitch();
	battleSwitch();
}

void crono::render()
{
	if (KEY->isToggledOn(VK_TAB))
	{
		char str[256];
		sprintf_s(str, "현재 플레이 인덱스 : %d", _chaSet.ani->getCurrPlaylistIdx());
		TextOut(getMemDC(), WINW - 200, 0, str, static_cast<int>(strlen(str)));
		sprintf_s(str, "캐릭터 포인트 : %d, %d", _chaSet.pt.x, _chaSet.pt.y);
		TextOut(getMemDC(), WINW - 200, 20, str, static_cast<int>(strlen(str)));
		sprintf_s(str, "어택 포인트 : %d, %d", _chaSet.atk.x, _chaSet.atk.y);
		TextOut(getMemDC(), WINW - 200, 40, str, static_cast<int>(strlen(str)));
		sprintf_s(str, "시작 포인트 : %d, %d", _chaSet.atkS.x, _chaSet.atkS.y);
		TextOut(getMemDC(), WINW - 200, 60, str, static_cast<int>(strlen(str)));
		sprintf_s(str, "각도 = %f", _chaSet.angle);
		TextOut(getMemDC(), WINW - 200, 80, str, static_cast<int>(strlen(str)));

		//DrawRct(getMemDC(), _chaSet.rc);
		DrawElpC(getMemDC(), _chaSet.pt.x, _chaSet.pt.y, dia, dia);
		DrawElpC(getMemDC(), _chaSet.atk.x, _chaSet.atk.y, dia, dia);
		DrawElpC(getMemDC(), _chaSet.atkS.x, _chaSet.atkS.y, dia, dia);
	}

	if (!Cleft)		IMG->animRenderZ(static_cast<int>(_chaSet.pt.y), _chaSet.img, getMemDC(), _chaSet.rc.left, _chaSet.rc.top, _chaSet.ani);
	else if (Cleft)	IMG->animRenderHZ(static_cast<int>(_chaSet.pt.y), _chaSet.img, getMemDC(), _chaSet.rc.left, _chaSet.rc.top, _chaSet.ani);
	if (!Cleft	  &&white())	IMG->animRenderZ(static_cast<int>(_chaSet.pt.y), _chaSet.Bimg, getMemDC(), _chaSet.rc.left, _chaSet.rc.top, _chaSet.ani);
	else if (Cleft&&white())	IMG->animRenderHZ(static_cast<int>(_chaSet.pt.y), _chaSet.Bimg, getMemDC(), _chaSet.rc.left, _chaSet.rc.top, _chaSet.ani);
	_chaSet.Bimg->changeAllColors(RGB(255, 255, 255), RGB(255, 0, 255));

}

void crono::imgSetting()
{
	im.idle = IMG->addF("크로노-보통", "res/images/character/chrono/idle.bmp", 660, 672, 3, 3, true, RGB(255, 0, 255));
	im.walk = IMG->addF("크로노-걷기", "res/images/character/chrono/walk.bmp", 1320, 672, 6, 3, true, RGB(255, 0, 255));
	im.run = IMG->addF("크로노-달리기", "res/images/character/chrono/run.bmp", 1320, 672, 6, 3, true, RGB(255, 0, 255));
	im.Bready = IMG->addF("크로노-배틀레디", "res/images/character/chrono/Bready.bmp", 1320, 672, 6, 3, true, RGB(255, 0, 255));
	im.rush = IMG->addF("크로노-달려들기", "res/images/character/chrono/rush.bmp", 220, 672, 1, 3, true, RGB(255, 0, 255));
	im.atk = IMG->addF("크로노-공격", "res/images/character/chrono/atk.bmp", 1100, 672, 5, 3, true, RGB(255, 0, 255));
	im.Bwin = IMG->addF("크로노-배틀승리", "res/images/character/chrono/Bwin.bmp", 660, 896, 3, 4, true, RGB(255, 0, 255));
	im.cantRun = IMG->addF("크로노-못달림", "res/images/character/chrono/cantRun.bmp", 660, 672, 3, 3, true, RGB(255, 0, 255));
	im.down = IMG->addF("크로노-쓰러짐", "res/images/character/chrono/down.bmp", 220, 224, 1, 1, true, RGB(255, 0, 255));
	im.downs = IMG->addF("크로노-쓰러질때", "res/images/character/chrono/downs.bmp", 440, 672, 2, 3, true, RGB(255, 0, 255));
	im.drink = IMG->addF("크로노-마심", "res/images/character/chrono/drink.bmp", 440, 224, 2, 1, true, RGB(255, 0, 255));
	im.handsUp = IMG->addF("크로노-손들기", "res/images/character/chrono/handsUp.bmp", 220, 448, 1, 2, true, RGB(255, 0, 255));
	im.hit = IMG->addF("크로노-피격", "res/images/character/chrono/hit.bmp", 220, 672, 1, 3, true, RGB(255, 0, 255));
	im.hit2 = IMG->addF("크로노-피격2", "res/images/character/chrono/hit2.bmp", 220, 448, 1, 2, true, RGB(255, 0, 255));
	im.ladder = IMG->addF("크로노-사다리", "res/images/character/chrono/labber.bmp", 880, 224, 4, 1, true, RGB(255, 0, 255));
	im.spin = IMG->addF("크로노-회전", "res/images/character/chrono/spin.bmp", 880, 224, 4, 1, true, RGB(255, 0, 255));
	im.tied = IMG->addF("크로노-묶임", "res/images/character/chrono/tied.bmp", 440, 672, 2, 3, true, RGB(255, 0, 255));
	im.what = IMG->addF("크로노-궁금", "res/images/character/chrono/what.bmp", 220, 672, 1, 3, true, RGB(255, 0, 255));
	im.win = IMG->addF("크로노-승리", "res/images/character/chrono/win.bmp", 440, 224, 2, 1, true, RGB(255, 0, 255));
}

void crono::aniSetting()
{
	//1장 애니
	// 배틀 - 피격		: 3 (front / back / right)
	// 배틀 - 피격2		: 2 (front / right)
	// 배틀 - 쓰러짐		: 1
	// 배틀 - 달려들기	: 3 (front / back / right)
	// 노말 - ?			: 3 (front / back / right)
	// 노말 - 손들기		: 2	(front / back)
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
	// 배틀 - 쓰러지는중		: 3 - 2 (front / back / right)
	// 노말 - 승리 포즈		: 1 - 2 (아이템샾 등)
	// 노말 - 마시기			: 1 - 2
	// 노말 - 묶임(이벤트)	: 3 - 2 (front / back / right)
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

	//노말 _ 보통 애니
	{
		an.idle_0 = new animation;
		an.idle_0->init(660, 672, 220, 224);
		int Id_F_arr[] = { 0,1,2,1,0 };
		an.idle_0->setPlaylist(Id_F_arr, 5, false);
		an.idle_0->setFPS(1);

		an.idle_1 = new animation;
		an.idle_1->init(660, 672, 220, 224);
		int Id_B_arr[] = { 3 };
		an.idle_1->setPlaylist(Id_B_arr, 1, false);
		an.idle_1->setFPS(1);

		an.idle_2 = new animation;
		an.idle_2->init(660, 672, 220, 224);
		int Id_R_arr[] = { 6,7,8,7,6 };
		an.idle_2->setPlaylist(Id_R_arr, 5, false);
		an.idle_2->setFPS(1);
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
		int Br_F_arr[] = { 0,1,2,3,4,5 };
		an.Bready_0->setPlaylist(Br_F_arr, 6, false);
		an.Bready_0->setFPS(1);

		an.Bready_1 = new animation;
		an.Bready_1->init(1320, 672, 220, 224);
		int Br_B_arr[] = { 6,7,8,9,10,11 };
		an.Bready_1->setPlaylist(Br_B_arr, 6, false);
		an.Bready_1->setFPS(1);

		an.Bready_3 = new animation;
		an.Bready_3->init(1320, 672, 220, 224);
		int Br_R_arr[] = { 12,13,14,15,16,17 };
		an.Bready_3->setPlaylist(Br_R_arr, 6, false);
		an.Bready_3->setFPS(1);
	}
	//배틀 _ 스탠바이 애니
	{
		an.SB_0 = new animation;
		an.SB_0->init(1320, 672, 220, 224);
		int Sb_F_arr[] = { 4,3,4,5 };
		an.SB_0->setPlaylist(Sb_F_arr, 4, true);
		an.SB_0->setFPS(1);

		an.SB_1 = new animation;
		an.SB_1->init(1320, 672, 220, 224);
		int Sb_B_arr[] = { 10,9,10,11 };
		an.SB_1->setPlaylist(Sb_B_arr, 4, true);
		an.SB_1->setFPS(1);

		an.SB_3 = new animation;
		an.SB_3->init(1320, 672, 220, 224);
		int Sb_L_arr[] = { 16,15,16,17 };
		an.SB_3->setPlaylist(Sb_L_arr, 4, true);
		an.SB_3->setFPS(1);
	}
	//배틀 _ 공격 애니
	{
		an.atk_0 = new animation;
		an.atk_0->init(1100, 672, 220, 224);
		int At_F_arr[] = { 0,1,2 };
		an.atk_0->setPlaylist(At_F_arr, 3, false);
		an.atk_0->setFPS(1);

		an.atk_1 = new animation;
		an.atk_1->init(1100, 672, 220, 224);
		int At_B_arr[] = { 5,6,7 };
		an.atk_1->setPlaylist(At_B_arr, 3, false);
		an.atk_1->setFPS(1);

		an.atk_2 = new animation;
		an.atk_2->init(1100, 672, 220, 224);
		int At_R_arr[] = { 10,11,12 };
		an.atk_2->setPlaylist(At_R_arr, 3, false);
		an.atk_2->setFPS(1);
	}
	//배틀 _ 크리티컬 애니
	{
		an.cri_0 = new animation;
		an.cri_0->init(1100, 672, 220, 224);
		int Cr_F_arr[] = { 0,1,2,3,4 };
		an.cri_0->setPlaylist(Cr_F_arr, 5, false);
		an.cri_0->setFPS(1);

		an.cri_1 = new animation;
		an.cri_1->init(1100, 672, 220, 224);
		int Cr_B_arr[] = { 5,6,7,8,9 };
		an.cri_1->setPlaylist(Cr_B_arr, 5, false);
		an.cri_1->setFPS(1);

		an.cri_2 = new animation;
		an.cri_2->init(1100, 672, 220, 224);
		int Cr_R_arr[] = { 10,11,12,13,14 };
		an.cri_2->setPlaylist(Cr_R_arr, 5, false);
		an.cri_2->setFPS(1);
	}
	//배틀 _ 스핀 애니
	{
		an.spin = new animation;
		an.spin->init(880, 224, 220, 224);
		an.spin->setDefPlaylist(false, true);
		an.spin->setFPS(1);
	}
	//배틀 _ 승리
	{
		an.Bwin_0 = new animation;
		an.Bwin_0->init(660, 896, 220, 224);
		int Bw_F_arr[] = { 0,1,2,9,10 };
		an.Bwin_0->setPlaylist(Bw_F_arr, 5, false);
		an.Bwin_0->setFPS(1);

		an.Bwin_1 = new animation;
		an.Bwin_1->init(660, 896, 220, 224);
		int Bw_B_arr[] = { 3,4,5,9,10 };
		an.Bwin_1->setPlaylist(Bw_B_arr, 5, false);
		an.Bwin_1->setFPS(1);

		an.Bwin_3 = new animation;
		an.Bwin_3->init(660, 896, 220, 224);
		int Bw_L_arr[] = { 6,7,8,9,10 };
		an.Bwin_3->setPlaylist(Bw_L_arr, 5, false);
		an.Bwin_3->setFPS(1);
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

void crono::imgSwitch()
{
	switch (_chaSet.state)
	{
	case NORMAL_IDLE:
		_chaSet.img = im.idle;

		switch (_T)
		{
		case 0:
			_chaSet.ani = an.idle_0;
			break;
		case 1:
			_chaSet.ani = an.idle_1;
			break;
		case 2:
			if (Cleft)Cleft = false;
			_chaSet.ani = an.idle_2;
			break;
		case 3:
			if (!Cleft)Cleft = true;
			_chaSet.ani = an.idle_2;
			break;
		}

		_chaSet.ani->frameUpdate(TIME->getElapsedTime() * 4);
		if (Cc > 50 && _chaSet.ani->isPlay() == FALSE)
		{
			_chaSet.ani->start();
			Cc = 0;
		}
		if (_chaSet.ani->isPlay() == FALSE && _T != 1)Cc++;
		else if (_chaSet.ani->getCurrPlaylistIdx() != 4 && _chaSet.ani->getCurrPlaylistIdx() != 0)Cc = 0;

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
			_chaSet.ani = an.Bready_3;
			break;
		case 3:
			if (Cleft)Cleft = false;
			_chaSet.ani = an.Bready_3;
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
			if (!Cleft)Cleft = true;
			_chaSet.ani = an.SB_3;
			break;
		case 3:
			if (Cleft)Cleft = false;
			_chaSet.ani = an.SB_3;
			break;
		}
		_chaSet.ani->frameUpdate(TIME->getElapsedTime() * 4);

		break;

	case BATTLE_MOVE:
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
		_chaSet.img = im.rush;

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
			_chaSet.ani = an.Bwin_3;
			break;
		case 3:
			if (!Cleft)Cleft = false;
			_chaSet.ani = an.Bwin_3;
			break;
		}
		_chaSet.ani->frameUpdate(TIME->getElapsedTime() * 4);
		break;

	}

	_chaSet.rc = MakeRct(_chaSet.pt.x - (_rcW / 2), _chaSet.pt.y - (_rcH - 32), _rcW, _rcH);

}

void crono::keySetting()
{
	if (_isChrUnmovable) return;
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
		if (_chaSet.state != NORMAL_IDLE) { _chaSet.state = NORMAL_IDLE;  _chaSet.ani->stop(); }
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
		if (_chaSet.state != NORMAL_IDLE) { _chaSet.state = NORMAL_IDLE;  _chaSet.ani->stop(); }
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
			_chaSet.state = NORMAL_WALK;
			_chaSet.pt.y -= 5;
		}
	}
	else if (KEY->up(VK_UP))
	{
		if (_chaSet.state != NORMAL_IDLE) { _chaSet.state = NORMAL_IDLE;  _chaSet.ani->stop(); }
	}
	if (KEY->press(VK_DOWN))
	{
		if (_T != 0)_T = 0;
		if (_run)
		{
			_chaSet.state = NORMAL_RUN;
			_chaSet.pt.y += 8;
		}
		if (!_run)
		{
			_chaSet.state = NORMAL_WALK;
			_chaSet.pt.y += 5;
		}
	}
	else if (KEY->up(VK_DOWN))
	{
		if (_chaSet.state != NORMAL_IDLE) { _chaSet.state = NORMAL_IDLE;  _chaSet.ani->stop(); }
	}
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

void crono::battleSwitch()
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
		//멈춤 상태라면 start
		if (_chaSet.ani->isPlay() == FALSE)_chaSet.ani->start();
		break;
	case BATTLE_MOVE:
		//적의 방향으로 A*를 이용해서 움직인다

		break;
	case BATTLE_RUSH:
		//적의 방향으로 Angle을 통해 움직인다

		_chaSet.angle = Angle(static_cast<float> (_chaSet.pt.x), static_cast<float>(_chaSet.pt.y), static_cast<float>(_chaSet.atk.x), static_cast<float>(_chaSet.atk.y));
		_chaSet.Dis = Distance(static_cast<float>(_chaSet.pt.x), static_cast<float>(_chaSet.pt.y), static_cast<float>(_chaSet.atk.x), static_cast<float>(_chaSet.atk.y));

		_chaSet.pt.x += static_cast<int>(cosf(_chaSet.angle) * 15);
		_chaSet.pt.y += -static_cast<int>(sinf(_chaSet.angle) * 15);
		//스킬과 공격을 결정짓는 인트변수 등을 설정해야 한다
		if (_chaSet.Dis <= dia) { _chaSet.state = BATTLE_ATK; }

		break;
	case BATTLE_ATK:
		//공격시 이미지에 맞춰 적의 이미지를 설정, 효과음을 설정한다

		//공격 모션이 시작되어야 할 때 시작 / 끝났을 때 돌아가는 것을 설정한다
		if (_chaSet.ani->getCurrPlaylistIdx() == 0 && _chaSet.ani->isPlay() == FALSE) { _chaSet.ani->start(); }
		else if (_chaSet.ani->getCurrPlaylistIdx() != 0 && _chaSet.ani->isPlay() == FALSE)
		{

			_chaSet.state = BATTLE_RETURN;
			_chaSet.ani->stop();
		}

		break;
	case BATTLE_RETURN:
		//시작 위치로 Angle을 통해 움직인다
		_chaSet.angle = Angle(static_cast<float> (_chaSet.atkS.x), static_cast<float>(_chaSet.atkS.y), static_cast<float>(_chaSet.pt.x), static_cast<float>(_chaSet.pt.y));
		_chaSet.Dis = Distance(static_cast<float>(_chaSet.atkS.x), static_cast<float>(_chaSet.atkS.y), static_cast<float>(_chaSet.pt.x), static_cast<float>(_chaSet.pt.y));

		_chaSet.pt.x -= static_cast<int>(cosf(_chaSet.angle) * 15);
		_chaSet.pt.y -= -static_cast<int>(sinf(_chaSet.angle) * 15);
		//if (dia >= Distance(static_cast<float>(_chaSet.pt.x), static_cast<float>(_chaSet.pt.y), static_cast<float>(_chaSet.atkS.x), static_cast<float>(_chaSet.atkS.y))) { _chaSet.state = BATTLE_READY; }

		if (dia >= _chaSet.Dis) { _chaSet.state = BATTLE_READY; }
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
