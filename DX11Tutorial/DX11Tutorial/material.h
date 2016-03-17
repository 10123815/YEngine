#pragma once

#include "d3d_utility.h"

#include <d3d11.h>
#include <D3DX11async.h>

namespace  ysd_simple_engine
{

	class Shader
	{
	public:
		Shader( )
		{
			p_vertex_shader_ = 0;
			p_pixel_shader_ = 0;
			p_input_layout_ = 0;
			p_matrix_buffer_ = 0;
		}

		virtual	void Init(WCHAR* vs_file_name, WCHAR* ps_file_name);

		virtual void SetParams(D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX proj);

		virtual void Shade(UINT count);

		void ReleaseShader( )
		{
			if (p_input_layout_)
			{
				p_input_layout_->Release( );
				p_input_layout_ = 0;
			}

			if (p_matrix_buffer_)
			{
				p_matrix_buffer_->Release( );
				p_matrix_buffer_ = 0;
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

	private:

		void CreateShaderAndLayout(WCHAR* vs_file_name, WCHAR* ps_file_name);

		void CreateMatrixBuffer( );

		ID3D11VertexShader* p_vertex_shader_;
		ID3D11PixelShader*	p_pixel_shader_;
		ID3D11Buffer*		p_matrix_buffer_;
	};

	class ColorShader : public Shader
	{
	public:

	protected:

		// Í¨¹ý Shader ¼Ì³Ð
		virtual void SetInputLayout(ID3D10Blob* p_vs_code_buffer, ID3D10Blob* p_ps_code_buffer) override;

	private:

	};
}


