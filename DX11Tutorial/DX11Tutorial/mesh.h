#pragma once

#include <D3D11.h>
#include <D3DX10math.h>

namespace ysd_simple_engine
{
	class Mesh
	{
	public:
		Mesh( );
		~Mesh( );

		void Release( )
		{
			if (vertices_)
			{
				delete[] vertices_;
				vertices_ = 0;
			}

			if (indices_)
			{
				delete[] indices_;
				indices_ = 0;
			}

		}

		bool set_vertices(D3DXVECTOR3* vertices, UINT* indices, UINT vt_count, UINT id_count);
		bool vertices(D3DXVECTOR3* v, UINT* i) const;

		bool set_colors(D3DXVECTOR4* colors, UINT count);
		bool colors(D3DXVECTOR4* color) const;

		UINT vertices_count( ) const { return vertices_count_; }
		UINT indices_count( ) const { return indices_count_; }

	private:

		// Vertices' position
		D3DXVECTOR3* vertices_;

		UINT vertices_count_;

		// Default color
		D3DXVECTOR4* colors_;

		// Texture coordinats
		D3DXVECTOR2* uv0_;

		// Second texture coordinates
		D3DXVECTOR2* uv1_;

		UINT* indices_;
		UINT indices_count_;

	};
}

