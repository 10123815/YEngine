#include "stdafx.h"
#include "texture.h"

using namespace ysd_simple_engine;

Textrue::Textrue( )
{
	p_texture_ = 0;
}


Textrue::~Textrue( )
{
}

void ysd_simple_engine::Textrue::Load(WCHAR * file_name)
{
	ID3D11ShaderResourceView* p_tex;
	HRESULT result = D3DUtility::Instance( )->LoadTexture(file_name, &p_tex);
	if (FAILED(result))
		throw std::exception("Load texture failed.");
	std::shared_ptr<ID3D11ShaderResourceView> sp_tex(p_tex);
	p_texture_ = sp_tex;
}

void ysd_simple_engine::Textrue::Release( )
{
	if (p_texture_)
	{
		p_texture_->Release( );
		p_texture_.reset( );
	}
}
