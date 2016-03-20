#pragma once

#include <D3DX10math.h>

namespace ysd_simple_engine
{
	class DirectLight
	{
	public:

		D3DXVECTOR3 direction( ) const { return direction_; };
		D3DXVECTOR4 color( ) const { return color_; }
		void set_dir(D3DXVECTOR3 dir) { direction_ = dir; }
		void set_color(D3DXVECTOR4 color) { color_ = color; }

	private:

		D3DXVECTOR3 direction_;
		D3DXVECTOR4 color_;

	};


}