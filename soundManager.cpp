#include "stdafx.h"
#include "soundManager.h"

soundManager::soundManager(): _system(nullptr), _channel(nullptr), _sound(nullptr) {}

soundManager::~soundManager(){}

HRESULT soundManager::init()
{
	System_Create(&_system);

	_system->init(TOTALSOUNDBUFFER, FMOD_INIT_NORMAL, NULL);
	_sound = new Sound*[TOTALSOUNDBUFFER];
	_channel = new Channel*[TOTALSOUNDBUFFER];

	memset(_sound, 0, sizeof(Sound*) * TOTALSOUNDBUFFER);
	memset(_channel, 0, sizeof(Channel*) * TOTALSOUNDBUFFER);
	return S_OK;
}

void soundManager::release()
{
	if (_system != nullptr)
	{
		_system->release();
		_system->close();
	}
	delete[] _sound;
	delete[] _channel;
}

void soundManager::update()
{
	_system->update();
}

Sound** soundManager::findSound(string strKey)
{
	_soundListIter = _soundList.find(strKey);
	if (_soundListIter != _soundList.end())
	{
		return _soundListIter->second;
	}
	return nullptr;
}

Channel * soundManager::findChannel(string strKey)
{
	_soundListIter = _soundList.begin();

	int count = 0;

	for (_soundListIter; _soundListIter != _soundList.end(); ++_soundListIter, ++count)
	{
		if (strKey == _soundListIter->first)
		{
			return _channel[count];
		}
	}
	return nullptr;
}

void soundManager::addSound(string strKey, string soundFileName, bool isBGM, bool isLoop)
{
	if (isLoop)
	{
		if (isBGM) _system->createStream(soundFileName.c_str(), FMOD_LOOP_NORMAL, NULL, &_sound[_soundList.size()]);
		else _system->createSound(soundFileName.c_str(), FMOD_LOOP_NORMAL, NULL, &_sound[_soundList.size()]);
	}
	else
	{
		if (isBGM) _system->createStream(soundFileName.c_str(), FMOD_DEFAULT, NULL, &_sound[_soundList.size()]);
		else _system->createSound(soundFileName.c_str(), FMOD_DEFAULT, NULL, &_sound[_soundList.size()]);
	}

	_soundList.insert(make_pair(strKey, &_sound[_soundList.size()]));
	_bgmOrNotList.insert(make_pair(strKey, isBGM));
}

void soundManager::play(string strKey, float volume)
{
	_soundListIter = _soundList.begin();

	int count = 0;

	for (_soundListIter; _soundListIter != _soundList.end(); ++_soundListIter, ++count)
	{
		if (strKey == _soundListIter->first)
		{
			_system->playSound(FMOD_CHANNEL_FREE, *_soundListIter->second, false, &_channel[count]);
			_channel[count]->setVolume(volume);
			break;
		}
	}
}

void soundManager::stop(string strKey)
{
	_soundListIter = _soundList.begin();

	int count = 0;

	for (_soundListIter; _soundListIter != _soundList.end(); ++_soundListIter, ++count)
	{
		if (strKey == _soundListIter->first)
		{
			_channel[count]->stop();
			break;
		}
	}
}

void soundManager::pause(string strKey)
{
	_soundListIter = _soundList.begin();

	int count = 0;

	for (_soundListIter; _soundListIter != _soundList.end(); ++_soundListIter, ++count)
	{
		if (strKey == _soundListIter->first)
		{
			_channel[count]->setPaused(true);
			break;
		}
	}
}

void soundManager::resume(string strKey)
{
	_soundListIter = _soundList.begin();

	int count = 0;

	for (_soundListIter; _soundListIter != _soundList.end(); ++_soundListIter, ++count)
	{
		if (strKey == _soundListIter->first)
		{
			_channel[count]->setPaused(false);
			break;
		}
	}
}

bool soundManager::isPlaying(string strKey)
{
	bool isPlaying = false;
	_soundListIter = _soundList.begin();

	int count = 0;

	for (_soundListIter; _soundListIter != _soundList.end(); ++_soundListIter, ++count)
	{
		if (strKey == _soundListIter->first)
		{
			_channel[count]->isPlaying(&isPlaying);
			break;
		}
	}

	return isPlaying;
}

bool soundManager::isPaused(string strKey)
{
	bool isPaused = false;
	_soundListIter = _soundList.begin();
	int count = 0;
	for (_soundListIter; _soundListIter != _soundList.end(); ++_soundListIter, ++count)
	{
		if (strKey == _soundListIter->first)
		{
			_channel[count]->getPaused(&isPaused);
			break;
		}
	}

	return isPaused;
}

void soundManager::pauseAll()
{
	_soundListIter = _soundList.begin();
	int count = 0;
	bool isPlaying;
	for (_soundListIter; _soundListIter != _soundList.end(); ++_soundListIter, ++count)
	{
		_channel[count]->isPlaying(&isPlaying);
		if (isPlaying) _channel[count]->setPaused(true);
	}
}

void soundManager::resumeAll()
{
	_soundListIter = _soundList.begin();
	int count = 0;
	bool isPaused;
	for (_soundListIter; _soundListIter != _soundList.end(); ++_soundListIter, ++count)
	{
		_channel[count]->getPaused(&isPaused);
		if (isPaused) _channel[count]->setPaused(false);
	}
}

void soundManager::stopAll()
{
	_soundListIter = _soundList.begin();
	int count = 0;
	bool isPlaying;
	for (_soundListIter; _soundListIter != _soundList.end(); ++_soundListIter, ++count)
	{
		_channel[count]->isPlaying(&isPlaying);
		if (isPlaying) _channel[count]->stop();
	}
}

void soundManager::updateMasterVolume()
{
	_soundListIter = _soundList.begin();
	int count = 0;
	bool isPlaying;
	for (_soundListIter; _soundListIter != _soundList.end(); ++_soundListIter, ++count)
	{
		_channel[count]->isPlaying(&isPlaying);
		if (!isPlaying) continue;
		if (_bgmOrNotList.find(_soundListIter->first)->second) _channel[count]->setVolume(_currMasterVolume * _currBGMVolume);
		else _channel[count]->setVolume(_currMasterVolume * _currSFXVolume);
	}
}

void soundManager::updateBGMVolume()
{
	_soundListIter = _soundList.begin();
	int count = 0;
	bool isPlaying;
	for (_soundListIter; _soundListIter != _soundList.end(); ++_soundListIter, ++count)
	{
		_channel[count]->isPlaying(&isPlaying);
		if (!isPlaying) continue;
		if (_bgmOrNotList.find(_soundListIter->first)->second) _channel[count]->setVolume(_currMasterVolume * _currBGMVolume);
	}
}

void soundManager::updateSFXVolume()
{
	_soundListIter = _soundList.begin();
	int count = 0;
	bool isPlaying;
	for (_soundListIter; _soundListIter != _soundList.end(); ++_soundListIter, ++count)
	{
		_channel[count]->isPlaying(&isPlaying);
		if (!isPlaying) continue;
		if (!_bgmOrNotList.find(_soundListIter->first)->second) _channel[count]->setVolume(_currMasterVolume * _currSFXVolume);
	}
}