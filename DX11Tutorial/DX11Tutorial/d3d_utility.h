#pragma once

#include "interface.h"
#include "singleton.h"

#include <queue>
#include <memory>

#pragma region d3d_linking
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dx10.lib")
#pragma endregion

#pragma region d3d_include
#include <dxgi.h>
#include <d3dcommon.h>
#include <d3d11.h>
#include <D3DX11tex.h>
#include <D3DX10math.h>
#pragma endregion


namespace ysd_simple_engine
{

	struct MatrixBuffer
	{
		D3DXMATRIX world;
		D3DXMATRIX view;
		D3DXMATRIX proj;
	};

	struct LightBuffer
	{
		D3DXVECTOR4 diffCol;
		D3DXVECTOR3 lightDir;
		float padding;
	};

	class D3DUtility : public ISubSystem
	{

		friend class Engine;

	public:
		D3DUtility( );
		D3DUtility(const D3DUtility&) = delete;
		~D3DUtility( );

		static std::unique_ptr<D3DUtility>& Instance( )
		{
			return Singleton<D3DUtility>::Instance( );
		}

		// Í¨¹ý ISystem ¼Ì³Ð
		virtual void Initialize(HWND hwnd) override;
		virtual void ShutDown( ) override;

		void BeginScene( );
		void EndScene( );

#pragma region set&get

		void set_clear_color(float r, float g, float b, float a)
		{
			clear_color_[0] = r;
			clear_color_[1] = g;
			clear_color_[2] = b;
			clear_color_[3] = a;
		}

#pragma endregion

		bool LoadShaderResource(WCHAR* file_name, ID3D11ShaderResourceView** pp_shader_res_)
		{
			if (FAILED(D3DX11CreateShaderResourceViewFromFile(p_d3d_device_, file_name, NULL, NULL, pp_shader_res_, NULL)))
				return false;
			return true;
		}

		// Create shader from compiled code buufer.
		bool CreateShader(ID3D10Blob* p_vs_code_buffer, ID3D10Blob* p_ps_code_buffer, ID3D11VertexShader** pp_vs, ID3D11PixelShader** pp_ps)
		{
			if (p_vs_code_buffer == NULL || p_ps_code_buffer == NULL)
			{
				return false;
			}

			if (SUCCEEDED(p_d3d_device_->CreateVertexShader(p_vs_code_buffer->GetBufferPointer( ), p_vs_code_buffer->GetBufferSize( ), NULL, pp_vs)) &&
				SUCCEEDED(p_d3d_device_->CreatePixelShader(p_ps_code_buffer->GetBufferPointer( ), p_ps_code_buffer->GetBufferSize( ), NULL, pp_ps)))
			{
				return true;
			}
			return false;
		}

		// Create buufer with description
		bool CreateBuffer(D3D11_BUFFER_DESC* p_buffer_desc, D3D11_SUBRESOURCE_DATA* p_data, ID3D11Buffer** pp_buffer)
		{
			if (FAILED(p_d3d_device_->CreateBuffer(p_buffer_desc, p_data, pp_buffer)))
				return false;
			return true;
		}

		bool CreateInputLayout(D3D11_INPUT_ELEMENT_DESC* ied, UINT num, ID3D10Blob* p_vs_code_buffer, ID3D11InputLayout** pp_input_layout)
		{
			if (FAILED(p_d3d_device_->CreateInputLayout(ied, num, p_vs_code_buffer->GetBufferPointer( ), p_vs_code_buffer->GetBufferSize( ), pp_input_layout)))
				return false;
			return true;
		}

		bool CreateSamplerState(D3D11_SAMPLER_DESC* p_sampler_desc, ID3D11SamplerState** pp_sampler_state)
		{
			if (FAILED(p_d3d_device_->CreateSamplerState(p_sampler_desc, pp_sampler_state)))
				return false;
			return true;
		}

		void SetWVPMatrix(ID3D11Buffer* p_matrix_buffer, const D3DXMATRIX &world, const D3DXMATRIX &view, const D3DXMATRIX &proj)
		{
			// Lock
			D3D11_MAPPED_SUBRESOURCE mapped_resource;
			if (FAILED(p_d3d_device_context_->Map(p_matrix_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_resource)))
			{
				throw std::exception("Lock constant buffer failed.");
			}

			// Write. pData is a void*
			MatrixBuffer* p_data = (MatrixBuffer*)mapped_resource.pData;
			p_data->world = world;
			p_data->view = view;
			p_data->proj = proj;

			// Unlock
			p_d3d_device_context_->Unmap(p_matrix_buffer, 0);

			// Set the constance buffer with the updated data.
			p_d3d_device_context_->VSSetConstantBuffers(0, 1, &p_matrix_buffer);
		}

		void SetLight(ID3D11Buffer* p_light_buffer, D3DXVECTOR4 diffuse_color, D3DXVECTOR3 light_dir)
		{
			D3D11_MAPPED_SUBRESOURCE mapped_resource;
			if (FAILED(p_d3d_device_context_->Map(p_light_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_resource)))
			{
				throw std::exception("Lock constant buffer failed.");
			}

			LightBuffer* p_data = (LightBuffer*)mapped_resource.pData;
			p_data->diffCol = diffuse_color;
			p_data->lightDir = light_dir;
			p_data->padding = 0;

			p_d3d_device_context_->Unmap(p_light_buffer, 0);

			p_d3d_device_context_->PSSetConstantBuffers(0, 1, &p_light_buffer);
		}

		void SetShaderResourceView(ID3D11ShaderResourceView* p_shader_res_view)
		{
			p_d3d_device_context_->PSSetShaderResources(0, 1, &p_shader_res_view);
		}

		void SetShader(ID3D11InputLayout* p_input_layout, ID3D11VertexShader* p_vs, ID3D11PixelShader* p_ps)
		{

			// Set the vertex layout
			p_d3d_device_context_->IASetInputLayout(p_input_layout);

			// Set vs and ps
			p_d3d_device_context_->VSSetShader(p_vs, NULL, 0);
			p_d3d_device_context_->PSSetShader(p_ps, NULL, 0);
		}

		void SetSampler(ID3D11SamplerState* p_samplers)
		{
			p_d3d_device_context_->PSSetSamplers(0, 1, &p_samplers);
		}

		void ShadeWithShader(UINT count)
		{
			// Draw 
			p_d3d_device_context_->DrawIndexed(count, 0, 0);
		}

		void VerticesActive(ID3D11Buffer** pp_vertex_buffer, ID3D11Buffer* p_index_buffer, UINT* pStride, UINT* pOffset)
		{
			p_d3d_device_context_->IASetVertexBuffers(0, 1, pp_vertex_buffer, pStride, pOffset);
			p_d3d_device_context_->IASetIndexBuffer(p_index_buffer, DXGI_FORMAT_R32_UINT, 0);

			// Set primitive type : triangle
			p_d3d_device_context_->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		}

	private:

		UINT video_card_mem_;
		WCHAR* p_video_card_desc_;

		float clear_color_[4];

#pragma region d3d_var
		IDXGISwapChain*			p_swap_chain_;
		ID3D11Device*			p_d3d_device_;
		ID3D11DeviceContext*	p_d3d_device_context_;

		ID3D11RenderTargetView*		p_render_target_view_;
		ID3D11Texture2D*			p_depth_stencil_buffer_;
		ID3D11DepthStencilState*	p_depth_stencil_state_;
		ID3D11DepthStencilView*		p_depth_stencil_view_;
		ID3D11RasterizerState*		p_rasterizer_state_;
#pragma endregion

	};
}

