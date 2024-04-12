#pragma once
template<typename T>
class Singleton
{
	static T* m_This;

public:
	static T* GetInst()
	{
		if (m_This == nullptr)
		{
			m_This = new T;
		}
		return m_This;
	}

	static void Destroy()
	{
		if (m_This != nullptr)
		{
			delete m_This;
			m_This = nullptr;
		}
	}

protected:
	Singleton()
	{
		void(*pFunc)(void) = &Destroy;
		atexit(pFunc);
	}
	~Singleton(){}
};

template<typename T>
T* Singleton<T>::m_This = nullptr;


