#include "stdafx.h"
#include "camera.h"

using namespace ysd_simple_engine;

Camera::Camera ( )
{
}


Camera::~Camera ( )
{
}

void ysd_simple_engine::Camera::Update ( )
{
	D3DXVECTOR3 up = { 0.0f, 0.1f, 0.0f };

	D3DXVECTOR3 look_at = { 0.0f, 0.0f, 0.1f };
	look_at += position_;

	D3DXMatrixLookAtLH (&view_matrix_, &position_, &look_at, &up);
}
