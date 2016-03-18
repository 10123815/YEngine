#pragma once

#include "d3d_utility.h"

#include "material.h"
#include "mesh_renderer.h"
#include "camera.h"

namespace ysd_simple_engine
{
	class Graphic : public ISystem
	{
	public:
		Graphic( );
		~Graphic( );

		static std::unique_ptr<Graphic>& Instance( )
		{
			return Singleton<Graphic>::Instance( );
		}

		void RenderFrame( );

		void AddRendererObj(std::shared_ptr<MeshRenderer> render_obj)
		{
			render_obj->Init( );
			render_obj_queue_.push_back(render_obj);
		}

		// Í¨¹ý ISystem ¼Ì³Ð
		virtual void Initialize(HWND hwnd) override;
		virtual void ShutDown( ) override;

	private:

		std::vector<std::shared_ptr<MeshRenderer>> render_obj_queue_;

		std::unique_ptr<Camera> p_camera_;

		std::unique_ptr<ShaderConstantBuffer> p_cbuffer_;

		D3DXMATRIX proj_matrix_;
		D3DXMATRIX world_matrix_;
		D3DXMATRIX ortho_matrix_;
	};

}
