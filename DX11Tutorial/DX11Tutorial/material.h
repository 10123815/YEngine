#pragma once

#include "d3d_utility.h"
#include "texture.h"

#include <d3d11.h>
#include <D3DX11async.h>

namespace  ysd_simple_engine
{

	class ShaderConstantBuffer
	{
	public:
		ShaderConstantBuffer( )
		{
			p_matrix_buffer_ = 0;
			p_light_buffer_ = 0;
		}

		ShaderConstantBuffer(const ShaderConstantBuffer&) = delete;

		void CreateMatrixBuffer( );
		void SetWVPMatrix(D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX proj);

		void CreateLightBuffer( );
		void SetLightBUffer(D3DXVECTOR3 dir, D3DXVECTOR4 col);

		void Release( )
		{
			if (p_matrix_buffer_)
			{
				p_matrix_buffer_->Release( );
				p_matrix_buffer_ = 0;
			}

			if (p_light_buffer_)
			{
				p_light_buffer_->Release( );
				p_light_buffer_ = 0;
			}
		}

	private:
		ID3D11Buffer* p_matrix_buffer_;
		ID3D11Buffer* p_light_buffer_;
	};

	class Shader
	{
	public:
		Shader( )
		{
			p_vertex_shader_ = 0;
			p_pixel_shader_ = 0;
			p_input_layout_ = 0;
		}

		virtual	void Init(const WCHAR* vs_file_name, const WCHAR* ps_file_name);

		virtual void SetParams( ) = 0;
		virtual void Shade(UINT count);

		virtual void ReleaseShader( )
		{
			if (p_input_layout_)
			{
				p_input_layout_->Release( );
				p_input_layout_ = 0;
			}

			if (p_pixel_shader_)
			{
				p_pixel_shader_->Release( );
				p_pixel_shader_ = 0;
			}

			if (p_vertex_shader_)
			{
				p_vertex_shader_->Release( );
				p_vertex_shader_ = 0;
			}
		}

	protected:

		virtual void SetInputLayout(ID3D10Blob* p_vs_code_buffer, ID3D10Blob* p_ps_code_buffer) = 0;

		ID3D11InputLayout*  p_input_layout_;
		ID3D11VertexShader* p_vertex_shader_;
		ID3D11PixelShader*	p_pixel_shader_;

	private:

		void CreateShaderAndLayout(const WCHAR* vs_file_name, const WCHAR* ps_file_name);
	};

	class ColorShader : public Shader
	{
	protected:

		// 通过 Shader 继承
		virtual void SetInputLayout(ID3D10Blob* p_vs_code_buffer, ID3D10Blob* p_ps_code_buffer) override;

	};

	class TexShader : public Shader
	{
	public:
		TexShader(Texture* p_tex)
			:p_texture_(p_tex)
		{
			p_sampler_state_ = 0;
		}

		virtual void Init(const WCHAR* vs_file_name, const WCHAR* ps_file_name) override;

		virtual void SetParams( ) override;
		virtual void Shade(UINT count) override;

		virtual void ReleaseShader( ) override
		{
			if (p_sampler_state_)
			{
				p_sampler_state_->Release( );
				p_sampler_state_ = 0;
			}

			Shader::ReleaseShader( );

			if (p_texture_)
			{
				p_texture_->Release( );
				p_texture_.reset( );
			}
		}

	protected:
		// 通过 Shader 继承
		virtual void SetInputLayout(ID3D10Blob * p_vs_code_buffer, ID3D10Blob * p_ps_code_buffer) override;

		virtual void SetSamplerState( );

		ID3D11SamplerState* p_sampler_state_;

		std::shared_ptr<Texture> p_texture_;
	};

	class DiffuseLightShader : public TexShader
	{
	public:
		DiffuseLightShader(Texture* p_tex)
			: TexShader(p_tex)
		{
		}

	protected:
		// 通过 Shader 继承
		virtual void SetInputLayout(ID3D10Blob * p_vs_code_buffer, ID3D10Blob * p_ps_code_buffer) override;

	};

}


