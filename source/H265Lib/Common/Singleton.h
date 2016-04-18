#pragma once

template<typename T>
class Singleton
{
protected:
	Singleton()
	{

	};
	virtual ~Singleton()
	{
		
	}

public:
	static T& instance()
	{
		static T m_instance;
		return m_instance;
	}
};