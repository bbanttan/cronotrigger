#pragma once

// fnt 바이너리 파일 분석용 구조체
// (DWORD, WORD, WORD, WORD, WORD, short, short, short, BYTE, BYTE가 한 단위이다.)
// (한 단위에서 각 값은 id, x, y, width, height, xOffset, yOffset, xAdvance, page, chnl을 의미한다.)
typedef struct tagCharacterDescriptor
{
	int x, y; // 문자 그림 좌상단 위치
	int width, height; // 문자 그림 너비, 높이

	// 생성자(ctor) 정의
	tagCharacterDescriptor(): x(0), y(0), width(0), height(0) {}
} CD;

class fontParser
{
	private:
	map<int, vector<CD>*> fontInfoList;

	public:
	fontParser();
	~fontParser();

	void parseFnt(int fontIdx, string fileName);

	vector<CD>* getFontInfo(int fontIdx) { return fontInfoList.find(fontIdx)->second; }
};

