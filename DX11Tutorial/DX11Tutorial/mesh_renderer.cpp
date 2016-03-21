#include "stdafx.h"
#include "mesh_renderer.h"

using namespace ysd_simple_engine;

void ysd_simple_engine::ColorVertexDataType::CreateVertexIndexBuffer(const Mesh & mesh, ID3D11Buffer** pp_vb, ID3D11Buffer** pp_ib)
{
	UINT vertex_size = mesh.vertices_count( );
	UINT index_size = mesh.indices_count( );

	Vertex* vertices = new Vertex[vertex_size];
	UINT* indices = new UINT[index_size];

	// Position and color
	for (size_t i = 0; i < vertex_size; i++)
	{
		vertices[i].position = mesh.positions( )[i];
		vertices[i].color = mesh.colors( )[i];
	}

	// indices
	for (size_t i = 0; i < index_size; i++)
	{
		indices[i] = mesh.indices( )[i];
	}

	// Create buffer
	Create(vertices, vertex_size, indices, index_size, pp_vb, pp_ib);


	// Release the tmp buffer
	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;
}

void ysd_simple_engine::TexVertexDataType::CreateVertexIndexBuffer(const Mesh & mesh, ID3D11Buffer ** pp_vb, ID3D11Buffer ** pp_ib)
{
	UINT vertex_size = mesh.vertices_count( );
	UINT index_size = mesh.indices_count( );

	Vertex* vertices = new Vertex[vertex_size];
	UINT* indices = new UINT[index_size];

	// Position and color
	for (size_t i = 0; i < vertex_size; i++)
	{
		vertices[i].position = mesh.positions( )[i];
		vertices[i].uv = mesh.uv0( )[i];
	}

	// indices
	for (size_t i = 0; i < index_size; i++)
	{
		indices[i] = mesh.indices( )[i];
	}

	// Create buffer
	Create(vertices, vertex_size, indices, index_size, pp_vb, pp_ib);


	// Release the tmp buffer
	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;
}

void ysd_simple_engine::DiffuseLightVertexDataType::CreateVertexIndexBuffer(const Mesh & mesh, ID3D11Buffer ** pp_vb, ID3D11Buffer ** pp_ib)
{
	UINT vertex_size = mesh.vertices_count( );
	UINT index_size = mesh.indices_count( );

	Vertex* vertices = new Vertex[vertex_size];
	UINT* indices = new UINT[index_size];

	// Position and color
	for (size_t i = 0; i < vertex_size; i++)
	{
		vertices[i].position = mesh.positions( )[i];
		vertices[i].uv = mesh.uv0( )[i];
		vertices[i].normal = mesh.normals( )[i];
	}

	// indices
	for (size_t i = 0; i < index_size; i++)
	{
		indices[i] = mesh.indices( )[i];
	}
	// Create buffer
	Create(vertices, vertex_size, indices, index_size, pp_vb, pp_ib);


	// Release the tmp buffer
	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;
}

void ysd_simple_engine::MeshRenderer::Init( )
{
	p_vertex_data_type_->CreateVertexIndexBuffer(*p_mesh_, pp_vertex_buffer_.get( ), pp_index_buffer_.get( ));
	std::wstring w_vs_fie_name(vs_file_name_.begin( ), vs_file_name_.end( ));
	std::wstring w_ps_file_name(ps_file_name_.begin( ), ps_file_name_.end( ));
	p_shader_->Init(w_vs_fie_name.c_str( ), w_ps_file_name.c_str( ));
}

void ysd_simple_engine::MeshRenderer::Render( )
{
	UINT stride = p_vertex_data_type_->size_;
	UINT offset = 0;

	// Input assembler
	auto pp_ib = pp_index_buffer_.get( );
	D3DUtility::Instance( )->VerticesActive(pp_vertex_buffer_.get( ), *pp_ib, &stride, &offset);

	// PS
	p_shader_->SetParams( );
	p_shader_->Shade(index_count_);
}
