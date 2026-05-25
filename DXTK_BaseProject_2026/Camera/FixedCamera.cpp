//--------------------------------------------------------------------------------------
// File: FixedCamera.cpp
//
// 固定カメラ
//
// Date: 2026.5.14
// Author: Hideyasu Imase
//--------------------------------------------------------------------------------------
#include "pch.h"
#include "FixedCamera.h"

//	コンストラクタ
Imase::FixedCamera::FixedCamera(
	const DirectX::SimpleMath::Vector3& eye,
	const DirectX::SimpleMath::Vector3& target
)
    : CameraBase(eye, target)
{
	// カメラを初期化する
    SetCamera(eye, target);
}

// カメラを設定する関数
void Imase::FixedCamera::SetCamera(
	const DirectX::SimpleMath::Vector3& eye,
	const DirectX::SimpleMath::Vector3& target
)
{
    SetEyePosition(eye);
    SetTargetPosition(target);
}
