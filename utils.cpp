#include "stdafx.h"
#include "utils.h"

namespace utils
{
	void Polygon(HDC hDC, vector<POINT> vertices, POINT& pivot, float angle)
	{
		if (vertices.empty()) return;

		if (angle != 0.f)
		{
			float cosA = cosf(angle);
			float sinA = sinf(angle);

			for (POINT& pt : vertices)
			{
				int tempX = static_cast<int>(roundf(cosA * (pt.x - pivot.x) + sinA * (pt.y - pivot.y) + pivot.x));

				if (!_shouldRenderUsingWindowCoords)
				{
					pt.y = static_cast<int>(roundf(pivot.y + sinA * (pivot.x - pt.x) + cosA * (pt.y - pivot.y))) - _currOrg.y;
					pt.x = tempX - _currOrg.x;
				}
				else
				{
					pt.y = static_cast<int>(roundf(pivot.y + sinA * (pivot.x - pt.x) + cosA * (pt.y - pivot.y)));
					pt.x = tempX;
				}
			}
		}

		::Polygon(hDC, vertices.data(), static_cast<int>(vertices.size()));
	}
}
