#include "stdafx.h"
#include "keyAnimManager.h"
#include "image.h"
#include "animation.h"

HRESULT keyAnimManager::init()
{
	return S_OK;
}

void keyAnimManager::release()
{
	delAll();
}

void keyAnimManager::update()
{
	animListIter iter = _animList.begin();

	for (iter; iter != _animList.end(); ++iter)
	{
		if (!iter->second->isPlay()) continue;
		iter->second->frameUpdate(TIME->getElapsedTime() * 1.0f);
	}
}

void keyAnimManager::render()
{
}

void keyAnimManager::addDefaultFrameAnimation(string animationKeyName, const char * imageKeyName, int fps, bool reverse, bool loop)
{
	image* img = IMG->find(imageKeyName);
	animation* anim = new animation;

	anim->init(img->getWidth(), img->getHeight(), img->getFrameWidth(), img->getFrameHeight());
	anim->setDefPlaylist(reverse, loop);
	anim->setFPS(fps);

	_animList.insert(pair<string, animation*>(animationKeyName, anim));
}

void keyAnimManager::addDefaultFrameAnimation(string animationKeyName, const char * imageKeyName, int fps, bool reverse, bool loop, void * cbFunction)
{
	image* img = IMG->find(imageKeyName);
	animation* anim = new animation;

	anim->init(img->getWidth(), img->getHeight(), img->getFrameWidth(), img->getFrameHeight());
	anim->setDefPlaylist(reverse, loop, CALLBACK_FUNCTION(cbFunction));
	anim->setFPS(fps);

	_animList.insert(pair<string, animation*>(animationKeyName, anim));
}

void keyAnimManager::addDefaultFrameAnimation(string animationKeyName, const char * imageKeyName, int fps, bool reverse, bool loop, void * cbFunction, void * obj)
{
	image* img = IMG->find(imageKeyName);
	animation* anim = new animation;

	anim->init(img->getWidth(), img->getHeight(), img->getFrameWidth(), img->getFrameHeight());
	anim->setDefPlaylist(reverse, loop, CALLBACK_FUNCTION_PARAMETER(cbFunction), obj);
	anim->setFPS(fps);

	_animList.insert(pair<string, animation*>(animationKeyName, anim));
}

void keyAnimManager::addArrayFrameAnimation(string animationKeyName, const char * imageKeyName, int * arr, int arrLen, int fps, bool loop)
{
	image* img = IMG->find(imageKeyName);
	animation* anim = new animation;

	anim->init(img->getWidth(), img->getHeight(), img->getFrameWidth(), img->getFrameHeight());
	anim->setPlaylist(arr, arrLen, loop);
	anim->setFPS(fps);

	_animList.insert(pair<string, animation*>(animationKeyName, anim));
}

void keyAnimManager::addArrayFrameAnimation(string animationKeyName, const char * imageKeyName, int * arr, int arrLen, int fps, bool loop, void * cbFunction)
{
	image* img = IMG->find(imageKeyName);
	animation* anim = new animation;

	anim->init(img->getWidth(), img->getHeight(), img->getFrameWidth(), img->getFrameHeight());
	anim->setPlaylist(arr, arrLen, loop, CALLBACK_FUNCTION(cbFunction));
	anim->setFPS(fps);

	_animList.insert(pair<string, animation*>(animationKeyName, anim));
}

void keyAnimManager::addArrayFrameAnimation(string animationKeyName, const char * imageKeyName, int * arr, int arrLen, int fps, bool loop, void * cbFunction, void * obj)
{
	image* img = IMG->find(imageKeyName);
	animation* anim = new animation;

	anim->init(img->getWidth(), img->getHeight(), img->getFrameWidth(), img->getFrameHeight());
	anim->setPlaylist(arr, arrLen, loop, CALLBACK_FUNCTION_PARAMETER(cbFunction), obj);
	anim->setFPS(fps);

	_animList.insert(pair<string, animation*>(animationKeyName, anim));
}

void keyAnimManager::addCoordinateFrameAnimation(string animationKeyName, const char * imageKeyName, int start, int end, int fps, bool reverse, bool loop)
{
	image* img = IMG->find(imageKeyName);
	animation* anim = new animation;

	anim->init(img->getWidth(), img->getHeight(), img->getFrameWidth(), img->getFrameHeight());
	anim->setPlaylist(start, end, reverse, loop);
	anim->setFPS(fps);

	_animList.insert(pair<string, animation*>(animationKeyName, anim));
}

void keyAnimManager::addCoordinateFrameAnimation(string animationKeyName, const char * imageKeyName, int start, int end, int fps, bool reverse, bool loop, void * cbFunction)
{
	image* img = IMG->find(imageKeyName);
	animation* anim = new animation;

	anim->init(img->getWidth(), img->getHeight(), img->getFrameWidth(), img->getFrameHeight());
	anim->setPlaylist(start, end, reverse, loop, CALLBACK_FUNCTION(cbFunction));
	anim->setFPS(fps);

	_animList.insert(pair<string, animation*>(animationKeyName, anim));
}

void keyAnimManager::addCoordinateFrameAnimation(string animationKeyName, const char * imageKeyName, int start, int end, int fps, bool reverse, bool loop, void * cbFunction, void * obj)
{
	image* img = IMG->find(imageKeyName);
	animation* anim = new animation;

	anim->init(img->getWidth(), img->getHeight(), img->getFrameWidth(), img->getFrameHeight());
	anim->setPlaylist(start, end, reverse, loop, CALLBACK_FUNCTION_PARAMETER(cbFunction), obj);
	anim->setFPS(fps);

	_animList.insert(pair<string, animation*>(animationKeyName, anim));
}

void keyAnimManager::start(string animationKeyName)
{
	animListIter iter = _animList.find(animationKeyName);

	iter->second->start();
}

void keyAnimManager::stop(string animationKeyName)
{
	animListIter iter = _animList.find(animationKeyName);

	iter->second->stop();
}

void keyAnimManager::pause(string animationKeyName)
{
	animListIter iter = _animList.find(animationKeyName);

	iter->second->pause();
}

void keyAnimManager::resume(string animationKeyName)
{
	animListIter iter = _animList.find(animationKeyName);

	iter->second->resume();
}

animation* keyAnimManager::findAnimation(string animationKeyName)
{
	animListIter iter = _animList.find(animationKeyName);

	if (iter != _animList.end()) return iter->second;

	return nullptr;
}

void keyAnimManager::delAll()
{
	for (animListIter iter = _animList.begin(); iter != _animList.end();)
	{
		if (iter->second != NULL)
		{
			iter->second->release();
			SAFE_DEL(iter->second);
			iter = _animList.erase(iter);
		}
		else ++iter;
	}

	_animList.clear();
}
