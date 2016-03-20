#include "stdafx.h"
#include "Graphic.h"

using namespace ysd_simple_engine;

Graphic::Graphic( )
{
}


Graphic::~Graphic( )
{
}

void ysd_simple_engine::Graphic::RenderFrame( )
{
	D3DUtility::Instance( )->BeginScene( );

	// scene setup
	p_camera_->Update( );
	D3DXVECTOR3 light_dir = p_light_->direction( );
	D3DXVECTOR4 light_col = p_light_->color( );

	// constant buffer
	p_cbuffer_->SetWVPMatrix(world_matrix_, p_camera_->view_matrix( ), proj_matrix_);
	p_cbuffer_->SetLightBUffer(light_dir, light_col);

	// Render all objects in th queue
	for (auto& obj : render_obj_queue_)
	{
		obj->Render( );
	}

	D3DUtility::Instance( )->EndScene( );
}

void ysd_simple_engine::Graphic::Initialize(HWND hwnd)
{
	p_camera_ = std::make_unique<Camera>( );
	p_camera_->set_position({0.0f, 0.0f, -10.0f});

	p_light_ = std::make_unique<DirectLight>( );
	p_light_->set_dir({0, 0, 1});
	p_light_->set_color({1, 0, 0, 1});

	p_cbuffer_ = std::make_unique<ShaderConstantBuffer>( );
	p_cbuffer_->CreateMatrixBuffer( );
	p_cbuffer_->CreateLightBuffer( );

	float field_view, screen_aspect;// Setup the projection matrix.
	field_view = (float)D3DX_PI / 4.0f;
	screen_aspect = (float)800 / (float)600;

	// Create the projection matrix for 3D rendering.
	D3DXMatrixPerspectiveFovLH(&proj_matrix_, field_view, screen_aspect, 0.1f, 1000);

	// Initialize the world matrix to the identity matrix.
	D3DXMatrixIdentity(&world_matrix_);

	// Create an orthographic projection matrix for 2D rendering.
	D3DXMatrixOrthoLH(&ortho_matrix_, (float)800, (float)600, 0.1f, 1000);
}

void ysd_simple_engine::Graphic::ShutDown( )
{

	if (p_cbuffer_)
	{
		p_cbuffer_->Release( );
		p_cbuffer_.reset(nullptr);
	}

	if (p_light_)
	{
		p_light_.reset(nullptr);
	}

	if (p_camera_)
	{
		p_camera_.reset(nullptr);
	}

	for (auto& obj : render_obj_queue_)
	{
		obj->Release( );
		obj.reset( );
	}
}
