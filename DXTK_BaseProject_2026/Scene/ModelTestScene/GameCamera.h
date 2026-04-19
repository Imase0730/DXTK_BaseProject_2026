#pragma once

#include "ImaseLib/SpringCamera.h"

class GameCamera : public Imase::SpringCamera
{

public:

	// コンストラクタ
	GameCamera(const DirectX::SimpleMath::Vector3& eye, const DirectX::SimpleMath::Vector3& target);

	// 更新
	void Update(float elapsedTime) override;

	// ビュー行列を取得する関数
	DirectX::SimpleMath::Matrix GetViewMatrix();
};