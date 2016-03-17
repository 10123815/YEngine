#include "stdafx.h"
#include "material.h"

using namespace ysd_simple_engine;

#pragma region BaseShader

void ysd_simple_engine::Shader::Init(WCHAR * vs_file_name, WCHAR * ps_file_name)
{
	CreateShaderAndLayout(vs_file_name, ps_file_name);
	CreateMatrixBuffer( );
}

void ysd_simple_engine::Shader::SetParams(D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX proj)
{
	D3DXMatrixTranspose(&world, &world);
	D3DXMatrixTranspose(&view, &view);
	D3DXMatrixTranspose(&proj, &proj);

	D3DUtility::Instance( )->SetWVPMatrix(p_matrix_buffer_, world, view, proj);
}

void ysd_simple_engine::Shader::Shade(UINT count)
{
	D3DUtility::Instance( )->SetShader(p_input_layout_, p_vertex_shader_, p_pixel_shader_);
	D3DUtility::Instance( )->ShadeWithShader(count);
}

void ysd_simple_engine::Shader::CreateShaderAndLayout(WCHAR * vs_file_name, WCHAR * ps_file_name)
{

	// Compile
	ID3D10Blob* p_vs_code_buffer;
	if (FAILED(D3DX11CompileFromFile(vs_file_name, NULL, NULL, "main", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL,
									 &p_vs_code_buffer, NULL, NULL)))
	{
		throw std::exception("Compile vs failed.");
	}

	ID3D10Blob* p_ps_code_buffer;
	if (FAILED(D3DX11CompileFromFile(ps_file_name, NULL, NULL, "main", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL,
									 &p_ps_code_buffer, NULL, NULL)))
	{
		throw std::exception("Compile ps failed.");
	}

	// Create
	if (!D3DUtility::Instance( )->CreateShader(p_vs_code_buffer, p_ps_code_buffer, &p_vertex_shader_, &p_pixel_shader_))
	{
		throw std::exception("Create shader failed.");
	}

	SetInputLayout(p_vs_code_buffer, p_ps_code_buffer);

	p_vs_code_buffer->Release( );
	p_vs_code_buffer = 0;

	p_ps_code_buffer->Release( );
	p_ps_code_buffer = 0;
}

void ysd_simple_engine::Shader::CreateMatrixBuffer( )
{

	// Set the cbuffer
	D3D11_BUFFER_DESC cbuffer_desc;
	cbuffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbuffer_desc.ByteWidth = sizeof(MatrixBuffer);
	cbuffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbuffer_desc.Usage = D3D11_USAGE_DYNAMIC;
	cbuffer_desc.MiscFlags = 0;
	cbuffer_desc.StructureByteStride = 0;

	// Create cbuffer
	if (!D3DUtility::Instance( )->CreateBuffer(&cbuffer_desc, NULL, &p_matrix_buffer_))
	{
		throw std::exception("Create constant buffer failed.");
	}
}
#pragma endregion

#pragma region ColorShader

void ysd_simple_engine::ColorShader::SetInputLayout(ID3D10Blob* p_vs_code_buffer, ID3D10Blob* p_ps_code_buffer)
{
	D3D11_INPUT_ELEMENT_DESC input_ele_desc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};


	UINT num = sizeof(input_ele_desc) / sizeof(input_ele_desc[0]);
	if (!D3DUtility::Instance( )->CreateInputLayout(input_ele_desc, num, p_vs_code_buffer, &p_input_layout_))
	{
		throw std::exception("Create layout failed.");
	}
}

#pragma endregion

