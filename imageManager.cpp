#include "stdafx.h"
#include "imageManager.h"
#include <algorithm>

imageManager::imageManager()
{
}


imageManager::~imageManager()
{
}

HRESULT imageManager::init()
{
	return S_OK;
}

void imageManager::release()
{
	delAll();
}

image* imageManager::add(string strKey, int width, int height)
{
	image* img = find(strKey);
	if (img) return img;

	img = new image;

	if (FAILED(img->init(width, height)))
	{
		SAFE_DEL(img);
		return nullptr;
	}

	_imageList.insert(make_pair(strKey, img));

	return img;
}

image* imageManager::add(string strKey, const char * fileName, int width, int height, BOOL hasTransparentColor, COLORREF transparentColor)
{
	image* img = find(strKey);
	if (img) return img;

	img = new image;

	if (FAILED(img->init(fileName, width, height, hasTransparentColor, transparentColor)))
	{
		SAFE_DEL(img);
		return nullptr;
	}

	_imageList.insert(make_pair(strKey, img));

	return img;
}

image* imageManager::addF(string strKey, const char * fileName, int width, int height, int frameX, int frameY, BOOL hasTransparentColor, COLORREF transparentColor)
{
	image* img = find(strKey);
	if (img) return img;

	img = new image;

	if (FAILED(img->init(fileName, width, height, frameX, frameY, hasTransparentColor, transparentColor)))
	{
		SAFE_DEL(img);
		return nullptr;
	}

	_imageList.insert(make_pair(strKey, img));

	return img;
}


image* imageManager::addF(string strKey, const char* fileName, int x, int y, int width, int height, int frameX, int frameY, BOOL hasTransparentColor, COLORREF transparentColor)
{
	image* img = find(strKey);
	if (img) return img;

	img = new image;

	if (FAILED(img->init(fileName, x, y, width, height, frameX, frameY, hasTransparentColor, transparentColor)))
	{
		SAFE_DEL(img);
		return nullptr;
	}

	_imageList.insert(make_pair(strKey, img));

	return img;
}


image* imageManager::find(string strKey)
{
	imageListIter key = _imageList.find(strKey);

	if (key != _imageList.end())
	{
		return key->second;
	}

	return nullptr;
}

BOOL imageManager::del(string strKey)
{
	imageListIter key = _imageList.find(strKey);

	if (key != _imageList.end())
	{
		key->second->release();
		SAFE_DEL(key->second);
		_imageList.erase(key);
		return TRUE;
	}

	return FALSE;
}

BOOL imageManager::delAll()
{
	imageListIter iter = _imageList.begin();

	for (; iter != _imageList.end();)
	{
		if (iter->second != NULL)
		{
			iter->second->release();
			SAFE_DEL(iter->second);
			iter = _imageList.erase(iter);
		}
		else ++iter;
	}
	_imageList.clear();
	return TRUE;
}

void imageManager::render(string strKey, HDC hDC)
{
	image* img = find(strKey);
	if (img) img->render(hDC);
}

void imageManager::render(string strKey, HDC hDC, int destX, int destY)
{
	image* img = find(strKey);
	if (img) img->render(hDC, destX, destY);
}

void imageManager::render(string strKey, HDC hDC, int destX, int destY, int srcX, int srcY, int srcWidth, int srcHeight)
{
	image* img = find(strKey);
	if (img) img->render(hDC, destX, destY, srcX, srcY, srcWidth, srcHeight);
}

void imageManager::renderH(string strKey, HDC hDC, int destX, int destY, int srcX, int srcY, int srcWidth, int srcHeight)
{
	image* img = find(strKey);
	if (img) img->renderH(hDC, destX, destY, srcX, srcY, srcWidth, srcHeight);
}

void imageManager::renderV(string strKey, HDC hDC, int destX, int destY, int srcX, int srcY, int srcWidth, int srcHeight)
{
	image* img = find(strKey);
	if (img) img->renderV(hDC, destX, destY, srcX, srcY, srcWidth, srcHeight);
}

void imageManager::renderHV(string strKey, HDC hDC, int destX, int destY, int srcX, int srcY, int srcWidth, int srcHeight)
{
	image* img = find(strKey);
	if (img) img->renderHV(hDC, destX, destY, srcX, srcY, srcWidth, srcHeight);
}

void imageManager::frameRender(string strKey, HDC hDC, int destX, int destY)
{
	image* img = find(strKey);
	if (img) img->frameRender(hDC, destX, destY);
}

void imageManager::frameRender(string strKey, HDC hDC, int destX, int destY, int currentFrameX, int currentFrameY)
{
	image* img = find(strKey);
	if (img) img->frameRender(hDC, destX, destY, currentFrameX, currentFrameY);
}

void imageManager::frameRenderH(string strKey, HDC hDC, int destX, int destY, int currentFrameX, int currentFrameY)
{
	image* img = find(strKey);
	if (img) img->frameRenderH(hDC, destX, destY, currentFrameX, currentFrameY);
}

void imageManager::frameRenderV(string strKey, HDC hDC, int destX, int destY, int currentFrameX, int currentFrameY)
{
	image* img = find(strKey);
	if (img) img->frameRenderV(hDC, destX, destY, currentFrameX, currentFrameY);
}

void imageManager::frameRenderHV(string strKey, HDC hDC, int destX, int destY, int currentFrameX, int currentFrameY)
{
	image* img = find(strKey);
	if (img) img->frameRenderHV(hDC, destX, destY, currentFrameX, currentFrameY);
}

void imageManager::adjFrameRender(string strKey, HDC hDC, int destX, int destY, int currentFrameX, int currentFrameY, int adjWidth, int adjHeight)
{
	image* img = find(strKey);
	if (img) img->adjFrameRender(hDC, destX, destY, currentFrameX, currentFrameY, adjWidth, adjHeight);
}

void imageManager::alphaRender(string strKey, HDC hDC, BYTE alpha)
{
	image* img = find(strKey);
	if (img) img->alphaRender(hDC, alpha);
}

void imageManager::alphaRender(string strKey, HDC hDC, int destX, int destY, BYTE alpha)
{
	image* img = find(strKey);
	if (img) img->alphaRender(hDC, destX, destY, alpha);
}

void imageManager::alphaRender(string strKey, HDC hDC, int destX, int destY, int srcX, int srcY, int srcWidth, int srcHeight, BYTE alpha)
{
	image* img = find(strKey);
	if (img) img->alphaRender(hDC, destX, destY, srcX, srcY, srcWidth, srcHeight, alpha);
}

void imageManager::animRender(string strKey, HDC hDC, int destX, int destY, animation* anim)
{
	image* img = find(strKey);
	if (img) img->animRender(hDC, destX, destY, anim);
}

void imageManager::animRenderH(string strKey, HDC hDC, int destX, int destY, animation * anim)
{
	image* img = find(strKey);
	if (img) img->animRenderH(hDC, destX, destY, anim);
}

void imageManager::loopRender(string strKey, HDC hDC, const LPRECT drawArea, int offSetX, int offSetY)
{
	image* img = find(strKey);
	if (img) img->loopRender(hDC, drawArea, offSetX, offSetY);
}

void imageManager::setRctClipRgn(HDC hDC, int x, int y, int width, int height)
{
	_hClipRgn = CreateRectRgn(x, y, x + width, y + height);
	SelectClipRgn(hDC, _hClipRgn);
}

void imageManager::setEllipticClipRgn(HDC hDC, int x, int y, int width, int height)
{
	_hClipRgn = CreateEllipticRgn(x, y, x + width, y + height);
	SelectClipRgn(hDC, _hClipRgn);
}

void imageManager::resetClipRgn(HDC hDC)
{
	SelectClipRgn(hDC, nullptr);
}

void imageManager::execZ()
{
	if (!_funcListKeys.empty()) _funcListKeys.clear();
	if (_funcsToCall.empty()) return;
	_funcListKeys.reserve(_funcsToCall.size());

	for (auto& iter : _funcsToCall)
	{
		_funcListKeys.push_back(iter.first);
	}

	sort(_funcListKeys.begin(), _funcListKeys.end());

	for (LONG& key : _funcListKeys)
	{
		auto iterRange = _funcsToCall.equal_range(key);
		for (auto& it = iterRange.first; it != iterRange.second; ++it)
		{
			it->second();
		}
	}
	_funcsToCall.clear();
}

void imageManager::takeScreenshot(HDC hDC, BOOL shouldSaveAsImage, int x, int y, int width, int height)
{
	HDC hTempDC = CreateCompatibleDC(hDC);

	if (shouldSaveAsImage)
	{
		BITMAPINFOHEADER bitmapInfoHeader{ 0 };

		bitmapInfoHeader.biSize = sizeof(BITMAPINFOHEADER);
		bitmapInfoHeader.biWidth = width;
		bitmapInfoHeader.biHeight = height;
		bitmapInfoHeader.biPlanes = 1;
		bitmapInfoHeader.biBitCount = 32;
		bitmapInfoHeader.biCompression = BI_RGB; // ????????? RGB
		bitmapInfoHeader.biSizeImage = 0; // ????????? RGB ??????????????? 0??? ??????.
		bitmapInfoHeader.biXPelsPerMeter = 0;
		bitmapInfoHeader.biYPelsPerMeter = 0;
		bitmapInfoHeader.biClrUsed = 0; // 0?????? ???????????? '2??? biBitCount ??????' ?????? ????????????.
		bitmapInfoHeader.biClrImportant = 0; // ?????? ?????? ???????????? 0?????? ????????????.

		BITMAPINFO& bI = *(LPBITMAPINFO)&bitmapInfoHeader;

		//// BITMAPINFO bI{ 0 };
		//// bI.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		//// bI.bmiHeader.biBitCount = 32;
		//// bI.bmiHeader.biWidth = width;
		//// bI.bmiHeader.biHeight = height;
		//// bI.bmiHeader.biCompression = BI_RGB; // ????????? RGB
		//// bI.bmiHeader.biPlanes = 1;

		BYTE* bitmapData = nullptr;

		SIZE_T bitmapSize = static_cast<SIZE_T>(width) * height * 4;
		SIZE_T dIBSize = static_cast<SIZE_T>(sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + bitmapSize);

		HBITMAP hTempBitmap = CreateDIBSection(hTempDC, &bI, DIB_RGB_COLORS, reinterpret_cast<void**>(&bitmapData), 0, 0);
		HBITMAP hOTempBitmap;
		if (hTempBitmap != nullptr) hOTempBitmap = (HBITMAP)SelectObject(hTempDC, hTempBitmap);
		else
		{
			DeleteDC(hTempDC);
			return;
		}
		BitBlt(hTempDC, 0, 0, width, height, hDC, x, y, SRCCOPY);

		BITMAPFILEHEADER bitmapFileHeader{ 0 };

		bitmapFileHeader.bfOffBits = static_cast<DWORD>(sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER));
		bitmapFileHeader.bfSize = static_cast<DWORD>(dIBSize);
		bitmapFileHeader.bfType = 0x4D42; // 0x4D42??? 'BM'??? ????????????.

		//// HGLOBAL hDIB = GlobalAlloc(GMEM_MOVEABLE, dIBSize); // 32 ?????? ??????????????? GlobalAlloc??? HeapAlloc??? wrapper function??????. GMEM_MOVEABLE??? ???????????? ????????? ??? ??? ?????? ????????? ????????? ????????????.
		//// if (hDIB)
		{
			//// BYTE* dIB = (BYTE*)GlobalLock(hDIB); // ????????? ???????????? ??????????????? GlobalLock??? ??????????????? ??????.

			//// copy_n(reinterpret_cast<BYTE*>(&bitmapFileHeader), sizeof(BITMAPFILEHEADER), dIB);
			//// copy_n(reinterpret_cast<BYTE*>(&bitmapInfoHeader), sizeof(BITMAPINFOHEADER), dIB + sizeof(BITMAPFILEHEADER));
			//// copy_n(bitmapData, bitmapSize, dIB + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER));

			SYSTEMTIME localTime{0};
			GetLocalTime(&localTime);

			string fileName = "Screenshot " + to_string(localTime.wYear) + "??? " + to_string(localTime.wMonth) + "??? " + to_string(localTime.wDay) + "??? "
				+ to_string(localTime.wHour) + "??? " + to_string(localTime.wMinute) + "??? " + to_string(localTime.wSecond) + "??? " + to_string(localTime.wMilliseconds) + ".bmp";

			HANDLE hFile = CreateFile(fileName.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

			//// WriteFile(hFile, dIB, static_cast<DWORD>(dIBSize), NULL, NULL);

			WriteFile(hFile, reinterpret_cast<BYTE*>(&bitmapFileHeader), static_cast<DWORD>(sizeof(BITMAPFILEHEADER)), NULL, NULL);
			WriteFile(hFile, reinterpret_cast<BYTE*>(&bitmapInfoHeader), static_cast<DWORD>(sizeof(BITMAPINFOHEADER)), NULL, NULL);
			WriteFile(hFile, bitmapData, static_cast<DWORD>(bitmapSize), NULL, NULL);

			CloseHandle(hFile); // ?????? ????????? ???????????? CloseHandle??? ??????????????? ??????.

			//// GlobalUnlock(hDIB); // GlobalLock??? ?????????????????? GlobalUnlock??? ??????????????? ??????.
			//// GlobalFree(hDIB); // GlobalAlloc??? ?????????????????? GlobalFree??? ??????????????? ??????.
		}

		DeleteObject(SelectObject(hTempDC, hOTempBitmap));
	}
	else
	{
		//HBITMAP hTempBitmap = CreateBitmap(width, height, 1, 32, NULL);
		HBITMAP hTempBitmap = CreateCompatibleBitmap(hDC, width, height);
		HBITMAP hOTempBitmap = (HBITMAP)SelectObject(hTempDC, hTempBitmap);
		BitBlt(hTempDC, 0, 0, width, height, hDC, x, y, SRCCOPY);
		SelectObject(hTempDC, hOTempBitmap); // ??????: ????????? ???????????? DC??? ???????????? ????????? ??? ????????? ??? ?????? ????????????.

		if (OpenClipboard(NULL))
		{
			EmptyClipboard();
			SetClipboardData(CF_BITMAP, hTempBitmap);
			CloseClipboard();
		}
		DeleteObject(hTempBitmap);
	}

	DeleteDC(hTempDC);
}