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
#pragma region test

	std::string vfn = "diffuse_light_v.hlsl";
	std::string pfn = "diffuse_light_p.hlsl";

	D3DXVECTOR3 positions[] = {{-1, -1, 0}, {-1, 1, 0}, {1, -1, 0}, {1, 1, 0}};
	D3DXVECTOR3 normals[] = {{0, 0, -1}, {0, 0, -1}, {0, 0, -1}, {0, 0, -1}};
	UINT indices[] = {0, 1, 2, 2, 1, 3};
	D3DXVECTOR4 colors[] = {{1, 1, 1, 1}, {1, 1, 1, 1}, {0, 0, 0, 1}};
	D3DXVECTOR2 uvs[] = {{0, 1}, {0, 0}, {1, 1}, {1, 0}};
	Mesh mesh;
	mesh.set_vertices(positions, indices, 4, 6);
	mesh.set_colors(colors, 4);
	mesh.set_uv0(uvs, 4);
	mesh.set_normals(normals, 4);

	Texture* tex = new Texture( );
	tex->Load(L"a.dds");
	Shader* p_shader = new DiffuseLightShader(tex);

	VertexDataType* p_data_type = new DiffuseLightVertexDataType( );


	auto mesh_renderer = std::make_shared<MeshRenderer>(vfn, pfn, mesh, p_shader, p_data_type);
	Graphic::Instance( )->AddRendererObj(mesh_renderer);

#pragma endregion

}
