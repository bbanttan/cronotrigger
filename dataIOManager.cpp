#include "stdafx.h"
#include "dataIOManager.h"

dataIOManager::dataIOManager()
{
}


dataIOManager::~dataIOManager()
{
}

HRESULT dataIOManager::init()
{
	return S_OK;
}

void dataIOManager::addData(const char * subject, const char * title, const char * body)
{
	tagIniDatum iniData;
	iniData.subject = subject;
	iniData.title = title;
	iniData.body = body;

	iniDatum vIniData;
	vIniData.push_back(iniData);

	_vIniData.push_back(vIniData);

}

void dataIOManager::iniSave(const char * fileName)
{
	char str[256];
	char dir[256];

	ZeroMemory(dir, sizeof(dir));
	sprintf_s(dir, "\\%s.ini", fileName);

	GetCurrentDirectory(256, str);
	strncat_s(str, 256, dir, 254);

	for (int i = 0; i < _vIniData.size(); i++)
	{
		iniDatum vData = _vIniData[i];
		WritePrivateProfileString(vData[0].subject, vData[0].title,
			vData[0].body, str);
		vData.clear();
	}

	_vIniData.clear();

}

string dataIOManager::loadDataString(const char * fileName, const char * subject, const char * title)
{
	char str[256];
	char dir[256];

	ZeroMemory(dir, sizeof(dir));
	sprintf_s(dir, "\\%s.ini", fileName);

	GetCurrentDirectory(256, str);
	strncat_s(str, 256, dir, 254);

	char data[64] = {};
	GetPrivateProfileString(subject, title, "", data, 64, str);

	string strData = data;


	return strData;
}

int dataIOManager::loadDataInteger(const char * fileName, const char * subject, const char * title)
{
	char str[256];
	char dir[256];

	ZeroMemory(dir, sizeof(dir));
	sprintf_s(dir, "\\%s.ini", fileName);

	GetCurrentDirectory(256, str);
	strncat_s(str, 256, dir, 254);

	
	return GetPrivateProfileInt(subject, title, 0, str);
}
