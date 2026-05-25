//--------------------------------------------------------------------------------------
// File: SpringCamera.h
//
// スプリングカメラ
//
// Date: 2026.5.17
// Author: Hideyasu Imase
//--------------------------------------------------------------------------------------
#pragma once

#include "CameraBase.h"

namespace Imase
{
	class SpringCamera : public CameraBase
	{
	public:

		// コンストラクタ
		SpringCamera(
			const DirectX::SimpleMath::Vector3& eye,
			const DirectX::SimpleMath::Vector3& target
		);

		// 更新
		void Update(float elapsedTime);

		// カメラを設定する関数
		void SetCamera(
			const DirectX::SimpleMath::Vector3& eye,
			const DirectX::SimpleMath::Vector3& target
		);

		// ターゲットを設定する関数
		void SetTarget(
			const DirectX::SimpleMath::Vector3& eye,
			const DirectX::SimpleMath::Vector3& target,
			float time
		);

		// 移動完了までのおおよその時間を設定する関数
        void SetMoveTime(float time);

		// 移動終了か調べる関数
		bool IsArrived() const;

    private:
 
        // ----- 視点 ----- //
        // 目標の位置
        DirectX::SimpleMath::Vector3 m_eyeTarget;
        // 現在の速度
        DirectX::SimpleMath::Vector3 m_eyeVelocity;

        // ----- 注視点 ----- //
        // 目標の位置
        DirectX::SimpleMath::Vector3 m_targetTarget;
        // 現在の速度
        DirectX::SimpleMath::Vector3 m_targetVelocity;

        // バネの剛性
        float m_stiffness = 0.0f;

        // 減衰係数
        float m_damping = 0.0f;
	
		// バネによる位置の更新
		void UpdateSpring(
			const DirectX::SimpleMath::Vector3& target,
			DirectX::SimpleMath::Vector3& current,
			DirectX::SimpleMath::Vector3& velocity,
			float elapsedTime
		) const;

		// カメラの追従速度を設定する
        // 値が大きいほど素早く目標に収束する（臨界減衰・オーバーシュートなし）
        // 収束時間の目安: 約 4 / freq 秒
        void SetFrequency(float freq);

	};
}

