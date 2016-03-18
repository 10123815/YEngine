#pragma once

#include "d3d_utility.h"

#include <d3d11.h>

namespace ysd_simple_engine
{
	class Texture
	{
	public:
		Texture( );
		~Texture( );

		void Load(WCHAR* file_name);
		void Release( );

		ID3D11ShaderResourceView* texture( ) { return p_texture_shader_res_; }

		void SetResource( );

	private:
		ID3D11ShaderResourceView* p_texture_shader_res_;
	};

}
