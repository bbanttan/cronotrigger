#pragma once
//SingletonBase

template <typename T>
class singleton
{
protected:
	static T* sgt;

	singleton() {}
	~singleton() {}
public:
	static T* getSingleton();
	void releaseSingleton();
};

template<typename T>
T* singleton<T>::sgt = nullptr;

template<typename T>
inline T* singleton<T>::getSingleton()
{
	if (sgt == nullptr) sgt = new T;
	return sgt;
}

template<typename T>
inline void singleton<T>::releaseSingleton()
{
	if (sgt)
	{
		delete sgt;
		sgt = nullptr;
	}
}
