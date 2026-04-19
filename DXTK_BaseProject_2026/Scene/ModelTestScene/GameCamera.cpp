#include "pch.h"
#include "GameCamera.h"

using namespace DirectX;

// コンストラクタ
GameCamera::GameCamera(
	const DirectX::SimpleMath::Vector3& eye,
	const DirectX::SimpleMath::Vector3& target
)
	: Imase::SpringCamera(eye, target)
{
}

// 更新
void GameCamera::Update(float elapsedTime)
{
	// 基底クラスの更新
	Imase::SpringCamera::Update(elapsedTime);
}

DirectX::SimpleMath::Matrix GameCamera::GetViewMatrix()
{
	SimpleMath::Matrix view;

	// ビュー行列を作成
	view = SimpleMath::Matrix::CreateLookAt(GetEyePosition(), GetTargetPosition(), SimpleMath::Vector3::Up);

	return view;
}
