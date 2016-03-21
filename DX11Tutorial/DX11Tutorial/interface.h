#pragma once

namespace ysd_simple_engine
{

	class ISubSystem
	{
		virtual void Initialize (HWND hwnd) = 0;
		virtual void ShutDown ( ) = 0;
	};

}