#pragma once

constexpr float PI = 3.1415927f; // pi
constexpr float PI2 = 6.283185f; // 2 pi

namespace utils
{
	void Polygon(HDC hDC, vector<POINT> vertices, POINT& pivot, float angle = 0.f);
	inline float Distance(float x1, float y1, float x2, float y2);
	inline float Distance(int x1, int y1, int x2, int y2);
	inline float Angle(float x1, float y1, float x2, float y2);
	inline float Angle(int x1, int y1, int x2, int y2);

	inline void DrawLine(HDC hDC, int x1, int y1, int x2, int y2);
	inline POINT MakePt(int x, int y);
	inline RECT MakeRct(int x, int y, int width, int height);
	inline RECT MakeRctC(int x, int y, int width, int height);
	inline void DrawRct(HDC hDC, int x, int y, int width, int height);
	inline void DrawRctC(HDC hDC, int x, int y, int width, int height);
	inline void DrawRct(HDC hDC, RECT& rct);
	inline void DrawElp(HDC hDC, int x, int y, int width, int height);
	inline void DrawElpC(HDC hDC, int x, int y, int width, int height);
	inline void DrawElp(HDC hDC, RECT& rct);
}

inline float utils::Distance(float x1, float y1, float x2, float y2)
{
	return sqrtf(pow(x2 - x1, 2) + pow(y1 - y2, 2));
}

float utils::Distance(int x1, int y1, int x2, int y2)
{
	return sqrtf(pow(static_cast<float>(x2 - x1), 2) + pow(static_cast<float>(y1 - y2), 2));
}

inline float utils::Angle(float x1, float y1, float x2, float y2)
{
	return atan2f(y1 - y2, x2 - x1);
}

float utils::Angle(int x1, int y1, int x2, int y2)
{
	return atan2f(static_cast<float>(y1 - y2), static_cast<float>(x2 - x1));
}

inline void utils::DrawLine(HDC hDC, int x1, int y1, int x2, int y2)
{
	MoveToEx(hDC, (_shouldRenderUsingWindowCoords ? x1 : (x1 - _currOrg.x)), (_shouldRenderUsingWindowCoords ? y1 : (y1 - _currOrg.y)), nullptr);
	LineTo(hDC, (_shouldRenderUsingWindowCoords ? x2 : (x2 - _currOrg.x)), (_shouldRenderUsingWindowCoords ? y2 : (y2 - _currOrg.y)));
}

inline POINT utils::MakePt(int x, int y)
{
	POINT pt = { x, y };
	return pt;
}

inline RECT utils::MakeRct(int x, int y, int width, int height)
{
	RECT rct = { x, y, x + width, y + height };

	return rct;
}

inline RECT utils::MakeRctC(int x, int y, int width, int height)
{
	RECT rct = { x - (width / 2) , y - (height / 2), x + (width / 2), y + (height / 2) };
	return rct;
}

inline void utils::DrawRct(HDC hDC, int x, int y, int width, int height)
{
	Rectangle(hDC, (_shouldRenderUsingWindowCoords ? x : (x - _currOrg.x)),
			  (_shouldRenderUsingWindowCoords ? y : (y - _currOrg.y)),
			  (_shouldRenderUsingWindowCoords ? x : (x - _currOrg.x)) + width,
			  (_shouldRenderUsingWindowCoords ? y : (y - _currOrg.y)) + height);
}

inline void utils::DrawRctC(HDC hDC, int x, int y, int width, int height)
{
	Rectangle(hDC, (_shouldRenderUsingWindowCoords ? x : (x - _currOrg.x)) - (width / 2),
			  (_shouldRenderUsingWindowCoords ? y : (y - _currOrg.y)) - (height / 2),
			  (_shouldRenderUsingWindowCoords ? x : (x - _currOrg.x)) + (width / 2),
			  (_shouldRenderUsingWindowCoords ? y : (y - _currOrg.y)) + (height / 2));
}

inline void utils::DrawRct(HDC hDC, RECT& rct)
{
	Rectangle(hDC, (_shouldRenderUsingWindowCoords ? rct.left : (rct.left - _currOrg.x)),
			  (_shouldRenderUsingWindowCoords ? rct.top : (rct.top - _currOrg.y)),
			  (_shouldRenderUsingWindowCoords ? rct.right : (rct.right - _currOrg.x)),
			  (_shouldRenderUsingWindowCoords ? rct.bottom : (rct.bottom - _currOrg.y)));
}

inline void utils::DrawElp(HDC hDC, int x, int y, int width, int height)
{
	Ellipse(hDC, (_shouldRenderUsingWindowCoords ? x : (x - _currOrg.x)),
			(_shouldRenderUsingWindowCoords ? y : (y - _currOrg.y)),
			(_shouldRenderUsingWindowCoords ? x : (x - _currOrg.x)) + width,
			(_shouldRenderUsingWindowCoords ? y : (y - _currOrg.y)) + height);
}

inline void utils::DrawElpC(HDC hDC, int x, int y, int width, int height)
{
	Ellipse(hDC, (_shouldRenderUsingWindowCoords ? x : (x - _currOrg.x)) - (width / 2),
			(_shouldRenderUsingWindowCoords ? y : (y - _currOrg.y)) - (height / 2),
			  (_shouldRenderUsingWindowCoords ? x : (x - _currOrg.x)) + (width / 2),
			  (_shouldRenderUsingWindowCoords ? y : (y - _currOrg.y)) + (height / 2));
}

inline void utils::DrawElp(HDC hDC, RECT& rct)
{
	Ellipse(hDC, (_shouldRenderUsingWindowCoords ? rct.left : (rct.left - _currOrg.x)),
			  (_shouldRenderUsingWindowCoords ? rct.top : (rct.top - _currOrg.y)),
			  (_shouldRenderUsingWindowCoords ? rct.right : (rct.right - _currOrg.x)),
			  (_shouldRenderUsingWindowCoords ? rct.bottom : (rct.bottom - _currOrg.y)));
}
