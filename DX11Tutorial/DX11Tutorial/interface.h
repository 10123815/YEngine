#pragma once

namespace ysd_simple_engine
{

	__interface ISystem
	{
		virtual void Initialize (HWND hwnd) = 0;
		virtual void ShutDown ( ) = 0;
	};

}