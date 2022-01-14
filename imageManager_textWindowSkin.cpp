#include "stdafx.h"
#include "imageManager.h"

void imageManager::setWindowSkin(string tilesetStrKey, string skinStrKey, int width, int height, int destX, int destY)
{
	if (width < 96 || height < 96) return;
	image* img = find(tilesetStrKey);
	image* img2 = find(skinStrKey);
	if (!img || !img2) return;
	HDC hTempDC = CreateCompatibleDC(img->getMemDC());
	BITMAPINFO bI = { 0 };
	DWORD* bitmapData = nullptr;
	bI.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bI.bmiHeader.biBitCount = 32;
	bI.bmiHeader.biWidth = width;
	bI.bmiHeader.biHeight = height;
	bI.bmiHeader.biCompression = BI_RGB;
	bI.bmiHeader.biPlanes = 1;
	HBITMAP hTempBitmap = CreateDIBSection(hTempDC, &bI, DIB_RGB_COLORS, reinterpret_cast<void**>(&bitmapData), 0, 0);
	if (hTempBitmap == nullptr)
	{
		DeleteDC(hTempDC);
		return;
	}
	HBITMAP hTempOBitmap = (HBITMAP)SelectObject(hTempDC, hTempBitmap);

	// 명암을 주기 전에 임시 DC 비트맵에 글 출력 창 스킨을 그린다.
	img->loopRenderP(hTempDC, &RECT({32, 32, width, height}), &RECT({128, _textWindowSkin * 128, 256, _textWindowSkin * 128 + 128}), 0, 0); // 바탕
	img->loopRenderP(hTempDC, &RECT({32, 0, width, 32}), &RECT({32, _textWindowSkin * 128, 96, _textWindowSkin * 128 + 32}), 0, 0); // 상단
	img->loopRenderP(hTempDC, &RECT({32, height - 32, width, height}), &RECT({32, _textWindowSkin * 128 + 96, 96, _textWindowSkin * 128 + 128}), 0, 0); // 하단
	img->loopRenderP(hTempDC, &RECT({0, 32, 32, height}), &RECT({0, _textWindowSkin * 128 + 32, 32, _textWindowSkin * 128 + 96}), 0, 0); // 좌단
	img->loopRenderP(hTempDC, &RECT({width - 32, 32, width, height }), &RECT({96, _textWindowSkin * 128 + 32, 128, _textWindowSkin * 128 + 96}), 0, 0); // 우단
	img->render(hTempDC, 0, 0, 0, _textWindowSkin * 128, 32, 32);  // 좌상단 모서리
	img->render(hTempDC, width - 32, 0, 96, _textWindowSkin * 128, 32, 32); // 우상단 모서리
	img->render(hTempDC, 0, height - 32, 0, _textWindowSkin * 128 + 96, 32, 32); // 좌하단 모서리
	img->render(hTempDC, width - 32, height - 32, 96, _textWindowSkin * 128 + 96, 32, 32); // 우하단 모서리

	int sectionHeight = (height + 15) / 16; // 올림
	int sectionArea = width * sectionHeight;
	int bitmapArea = width * height;
	int sectionAreaOffset = 16 * sectionArea - bitmapArea; // 이 값이 0보다 크면 이 값에 해당하는 영역만큼은 아래 반복문에서 무시된다. 높이가 16으로 나누어 떨어지지 않을 때 제일 밝은 영역을 제일 어두운 영역보다 더 크게 하려면 필요한 변수이다.

	for (int sectionNum = 0; sectionNum < 16; ++sectionNum)
	{
		if (sectionNum == 7) continue;
		signed char valueChange = static_cast<signed char>(8 * (static_cast<int>(sectionNum) - 7));

		// bI.bmiHeader.biHeight를 음수로 설정하지 않아 높이 방향으로는 거꾸로 연산하여야 하므로 아래처럼 sectionAreaOffset을 감하여야 한다.
		// i가 음수가 될 수 있으므로 size_t를 사용하지 않는다.
		for (int i = sectionNum * sectionArea - sectionAreaOffset; i < (sectionNum + 1) * sectionArea - sectionAreaOffset; ++i)
		{
			if (i < 0) continue;
			BYTE* currPixelData = reinterpret_cast<BYTE*>(&bitmapData[i]);
			currPixelData[0] = min(max(currPixelData[0] + valueChange, 0x00), 0xFF);
			currPixelData[1] = min(max(currPixelData[1] + valueChange, 0x00), 0xFF);
			currPixelData[2] = min(max(currPixelData[2] + valueChange, 0x00), 0xFF);
		}
	}

	BitBlt(img2->getMemDC(), 0, 0, width, height, hTempDC, 0, 0, SRCCOPY);
	DeleteObject(SelectObject(hTempDC, hTempOBitmap));
	DeleteDC(hTempDC);
}

void imageManager::setAllWindowSkins()
{
	IMG->setWindowSkin("글 출력 창 스킨 타일셋", "대사 출력 창 스킨", 1024, 320);
	IMG->setWindowSkin("글 출력 창 스킨 타일셋", "전투 메시지 창 스킨", 1024, 96);
	IMG->setWindowSkin("글 출력 창 스킨 타일셋", "설정 메시지 창 스킨", 898, 128);
	IMG->setWindowSkin("글 출력 창 스킨 타일셋", "전투 창 스킨", 225, 192);
	IMG->setWindowSkin("글 출력 창 스킨 타일셋", "전투 스탯 창 스킨", 640, 192);
	IMG->setWindowSkin("글 출력 창 스킨 타일셋", "전투 적 목록 창 스킨", WINW / 2, 192);
	IMG->setWindowSkin("글 출력 창 스킨 타일셋", "전투 모드 선택 메시지 창 스킨", 896, 316);
	IMG->setWindowSkin("글 출력 창 스킨 타일셋", "이름 입력 창 1 스킨", 572, 252);
	IMG->setWindowSkin("글 출력 창 스킨 타일셋", "이름 입력 창 2 스킨", 768, 572);
	IMG->setWindowSkin("글 출력 창 스킨 타일셋", "이름 입력 안내 창 스킨", 896, 192);


	//카테고리창
	IMG->setWindowSkin("글 출력 창 스킨 타일셋", "카테고리선택", 512, 192);
	//캐릭터창
	IMG->setWindowSkin("글 출력 창 스킨 타일셋", "캐릭터창", 384, 192);//캐릭터 hp mp
	IMG->setWindowSkin("글 출력 창 스킨 타일셋", "장비창", 384, 128); //아이템 장비 카테고리 출력
	IMG->setWindowSkin("글 출력 창 스킨 타일셋", "목록", 384, 448);
	IMG->setWindowSkin("글 출력 창 스킨 타일셋", "0번 스텟창 캐릭터", 512, 768);
	//아이템창
	IMG->setWindowSkin("글 출력 창 스킨 타일셋", "아이템창1", 896, 128);
	IMG->setWindowSkin("글 출력 창 스킨 타일셋", "소지아이템창", 896, 512);
	//스킬창
	IMG->setWindowSkin("글 출력 창 스킨 타일셋", "스킬창1", 448, 576);//왼쪽위
	IMG->setWindowSkin("글 출력 창 스킨 타일셋", "스킬창2", 448, 128);//오른쪽위
	IMG->setWindowSkin("글 출력 창 스킨 타일셋", "스킬창3", 448, 448);//오른쪽아래
	IMG->setWindowSkin("글 출력 창 스킨 타일셋", "스킬창4", 896, 192);//아래
	//설정창
	IMG->setWindowSkin("글 출력 창 스킨 타일셋", "설정창1", 384, 640);//왼쪽
	IMG->setWindowSkin("글 출력 창 스킨 타일셋", "설정창2", 512, 640);//오른쪽
	IMG->setWindowSkin("글 출력 창 스킨 타일셋", "설정창3", 896, 128);//아래
	//위치바꾸는창
	IMG->setWindowSkin("글 출력 창 스킨 타일셋", "위치창", 512, 192);//1번째
	//세이브로드
	IMG->setWindowSkin("글 출력 창 스킨 타일셋", "세이브번호", 128, 128);
	IMG->setWindowSkin("글 출력 창 스킨 타일셋", "세이브창", 768, 128);
	IMG->setWindowSkin("글 출력 창 스킨 타일셋", "내용물1", 6*64, 5*64);
	IMG->setWindowSkin("글 출력 창 스킨 타일셋", "내용물2", 8*64, 5 * 64);
}
