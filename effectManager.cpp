#include "stdafx.h"
#include "effectManager.h"
#include "effect.h"

effectManager::effectManager()
{
}


effectManager::~effectManager()
{
	release();
}

HRESULT effectManager::init()
{

	return S_OK;
}

void effectManager::release()
{
	totalEffectsIter vIter;
	effectGroupsIter mIter;
	sameEffectsIter vArrIter;

	for (vIter = _totEffects.begin(); vIter != _totEffects.end(); ++vIter)
	{
		for (mIter = vIter->begin(); mIter != vIter->end();)
		{
			if (mIter->second.size() != 0)
			{
				for (vArrIter = mIter->second.begin(); vArrIter != mIter->second.end();)
				{
					(*vArrIter)->release();
					delete *vArrIter;
					vArrIter = mIter->second.erase(vArrIter);
				}
				mIter->second.clear();
			}
			else ++mIter;
		}
	}
}

void effectManager::update()
{
	totalEffectsIter vIter;
	effectGroupsIter mIter;
	sameEffectsIter vArrIter;
	for (vIter = _totEffects.begin(); vIter != _totEffects.end(); ++vIter)
	{
		for (mIter = vIter->begin(); mIter != vIter->end(); ++mIter)
		{
			for (vArrIter = mIter->second.begin(); vArrIter != mIter->second.end(); ++vArrIter)
				(*vArrIter)->update();
		}
	}
}

void effectManager::render()
{
	totalEffectsIter vIter;
	effectGroupsIter mIter;
	sameEffectsIter vArrIter;

	for (vIter = _totEffects.begin(); vIter != _totEffects.end(); ++vIter)
	{
		for (mIter = vIter->begin(); mIter != vIter->end(); ++mIter)
		{
			for (vArrIter = mIter->second.begin(); vArrIter != mIter->second.end(); ++vArrIter)
				(*vArrIter)->render();
		}
	}
}


void effectManager::add(string effectName, const char* imageName, int imageWidth, int imageHeight, int effectWidth, int effectHeight, int fps, float elapsedTime, int buffer)
{
	image* img;
	sameEffects vEffectBuffer;
	effectGroups mArrEffect;

	if (IMG->find(imageName))
	{
		img = IMG->find(imageName);
	}
	else
	{
		img = IMG->add(imageName, imageName, imageWidth, imageHeight, true, RGB(255, 0, 255));
	}

	for (int i = 0; i < buffer; i++)
	{
		vEffectBuffer.push_back(new effect);
		vEffectBuffer[i]->init(img, effectWidth, effectHeight, fps, elapsedTime);
	}

	mArrEffect.insert(pair<string, sameEffects>(effectName, vEffectBuffer));

	_totEffects.push_back(mArrEffect);

}


void effectManager::play(string effectName, int x, int y)
{
	totalEffectsIter vIter;
	effectGroupsIter mIter;
	sameEffectsIter vArrIter;

	for (vIter = _totEffects.begin(); vIter != _totEffects.end(); ++vIter)
	{
		for (mIter = vIter->begin(); mIter != vIter->end(); ++mIter)
		{
			if (!(mIter->first == effectName)) break;
			for (vArrIter = mIter->second.begin(); vArrIter != mIter->second.end(); ++vArrIter)
			{
				if ((*vArrIter)->getIsRunning()) continue;

				(*vArrIter)->start(x, y);
				return;
			}
		}
	}
}


void effectManager::killAll()
{
	totalEffectsIter vIter;
	effectGroupsIter mIter;
	sameEffectsIter vArrIter;
	for (vIter = _totEffects.begin(); vIter != _totEffects.end(); ++vIter)
	{
		for (mIter = vIter->begin(); mIter != vIter->end(); ++mIter)
		{
			if (mIter->second.size() != 0)
			{
				for (vArrIter = mIter->second.begin(); vArrIter != mIter->second.end(); ++vArrIter)
				{
					if (!(*vArrIter)->getIsRunning()) continue;
					(*vArrIter)->kill();
				}
			}
		}
	}
}
