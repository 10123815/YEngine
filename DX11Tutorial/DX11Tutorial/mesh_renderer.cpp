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

void ysd_simple_engine::MeshRenderer::Init( )
{
	p_vertex_data_type_->CreateVertexIndexBuffer(mesh_, &p_vertex_buffer_, &p_index_buffer_);
	p_shader_->Init(vs_file_name_, ps_file_name_);
}

void ysd_simple_engine::MeshRenderer::Render(D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX proj)
{
	UINT stride = p_vertex_data_type_->size_;
	UINT offset = 0;
	// Input assembler
	D3DUtility::Instance( )->VerticesActive(&p_vertex_buffer_, p_index_buffer_, &stride, &offset);

	p_shader_->SetParams(world, view, proj);
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
