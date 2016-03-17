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

	p_camera_->Update( );

	// Render all objects in th queue
	for (auto& obj : render_obj_queue_)
	{
		obj->Render(world_matrix_, p_camera_->view_matrix( ), proj_matrix_);
	}

	D3DUtility::Instance( )->EndScene( );
}

void ysd_simple_engine::Graphic::Initialize(HWND hwnd)
{
	p_camera_ = std::make_unique<Camera>( );
	p_camera_->set_position({ 0.0f, 0.0f, -10.0f });

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

	if (p_camera_)
	{
		p_camera_.reset( );
	}

	for (auto& obj : render_obj_queue_)
	{
		obj->Release( );
		obj.reset( );
	}
}
