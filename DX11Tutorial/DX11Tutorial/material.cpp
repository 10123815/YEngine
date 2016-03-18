#include "stdafx.h"
#include "material.h"

using namespace ysd_simple_engine;

#pragma region ShaderConstanceBuffer

void ysd_simple_engine::ShaderConstantBuffer::CreateMatrixBuffer( )
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
		throw std::exception("Create matrix constant buffer failed.");
	}
}

void ysd_simple_engine::ShaderConstantBuffer::SetWVPMatrix(D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX proj)
{
	D3DXMatrixTranspose(&world, &world);
	D3DXMatrixTranspose(&view, &view);
	D3DXMatrixTranspose(&proj, &proj);

	D3DUtility::Instance( )->SetWVPMatrix(p_matrix_buffer_, world, view, proj);
}

void ysd_simple_engine::ShaderConstantBuffer::CreateLightBuffer( )
{
	// Set the cbuffer
	D3D11_BUFFER_DESC cbuffer_desc;
	cbuffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbuffer_desc.ByteWidth = sizeof(LightBuffer);
	cbuffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbuffer_desc.Usage = D3D11_USAGE_DYNAMIC;
	cbuffer_desc.MiscFlags = 0;
	cbuffer_desc.StructureByteStride = 0;

	// Create cbuffer
	if (!D3DUtility::Instance( )->CreateBuffer(&cbuffer_desc, NULL, &p_light_buffer_))
	{
		throw std::exception("Create light constant buffer failed.");
	}
}

void ysd_simple_engine::ShaderConstantBuffer::SetLightBUffer( )
{
	// D3DUtility::Instance()->SetLight(p_light_buffer_)
}

#pragma endregion


#pragma region BaseShader

void ysd_simple_engine::Shader::Init(WCHAR * vs_file_name, WCHAR * ps_file_name)
{
	CreateShaderAndLayout(vs_file_name, ps_file_name);
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

#pragma region TexShader

void ysd_simple_engine::TexShader::Init(WCHAR * vs_file_name, WCHAR * ps_file_name)
{
	Shader::Init(vs_file_name, ps_file_name);
	SetSamplerState( );
}

void ysd_simple_engine::TexShader::SetParams( )
{
	p_texture_->SetResource( );
}

void ysd_simple_engine::TexShader::Shade(UINT count)
{
	D3DUtility::Instance( )->SetShader(p_input_layout_, p_vertex_shader_, p_pixel_shader_);
	D3DUtility::Instance( )->SetSampler(p_sampler_state_);
	D3DUtility::Instance( )->ShadeWithShader(count);
}

void ysd_simple_engine::TexShader::SetInputLayout(ID3D10Blob * p_vs_code_buffer, ID3D10Blob * p_ps_code_buffer)
{
	D3D11_INPUT_ELEMENT_DESC input_ele_desc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA},
		{"TEXCOOR", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA}
	};

	UINT num = sizeof(input_ele_desc) / sizeof(input_ele_desc[0]);
	if (!D3DUtility::Instance( )->CreateInputLayout(input_ele_desc, num, p_vs_code_buffer, &p_input_layout_))
	{
		throw std::exception("Create layout failed.");
	}
}

void ysd_simple_engine::TexShader::SetSamplerState( )
{
	D3D11_SAMPLER_DESC sampler_desc;
	sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampler_desc.MipLODBias = 0.0f;
	sampler_desc.MaxAnisotropy = 1;
	sampler_desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	sampler_desc.BorderColor[0] = 0;
	sampler_desc.BorderColor[1] = 0;
	sampler_desc.BorderColor[2] = 0;
	sampler_desc.BorderColor[3] = 0;
	sampler_desc.MinLOD = 0;
	sampler_desc.MaxLOD = D3D11_FLOAT32_MAX;

	if (!D3DUtility::Instance( )->CreateSamplerState(&sampler_desc, &p_sampler_state_))
	{
		throw std::exception("Create sampler state failed.");
	}
}

#pragma endregion

void ysd_simple_engine::DiffuseLightShader::SetInputLayout(ID3D10Blob * p_vs_code_buffer, ID3D10Blob * p_ps_code_buffer)
{
}

