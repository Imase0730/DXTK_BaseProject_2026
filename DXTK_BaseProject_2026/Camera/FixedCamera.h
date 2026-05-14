//--------------------------------------------------------------------------------------
// File: FixedCamera.h
//
// 固定カメラ
//
// Date: 2026.5.14
// Author: Hideyasu Imase
//--------------------------------------------------------------------------------------
#pragma once

#include "CameraBase.h"

namespace Imase
{
	// 固定カメラクラス
	class FixedCamera : public CameraBase
	{
    public:

		// コンストラクタ
        FixedCamera(
			const DirectX::SimpleMath::Vector3& eye,
			const DirectX::SimpleMath::Vector3& target
		);

		// カメラを設定する関数
        void SetCamera(
			const DirectX::SimpleMath::Vector3& eye,
			const DirectX::SimpleMath::Vector3& target
		);

	};
}
