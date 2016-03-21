#include "stdafx.h"
#include "mesh.h"

using namespace ysd_simple_engine;

bool ysd_simple_engine::Mesh::set_vertices(D3DXVECTOR3 * vertices, UINT * indices, UINT vt_count, UINT id_count)
{

	if (id_count % 3 != 0 || vt_count == 0)
	{
		return false;
	}

	vertices_count_ = vt_count;
	indices_count_ = id_count;

	UINT i = 0;
	while (i < vt_count)
	{
		vertex_positions_.push_back(vertices[i]);
		i++;
	}

	i = 0;
	while (i < id_count)
	{
		indices_.push_back(indices[i]);
		i++;
	}

	return true;
}

bool ysd_simple_engine::Mesh::set_colors(D3DXVECTOR4 * colors, UINT count)
{
	if (count != vertices_count_)
		return false;

	for (size_t i = 0; i < count; i++)
	{
		colors_.push_back(colors[i]);
	}
	return true;
}

bool ysd_simple_engine::Mesh::set_uv0(D3DXVECTOR2 * uvs, UINT count)
{
	if (count != vertices_count_)
		return false;

	for (size_t i = 0; i < count; i++)
	{
		uv0_.push_back(uvs[i]);
	}
	return true;
}

bool ysd_simple_engine::Mesh::set_normals(D3DXVECTOR3 * normals, UINT count)
{
	if (count != vertices_count_)
		return false;

	for (size_t i = 0; i < count; i++)
	{
		normals_.push_back(normals[i]);
	}
	return true;
}