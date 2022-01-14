#include "stdafx.h"
#include "sceneManager.h"
#include "gameNode.h"

gameNode* sceneManager::_currentScene;

HRESULT sceneManager::init()
{
	_currentScene = nullptr;

	return S_OK;
}

void sceneManager::release()
{
	sceneListIter _sceneListIter = _sceneList.begin();
	for (; _sceneListIter != _sceneList.end();)
	{
		if (_sceneListIter->second != nullptr)
		{
			if (_sceneListIter->second == _currentScene) _sceneListIter->second->release();
			SAFE_DEL(_sceneListIter->second);
			_sceneListIter = _sceneList.erase(_sceneListIter);
		}
		else ++_sceneListIter;
	}
	_sceneList.clear();
}

void sceneManager::update()
{
	if (_currentScene) _currentScene->update();
}

void sceneManager::render()
{
	if (_currentScene) _currentScene->render();
}

gameNode* sceneManager::addScene(string sceneName, gameNode* scene)
{
	if (!scene) return nullptr;

	_sceneList.insert(make_pair(sceneName, scene));

	return scene;
}

HRESULT sceneManager::changeScene(string sceneName)
{
	sceneListIter _sceneListIter = _sceneList.find(sceneName);
	if (_sceneListIter == _sceneList.end()) return E_FAIL;
	if (_sceneListIter->second == _currentScene) return S_OK;
	if (SUCCEEDED(_sceneListIter->second->init()))
	{
		_sceneListIter = _sceneList.find(sceneName); // 지운 장면이 있을 수도 있으므로 다시 찾는다.
		if (_currentScene) _currentScene->release();
		_currentScene = _sceneListIter->second;
		return S_OK;
	}
	return E_FAIL;
}

void sceneManager::delScene(string sceneName)
{
	sceneListIter _sceneListIter = _sceneList.find(sceneName);
	if (_sceneListIter == _sceneList.end()) return;
	_sceneListIter->second->release();
	SAFE_DEL(_sceneListIter->second);
	_sceneList.erase(_sceneListIter);
	_currentScene = nullptr;
}