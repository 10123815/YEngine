#pragma once

#include <memory>

namespace ysd_simple_engine
{
	template <typename T>
	class Singleton
	{

	private:
		Singleton( ) {}
		~Singleton( ) {}

		static std::unique_ptr<T> instance_;

	public:
		inline static std::unique_ptr<T>& Instance( )
		{
			if (instance_.get( ) == 0)
			{
				instance_.reset(new T( ));
			}
			return instance_;
		}

	};

	template <typename T>
	std::unique_ptr<T> Singleton<T>::instance_;
}
