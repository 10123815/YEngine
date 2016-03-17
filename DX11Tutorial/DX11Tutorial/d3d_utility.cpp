#include "stdafx.h"
#include "d3d_utility.h"

using namespace ysd_simple_engine;

D3DUtility::D3DUtility ( )
{
	p_d3d_device_ = 0;
	p_d3d_device_context_ = 0;
	p_swap_chain_ = 0;
	p_render_target_view_ = 0;
	p_depth_stencil_buffer_ = 0;
	p_depth_stencil_state_ = 0;
	p_depth_stencil_view_ = 0;
	p_rasterizer_state_ = 0;

	clear_color_[0] = 0;
	clear_color_[1] = 0;
	clear_color_[2] = 1;
	clear_color_[3] = 1;
}


D3DUtility::~D3DUtility ( )
{
}

void ysd_simple_engine::D3DUtility::Initialize (HWND hwnd)
{

#pragma region refresh rate
	// graphic interface factory
	IDXGIFactory* p_factory;
	if (FAILED (CreateDXGIFactory (__uuidof(IDXGIFactory), (void**)&p_factory)))
	{
		throw std::exception ("Create IDXGIFactory failed.");
	}

	// use factory to create an adapter
	IDXGIAdapter* p_adapter;
	if (FAILED (p_factory->EnumAdapters (0, &p_adapter)))
	{
		throw std::exception ("Create adapter failed.");
	}

	// create adapter output (monitor)
	IDXGIOutput* p_output;
	if (FAILED (p_adapter->EnumOutputs (0, &p_output)))
	{
		throw std::exception ("Create adapter output failed.");
	}

	// get number of modes that fit the monitor
	UINT number;
	if (FAILED (p_output->GetDisplayModeList (DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &number, NULL)) ||
		number == 0)
	{
		throw std::exception ("Get monitor display format mode number failed.");
	}

	// a array to store all possible display mode
	DXGI_MODE_DESC* p_display_mode_list = new DXGI_MODE_DESC[number];
	if (FAILED (p_output->GetDisplayModeList (DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &number, p_display_mode_list)))
	{
		throw std::exception ("Get display mode failed.");
	}

	// fresh rate
	UINT n, d;
	for (UINT i = 0; i < number; i++)
	{
		if (p_display_mode_list[i].Width == 800 &&
			p_display_mode_list[i].Height == 600)
		{
			n = p_display_mode_list[i].RefreshRate.Numerator;
			d = p_display_mode_list[i].RefreshRate.Denominator;
		}
	}

	// name and memory
	DXGI_ADAPTER_DESC adapter_desc;
	if (FAILED (p_adapter->GetDesc (&adapter_desc)))
	{
		throw std::exception ("Get adapter dexcription failed");
	}
	video_card_mem_ = (adapter_desc.DedicatedVideoMemory / 1024 / 1024);
	p_video_card_desc_ = adapter_desc.Description;

	// release
	delete[] p_display_mode_list;
	p_display_mode_list = 0;

	p_output->Release ( );
	p_output = 0;

	p_adapter->Release ( );
	p_adapter = 0;

	p_factory->Release ( );
	p_factory = 0;
#pragma endregion

#pragma region init the d3d

	// full out swap chain dexcription
	DXGI_SWAP_CHAIN_DESC swap_chain_desc;
	ZeroMemory (&swap_chain_desc, sizeof (swap_chain_desc));
	swap_chain_desc.BufferCount = 1;
	swap_chain_desc.BufferDesc.Width = 800;
	swap_chain_desc.BufferDesc.Height = 600;
	swap_chain_desc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;

	// refresh rate : how many times a second it draws the back buffer to the front buffer
	swap_chain_desc.BufferDesc.RefreshRate.Numerator = 0;
	swap_chain_desc.BufferDesc.RefreshRate.Denominator = 1;

	// Set the usage of the back buffer.
	swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	// Set the handle for the window to render to.
	swap_chain_desc.OutputWindow = hwnd;

	// Turn multisampling off.
	swap_chain_desc.SampleDesc.Count = 1;
	swap_chain_desc.SampleDesc.Quality = 0;

	swap_chain_desc.Windowed = true;

	// Set the scan line ordering and scaling to unspecified.
	swap_chain_desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swap_chain_desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;


	// Discard the back buffer contents after presenting to the front.
	swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	// Don't set the advanced flags.
	swap_chain_desc.Flags = 0;

	D3D_FEATURE_LEVEL featrue_level = D3D_FEATURE_LEVEL_11_0;

	// Create device and swap chain
	if (FAILED (D3D11CreateDeviceAndSwapChain (
		NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &featrue_level, 1,
		D3D11_SDK_VERSION, &swap_chain_desc, &p_swap_chain_, &p_d3d_device_, NULL, &p_d3d_device_context_)))
	{
		throw std::exception ("Create device or swap chain failed.");
	}

	// Get the pointer to the back buffer
	ID3D11Texture2D* p_back_buffer;
	if (FAILED (p_swap_chain_->GetBuffer (0, __uuidof(ID3D11Texture2D), (void**)&p_back_buffer)))
	{
		throw std::exception ("Get the pointer of back buffer failed.");
	}

	// Create the render target view (a kind of resource view that to tell GPU how to use resource)
	if (FAILED (p_d3d_device_->CreateRenderTargetView (p_back_buffer, NULL, &p_render_target_view_)))
	{
		throw std::exception ("Create render target view failed.");
	}

	p_back_buffer->Release ( );
	p_back_buffer = 0;

	// Set up a depth buffer description
	D3D11_TEXTURE2D_DESC depth_bufferd_desc;
	ZeroMemory (&depth_bufferd_desc, sizeof (depth_bufferd_desc));

	depth_bufferd_desc.Width = 800;
	depth_bufferd_desc.Height = 600;
	depth_bufferd_desc.MipLevels = 1;
	depth_bufferd_desc.ArraySize = 1;
	depth_bufferd_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depth_bufferd_desc.SampleDesc.Count = 1;
	depth_bufferd_desc.SampleDesc.Quality = 0;
	depth_bufferd_desc.Usage = D3D11_USAGE_DEFAULT;
	depth_bufferd_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depth_bufferd_desc.CPUAccessFlags = 0;
	depth_bufferd_desc.MiscFlags = 0;

	// Create texture for depth buffer
	if (FAILED (p_d3d_device_->CreateTexture2D (&depth_bufferd_desc, NULL, &p_depth_stencil_buffer_)))
	{
		throw std::exception ("Create depth buffer failed.");
	}

	// Set up stencil state description
	D3D11_DEPTH_STENCIL_DESC depth_stencil_desc;

	// Initialize the description of the stencil state.
	ZeroMemory (&depth_stencil_desc, sizeof (depth_stencil_desc));

	// Set up the description of the stencil state.
	depth_stencil_desc.DepthEnable = true;
	depth_stencil_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depth_stencil_desc.DepthFunc = D3D11_COMPARISON_LESS;

	depth_stencil_desc.StencilEnable = true;
	depth_stencil_desc.StencilReadMask = 0xFF;
	depth_stencil_desc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing.
	depth_stencil_desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;		// If stencil test failed, keep stencil buffer.
	depth_stencil_desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depth_stencil_desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depth_stencil_desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing.
	depth_stencil_desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depth_stencil_desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depth_stencil_desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depth_stencil_desc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create depth stencil state;
	if (FAILED (p_d3d_device_->CreateDepthStencilState (&depth_stencil_desc, &p_depth_stencil_state_)))
	{
		throw std::exception ("Craete depth stencil state failed.");
	}

	// Set depth stencil state
	p_d3d_device_context_->OMSetDepthStencilState (p_depth_stencil_state_, 1);

	D3D11_DEPTH_STENCIL_VIEW_DESC depth_stencil_view_desc;
	// Initailze the depth stencil view.
	ZeroMemory (&depth_stencil_view_desc, sizeof (depth_stencil_view_desc));

	// Set up the depth stencil view description.
	depth_stencil_view_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depth_stencil_view_desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depth_stencil_view_desc.Texture2D.MipSlice = 0;

	// Create the depth stencil view.
	if (FAILED (p_d3d_device_->CreateDepthStencilView (p_depth_stencil_buffer_, &depth_stencil_view_desc, &p_depth_stencil_view_)))
	{
		throw std::exception ("Create depth stencil view failed.");
	}

	// Bind the render target view and depth stencil buffer to the output render pipeline.
	p_d3d_device_context_->OMSetRenderTargets (1, &p_render_target_view_, p_depth_stencil_view_);

	D3D11_RASTERIZER_DESC raster_desc;
	// Setup the raster description which will determine how and what polygons will be drawn.
	raster_desc.AntialiasedLineEnable = false;
	raster_desc.CullMode = D3D11_CULL_BACK;
	raster_desc.DepthBias = 0;
	raster_desc.DepthBiasClamp = 0.0f;
	raster_desc.DepthClipEnable = true;
	raster_desc.FillMode = D3D11_FILL_SOLID;
	raster_desc.FrontCounterClockwise = false;
	raster_desc.MultisampleEnable = false;
	raster_desc.ScissorEnable = false;
	raster_desc.SlopeScaledDepthBias = 0.0f;

	// Create raster state
	if (FAILED (p_d3d_device_->CreateRasterizerState (&raster_desc, &p_rasterizer_state_)))
	{
		throw std::exception ("Create rasterizer state failed.");
	}

	p_d3d_device_context_->RSSetState (p_rasterizer_state_);


	D3D11_VIEWPORT viewport;
	// Setup the viewport for rendering.
	viewport.Width = (float)800;
	viewport.Height = (float)600;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;

	p_d3d_device_context_->RSSetViewports (1, &viewport);


#pragma endregion

}

void ysd_simple_engine::D3DUtility::ShutDown ( )
{

	if (p_rasterizer_state_)
	{
		p_rasterizer_state_->Release ( );
		p_rasterizer_state_ = 0;
	}

	if (p_depth_stencil_view_)
	{
		p_depth_stencil_view_->Release ( );
		p_depth_stencil_view_ = 0;
	}

	if (p_depth_stencil_state_)
	{
		p_depth_stencil_state_->Release ( );
		p_depth_stencil_state_ = 0;
	}

	if (p_depth_stencil_buffer_)
	{
		p_depth_stencil_buffer_->Release ( );
		p_depth_stencil_buffer_ = 0;
	}

	if (p_render_target_view_)
	{
		p_render_target_view_->Release ( );
		p_render_target_view_ = 0;
	}

	if (p_d3d_device_context_)
	{
		p_d3d_device_context_->Release ( );
		p_d3d_device_context_ = 0;
	}

	if (p_d3d_device_)
	{
		p_d3d_device_->Release ( );
		p_d3d_device_ = 0;
	}

	if (p_swap_chain_)
	{
		p_swap_chain_->Release ( );
		p_swap_chain_ = 0;
	}

}

void ysd_simple_engine::D3DUtility::BeginScene ( )
{
	// Clear the back buffer
	p_d3d_device_context_->ClearRenderTargetView (p_render_target_view_, clear_color_);

	// Clear the depth buffer
	p_d3d_device_context_->ClearDepthStencilView (p_depth_stencil_view_, D3D11_CLEAR_DEPTH, 1.0f, 0);

}

void ysd_simple_engine::D3DUtility::EndScene ( )
{
	p_swap_chain_->Present (0, 0);
}
