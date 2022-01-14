#pragma once
#include "singleton.h"
#include <string>
#include <map>

class gameNode;

class sceneManager : public singleton<sceneManager>
{
public:
	typedef map<string, gameNode*> sceneList;
	typedef map<string, gameNode*>::iterator sceneListIter;

private:
	static gameNode* _currentScene;

	sceneList _sceneList;

public:
	HRESULT init();
	void release();
	void update();
	void render();

	// 장면 추가 함수
	gameNode* addScene(string sceneName, gameNode* scene);

	// 장면 변경 함수
	HRESULT changeScene(string sceneName);

	// 장면 제거 함수
	void delScene(string sceneName);

	gameNode* getScene(string sceneName)
	{
		sceneListIter _sceneListIter = _sceneList.find(sceneName);
		if (_sceneListIter == _sceneList.end()) return nullptr;
		else return _sceneListIter->second;
	}

	sceneManager() {};
	~sceneManager() {};
};

