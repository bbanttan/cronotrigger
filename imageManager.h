#pragma once
#include "singleton.h"
#include "image.h"
#include <map>
#include <unordered_map>
#include <functional>

class imageManager : public singleton<imageManager>
{
private:
	typedef map<string, image*>				imageList;
	typedef map<string, image*>::iterator	imageListIter;
	HRGN _hClipRgn = nullptr, _hOClipRgn = nullptr; // 클리핑 영역 지정용 변수

	imageList _imageList;
	map<string, function<void()>> _availableFuncs;
	unordered_multimap<LONG, function<void()>> _funcsToCall;
	vector<LONG> _funcListKeys;

public:
	imageManager();
	~imageManager();

	HRESULT init();
	void release();

	// 이미지 추가
	image* add(string strKey, int width, int height);

	image * add(string strKey, int width, int height, BOOL hasTransparentColor, COLORREF transparentColor);

	// 이미지 추가
	image* add(string strKey, const char* fileName, int width, int height, BOOL hasTransparentColor = FALSE, COLORREF transparentColor = RGB(0, 0, 0));

	// 프레임 이미지 추가
	image* addF(string strKey, const char* fileName, int width, int height, int frameX, int frameY,
		BOOL hasTransparentColor = FALSE, COLORREF transparentColor = RGB(0, 0, 0));

	// 프레임 이미지 추가
	image* addF(string strKey, const char* fileName, int x, int y, int width, int height, int frameX, int frameY,
		BOOL hasTransparentColor = FALSE, COLORREF transparentColor = RGB(0, 0, 0));

	// 이미지 찾기
	image* find(string strKey);

	// 이미지 제거
	BOOL del(string strKey);

	// 이미지 전부 제거
	BOOL delAll();

	void render(string strKey, HDC hDC);
	void render(string strKey, HDC hDC, int destX, int destY);
	void render(string strKey, HDC hDC, int destX, int destY, int sourX, int sourY, int srcWidth, int srcHeight);

	// 좌우 반전 렌더링(일반)
	void renderH(string strKey, HDC hDC, int destX = 0, int destY = 0, int srcX = 0, int srcY = 0, int srcWidth = -1, int srcHeight = -1);
	// 상하 반전 렌더링(일반)
	void renderV(string strKey, HDC hDC, int destX = 0, int destY = 0, int srcX = 0, int srcY = 0, int srcWidth = -1, int srcHeight = -1);
	// 상하좌우 반전 렌더링(일반)
	void renderHV(string strKey, HDC hDC, int destX = 0, int destY = 0, int srcX = 0, int srcY = 0, int srcWidth = -1, int srcHeight = -1);

	void frameRender(string strKey, HDC hDC, int destX, int destY);
	void frameRender(string strKey, HDC hDC, int destX, int destY, int currentFrameX, int currentFrameY);

	// 좌우 반전 렌더링(프레임): currentFrameX, currentFrameY를 -1로 지정하면 현재 프레임 번호를 사용한다.
	void frameRenderH(string strKey, HDC hDC, int destX, int destY, int currentFrameX = -1, int currentFrameY = -1);
	// 상하 반전 렌더링(프레임): currentFrameX, currentFrameY를 -1로 지정하면 현재 프레임 번호를 사용한다.
	void frameRenderV(string strKey, HDC hDC, int destX, int destY, int currentFrameX = -1, int currentFrameY = -1);
	// 상하좌우 반전 렌더링(프레임): currentFrameX, currentFrameY를 -1로 지정하면 현재 프레임 번호를 사용한다.
	void frameRenderHV(string strKey, HDC hDC, int destX, int destY, int currentFrameX = -1, int currentFrameY = -1);

	void adjFrameRender(string strKey, HDC hDC, int destX, int destY, int currentFrameX, int currentFrameY, int adjWidth, int adjHeight);

	void alphaRender(string strKey, HDC hDC, BYTE alpha);
	void alphaRender(string strKey, HDC hDC, int destX, int destY, BYTE alpha);
	void alphaRender(string strKey, HDC hDC, int destX, int destY, int srcX, int srcY, int srcWidth, int srcHeight, BYTE alpha);

	void animRender(string strKey, HDC hDC, int destX, int destY, animation* anim);
	void animRenderH(string strKey, HDC hDC, int destX, int destY, animation* anim);

	void loopRender(string strKey, HDC hDC, const LPRECT drawArea, int offSetX, int offSetY);

	// 클리핑 영역을 사각형으로 지정하는 함수
	void setRctClipRgn(HDC hDC, int x, int y, int width, int height);

	// 클리핑 영역을 타원으로 지정하는 함수
	void setEllipticClipRgn(HDC hDC, int x, int y, int width, int height);
	
	// 지정하였던 클리핑 영역을 해제하는 함수
	void resetClipRgn(HDC hDC);

	// 현재 글 출력 창 스킨 인덱스를 반영하여 글 출력 창 스킨을 등록된 비트맵 특정 위치에 출력하는 함수
	// 너비 96 픽셀 이상, 높이 96 픽셀 이상이어야 하며 높이 값은 16의 배수여야 한다.
	void setWindowSkin(string tilesetStrKey, string skinStrKey, int width, int height, int destX = 0, int destY = 0);

	// 현재 글 출력 창 스킨 인덱스를 반영하여 글 출력 창 스킨을 전부 다시 그리는 함수
	void setAllWindowSkins();

	// zIndex 순서로 함수를 호출할 수 있도록 함수를 등록하는 함수(선언) - image* 사용
	template<typename...args>
	void renderZ(LONG zIndex, image* img, args... params);
	template<typename...args>
	void renderHZ(LONG zIndex, image* img, args... params);
	template<typename...args>
	void renderVZ(LONG zIndex, image* img, args... params);
	template<typename...args>
	void renderHVZ(LONG zIndex, image* img, args... params);
	template<typename...args>
	void frameRenderZ(LONG zIndex, image* img, args... params);
	template<typename...args>
	void frameRenderHZ(LONG zIndex, image* img, args... params);
	template<typename...args>
	void frameRenderVZ(LONG zIndex, image* img, args... params);
	template<typename...args>
	void frameRenderHVZ(LONG zIndex, image* img, args... params);
	template<typename...args>
	void adjFrameRenderZ(LONG zIndex, image* img, args... params);
	template<typename...args>
	void alphaRenderZ(LONG zIndex, image* img, args... params);
	template<typename...args>
	void animRenderZ(LONG zIndex, image* img, args... params);
	template<typename...args>
	void animRenderHZ(LONG zIndex, image* img, args... params);
	template<typename...args>
	void loopRenderZ(LONG zIndex, image* img, args... params);

	// zIndex 순서로 함수를 호출할 수 있도록 함수를 등록하는 함수(선언) - strKey 사용
	template<typename...args>
	void renderZ(LONG zIndex, string strKey, args... params);
	template<typename...args>
	void renderHZ(LONG zIndex, string strKey, args... params);
	template<typename...args>
	void renderVZ(LONG zIndex, string strKey, args... params);
	template<typename...args>
	void renderHVZ(LONG zIndex, string strKey, args... params);
	template<typename...args>
	void frameRenderZ(LONG zIndex, string strKey, args... params);
	template<typename...args>
	void frameRenderHZ(LONG zIndex, string strKey, args... params);
	template<typename...args>
	void frameRenderVZ(LONG zIndex, string strKey, args... params);
	template<typename...args>
	void frameRenderHVZ(LONG zIndex, string strKey, args... params);
	template<typename...args>
	void adjFrameRenderZ(LONG zIndex, string strKey, args... params);
	template<typename...args>
	void alphaRenderZ(LONG zIndex, string strKey, args... params);
	template<typename...args>
	void animRenderZ(LONG zIndex, string strKey, args... params);
	template<typename...args>
	void animRenderHZ(LONG zIndex, string strKey, args... params);
	template<typename...args>
	void loopRenderZ(LONG zIndex, string strKey, args... params);

	// zIndex 순서로 함수를 호출할 수 있도록 함수를 등록하는 함수(선언)
	template<typename...args>
	void setRctClipRgnZ(LONG zIndex, args... params);
	template<typename...args>
	void setEllipticClipRgnZ(LONG zIndex, args... params);
	template<typename...args>
	void resetClipRgnZ(LONG zIndex, args... params);

	// zIndex 순서로 함수를 호출하는 함수
	void execZ();

	// hDC에 연결된 비트맵을 클립보드로 복사하거나 그림 파일로 저장하는 함수
	void takeScreenshot(HDC hDC, BOOL shouldSaveAsImage = FALSE, int x = 0, int y = 0, int width = WINW, int height = WINH);
};


// zIndex 순서로 함수를 호출할 수 있도록 함수를 등록하는 함수(정의) - image* 사용
template<typename ...args>
inline void imageManager::renderZ(LONG zIndex, image* img, args ...params)
{
	if (img == nullptr) return;
	if (sizeof...(params) == 0) return;
	function<void()> func = [img, params...]() { return img->render(params...); };
	if (func == NULL) return;
	_funcsToCall.insert(make_pair(zIndex, func));
}

template<typename ...args>
inline void imageManager::renderHZ(LONG zIndex, image* img, args ...params)
{
	if (img == nullptr) return;
	if (sizeof...(params) == 0) return;
	function<void()> func = [img, params...]() { return img->renderH(params...); };
	if (func == NULL) return;
	_funcsToCall.insert(make_pair(zIndex, func));
}

template<typename ...args>
inline void imageManager::renderVZ(LONG zIndex, image* img, args ...params)
{
	if (img == nullptr) return;
	if (sizeof...(params) == 0) return;
	function<void()> func = [img, params...]() { return img->renderV(params...); };
	if (func == NULL) return;
	_funcsToCall.insert(make_pair(zIndex, func));
}

template<typename ...args>
inline void imageManager::renderHVZ(LONG zIndex, image* img, args ...params)
{
	if (img == nullptr) return;
	if (sizeof...(params) == 0) return;
	function<void()> func = [img, params...]() { return img->renderHV(params...); };
	if (func == NULL) return;
	_funcsToCall.insert(make_pair(zIndex, func));
}

template<typename ...args>
inline void imageManager::frameRenderZ(LONG zIndex, image* img, args ...params)
{
	if (img == nullptr) return;
	if (sizeof...(params) == 0) return;
	function<void()> func = [img, params...]() { return img->frameRender(params...); };
	if (func == NULL) return;
	_funcsToCall.insert(make_pair(zIndex, func));
}

template<typename ...args>
inline void imageManager::frameRenderHZ(LONG zIndex, image* img, args ...params)
{
	if (img == nullptr) return;
	if (sizeof...(params) == 0) return;
	function<void()> func = [img, params...]() { return img->frameRenderH(params...); };
	if (func == NULL) return;
	_funcsToCall.insert(make_pair(zIndex, func));
}

template<typename ...args>
inline void imageManager::frameRenderVZ(LONG zIndex, image* img, args ...params)
{
	if (img == nullptr) return;
	if (sizeof...(params) == 0) return;
	function<void()> func = [img, params...]() { return img->frameRenderV(params...); };
	if (func == NULL) return;
	_funcsToCall.insert(make_pair(zIndex, func));
}

template<typename ...args>
inline void imageManager::frameRenderHVZ(LONG zIndex, image* img, args ...params)
{
	if (img == nullptr) return;
	if (sizeof...(params) == 0) return;
	function<void()> func = [img, params...]() { return img->frameRenderHV(params...); };
	if (func == NULL) return;
	_funcsToCall.insert(make_pair(zIndex, func));
}

template<typename ...args>
inline void imageManager::adjFrameRenderZ(LONG zIndex, image* img, args ...params)
{
	if (img == nullptr) return;
	if (sizeof...(params) == 0) return;
	function<void()> func = [img, params...]() { return img->adjFrameRender(params...); };
	if (func == NULL) return;
	_funcsToCall.insert(make_pair(zIndex, func));
}

template<typename ...args>
inline void imageManager::alphaRenderZ(LONG zIndex, image* img, args ...params)
{
	if (img == nullptr) return;
	if (sizeof...(params) == 0) return;
	function<void()> func = [img, params...]() { return img->alphaRender(params...); };
	if (func == NULL) return;
	_funcsToCall.insert(make_pair(zIndex, func));
}

template<typename ...args>
inline void imageManager::animRenderZ(LONG zIndex, image* img, args ...params)
{
	if (img == nullptr) return;
	if (sizeof...(params) == 0) return;
	function<void()> func = [img, params...]() { return img->animRender(params...); };
	if (func == NULL) return;
	_funcsToCall.insert(make_pair(zIndex, func));
}

template<typename ...args>
inline void imageManager::animRenderHZ(LONG zIndex, image* img, args ...params)
{
	if (img == nullptr) return;
	if (sizeof...(params) == 0) return;
	function<void()> func = [img, params...]() { return img->animRenderH(params...); };
	if (func == NULL) return;
	_funcsToCall.insert(make_pair(zIndex, func));
}

template<typename ...args>
inline void imageManager::loopRenderZ(LONG zIndex, image* img, args ...params)
{
	if (img == nullptr) return;
	if (sizeof...(params) == 0) return;
	function<void()> func = [img, params...]() { return img->loopRender(params...); };
	if (func == NULL) return;
	_funcsToCall.insert(make_pair(zIndex, func));
}

// zIndex 순서로 함수를 호출할 수 있도록 함수를 등록하는 함수(정의) - strKey* 사용
template<typename ...args>
inline void imageManager::renderZ(LONG zIndex, string strKey, args ...params)
{
	image* img = find(strKey); if (img == nullptr) return;
	if (sizeof...(params) == 0) return;
	function<void()> func = [img, params...]() { return img->render(params...); };
	if (func == NULL) return;
	_funcsToCall.insert(make_pair(zIndex, func));
}

template<typename ...args>
inline void imageManager::renderHZ(LONG zIndex, string strKey, args ...params)
{
	image* img = find(strKey); if (img == nullptr) return;
	if (sizeof...(params) == 0) return;
	function<void()> func = [img, params...]() { return img->renderH(params...); };
	if (func == NULL) return;
	_funcsToCall.insert(make_pair(zIndex, func));
}

template<typename ...args>
inline void imageManager::renderVZ(LONG zIndex, string strKey, args ...params)
{
	image* img = find(strKey); if (img == nullptr) return;
	if (sizeof...(params) == 0) return;
	function<void()> func = [img, params...]() { return img->renderV(params...); };
	if (func == NULL) return;
	_funcsToCall.insert(make_pair(zIndex, func));
}

template<typename ...args>
inline void imageManager::renderHVZ(LONG zIndex, string strKey, args ...params)
{
	image* img = find(strKey); if (img == nullptr) return;
	if (sizeof...(params) == 0) return;
	function<void()> func = [img, params...]() { return img->renderHV(params...); };
	if (func == NULL) return;
	_funcsToCall.insert(make_pair(zIndex, func));
}

template<typename ...args>
inline void imageManager::frameRenderZ(LONG zIndex, string strKey, args ...params)
{
	image* img = find(strKey); if (img == nullptr) return;
	if (sizeof...(params) == 0) return;
	function<void()> func = [img, params...]() { return img->frameRender(params...); };
	if (func == NULL) return;
	_funcsToCall.insert(make_pair(zIndex, func));
}

template<typename ...args>
inline void imageManager::frameRenderHZ(LONG zIndex, string strKey, args ...params)
{
	image* img = find(strKey); if (img == nullptr) return;
	if (sizeof...(params) == 0) return;
	function<void()> func = [img, params...]() { return img->frameRenderH(params...); };
	if (func == NULL) return;
	_funcsToCall.insert(make_pair(zIndex, func));
}

template<typename ...args>
inline void imageManager::frameRenderVZ(LONG zIndex, string strKey, args ...params)
{
	image* img = find(strKey); if (img == nullptr) return;
	if (sizeof...(params) == 0) return;
	function<void()> func = [img, params...]() { return img->frameRenderV(params...); };
	if (func == NULL) return;
	_funcsToCall.insert(make_pair(zIndex, func));
}

template<typename ...args>
inline void imageManager::frameRenderHVZ(LONG zIndex, string strKey, args ...params)
{
	image* img = find(strKey); if (img == nullptr) return;
	if (sizeof...(params) == 0) return;
	function<void()> func = [img, params...]() { return img->frameRenderHV(params...); };
	if (func == NULL) return;
	_funcsToCall.insert(make_pair(zIndex, func));
}

template<typename ...args>
inline void imageManager::adjFrameRenderZ(LONG zIndex, string strKey, args ...params)
{
	image* img = find(strKey); if (img == nullptr) return;
	if (sizeof...(params) == 0) return;
	function<void()> func = [img, params...]() { return img->adjFrameRender(params...); };
	if (func == NULL) return;
	_funcsToCall.insert(make_pair(zIndex, func));
}

template<typename ...args>
inline void imageManager::alphaRenderZ(LONG zIndex, string strKey, args ...params)
{
	image* img = find(strKey); if (img == nullptr) return;
	if (sizeof...(params) == 0) return;
	function<void()> func = [img, params...]() { return img->alphaRender(params...); };
	if (func == NULL) return;
	_funcsToCall.insert(make_pair(zIndex, func));
}

template<typename ...args>
inline void imageManager::animRenderZ(LONG zIndex, string strKey, args ...params)
{
	image* img = find(strKey); if (img == nullptr) return;
	if (sizeof...(params) == 0) return;
	function<void()> func = [img, params...]() { return img->animRender(params...); };
	if (func == NULL) return;
	_funcsToCall.insert(make_pair(zIndex, func));
}

template<typename ...args>
inline void imageManager::animRenderHZ(LONG zIndex, string strKey, args ...params)
{
	image* img = find(strKey); if (img == nullptr) return;
	if (sizeof...(params) == 0) return;
	function<void()> func = [img, params...]() { return img->animRenderH(params...); };
	if (func == NULL) return;
	_funcsToCall.insert(make_pair(zIndex, func));
}

template<typename ...args>
inline void imageManager::loopRenderZ(LONG zIndex, string strKey, args ...params)
{
	image* img = find(strKey); if (img == nullptr) return;
	if (sizeof...(params) == 0) return;
	function<void()> func = [img, params...]() { return img->loopRender(params...); };
	if (func == NULL) return;
	_funcsToCall.insert(make_pair(zIndex, func));
}

// zIndex 순서로 함수를 호출할 수 있도록 함수를 등록하는 함수(정의) 

template<typename ...args>
inline void imageManager::setRctClipRgnZ(LONG zIndex, args ...params)
{
	if (sizeof...(params) == 0) return;
	function<void()> func = [params...]() { return imageManager::setRctClipRgn(params...); };
	if (func == NULL) return;
	_funcsToCall.insert(make_pair(zIndex, func));
}

template<typename ...args>
inline void imageManager::setEllipticClipRgnZ(LONG zIndex, args ...params)
{
	if (sizeof...(params) == 0) return;
	function<void()> func = [params...]() { return imageManager::setEllipticClipRgn(params...); };
	if (func == NULL) return;
	_funcsToCall.insert(make_pair(zIndex, func));
}

template<typename ...args>
inline void imageManager::resetClipRgnZ(LONG zIndex, args ...params)
{
	if (sizeof...(params) == 0) return;
	function<void()> func = [params...]() { return imageManager::resetClipRgn(params...); };
	if (func == NULL) return;
	_funcsToCall.insert(make_pair(zIndex, func));
}
