#pragma once

#include "mesh.h"
#include "material.h"
#include "d3d_utility.h"

#include <d3d11.h>
#include <D3DX10math.h>
#include <memory>

namespace ysd_simple_engine
{

	class VertexDataType
	{
	public:
		virtual void CreateVertexIndexBuffer(const Mesh& mesh, ID3D11Buffer** pp_vb, ID3D11Buffer** pp_ib) = 0;

		BYTE size_;
	protected:
		template<typename TVertex>
		void Create(TVertex* vertices, UINT vertex_size, UINT* indices, UINT index_size, ID3D11Buffer** pp_vb, ID3D11Buffer** pp_ib)
		{

			D3D11_BUFFER_DESC vertex_buffer_desc;
			D3D11_BUFFER_DESC index_buffer_desc;
			D3D11_SUBRESOURCE_DATA vertex_data;
			D3D11_SUBRESOURCE_DATA index_data;

			// Set up the description of the static vertex buffer.
			vertex_buffer_desc.Usage = D3D11_USAGE_DEFAULT;
			vertex_buffer_desc.ByteWidth = sizeof(TVertex) * vertex_size;
			vertex_buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			vertex_buffer_desc.CPUAccessFlags = 0;
			vertex_buffer_desc.MiscFlags = 0;
			vertex_buffer_desc.StructureByteStride = 0;

			// Give the subresource structure a pointer to the vertex data.
			vertex_data.pSysMem = vertices;
			vertex_data.SysMemPitch = 0;
			vertex_data.SysMemSlicePitch = 0;

			// Create vertex buffer
			if (FAILED(D3DUtility::Instance( )->CreateBuffer(&vertex_buffer_desc, &vertex_data, pp_vb)))
			{
				throw std::exception("Create vertex buffer failed.");
			}

			// Set up the description of the static index buffer
			index_buffer_desc.Usage = D3D11_USAGE_DEFAULT;
			index_buffer_desc.ByteWidth = sizeof(UINT) * index_size;
			index_buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
			index_buffer_desc.CPUAccessFlags = 0;
			index_buffer_desc.MiscFlags = 0;
			index_buffer_desc.StructureByteStride = 0;

			// Pointer to the index data
			index_data.pSysMem = indices;
			index_data.SysMemPitch = 0;
			index_data.SysMemSlicePitch = 0;

			// Create index buffer
			if (FAILED(D3DUtility::Instance( )->CreateBuffer(&index_buffer_desc, &index_data, pp_ib)))
			{
				throw std::exception("Create index buffer failed.");
			}
		}
	};

	class ColorVertexDataType : public VertexDataType
	{
	public:
		ColorVertexDataType( ) { size_ = sizeof(Vertex); }

		// 通过 VertexDataType 继承
		virtual void CreateVertexIndexBuffer(const Mesh & mesh, ID3D11Buffer** pp_vb, ID3D11Buffer** pp_ib) override;

		struct Vertex
		{
			D3DXVECTOR3 position;
			D3DXVECTOR4 color;
		};

	};

	class TexVertexDataType : public VertexDataType
	{
	public:
		TexVertexDataType( ) { size_ = sizeof(Vertex); }

		// 通过 VertexDataType 继承
		virtual void CreateVertexIndexBuffer(const Mesh & mesh, ID3D11Buffer ** pp_vb, ID3D11Buffer ** pp_ib) override;

		struct Vertex
		{
			D3DXVECTOR3 position;
			D3DXVECTOR2 uv;
		};
	};

	class DiffuseLightVertexDataType : public TexVertexDataType
	{
	public:

		virtual void CreateVertexIndexBuffer(const Mesh& mesh, ID3D11Buffer** pp_vb, ID3D11Buffer** pp_ib) override;

		DiffuseLightVertexDataType( ){ size_ = sizeof(Vertex); }
		struct Vertex
		{
			D3DXVECTOR3 position;
			D3DXVECTOR2 uv;
			D3DXVECTOR3 normal;
		};
	};

	class MeshRenderer
	{
	public:
		MeshRenderer(std::string vfn, std::string pfn, const Mesh& mesh, Shader* shader, VertexDataType* vertex_data_type)
			:vs_file_name_(vfn), ps_file_name_(pfn), p_shader_(shader), p_vertex_data_type_(vertex_data_type)
		{
			p_mesh_ = std::make_shared<Mesh>(mesh);
			index_count_ = p_mesh_->indices_count( );

			// assign value to the pointer of pointer
			pp_vertex_buffer_ = std::make_shared<ID3D11Buffer*>(nullptr);
			pp_index_buffer_ = std::make_shared<ID3D11Buffer*>(nullptr);
		}

		MeshRenderer(const MeshRenderer& renderer)
			:
			vs_file_name_(renderer.vs_file_name_),
			ps_file_name_(renderer.ps_file_name_),
			index_count_(renderer.index_count_),
			// point to the new instance
			p_mesh_(renderer.p_mesh_),
			p_vertex_data_type_(renderer.p_vertex_data_type_)
		{
			if (this == &renderer)
				return;

			if (p_shader_.unique( ))
			{
				p_shader_->ReleaseShader( );
			}
			p_shader_ = renderer.p_shader_;

			if (pp_vertex_buffer_.unique( ))
			{
				(*pp_vertex_buffer_)->Release( );
				(*pp_vertex_buffer_) = 0;
				pp_vertex_buffer_ = 0;
			}
			pp_vertex_buffer_ = renderer.pp_vertex_buffer_;

			if (pp_index_buffer_.unique( ))
			{
				(*pp_index_buffer_)->Release( );
				(*pp_index_buffer_) = 0;
				pp_index_buffer_ = 0;
			}
			pp_index_buffer_ = renderer.pp_index_buffer_;
		}

		MeshRenderer& operator=(const MeshRenderer& renderer)
		{
			if (this == &renderer)
				return;

			vs_file_name_ = renderer.vs_file_name_;
			ps_file_name_ = renderer.ps_file_name_;
			index_count_ = renderer.index_count_;

			// point to the new instance
			p_mesh_ = renderer.p_mesh_;
			p_vertex_data_type_ = renderer.p_vertex_data_type_;

			if (p_shader_.unique( ))
			{
				p_shader_->ReleaseShader( );
			}
			p_shader_ = renderer.p_shader_;

			if (pp_vertex_buffer_.unique( ))
			{
				(*pp_vertex_buffer_)->Release( );
				(*pp_vertex_buffer_) = 0;
				pp_vertex_buffer_ = 0;
			}
			pp_vertex_buffer_ = renderer.pp_vertex_buffer_;

			if (pp_index_buffer_.unique( ))
			{
				(*pp_index_buffer_)->Release( );
				(*pp_index_buffer_) = 0;
				pp_index_buffer_ = 0;
			}
			pp_index_buffer_ = renderer.pp_index_buffer_;
		}

		void Init( );
		void Render( );
		void Release( )
		{
			if (pp_vertex_buffer_.unique( ))
			{
				(*pp_vertex_buffer_)->Release( );
				(*pp_vertex_buffer_) = 0;
				pp_vertex_buffer_ = 0;
			}
			pp_vertex_buffer_.reset( );

			if (pp_index_buffer_.unique( ))
			{
				(*pp_index_buffer_)->Release( );
				(*pp_index_buffer_) = 0;
				pp_index_buffer_ = 0;
			}
			pp_index_buffer_.reset( );

			p_vertex_data_type_.reset( );

			if (p_shader_.unique( ))
			{
				p_shader_->ReleaseShader( );
			}
			p_shader_.reset( );

			p_mesh_.reset( );
		}

	private:
		std::string vs_file_name_;
		std::string ps_file_name_;

		std::shared_ptr<Mesh> p_mesh_;
		std::shared_ptr<Shader> p_shader_;
		std::shared_ptr<VertexDataType> p_vertex_data_type_;

		std::shared_ptr<ID3D11Buffer*> pp_vertex_buffer_;
		std::shared_ptr<ID3D11Buffer*> pp_index_buffer_;
		UINT index_count_;
	};

}

