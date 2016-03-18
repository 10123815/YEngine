#include "stdafx.h"
#include "Engine.h"

using namespace ysd_simple_engine;

Engine::Engine ( )
{
	
}

Engine::~Engine ( )
{
}

// Called every frame
void ysd_simple_engine::Engine::Update ( )
{
	Graphic::Instance( )->RenderFrame( );
}

void ysd_simple_engine::Engine::Initialize (HWND hwnd)
{
	// Utility
	D3DUtility::Instance( )->Initialize (hwnd);

	// Sub-engine
	Graphic::Instance( )->Initialize(hwnd);
}

void ysd_simple_engine::Engine::ShutDown ( )
{
	// Sub-engine
	if (Graphic::Instance( ))
	{
		Graphic::Instance( )->ShutDown( );
		// Release and delete the owned object.
		Graphic::Instance( ).reset(nullptr);
	}

	// Utility
	if (D3DUtility::Instance( ))
	{
		D3DUtility::Instance( )->ShutDown ( );
		// Release and delete the owned object.
		D3DUtility::Instance( ).reset (nullptr);
	}
}
