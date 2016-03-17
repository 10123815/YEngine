#pragma once

#include <D3DX10math.h>

namespace ysd_simple_engine
{
	class Camera
	{
	public:
		Camera ( );
		~Camera ( );

		// Use position and rotation to update view matrix.
		void Update ( );

#pragma region set&get
		void set_position (D3DXVECTOR3 pos) { position_ = pos; }
		D3DXVECTOR3 position ( ) const { return position_; }

		void set_rotation (D3DXVECTOR3 rot) { rotation_ = rotation_; }
		D3DXVECTOR3 rotation ( ) const { return rotation_; }

		D3DXMATRIX view_matrix ( ) const { return view_matrix_; }
#pragma endregion


	private:
		D3DXVECTOR3 position_;
		D3DXVECTOR3 rotation_;
		D3DXMATRIX view_matrix_;
	};

}
