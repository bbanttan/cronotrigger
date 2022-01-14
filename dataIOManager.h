#pragma once
#include "singleton.h"

struct tagIniDatum
{
	const char* subject;
	const char* title;
	const char* body;
};

class dataIOManager : public singleton<dataIOManager>
{
private:
	typedef vector<tagIniDatum>				iniDatum;
	typedef vector<tagIniDatum>::iterator	iniDatumIter;

	typedef vector<iniDatum>				iniData;
	typedef vector<iniDatum>::iterator	iniDataIter;

private:
	iniData _vIniData;


public:
	dataIOManager();
	~dataIOManager();

	HRESULT init();
	
	// 데이터 추가 함수
	void addData(const char* subject, const char* title, const char* body);

	// ini 파일 생성 함수
	void iniSave(const char* fileName);

	// 데이터를 문자열로 로딩하는 함수
	string loadDataString(const char* fileName, const char* subject, const char* title);

	// 데이터를 정수로 로딩하는 함수
	int loadDataInteger(const char* fileName, const char* subject, const char* title);

};

