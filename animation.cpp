#include "stdafx.h"
#include "animation.h"

animation::animation()
	: _totFrames(0),
	_frameWidth(0),
	_frameHeight(0),
	_loop(FALSE),
	_frameUpdateSec(0),
	_elapsedSec(0),
	_currPlayIndex(0),
	_play(FALSE)
{
}

animation::~animation()
{
	release();
}

HRESULT animation::init(int totalW, int totalH, int frameW, int frameH)
{
	_frameWidth = frameW;
	int _frameNumWidth = totalW / _frameWidth;

	_frameHeight = frameH;
	int _frameNumHeight = totalH / _frameHeight;

	_totFrames = _frameNumWidth * _frameNumHeight;

	for (int i = 0; i < _frameNumHeight; ++i)
	{
		for (int j = 0; j < _frameNumWidth; ++j)
		{
			POINT framePos;

			framePos.x = j * _frameWidth;
			framePos.y = i * _frameHeight;

			_frameList.push_back(framePos);
		}
	}

	setDefPlaylist();

	return S_OK;
}

void animation::release()
{
	_frameList.clear();
	_playlist.clear();
}

void animation::setDefPlaylist(BOOL reverse, BOOL loop)
{
	_obj = NULL;
	_callbackFunction = NULL;
	_callbackFunctionParameter = NULL;

	_loop = loop;
	_playlist.clear();

	if (reverse)
	{
		if (_loop)
		{
			for (int i = 0; i < _totFrames - 1; ++i)
			{
				_playlist.push_back(i);
			}

			for (int i = _totFrames - 1; i > 0; --i)
			{
				_playlist.push_back(i);
			}
		}
		else
		{
			for (int i = 0; i < _totFrames - 1; ++i)
			{
				_playlist.push_back(i);
			}

			for (int i = _totFrames - 1; i >= 0; --i)
			{
				_playlist.push_back(i);
			}
		}
	}
	else
	{
		if (_loop)
		{
			for (int i = 0; i < _totFrames - 1; ++i)
			{
				_playlist.push_back(i);
			}

		}
		else
		{
			for (int i = 0; i < _totFrames; ++i)
			{
				_playlist.push_back(i);
			}

		}
	}
}

void animation::setDefPlaylist(BOOL reverse, BOOL loop, CALLBACK_FUNCTION cbFunction)
{
	_obj = NULL;
	_callbackFunction = cbFunction;
	_callbackFunctionParameter = NULL;

	_loop = loop;
	_playlist.clear();

	if (reverse)
	{
		if (_loop)
		{
			for (int i = 0; i < _totFrames - 1; ++i)
			{
				_playlist.push_back(i);
			}

			for (int i = _totFrames - 1; i > 0; --i)
			{
				_playlist.push_back(i);
			}
		}
		else
		{
			for (int i = 0; i < _totFrames - 1; ++i)
			{
				_playlist.push_back(i);
			}

			for (int i = _totFrames - 1; i >= 0; --i)
			{
				_playlist.push_back(i);
			}
		}
	}
	else
	{
		if (_loop)
		{
			for (int i = 0; i < _totFrames - 1; ++i)
			{
				_playlist.push_back(i);
			}

		}
		else
		{
			for (int i = 0; i < _totFrames; ++i)
			{
				_playlist.push_back(i);
			}

		}
	}
}

void animation::setDefPlaylist(BOOL reverse, BOOL loop, CALLBACK_FUNCTION_PARAMETER cbFunction, void* obj)
{
	_obj = obj;
	_callbackFunction = NULL;
	_callbackFunctionParameter = cbFunction;

	_loop = loop;
	_playlist.clear();

	if (reverse)
	{
		if (_loop)
		{
			for (int i = 0; i < _totFrames - 1; ++i)
			{
				_playlist.push_back(i);
			}

			for (int i = _totFrames - 1; i > 0; --i)
			{
				_playlist.push_back(i);
			}
		}
		else
		{
			for (int i = 0; i < _totFrames - 1; ++i)
			{
				_playlist.push_back(i);
			}

			for (int i = _totFrames - 1; i >= 0; --i)
			{
				_playlist.push_back(i);
			}
		}
	}
	else
	{
		if (_loop)
		{
			for (int i = 0; i < _totFrames - 1; ++i)
			{
				_playlist.push_back(i);
			}

		}
		else
		{
			for (int i = 0; i < _totFrames; ++i)
			{
				_playlist.push_back(i);
			}

		}
	}
}

void animation::setPlaylist(int* playArr, int arrLen, BOOL loop)
{
	_obj = NULL;
	_callbackFunction = NULL;
	_callbackFunctionParameter = NULL;

	_loop = loop;

	_playlist.clear();

	for (int i = 0; i < arrLen; ++i)
	{
		_playlist.push_back(playArr[i]);
	}
}

void animation::setPlaylist(int* playArr, int arrLen, BOOL loop, CALLBACK_FUNCTION cbFunction)
{
	_obj = NULL;
	_callbackFunction = cbFunction;
	_callbackFunctionParameter = NULL;

	_loop = loop;

	_playlist.clear();

	for (int i = 0; i < arrLen; ++i)
	{
		_playlist.push_back(playArr[i]);
	}
}

void animation::setPlaylist(int* playArr, int arrLen, BOOL loop, CALLBACK_FUNCTION_PARAMETER cbFunction, void* obj)
{
	_obj = obj;
	_callbackFunction = NULL;
	_callbackFunctionParameter = cbFunction;

	_loop = loop;

	_playlist.clear();

	for (int i = 0; i < arrLen; ++i)
	{
		_playlist.push_back(playArr[i]);
	}
}

void animation::setPlaylist(int start, int end, BOOL reverse, BOOL loop)
{
	_obj = NULL;
	_callbackFunction = NULL;
	_callbackFunctionParameter = NULL;

	_loop = loop;

	_playlist.clear();

	if (start == end)
	{
		_playlist.clear();
		stop();
		return;
	}

	if (start > end)
	{
		if (reverse)
		{
			if (_loop)
			{
				for (int i = start; i >= end; --i)
				{
					_playlist.push_back(i);
				}

				for (int i = end + 1; i < start; ++i)
				{
					_playlist.push_back(i);
				}
			}
			else
			{
				for (int i = start; i >= end; --i)
				{
					_playlist.push_back(i);
				}

				for (int i = end + 1; i < start; ++i)
				{
					_playlist.push_back(i);
				}
			}
		}
		else
		{
			if (_loop)
			{
				for (int i = start; i >= end; --i)
				{
					_playlist.push_back(i);
				}
			}
			else
			{
				for (int i = start; i >= end; --i)
				{
					_playlist.push_back(i);
				}
			}
		}
	}
	else
	{
		if (reverse)
		{
			if (_loop)
			{
				for (int i = start; i < end; ++i)
				{
					_playlist.push_back(i);
				}

				for (int i = end - 1; i > start; --i)
				{
					_playlist.push_back(i);
				}
			}
			else
			{
				for (int i = start; i < end; ++i)
				{
					_playlist.push_back(i);
				}

				for (int i = end - 1; i > start; --i)
				{
					_playlist.push_back(i);
				}
			}
		}
		else
		{
			if (_loop)
			{
				for (int i = start; i < end; ++i)
				{
					_playlist.push_back(i);
				}
			}
			else
			{
				for (int i = start; i < end; ++i)
				{
					_playlist.push_back(i);
				}
			}
		}
	}

}

void animation::setPlaylist(int start, int end, BOOL reverse, BOOL loop, CALLBACK_FUNCTION cbFunction)
{
	_obj = NULL;
	_callbackFunction = cbFunction;
	_callbackFunctionParameter = NULL;

	_loop = loop;

	_playlist.clear();

	if (start == end)
	{
		_playlist.clear();
		stop();
		return;
	}

	if (start > end)
	{
		if (reverse)
		{
			if (_loop)
			{
				for (int i = start; i >= end; --i)
				{
					_playlist.push_back(i);
				}

				for (int i = end + 1; i < start; ++i)
				{
					_playlist.push_back(i);
				}
			}
			else
			{
				for (int i = start; i >= end; --i)
				{
					_playlist.push_back(i);
				}

				for (int i = end + 1; i < start; ++i)
				{
					_playlist.push_back(i);
				}
			}
		}
		else
		{
			if (_loop)
			{
				for (int i = start; i >= end; --i)
				{
					_playlist.push_back(i);
				}
			}
			else
			{
				for (int i = start; i >= end; --i)
				{
					_playlist.push_back(i);
				}
			}
		}
	}
	else
	{
		if (reverse)
		{
			if (_loop)
			{
				for (int i = start; i < end; ++i)
				{
					_playlist.push_back(i);
				}

				for (int i = end - 1; i > start; --i)
				{
					_playlist.push_back(i);
				}
			}
			else
			{
				for (int i = start; i < end; ++i)
				{
					_playlist.push_back(i);
				}

				for (int i = end - 1; i > start; --i)
				{
					_playlist.push_back(i);
				}
			}
		}
		else
		{
			if (_loop)
			{
				for (int i = start; i < end; ++i)
				{
					_playlist.push_back(i);
				}
			}
			else
			{
				for (int i = start; i < end; ++i)
				{
					_playlist.push_back(i);
				}
			}
		}
	}

}

void animation::setPlaylist(int start, int end, BOOL reverse, BOOL loop, CALLBACK_FUNCTION_PARAMETER cbFunction, void* obj)
{
	_obj = obj;
	_callbackFunction = NULL;
	_callbackFunctionParameter = cbFunction;

	_loop = loop;

	_playlist.clear();

	if (start == end)
	{
		_playlist.clear();
		stop();
		return;
	}

	if (start > end)
	{
		if (reverse)
		{
			if (_loop)
			{
				for (int i = start; i >= end; --i)
				{
					_playlist.push_back(i);
				}

				for (int i = end + 1; i < start; ++i)
				{
					_playlist.push_back(i);
				}
			}
			else
			{
				for (int i = start; i >= end; --i)
				{
					_playlist.push_back(i);
				}

				for (int i = end + 1; i < start; ++i)
				{
					_playlist.push_back(i);
				}
			}
		}
		else
		{
			if (_loop)
			{
				for (int i = start; i >= end; --i)
				{
					_playlist.push_back(i);
				}
			}
			else
			{
				for (int i = start; i >= end; --i)
				{
					_playlist.push_back(i);
				}
			}
		}
	}
	else
	{
		if (reverse)
		{
			if (_loop)
			{
				for (int i = start; i < end; ++i)
				{
					_playlist.push_back(i);
				}

				for (int i = end - 1; i > start; --i)
				{
					_playlist.push_back(i);
				}
			}
			else
			{
				for (int i = start; i < end; ++i)
				{
					_playlist.push_back(i);
				}

				for (int i = end - 1; i > start; --i)
				{
					_playlist.push_back(i);
				}
			}
		}
		else
		{
			if (_loop)
			{
				for (int i = start; i < end; ++i)
				{
					_playlist.push_back(i);
				}
			}
			else
			{
				for (int i = start; i < end; ++i)
				{
					_playlist.push_back(i);
				}
			}
		}
	}

}

void animation::setFPS(int framesPerSec)
{
	_frameUpdateSec = 1.0f / framesPerSec;
}

void animation::frameUpdate(float elapsedTime)
{
	if (_play)
	{
		_elapsedSec += elapsedTime;

		if (_elapsedSec >= _frameUpdateSec)
		{
			_elapsedSec -= _frameUpdateSec;
			++_currPlayIndex;

			if (_currPlayIndex >= _playlist.size())
			{
				if (_loop) _currPlayIndex = 0;
				else
				{
					if (_obj == NULL)
					{
						if (_callbackFunction != NULL) _callbackFunction();
					}
					else _callbackFunctionParameter(_obj);

					--_currPlayIndex;
					_play = FALSE;
				}
			}
		}
	}
}

void animation::start()
{
	_play = TRUE;
	_currPlayIndex = 0;
}

void animation::stop()
{
	_play = FALSE;
	_currPlayIndex = 0;
}

void animation::pause()
{
	_play = FALSE;
}

void animation::resume()
{
	_play = TRUE;
}
