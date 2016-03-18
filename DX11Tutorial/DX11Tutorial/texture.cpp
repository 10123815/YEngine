#include "stdafx.h"
#include "texture.h"

using namespace ysd_simple_engine;

Texture::Texture( )
{
	p_texture_shader_res_ = 0;
}


Texture::~Texture( )
{
}

void ysd_simple_engine::Texture::Load(WCHAR * file_name)
{
	if (!D3DUtility::Instance( )->LoadShaderResource(file_name, &p_texture_shader_res_))
		throw std::exception("Load texture failed.");
}

void ysd_simple_engine::Texture::Release( )
{
	if (p_texture_shader_res_)
	{
		p_texture_shader_res_->Release( );
		p_texture_shader_res_ = 0;
	}
}

void ysd_simple_engine::Texture::SetResource( )
{
	D3DUtility::Instance( )->SetShaderResourceView(p_texture_shader_res_);
}
