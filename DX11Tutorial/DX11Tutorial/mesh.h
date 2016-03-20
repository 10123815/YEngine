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

			if (colors_)
			{
				delete[] colors_;
				colors_ = 0;
			}

			if (uv0_)
			{
				delete[] uv0_;
				uv0_ = 0;
			}

			if (uv1_)
			{
				delete[] uv1_;
				uv1_ = 0;
			}

		}

		bool set_vertices(D3DXVECTOR3* vertices, UINT* indices, UINT vt_count, UINT id_count);
		bool vertices(D3DXVECTOR3* v, UINT* i) const;

		bool set_colors(D3DXVECTOR4* colors, UINT count);
		bool colors(D3DXVECTOR4* color) const;

		bool set_uv0(D3DXVECTOR2* uvs, UINT count);
		bool uv0(D3DXVECTOR2* uvs) const;

		bool set_normals(D3DXVECTOR3* normals, UINT count);
		bool normals(D3DXVECTOR3* normals) const;

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

		D3DXVECTOR3* normals_;

		UINT* indices_;
		UINT indices_count_;

	};
}

