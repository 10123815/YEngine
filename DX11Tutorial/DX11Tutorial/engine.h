#pragma once

#pragma region Common

// User defined
#include "interface.h"
#include "singleton.h"

// STL
#include <memory>

#pragma endregion

#pragma region Graphic

#include "d3d_utility.h"
#include "Graphic.h"

#pragma endregion

namespace ysd_simple_engine
{
	class Engine : public ISystem
	{
	public:
		Engine ( );
		~Engine ( );

		static std::unique_ptr<Engine>& Instance ( )
		{
			return Singleton<Engine>::Instance ( );
		}

		void Update ( );

		// Í¨¹ý ISystem ¼Ì³Ð
		virtual void Initialize (HWND hwnd) override;
		virtual void ShutDown ( ) override;

	private:


	};

}
