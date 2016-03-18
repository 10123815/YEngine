#include "stdafx.h"
#include "mesh.h"

using namespace ysd_simple_engine;

Mesh::Mesh( )
{
}


Mesh::~Mesh( )
{
}

bool ysd_simple_engine::Mesh::set_vertices(D3DXVECTOR3 * vertices, UINT * indices, UINT vt_count, UINT id_count)
{

	if (id_count % 3 != 0)
	{
		return false;
	}

	vertices_ = new D3DXVECTOR3[vt_count];
	indices_ = new UINT[id_count];
	vertices_count_ = vt_count;
	indices_count_ = id_count;

	UINT i = 0;
	while (i < vt_count)
	{
		vertices_[i] = vertices[i];
		i++;
	}

	i = 0;
	while (i < id_count)
	{
		indices_[i] = indices[i];
		i++;
	}

	return true;
}

bool ysd_simple_engine::Mesh::vertices(D3DXVECTOR3 * ver, UINT * id) const
{
	for (size_t i = 0; i < vertices_count_; i++)
	{
		ver[i] = vertices_[i];
	}
	for (size_t i = 0; i < indices_count_; i++)
	{
		id[i] = indices_[i];
	}
	return true;
}

bool ysd_simple_engine::Mesh::set_colors(D3DXVECTOR4 * colors, UINT count)
{
	if (count != vertices_count_)
		return false;

	colors_ = new D3DXVECTOR4[count];
	for (size_t i = 0; i < count; i++)
	{
		colors_[i] = colors[i];
	}
	return true;
}

bool ysd_simple_engine::Mesh::colors(D3DXVECTOR4 * color) const
{
	for (size_t i = 0; i < vertices_count_; i++)
	{
		color[i] = colors_[i];
	}
	return true;
}

bool ysd_simple_engine::Mesh::set_uv0(D3DXVECTOR2 * uvs, UINT count)
{
	if (count != vertices_count_)
		return false;

	uv0_ = new D3DXVECTOR2[count];
	for (size_t i = 0; i < count; i++)
	{
		uv0_[i] = uvs[i];
	}
	return true;
}

bool ysd_simple_engine::Mesh::uv0(D3DXVECTOR2 * uvs) const
{
	for (size_t i = 0; i < vertices_count_; i++)
	{
		uvs[i] = uv0_[i];
	}
	return true;
}

