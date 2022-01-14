#include "stdafx.h"
#include "image.h"

image::image(): _fileName(nullptr), _hasTransparentColor(FALSE), _transparentColor(RGB(0, 0, 0)) {}
image::~image() {}

HRESULT image::init(int width, int height)
{
	if (_imageInfo != nullptr) release();

	HDC hDC = GetDC(_hWnd);

	_imageInfo = new IMAGE_INFO;
	_imageInfo->loadType = IMAGE_LOAD_TYPE::LOAD_EMPTY;
	_imageInfo->resID = 0;
	_imageInfo->hMemDC = CreateCompatibleDC(hDC);
	_imageInfo->hBit = (HBITMAP)CreateCompatibleBitmap(hDC, width, height);
	_imageInfo->hOBit = (HBITMAP)SelectObject(_imageInfo->hMemDC, _imageInfo->hBit);
	_imageInfo->width = width;
	_imageInfo->height = height;

	_blendFunc.BlendFlags = 0;
	_blendFunc.AlphaFormat = 0;
	_blendFunc.BlendOp = AC_SRC_OVER;

	_blendImageInfo = new IMAGE_INFO;
	_blendImageInfo->loadType = IMAGE_LOAD_TYPE::LOAD_EMPTY;
	_blendImageInfo->resID = 0;
	_blendImageInfo->hMemDC = CreateCompatibleDC(hDC);
	_blendImageInfo->hBit = CreateCompatibleBitmap(hDC, width, height);
	_blendImageInfo->hOBit = (HBITMAP)SelectObject(_blendImageInfo->hMemDC, _blendImageInfo->hBit);
	_blendImageInfo->width = width;
	_blendImageInfo->height = height;
	
	if (_imageInfo == nullptr)
	{
		release();
		return E_FAIL;
	}

	ReleaseDC(_hWnd, hDC);
	return S_OK;
}

HRESULT image::init(const char* fileName, int width, int height, BOOL hasTransparentColor, COLORREF transparentColor)
{
	if (_imageInfo != nullptr) release();

	HDC hDC = GetDC(_hWnd);

	_imageInfo = new IMAGE_INFO;
	_imageInfo->loadType = IMAGE_LOAD_TYPE::LOAD_FILE;
	_imageInfo->resID = 0;
	_imageInfo->hMemDC = CreateCompatibleDC(hDC);
	_imageInfo->hBit = (HBITMAP)LoadImage(_hInst, fileName, IMAGE_BITMAP, width, height, LR_LOADFROMFILE);
	_imageInfo->hOBit = (HBITMAP)SelectObject(_imageInfo->hMemDC, _imageInfo->hBit);
	_imageInfo->width = width;
	_imageInfo->height = height;

	size_t len = strlen(fileName);

	_fileName = new CHAR[len + 1];
	strcpy_s(_fileName, len + 1, fileName);

	_hasTransparentColor = hasTransparentColor;
	_transparentColor = transparentColor;

	_blendFunc.BlendFlags = 0;
	_blendFunc.AlphaFormat = 0;
	_blendFunc.BlendOp = AC_SRC_OVER;

	_blendImageInfo = new IMAGE_INFO;
	_blendImageInfo->loadType = IMAGE_LOAD_TYPE::LOAD_EMPTY;
	_blendImageInfo->resID = 0;
	_blendImageInfo->hMemDC = CreateCompatibleDC(hDC);
	_blendImageInfo->hBit = CreateCompatibleBitmap(hDC, width, height);
	_blendImageInfo->hOBit = (HBITMAP)SelectObject(_blendImageInfo->hMemDC, _blendImageInfo->hBit);
	_blendImageInfo->width = width;
	_blendImageInfo->height = height;

	if (_imageInfo->hBit == nullptr)
	{
		release();
		return E_FAIL;
	}

	ReleaseDC(_hWnd, hDC);
	return S_OK;
}


HRESULT image::init(const char* fileName, int width, int height, int frameX, int frameY, BOOL hasTransparentColor, COLORREF transparentColor)
{
	if (_imageInfo != nullptr) release();

	HDC hDC = GetDC(_hWnd);

	_imageInfo = new IMAGE_INFO;
	_imageInfo->loadType = IMAGE_LOAD_TYPE::LOAD_FILE;
	_imageInfo->resID = 0;
	_imageInfo->hMemDC = CreateCompatibleDC(hDC);
	_imageInfo->hBit = (HBITMAP)LoadImage(_hInst, fileName, IMAGE_BITMAP, width, height, LR_LOADFROMFILE);
	_imageInfo->hOBit = (HBITMAP)SelectObject(_imageInfo->hMemDC, _imageInfo->hBit);
	_imageInfo->width = width;
	_imageInfo->height = height;
	_imageInfo->frameWidth = width / frameX;
	_imageInfo->frameHeight = height / frameY;
	_imageInfo->maxFrameX = frameX - 1;
	_imageInfo->maxFrameY = frameY - 1;

	size_t len = strlen(fileName);

	_fileName = new CHAR[len + 1];
	strcpy_s(_fileName, len + 1, fileName);

	_hasTransparentColor = hasTransparentColor;
	_transparentColor = transparentColor;

	_blendFunc.BlendFlags = 0;
	_blendFunc.AlphaFormat = 0;
	_blendFunc.BlendOp = AC_SRC_OVER;

	_blendImageInfo = new IMAGE_INFO;
	_blendImageInfo->loadType = IMAGE_LOAD_TYPE::LOAD_EMPTY;
	_blendImageInfo->resID = 0;
	_blendImageInfo->hMemDC = CreateCompatibleDC(hDC);
	_blendImageInfo->hBit = CreateCompatibleBitmap(hDC, width, height);
	_blendImageInfo->hOBit = (HBITMAP)SelectObject(_blendImageInfo->hMemDC, _blendImageInfo->hBit);
	_blendImageInfo->width = width;
	_blendImageInfo->height = height;

	if (_imageInfo->hBit == nullptr)
	{
		release();
		return E_FAIL;
	}

	ReleaseDC(_hWnd, hDC);
	return S_OK;
}


HRESULT image::init(const char* fileName, int x, int y, int width, int height, int frameX, int frameY, BOOL hasTransparentColor, COLORREF transparentColor)
{
	if (_imageInfo != nullptr) release();

	HDC hDC = GetDC(_hWnd);

	_imageInfo = new IMAGE_INFO;
	_imageInfo->loadType = IMAGE_LOAD_TYPE::LOAD_FILE;
	_imageInfo->resID = 0;
	_imageInfo->hMemDC = CreateCompatibleDC(hDC);
	HDC hTempDC = CreateCompatibleDC(hDC);
	HBITMAP hTempBitmap = (HBITMAP)LoadImage(_hInst, fileName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	HBITMAP hOTempBitmap = (HBITMAP)SelectObject(hTempDC, hTempBitmap);
	_imageInfo->hBit = CreateCompatibleBitmap(hDC, width, height);
	_imageInfo->hOBit = (HBITMAP)SelectObject(_imageInfo->hMemDC, _imageInfo->hBit);
	BitBlt(_imageInfo->hMemDC, 0, 0, width, height, hTempDC, x, y, SRCCOPY);
	DeleteObject(SelectObject(hTempDC, hOTempBitmap));
	DeleteDC(hTempDC);

	_imageInfo->width = width;
	_imageInfo->height = height;
	_imageInfo->frameWidth = width / frameX;
	_imageInfo->frameHeight = height / frameY;
	_imageInfo->maxFrameX = frameX - 1;
	_imageInfo->maxFrameY = frameY - 1;

	size_t len = strlen(fileName);

	_fileName = new CHAR[len + 1];
	strcpy_s(_fileName, len + 1, fileName);

	_hasTransparentColor = hasTransparentColor;
	_transparentColor = transparentColor;

	_blendFunc.BlendFlags = 0;
	_blendFunc.AlphaFormat = 0;
	_blendFunc.BlendOp = AC_SRC_OVER;

	_blendImageInfo = new IMAGE_INFO;
	_blendImageInfo->loadType = IMAGE_LOAD_TYPE::LOAD_EMPTY;
	_blendImageInfo->resID = 0;
	_blendImageInfo->hMemDC = CreateCompatibleDC(hDC);
	_blendImageInfo->hBit = CreateCompatibleBitmap(hDC, width, height);
	_blendImageInfo->hOBit = (HBITMAP)SelectObject(_blendImageInfo->hMemDC, _blendImageInfo->hBit);
	_blendImageInfo->width = width;
	_blendImageInfo->height = height;

	if (_imageInfo->hBit == nullptr)
	{
		release();

		return E_FAIL;
	}

	ReleaseDC(_hWnd, hDC);

	return S_OK;
}

void image::release()
{
	if (_imageInfo)
	{
		SelectObject(_imageInfo->hMemDC, _imageInfo->hOBit);
		DeleteObject(_imageInfo->hBit);
		DeleteDC(_imageInfo->hMemDC);

		SelectObject(_blendImageInfo->hMemDC, _blendImageInfo->hOBit);
		DeleteObject(_blendImageInfo->hBit);
		DeleteDC(_blendImageInfo->hMemDC);

		SAFE_DEL(_imageInfo);
		SAFE_DEL(_fileName);
		SAFE_DEL(_blendImageInfo);

		_hasTransparentColor = false;
		_transparentColor = RGB(0, 0, 0);
	}

}

void image::setTransparentColor(BOOL hasTransparentColor, COLORREF transparentColor)
{
	_hasTransparentColor = hasTransparentColor;
	_transparentColor = transparentColor;
}

void image::render(HDC hDC)
{
	if (_hasTransparentColor)
	{
		GdiTransparentBlt(
			hDC,
			(_shouldRenderUsingWindowCoords ? 0 : _currOrg.x),
			(_shouldRenderUsingWindowCoords ? 0 : _currOrg.y),
			_imageInfo->width,
			_imageInfo->height,
			_imageInfo->hMemDC,
			0, 0,
			_imageInfo->width,
			_imageInfo->height,
			_transparentColor
		);
	}
	else
	{
		BitBlt(hDC,
			(_shouldRenderUsingWindowCoords ? 0 : _currOrg.x),
			(_shouldRenderUsingWindowCoords ? 0 : _currOrg.y),
			_imageInfo->width,
			_imageInfo->height,
			_imageInfo->hMemDC,
			0, 0,
			SRCCOPY);
	}
}

void image::render(HDC hDC, int destX, int destY)
{
	if (_hasTransparentColor)
	{
		GdiTransparentBlt(
			hDC,
			(_shouldRenderUsingWindowCoords ? destX : (destX - _currOrg.x)),
			(_shouldRenderUsingWindowCoords ? destY : (destY - _currOrg.y)),
			_imageInfo->width,
			_imageInfo->height,
			_imageInfo->hMemDC,
			0, 0,
			_imageInfo->width,
			_imageInfo->height,
			_transparentColor
		);
	}
	else
	{
		BitBlt(hDC,
			(_shouldRenderUsingWindowCoords ? destX : (destX - _currOrg.x)),
			(_shouldRenderUsingWindowCoords ? destY : (destY - _currOrg.y)),
			_imageInfo->width,
			_imageInfo->height,
			_imageInfo->hMemDC,
			0, 0,
			SRCCOPY);
	}


}

void image::render(HDC hDC, int destX, int destY, int srcX, int srcY, int srcWidth, int srcHeight)
{
	if (_hasTransparentColor)
	{
		GdiTransparentBlt(
			hDC,
			(_shouldRenderUsingWindowCoords ? destX : (destX - _currOrg.x)),
			(_shouldRenderUsingWindowCoords ? destY : (destY - _currOrg.y)),
			srcWidth,
			srcHeight,
			_imageInfo->hMemDC,
			srcX, srcY,
			srcWidth,
			srcHeight,
			_transparentColor
		);
	}
	else
	{
		BitBlt(hDC,
			(_shouldRenderUsingWindowCoords ? destX : (destX - _currOrg.x)),
			(_shouldRenderUsingWindowCoords ? destY : (destY - _currOrg.y)),
			srcWidth,
			srcHeight,
			_imageInfo->hMemDC,
			srcX, srcY,
			SRCCOPY);
	}
}

void image::renderH(HDC hDC, int destX, int destY, int srcX, int srcY, int srcWidth, int srcHeight)
{
	if (srcWidth == -1 || srcHeight == -1)
	{
		srcWidth = _imageInfo->width;
		srcHeight = _imageInfo->height;
	}
	if (_hasTransparentColor)
	{
		StretchBlt(_blendImageInfo->hMemDC, srcWidth - 1, 0, -srcWidth, srcHeight,
				   _imageInfo->hMemDC, srcX, srcY, srcWidth, srcHeight, SRCCOPY);
		GdiTransparentBlt(hDC,
						  (_shouldRenderUsingWindowCoords ? destX : (destX - _currOrg.x)),
						  (_shouldRenderUsingWindowCoords ? destY : (destY - _currOrg.y)),
						  srcWidth, srcHeight,
						  _blendImageInfo->hMemDC, 0, 0, srcWidth, srcHeight, _transparentColor);
	}
	else
	{
		StretchBlt(hDC,
				   (_shouldRenderUsingWindowCoords ? destX : (destX - _currOrg.x)) + srcWidth - 1,
				   (_shouldRenderUsingWindowCoords ? destY : (destY - _currOrg.y)),
				   -srcWidth, srcHeight,
				   _imageInfo->hMemDC, srcX, srcY, srcWidth, srcHeight, SRCCOPY);
	}
}

void image::renderV(HDC hDC, int destX, int destY, int srcX, int srcY, int srcWidth, int srcHeight)
{
	if (srcWidth == -1 || srcHeight == -1)
	{
		srcWidth = _imageInfo->width;
		srcHeight = _imageInfo->height;
	}
	if (_hasTransparentColor)
	{
		StretchBlt(_blendImageInfo->hMemDC, 0, srcHeight - 1, srcWidth, -srcHeight,
				   _imageInfo->hMemDC, srcX, srcY, srcWidth, srcHeight, SRCCOPY);
		GdiTransparentBlt(hDC,
						  (_shouldRenderUsingWindowCoords ? destX : (destX - _currOrg.x)),
						  (_shouldRenderUsingWindowCoords ? destY : (destY - _currOrg.y)),
						  srcWidth, srcHeight,
						  _blendImageInfo->hMemDC, 0, 0, srcWidth, srcHeight, _transparentColor);
	}
	else
	{
		StretchBlt(hDC,
				   (_shouldRenderUsingWindowCoords ? destX : (destX - _currOrg.x)),
				   (_shouldRenderUsingWindowCoords ? destY : (destY - _currOrg.y)) + srcHeight - 1,
				   srcWidth, -srcHeight,
				   _imageInfo->hMemDC, srcX, srcY, srcWidth, srcHeight, SRCCOPY);
	}
}

void image::renderHV(HDC hDC, int destX, int destY, int srcX, int srcY, int srcWidth, int srcHeight)
{
	if (srcWidth == -1 || srcHeight == -1)
	{
		srcWidth = _imageInfo->width;
		srcHeight = _imageInfo->height;
	}
	if (_hasTransparentColor)
	{
		StretchBlt(_blendImageInfo->hMemDC, srcWidth - 1, srcHeight - 1, -srcWidth, -srcHeight,
				   _imageInfo->hMemDC, srcX, srcY, srcWidth, srcHeight, SRCCOPY);
		GdiTransparentBlt(hDC,
						  (_shouldRenderUsingWindowCoords ? destX : (destX - _currOrg.x)),
						  (_shouldRenderUsingWindowCoords ? destY : (destY - _currOrg.y)),
						  srcWidth, srcHeight,
						  _blendImageInfo->hMemDC, 0, 0, srcWidth, srcHeight, _transparentColor);
	}
	else
	{
		StretchBlt(hDC,
				   (_shouldRenderUsingWindowCoords ? destX : (destX - _currOrg.x)) + srcWidth - 1,
				   (_shouldRenderUsingWindowCoords ? destY : (destY - _currOrg.y)) + srcHeight - 1,
				   -srcWidth, -srcHeight,
				   _imageInfo->hMemDC, srcX, srcY, srcWidth, srcHeight, SRCCOPY);
	}
}

void image::frameRender(HDC hDC, int destX, int destY)
{
	if (_hasTransparentColor)
	{
		GdiTransparentBlt(
			hDC,
			(_shouldRenderUsingWindowCoords ? destX : (destX - _currOrg.x)),
			(_shouldRenderUsingWindowCoords ? destY : (destY - _currOrg.y)),
			_imageInfo->frameWidth,
			_imageInfo->frameHeight,
			_imageInfo->hMemDC,
			_imageInfo->currentFrameX * _imageInfo->frameWidth, 
			_imageInfo->currentFrameY * _imageInfo->frameHeight,					
			_imageInfo->frameWidth,
			_imageInfo->frameHeight,
			_transparentColor
		);
	}
	else
	{
		BitBlt(hDC,
			   (_shouldRenderUsingWindowCoords ? destX : (destX - _currOrg.x)),
			   (_shouldRenderUsingWindowCoords ? destY : (destY - _currOrg.y)),
			   _imageInfo->frameWidth,
			   _imageInfo->frameHeight,
			   _imageInfo->hMemDC,
			   _imageInfo->currentFrameX * _imageInfo->frameWidth,
			   _imageInfo->currentFrameY * _imageInfo->frameHeight,
			   SRCCOPY);
	}
}

void image::frameRender(HDC hDC, int destX, int destY, int currentFrameX, int currentFrameY)
{
	_imageInfo->currentFrameX = currentFrameX;
	_imageInfo->currentFrameY = currentFrameY;

	if (_hasTransparentColor)
	{
		GdiTransparentBlt(
			hDC,
			(_shouldRenderUsingWindowCoords ? destX : (destX - _currOrg.x)),
			(_shouldRenderUsingWindowCoords ? destY : (destY - _currOrg.y)),
			_imageInfo->frameWidth,
			_imageInfo->frameHeight,
			_imageInfo->hMemDC,
			_imageInfo->currentFrameX * _imageInfo->frameWidth,
			_imageInfo->currentFrameY * _imageInfo->frameHeight,
			_imageInfo->frameWidth,
			_imageInfo->frameHeight,
			_transparentColor
		);
	}
	else
	{
		BitBlt(hDC,
			   (_shouldRenderUsingWindowCoords ? destX : (destX - _currOrg.x)),
			   (_shouldRenderUsingWindowCoords ? destY : (destY - _currOrg.y)),
			   _imageInfo->frameWidth,
			   _imageInfo->frameHeight,
			   _imageInfo->hMemDC,
			   _imageInfo->currentFrameX * _imageInfo->frameWidth,
			   _imageInfo->currentFrameY * _imageInfo->frameHeight,
			   SRCCOPY);
	}
}

void image::frameRenderH(HDC hDC, int destX, int destY, int currentFrameX, int currentFrameY)
{
	if (currentFrameX != -1) _imageInfo->currentFrameX = currentFrameX;
	if (currentFrameY != -1) _imageInfo->currentFrameY = currentFrameY;

	if (_hasTransparentColor)
	{
		StretchBlt(_blendImageInfo->hMemDC,
				   _imageInfo->frameWidth - 1, 0, -_imageInfo->frameWidth, _imageInfo->frameHeight,
				   _imageInfo->hMemDC,
				   _imageInfo->currentFrameX * _imageInfo->frameWidth, _imageInfo->currentFrameY * _imageInfo->frameHeight,
				   _imageInfo->frameWidth, _imageInfo->frameHeight,
				   SRCCOPY);
		GdiTransparentBlt(hDC, (_shouldRenderUsingWindowCoords ? destX : (destX - _currOrg.x)),
						  (_shouldRenderUsingWindowCoords ? destY : (destY - _currOrg.y)),
						  _imageInfo->frameWidth, _imageInfo->frameHeight,
						  _blendImageInfo->hMemDC, 0, 0, _imageInfo->frameWidth, _imageInfo->frameHeight,
						  _transparentColor);
	}
	else
	{
		StretchBlt(hDC, (_shouldRenderUsingWindowCoords ? destX : (destX - _currOrg.x)) + _imageInfo->frameWidth - 1,
				   (_shouldRenderUsingWindowCoords ? destY : (destY - _currOrg.y)),
				   -_imageInfo->frameWidth, _imageInfo->frameHeight,
				   _imageInfo->hMemDC, _imageInfo->currentFrameX * _imageInfo->frameWidth, _imageInfo->currentFrameY * _imageInfo->frameHeight,
				   _imageInfo->frameWidth, _imageInfo->frameHeight,
				   SRCCOPY);
	}
}

void image::frameRenderV(HDC hDC, int destX, int destY, int currentFrameX, int currentFrameY)
{
	if (currentFrameX != -1) _imageInfo->currentFrameX = currentFrameX;
	if (currentFrameY != -1) _imageInfo->currentFrameY = currentFrameY;

	if (_hasTransparentColor)
	{
		StretchBlt(_blendImageInfo->hMemDC,
				   0, _imageInfo->frameHeight - 1, _imageInfo->frameWidth, -_imageInfo->frameHeight,
				   _imageInfo->hMemDC,
				   _imageInfo->currentFrameX * _imageInfo->frameWidth, _imageInfo->currentFrameY * _imageInfo->frameHeight,
				   _imageInfo->frameWidth, _imageInfo->frameHeight,
				   SRCCOPY);
		GdiTransparentBlt(hDC, (_shouldRenderUsingWindowCoords ? destX : (destX - _currOrg.x)),
						  (_shouldRenderUsingWindowCoords ? destY : (destY - _currOrg.y)),
						  _imageInfo->frameWidth, _imageInfo->frameHeight,
						  _blendImageInfo->hMemDC, 0, 0, _imageInfo->frameWidth, _imageInfo->frameHeight,
						  _transparentColor);
	}
	else
	{
		StretchBlt(hDC, (_shouldRenderUsingWindowCoords ? destX : (destX - _currOrg.x)),
				   (_shouldRenderUsingWindowCoords ? destY : (destY - _currOrg.y)) + _imageInfo->frameHeight - 1,
				   _imageInfo->frameWidth, -_imageInfo->frameHeight,
				   _imageInfo->hMemDC, _imageInfo->currentFrameX * _imageInfo->frameWidth, _imageInfo->currentFrameY * _imageInfo->frameHeight,
				   _imageInfo->frameWidth, _imageInfo->frameHeight,
				   SRCCOPY);
	}
}

void image::frameRenderHV(HDC hDC, int destX, int destY, int currentFrameX, int currentFrameY)
{
	if (currentFrameX != -1) _imageInfo->currentFrameX = currentFrameX;
	if (currentFrameY != -1) _imageInfo->currentFrameY = currentFrameY;

	if (_hasTransparentColor)
	{
		StretchBlt(_blendImageInfo->hMemDC,
				   _imageInfo->frameWidth - 1, _imageInfo->frameHeight - 1, -_imageInfo->frameWidth, -_imageInfo->frameHeight,
				   _imageInfo->hMemDC,
				   _imageInfo->currentFrameX * _imageInfo->frameWidth, _imageInfo->currentFrameY * _imageInfo->frameHeight,
				   _imageInfo->frameWidth, _imageInfo->frameHeight,
				   SRCCOPY);
		GdiTransparentBlt(hDC, (_shouldRenderUsingWindowCoords ? destX : (destX - _currOrg.x)),
						  (_shouldRenderUsingWindowCoords ? destY : (destY - _currOrg.y)),
						  _imageInfo->frameWidth, _imageInfo->frameHeight,
						  _blendImageInfo->hMemDC, 0, 0, _imageInfo->frameWidth, _imageInfo->frameHeight,
						  _transparentColor);
	}
	else
	{
		StretchBlt(hDC, (_shouldRenderUsingWindowCoords ? destX : (destX - _currOrg.x)) + _imageInfo->frameWidth - 1,
				   (_shouldRenderUsingWindowCoords ? destY : (destY - _currOrg.y)) + _imageInfo->frameHeight - 1,
				   -_imageInfo->frameWidth, -_imageInfo->frameHeight,
				   _imageInfo->hMemDC, _imageInfo->currentFrameX * _imageInfo->frameWidth, _imageInfo->currentFrameY * _imageInfo->frameHeight,
				   _imageInfo->frameWidth, _imageInfo->frameHeight,
				   SRCCOPY);
	}
}

void image::adjFrameRender(HDC hDC, int destX, int destY, int currentFrameX, int currentFrameY, int adjWidth, int adjHeight)
{
	_imageInfo->currentFrameX = currentFrameX;
	_imageInfo->currentFrameY = currentFrameY;

	if (_hasTransparentColor)
	{
		GdiTransparentBlt(
			hDC,
			(_shouldRenderUsingWindowCoords ? destX : (destX - _currOrg.x)),
			(_shouldRenderUsingWindowCoords ? destY : (destY - _currOrg.y)),
			adjWidth,
			adjHeight,
			_imageInfo->hMemDC,
			_imageInfo->currentFrameX * _imageInfo->frameWidth,
			_imageInfo->currentFrameY * _imageInfo->frameHeight,
			_imageInfo->frameWidth,
			_imageInfo->frameHeight,
			_transparentColor
		);
	}
	else
	{
		SetStretchBltMode(hDC, HALFTONE);
		StretchBlt(hDC,
				   (_shouldRenderUsingWindowCoords ? destX : (destX - _currOrg.x)),
				   (_shouldRenderUsingWindowCoords ? destY : (destY - _currOrg.y)),
				   adjWidth,
				   adjHeight,
				   _imageInfo->hMemDC,
				   _imageInfo->currentFrameX * _imageInfo->frameWidth,
				   _imageInfo->currentFrameY * _imageInfo->frameHeight,
				   _imageInfo->frameWidth,
				   _imageInfo->frameHeight,
				   SRCCOPY
		);
	}
}

void image::alphaRender(HDC hDC, BYTE alpha)
{
	_blendFunc.SourceConstantAlpha = alpha;

	if (_hasTransparentColor)
	{
		BitBlt(_blendImageInfo->hMemDC, 0, 0, _imageInfo->width, _imageInfo->height,
			hDC, (_shouldRenderUsingWindowCoords ? 0 : _currOrg.x), (_shouldRenderUsingWindowCoords ? 0 : _currOrg.y), SRCCOPY);

		GdiTransparentBlt(_blendImageInfo->hMemDC, 0, 0, _imageInfo->width,
			_imageInfo->height, _imageInfo->hMemDC, 0, 0, _imageInfo->width,
			_imageInfo->height, _transparentColor);

		GdiAlphaBlend(hDC, (_shouldRenderUsingWindowCoords ? 0 : _currOrg.x), (_shouldRenderUsingWindowCoords ? 0 : _currOrg.y), _imageInfo->width,
			_imageInfo->height, _blendImageInfo->hMemDC, 0, 0,
			_imageInfo->width, _imageInfo->height, _blendFunc);
	}
	else
	{
		GdiAlphaBlend(hDC, (_shouldRenderUsingWindowCoords ? 0 : _currOrg.x), (_shouldRenderUsingWindowCoords ? 0 : _currOrg.y), _imageInfo->width,
			_imageInfo->height, _imageInfo->hMemDC, 0, 0,
			_imageInfo->width, _imageInfo->height, _blendFunc);
	}
}

void image::alphaRender(HDC hDC, int destX, int destY, BYTE alpha)
{
	_blendFunc.SourceConstantAlpha = alpha;

	if (_hasTransparentColor)
	{
		BitBlt(_blendImageInfo->hMemDC, 0, 0, _imageInfo->width, _imageInfo->height,
			hDC, (_shouldRenderUsingWindowCoords ? destX : (destX - _currOrg.x)), (_shouldRenderUsingWindowCoords ? destY : (destY - _currOrg.y)), SRCCOPY);

		GdiTransparentBlt(_blendImageInfo->hMemDC, 0, 0, _imageInfo->width,
			_imageInfo->height, _imageInfo->hMemDC, 0, 0, _imageInfo->width,
			_imageInfo->height, _transparentColor);

		GdiAlphaBlend(hDC, (_shouldRenderUsingWindowCoords ? destX : (destX - _currOrg.x)), (_shouldRenderUsingWindowCoords ? destY : (destY - _currOrg.y)), _imageInfo->width,
			_imageInfo->height, _blendImageInfo->hMemDC, 0, 0,
			_imageInfo->width, _imageInfo->height, _blendFunc);
	}
	else
	{
		GdiAlphaBlend(hDC, (_shouldRenderUsingWindowCoords ? destX : (destX - _currOrg.x)), (_shouldRenderUsingWindowCoords ? destY : (destY - _currOrg.y)), _imageInfo->width,
			_imageInfo->height, _imageInfo->hMemDC, 0, 0,
			_imageInfo->width, _imageInfo->height, _blendFunc);
	}
}

void image::alphaRender(HDC hDC, int destX, int destY, int srcX, int srcY, int srcWidth, int srcHeight, BYTE alpha)
{
	_blendFunc.SourceConstantAlpha = alpha;
	if (srcWidth == 0 && srcHeight == 0) { srcWidth = _imageInfo->width - srcX; srcHeight = _imageInfo->height - srcY; }

	if (_hasTransparentColor)
	{
		BitBlt(_blendImageInfo->hMemDC, 0, 0, srcWidth, srcHeight,
			   hDC, (_shouldRenderUsingWindowCoords ? destX : (destX - _currOrg.x)), (_shouldRenderUsingWindowCoords ? destY : (destY - _currOrg.y)), SRCCOPY);

		GdiTransparentBlt(_blendImageInfo->hMemDC, 0, 0, srcWidth, srcHeight,
						  _imageInfo->hMemDC, srcX, srcY, srcWidth, srcHeight,
						  _transparentColor);

		GdiAlphaBlend(hDC, (_shouldRenderUsingWindowCoords ? destX : (destX - _currOrg.x)), (_shouldRenderUsingWindowCoords ? destY : (destY - _currOrg.y)), srcWidth, srcHeight,
					  _blendImageInfo->hMemDC, 0, 0, srcWidth, srcHeight,
					  _blendFunc);
	}
	else
	{
		GdiAlphaBlend(hDC, (_shouldRenderUsingWindowCoords ? destX : (destX - _currOrg.x)), (_shouldRenderUsingWindowCoords ? destY : (destY - _currOrg.y)), srcWidth, srcHeight,
					  _imageInfo->hMemDC, srcX, srcY, srcWidth, srcHeight,
					  _blendFunc);
	}
}

void image::animRender(HDC hDC, int destX, int destY, animation* anim)
{
	render(hDC, destX, destY, anim->getFramePos().x, anim->getFramePos().y,
		anim->getFrameWidth(), anim->getFrameHeight());
}

void image::animRenderH(HDC hDC, int destX, int destY, animation * anim)
{
	renderH(hDC, destX, destY, anim->getFramePos().x, anim->getFramePos().y,
		anim->getFrameWidth(), anim->getFrameHeight());
}

void image::loopRender(HDC hDC, const LPRECT destArea, int offSetX, int offSetY)
{
	if (offSetX < 0) offSetX = _imageInfo->width + (offSetX % _imageInfo->width);
	if (offSetY < 0) offSetY = _imageInfo->height + (offSetY % _imageInfo->height);

	int tempSrcW{}, tempSrcH{};
	RECT tempDestRct{}, tempSrcRct{};

	int destAreaX = destArea->left;
	int destAreaY = destArea->top;
	int destAreaW = destArea->right - destAreaX;
	int destAreaH = destArea->bottom - destAreaY;

	for (int y = 0; y < destAreaH; y += tempSrcH)
	{
		tempSrcRct.top = (y + offSetY) % _imageInfo->height;
		tempSrcRct.bottom = _imageInfo->height;
		tempSrcH = tempSrcRct.bottom - tempSrcRct.top;

		if (y + tempSrcH > destAreaH)
		{
			tempSrcRct.bottom -= (y + tempSrcH) - destAreaH;
			tempSrcH = tempSrcRct.bottom - tempSrcRct.top;
		}

		tempDestRct.top = y + destAreaY;
		// tempDestRct.bottom = destRct.top + srcHeight;

		for (int x = 0; x < destAreaW; x += tempSrcW)
		{
			tempSrcRct.left = (x + offSetX) % _imageInfo->width;
			tempSrcRct.right = _imageInfo->width;
			tempSrcW = tempSrcRct.right - tempSrcRct.left;

			if (x + tempSrcW > destAreaW)
			{
				tempSrcRct.right -= (x + tempSrcW) - destAreaW;
				tempSrcW = tempSrcRct.right - tempSrcRct.left;
			}

			tempDestRct.left = x + destAreaX;
			// tempDestRct.right = destRct.left + srcWidth;

			render(hDC, tempDestRct.left, tempDestRct.top,
				tempSrcRct.left, tempSrcRct.top, tempSrcW, tempSrcH);
		}
	}
}

void image::loopRenderP(HDC hDC, const LPRECT destArea, const LPRECT srcArea, int offSetX, int offSetY)
{
	if (offSetX < 0) offSetX = srcArea->right + (offSetX % (srcArea->right - srcArea->left));
	if (offSetY < 0) offSetY = srcArea->bottom + (offSetY % (srcArea->bottom - srcArea->top));

	int tempSrcW{}, tempSrcH{};
	RECT tempDestRct{}, tempSrcRct{};

	int destAreaX = destArea->left;
	int destAreaY = destArea->top;
	int destAreaW = destArea->right - destAreaX;
	int destAreaH = destArea->bottom - destAreaY;

	for (int y = 0; y < destAreaH; y += tempSrcH)
	{
		tempSrcRct.top = srcArea->top + (y + offSetY) % (srcArea->bottom - srcArea->top);
		tempSrcRct.bottom = srcArea->top + (srcArea->bottom - srcArea->top);
		tempSrcH = tempSrcRct.bottom - tempSrcRct.top;

		if (y + tempSrcH > destAreaH)
		{
			tempSrcRct.bottom -= (y + tempSrcH) - destAreaH;
			tempSrcH = tempSrcRct.bottom - tempSrcRct.top;
		}

		tempDestRct.top = y + destAreaY;
		// tempDestRct.bottom = destRct.top + tempSrcH;

		for (int x = 0; x < destAreaW; x += tempSrcW)
		{
			tempSrcRct.left = srcArea->left + (x + offSetX) % (srcArea->right - srcArea->left);
			tempSrcRct.right = srcArea->left + (srcArea->right - srcArea->left);
			tempSrcW = tempSrcRct.right - tempSrcRct.left;

			if (x + tempSrcW > destAreaW)
			{
				tempSrcRct.right -= (x + tempSrcW) - destAreaW;
				tempSrcW = tempSrcRct.right - tempSrcRct.left;
			}

			tempDestRct.left = x + destAreaX;
			// tempDestRct.right = destRct.left + tempSrcW;

			render(hDC, tempDestRct.left, tempDestRct.top,
				tempSrcRct.left, tempSrcRct.top, tempSrcW, tempSrcH);
		}
	}
}

COLORREF image::changeColor(COLORREF originalColor, COLORREF newColor)
{
	HDC hTempDC = CreateCompatibleDC(_imageInfo->hMemDC);
	BITMAPINFO bI = { 0 };
	DWORD* bitmapData = nullptr;
	bI.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bI.bmiHeader.biBitCount = 32;
	bI.bmiHeader.biWidth = _imageInfo->width;
	bI.bmiHeader.biHeight = _imageInfo->height;
	bI.bmiHeader.biCompression = BI_RGB;
	bI.bmiHeader.biPlanes = 1;

	BYTE originalB = static_cast<BYTE>((originalColor >> 16) & 0xFF);
	BYTE originalG = static_cast<BYTE>((originalColor >> 8) & 0xFF);
	BYTE originalR = static_cast<BYTE>((originalColor) & 0xFF);

	BYTE newB = static_cast<BYTE>((newColor >> 16) & 0xFF);
	BYTE newG = static_cast<BYTE>((newColor >> 8) & 0xFF);
	BYTE newR = static_cast<BYTE>((newColor) & 0xFF);

	HBITMAP hTempBitmap = CreateDIBSection(hTempDC, &bI, DIB_RGB_COLORS, reinterpret_cast<void**>(&bitmapData), 0, 0);
	if (hTempBitmap == nullptr) return originalColor;
	HBITMAP hTempOBitmap = (HBITMAP)SelectObject(hTempDC, hTempBitmap);
	BitBlt(hTempDC, 0, 0, _imageInfo->width, _imageInfo->height, _imageInfo->hMemDC, 0, 0, SRCCOPY);
	size_t area = static_cast<size_t>(_imageInfo->width * _imageInfo->height);
	for (size_t i = 0; i < area; ++i)
	{
		BYTE* currPixelData = reinterpret_cast<BYTE*>(&bitmapData[i]);
		if ((currPixelData[0] == originalB) && (currPixelData[1] == originalG) && (currPixelData[2] == originalR))
		{
			currPixelData[0] = newB; currPixelData[1] = newG; currPixelData[2] = newR;
		}
	}
	BitBlt(_imageInfo->hMemDC, 0, 0, _imageInfo->width, _imageInfo->height, hTempDC, 0, 0, SRCCOPY);
	DeleteObject(SelectObject(hTempDC, hTempOBitmap));
	DeleteDC(hTempDC);
	return newColor;
}

COLORREF image::changeAllColors(COLORREF newColor, COLORREF exceptionalColor)
{
	HDC hTempDC = CreateCompatibleDC(_imageInfo->hMemDC);
	BITMAPINFO bI = { 0 };
	DWORD* bitmapData = nullptr;
	bI.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bI.bmiHeader.biBitCount = 32;
	bI.bmiHeader.biWidth = _imageInfo->width;
	bI.bmiHeader.biHeight = _imageInfo->height;
	bI.bmiHeader.biCompression = BI_RGB;
	bI.bmiHeader.biPlanes = 1;

	BYTE newB = static_cast<BYTE>((newColor >> 16) & 0xFF);
	BYTE newG = static_cast<BYTE>((newColor >> 8) & 0xFF);
	BYTE newR = static_cast<BYTE>(newColor & 0xFF);

	BYTE exceptB = static_cast<BYTE>((exceptionalColor >> 16) & 0xFF);
	BYTE exceptG = static_cast<BYTE>((exceptionalColor >> 8) & 0xFF);
	BYTE exceptR = static_cast<BYTE>(exceptionalColor & 0xFF);

	HBITMAP hTempBitmap = CreateDIBSection(hTempDC, &bI, DIB_RGB_COLORS, reinterpret_cast<void**>(&bitmapData), 0, 0);
	if (hTempBitmap == nullptr) return newColor;
	HBITMAP hTempOBitmap = (HBITMAP)SelectObject(hTempDC, hTempBitmap);
	BitBlt(hTempDC, 0, 0, _imageInfo->width, _imageInfo->height, _imageInfo->hMemDC, 0, 0, SRCCOPY);
	size_t area = static_cast<size_t>(_imageInfo->width * _imageInfo->height);
	for (size_t i = 0; i < area; ++i)
	{
		BYTE* currPixelData = reinterpret_cast<BYTE*>(&bitmapData[i]);
		if ((currPixelData[0] != exceptB) || (currPixelData[1] != exceptG) || (currPixelData[2] != exceptR))
		{
			currPixelData[0] = newB; currPixelData[1] = newG; currPixelData[2] = newR;
		}
	}
	BitBlt(_imageInfo->hMemDC, 0, 0, _imageInfo->width, _imageInfo->height, hTempDC, 0, 0, SRCCOPY);
	DeleteObject(SelectObject(hTempDC, hTempOBitmap));
	DeleteDC(hTempDC);
	return newColor;
}

image* image::copyNew() const
{
	image* newImg = new image;

	newImg->_imageInfo = new IMAGE_INFO;
	newImg->_imageInfo->resID = this->_imageInfo->resID;
	newImg->_imageInfo->hMemDC = CreateCompatibleDC(this->_imageInfo->hMemDC);
	newImg->_imageInfo->width = this->_imageInfo->width;
	newImg->_imageInfo->height = this->_imageInfo->height;
	newImg->_imageInfo->hBit = CreateCompatibleBitmap(this->_imageInfo->hMemDC, newImg->_imageInfo->width, newImg->_imageInfo->height); // 주의: newImg->_imageInfo->hMemDC를 this->_imageInfo->hMemDC 대신 사용하면 정상 작동을 하지 않는다. newImg->_imageInfo->hMemDC에 선택된 비트맵이 무엇인지 생각하면 이해하기 쉽다.
	newImg->_imageInfo->hOBit = (HBITMAP)SelectObject(newImg->_imageInfo->hMemDC, newImg->_imageInfo->hBit);
	if (!BitBlt(newImg->_imageInfo->hMemDC, 0, 0, newImg->_imageInfo->width, newImg->_imageInfo->height, this->_imageInfo->hMemDC, 0, 0, SRCCOPY))
	{
		DeleteObject(SelectObject(newImg->_imageInfo->hMemDC, newImg->_imageInfo->hOBit));
		DeleteDC(newImg->_imageInfo->hMemDC);
		return nullptr;
	}
	newImg->_imageInfo->currentFrameX = this->_imageInfo->currentFrameX;
	newImg->_imageInfo->currentFrameY = this->_imageInfo->currentFrameY;
	newImg->_imageInfo->maxFrameX = this->_imageInfo->maxFrameX;
	newImg->_imageInfo->maxFrameY = this->_imageInfo->maxFrameY;
	newImg->_imageInfo->frameWidth = this->_imageInfo->frameWidth;
	newImg->_imageInfo->frameHeight = this->_imageInfo->frameHeight;
	newImg->_imageInfo->loadType = this->_imageInfo->loadType;

	if (this->_fileName != nullptr)
	{
		size_t len = strlen(this->_fileName);
		newImg->_fileName = new CHAR[len + 1];
		strcpy_s(newImg->_fileName, len + 1, this->_fileName);
	}

	newImg->_hasTransparentColor = this->_hasTransparentColor;
	newImg->_transparentColor = this->_transparentColor;

	newImg->_blendFunc.BlendFlags = 0;
	newImg->_blendFunc.AlphaFormat = 0;
	newImg->_blendFunc.BlendOp = AC_SRC_OVER;

	newImg->_blendImageInfo = new IMAGE_INFO;
	newImg->_blendImageInfo->loadType = this->_blendImageInfo->loadType;
	newImg->_blendImageInfo->resID = this->_blendImageInfo->resID;
	newImg->_blendImageInfo->hMemDC = CreateCompatibleDC(this->_imageInfo->hMemDC);
	newImg->_blendImageInfo->hBit = CreateCompatibleBitmap(newImg->_imageInfo->hMemDC, newImg->_imageInfo->width, newImg->_imageInfo->height);
	newImg->_blendImageInfo->hOBit = (HBITMAP)SelectObject(newImg->_blendImageInfo->hMemDC, newImg->_blendImageInfo->hBit);
	newImg->_blendImageInfo->width = newImg->_imageInfo->width;
	newImg->_blendImageInfo->height = newImg->_imageInfo->height;

 	return newImg;
}
