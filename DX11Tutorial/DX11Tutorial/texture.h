#pragma once

#include <d3d11.h>

#include "Engine.h"

namespace ysd_simple_engine
{
	class Textrue
	{
	public:
		Textrue ( );
		~Textrue ( );

		void Load (WCHAR* file_name);
		void Release ( );

		std::shared_ptr<ID3D11ShaderResourceView> texture ( ) const { return p_texture_; }

	private:
		std::shared_ptr<ID3D11ShaderResourceView> p_texture_;
	};

}
