#pragma once
#include "animation.h"

enum class IMAGE_LOAD_TYPE
{
	LOAD_RESOURCE,
	LOAD_FILE,
	LOAD_EMPTY
};

typedef struct tagImageInfo
{
	DWORD resID = 0;
	HDC hMemDC = nullptr;
	HBITMAP hBit = nullptr;
	HBITMAP hOBit = nullptr;
	int width = 0;
	int height = 0;
	int currentFrameX = 0;
	int currentFrameY = 0;
	int maxFrameX = 0;
	int maxFrameY = 0;
	int frameWidth = 0;
	int frameHeight = 0;
	enum IMAGE_LOAD_TYPE loadType = IMAGE_LOAD_TYPE::LOAD_RESOURCE;
} IMAGE_INFO, * LPIMAGE_INFO;

class image
{
private:
	LPIMAGE_INFO	_imageInfo;	
	CHAR*			_fileName;
	BOOL			_hasTransparentColor;
	COLORREF		_transparentColor;
	LPIMAGE_INFO	_blendImageInfo;
	BLENDFUNCTION	_blendFunc;
public:

	image();
	~image();

	HRESULT init(int width, int height);

	HRESULT init(const char* fileName, int width, int height,
		BOOL hasTransparentColor = FALSE, COLORREF transparentColor = RGB(0, 0, 0));

	// 그림 전체 영역을 프레임 시트로 다루고 싶을 때 사용하는 초기화 함수
	HRESULT init(const char* fileName, int width, int height, 
		int frameX, int frameY,
		BOOL hasTransparentColor = FALSE, COLORREF transparentColor = RGB(0, 0, 0));

	// 그림 일부 영역을 프레임 시트로 다루고 싶을 때 사용하는 초기화 함수
	HRESULT init(const char* fileName, int x, int y,
	int width, int height, int frameX, int frameY,
	BOOL hasTransparentColor = FALSE, COLORREF transtransparentColorColor = RGB(0, 0, 0));

	void release();

	void setTransparentColor(BOOL hasTransparentColor, COLORREF transparentColor);

	void render(HDC hDC);
	void render(HDC hDC, int destX, int destY);
	void render(HDC hDC, int destX, int destY, int srcX, int srcY, int srcWidth, int srcHeight);

	// 좌우 반전 렌더링(일반)
	void renderH(HDC hDC, int destX = 0, int destY = 0, int srcX = 0, int srcY = 0, int srcWidth = -1, int srcHeight = -1);
	// 상하 반전 렌더링(일반)
	void renderV(HDC hDC, int destX = 0, int destY = 0, int srcX = 0, int srcY = 0, int srcWidth = -1, int srcHeight = -1);
	// 상하좌우 반전 렌더링(일반)
	void renderHV(HDC hDC, int destX = 0, int destY = 0, int srcX = 0, int srcY = 0, int srcWidth = -1, int srcHeight = -1);

	void frameRender(HDC hDC, int destX, int destY);
	void frameRender(HDC hDC, int destX, int destY,	int currentFrameX, int currentFrameY);

	// 좌우 반전 렌더링(프레임): currentFrameX, currentFrameY를 -1로 지정하면 현재 프레임 번호를 사용한다.
	void frameRenderH(HDC hDC, int destX, int destY, int currentFrameX = -1, int currentFrameY = -1);
	// 상하 반전 렌더링(프레임): currentFrameX, currentFrameY를 -1로 지정하면 현재 프레임 번호를 사용한다.
	void frameRenderV(HDC hDC, int destX, int destY, int currentFrameX = -1, int currentFrameY = -1);
	// 상하좌우 반전 렌더링(프레임): currentFrameX, currentFrameY를 -1로 지정하면 현재 프레임 번호를 사용한다.
	void frameRenderHV(HDC hDC, int destX, int destY, int currentFrameX = -1, int currentFrameY = -1);

	void adjFrameRender(HDC hDC, int destX, int destY, int currentFrameX, int currentFrameY, int adjWidth, int adjHeight);

	void alphaRender(HDC hDC, BYTE alpha);
	void alphaRender(HDC hDC, int destX, int destY, BYTE alpha);
	void alphaRender(HDC hDC, int destX, int destY, int srcX, int srcY, int srcWidth, int srcHeight, BYTE alpha);

	void animRender(HDC hDC, int destX, int destY, animation* anim);
	void animRenderH(HDC hDC, int destX, int destY, animation* anim);

	// 지정 영역이 채워지도록 그림을 반복 출력 하는 함수
	void loopRender(HDC hDC, const LPRECT drawArea, int offSetX = 0, int offSetY = 0);

	// 지정 영역(destArea로 지정한다.)이 채워지도록 그림 일부(srcArea로 영역을 지정한다.)를 반복 출력 하는 함수
	void loopRenderP(HDC hDC, const LPRECT destArea, const LPRECT srcArea, int offSetX = 0, int offSetY = 0);

	// 이미지 특정 색을 변경하고 새 색을 반환하는 함수(32 비트 비트맵 전용)
	COLORREF changeColor(COLORREF originalColor, COLORREF newColor);

	// 이미지 색을 예외 색을 제외하고 전부 다른 특정 색으로 변경하는 함수(32 비트 비트맵 전용)
	COLORREF changeAllColors(COLORREF newColor, COLORREF exceptionalColor = RGB(255, 0, 255));

	// 기존 image를 복사하여 새 image를 만드는 함수(사용이 끝나면 release를 호출하고 delete를 호출하여야 한다.)
	image* copyNew() const;

	inline HDC getMemDC() { return _imageInfo->hMemDC; }

	inline int getWidth() { return _imageInfo->width; }
	inline int getHeight() { return _imageInfo->height; }

	inline void setFrameX(int frameX)
	{
		_imageInfo->currentFrameX = frameX;

		if (frameX > _imageInfo->maxFrameX)
		{
			_imageInfo->currentFrameX = _imageInfo->maxFrameX;
		}
	}

	inline void setFrameY(int frameY)
	{
		_imageInfo->currentFrameY = frameY;

		if (frameY > _imageInfo->maxFrameY)
		{
			_imageInfo->currentFrameY = _imageInfo->maxFrameY;
		}
	}

	inline int getMaxFrameX() { return _imageInfo->maxFrameX; }
	inline int getMaxFrameY() { return _imageInfo->maxFrameY; }

	inline int getFrameX() { return _imageInfo->currentFrameX; }
	inline int getFrameY() { return _imageInfo->currentFrameY; }

	inline int getFrameWidth() { return _imageInfo->frameWidth; }
	inline int getFrameHeight() { return _imageInfo->frameHeight; }
};

