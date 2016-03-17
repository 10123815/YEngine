#include "stdafx.h"
#include "Scene.h"

using namespace ysd_simple_engine;

Scene::Scene( )
{
}


Scene::~Scene( )
{
}

void ysd_simple_engine::Scene::Init( )
{
	WCHAR* vfn = L"VertexShader.hlsl";
	WCHAR* pfn = L"PixelShader.hlsl";

	D3DXVECTOR3 positions[] = { { -1,-1,0 },{ 0,1,0 },{ 1,-1,0 } };
	UINT indices[] = { 0,1,2 };
	D3DXVECTOR4 colors[] = { {1,1,1,1}, {1,1,1,1},{0,0,0,1 } };
	Mesh mesh;
	mesh.set_vertices(positions, indices, 3, 3);
	mesh.set_colors(colors, 3);

	Shader* p_shader = new ColorShader( );

	VertexDataType* p_data_type = new ColorVertexDataType( );

	auto mesh_renderer = std::make_shared<MeshRenderer>(vfn, pfn, mesh, p_shader, p_data_type);
	Graphic::Instance( )->AddRendererObj(mesh_renderer);
}
