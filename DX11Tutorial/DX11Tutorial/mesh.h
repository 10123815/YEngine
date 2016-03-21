#pragma once

#include <D3D11.h>
#include <D3DX10math.h>
#include <vector>

namespace ysd_simple_engine
{
	class Mesh
	{
	public:
		Mesh( ) = default;

		bool set_vertices(D3DXVECTOR3* vertices, UINT* indices, UINT vt_count, UINT id_count);
		const std::vector<D3DXVECTOR3>& positions( ) const { return vertex_positions_; }
		const std::vector<UINT>& indices( ) const { return indices_; }

		bool set_colors(D3DXVECTOR4* colors, UINT count);
		const std::vector<D3DXVECTOR4>& colors( ) const { return colors_; }

		bool set_uv0(D3DXVECTOR2* uvs, UINT count);
		const std::vector<D3DXVECTOR2>& uv0( ) const { return uv0_; }

		bool set_normals(D3DXVECTOR3* normals, UINT count);
		const std::vector<D3DXVECTOR3>& normals( ) const { return normals_; }

		UINT vertices_count( ) const { return vertices_count_; }
		UINT indices_count( ) const { return indices_count_; }

		Mesh(const Mesh& other) = default;

	private:

		// Vertices' position
		std::vector<D3DXVECTOR3> vertex_positions_;

		std::vector<D3DXVECTOR3>::size_type vertices_count_;

		// Default color
		std::vector<D3DXVECTOR4> colors_;

		// Texture coordinats
		std::vector<D3DXVECTOR2> uv0_;

		// Second texture coordinates
		std::vector<D3DXVECTOR2> uv1_;

		std::vector<D3DXVECTOR3> normals_;

		std::vector<UINT> indices_;
		std::vector<UINT>::size_type indices_count_;

	};
}

