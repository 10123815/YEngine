#include "stdafx.h"
#include "mesh_renderer.h"

using namespace ysd_simple_engine;

void ysd_simple_engine::ColorVertexDataType::CreateVertexIndexBuffer(const Mesh & mesh, ID3D11Buffer** pp_vb, ID3D11Buffer** pp_ib)
{
	UINT vertex_size = mesh.vertices_count( );
	UINT index_size = mesh.indices_count( );

	Vertex* vertices = new Vertex[vertex_size];
	D3DXVECTOR3* positions = new D3DXVECTOR3[vertex_size];
	D3DXVECTOR4* colors = new D3DXVECTOR4[vertex_size];
	UINT* indices = new UINT[index_size];

	mesh.vertices(positions, indices);
	mesh.colors(colors);

	// Position and color
	for (size_t i = 0; i < vertex_size; i++)
	{
		vertices[i].position = positions[i];
		vertices[i].color = colors[i];
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
	D3DXVECTOR3* positions = new D3DXVECTOR3[vertex_size];
	D3DXVECTOR2* uvs = new D3DXVECTOR2[vertex_size];
	UINT* indices = new UINT[index_size];

	mesh.vertices(positions, indices);
	mesh.uv0(uvs);

	// Position and color
	for (size_t i = 0; i < vertex_size; i++)
	{
		vertices[i].position = positions[i];
		vertices[i].uv = uvs[i];
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
	D3DXVECTOR3* positions = new D3DXVECTOR3[vertex_size];
	D3DXVECTOR2* uvs = new D3DXVECTOR2[vertex_size];
	D3DXVECTOR3* normals = new D3DXVECTOR3[vertex_size];
	UINT* indices = new UINT[index_size];

	mesh.vertices(positions, indices);
	mesh.uv0(uvs);
	mesh.normals(normals);

	// Position and color
	for (size_t i = 0; i < vertex_size; i++)
	{
		vertices[i].position = positions[i];
		vertices[i].uv = uvs[i];
		vertices[i].normal = normals[i];
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
	p_vertex_data_type_->CreateVertexIndexBuffer(mesh_, &p_vertex_buffer_, &p_index_buffer_);
	p_shader_->Init(vs_file_name_, ps_file_name_);
}

void ysd_simple_engine::MeshRenderer::Render( )
{
	UINT stride = p_vertex_data_type_->size_;
	UINT offset = 0;
	// Input assembler
	D3DUtility::Instance( )->VerticesActive(&p_vertex_buffer_, p_index_buffer_, &stride, &offset);

	// PS
	p_shader_->SetParams( );
	p_shader_->Shade(index_count_);
}

void ysd_simple_engine::MeshRenderer::Release( )
{

	if (p_index_buffer_)
	{
		p_index_buffer_->Release( );
		p_index_buffer_ = 0;
	}

	if (p_vertex_buffer_)
	{
		p_vertex_buffer_->Release( );
		p_vertex_buffer_ = 0;
	}

	if (p_vertex_data_type_)
	{
		p_vertex_data_type_.reset( );
	}

	if (p_shader_)
	{
		p_shader_->ReleaseShader( );
		p_shader_.reset();
	}
}
