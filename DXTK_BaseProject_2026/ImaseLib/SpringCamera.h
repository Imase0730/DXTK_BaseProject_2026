//--------------------------------------------------------------------------------------
// File: SpringCamera.h
//
// スプリングカメラベースクラス
//
// Date: 2026.4.19
// Author: Hideyasu Imase
//--------------------------------------------------------------------------------------
#pragma once

#include "CameraBase.h"

namespace Imase
{
	class SpringCamera : public CameraBase
	{
	private:

		// 臨界減衰バネの周波数
		static constexpr float DEFAULT_FREQ = 8.0f;

		// ----- 視点 ----- //
		// 現在の位置
		DirectX::SimpleMath::Vector3 m_currentPosition;
		// 目標の位置
		DirectX::SimpleMath::Vector3 m_targetPosition;
		// 現在の速度
		DirectX::SimpleMath::Vector3 m_positionVelocity;

		// ----- 注視点 ----- //
		// 現在の位置
		DirectX::SimpleMath::Vector3 m_currentLookAt;
		// 目標の位置
		DirectX::SimpleMath::Vector3 m_targetLookAt;
		// 現在の速度
		DirectX::SimpleMath::Vector3 m_lookAtVelocity;

		// バネの剛性
		float m_stiffness = 0.0f;

		// 減衰係数
		float m_damping = 0.0f;
	
	public:

		// コンストラクタ
		SpringCamera(
			const DirectX::SimpleMath::Vector3& eye,
			const DirectX::SimpleMath::Vector3& target
		);

		// 更新
		void Update(float elapsedTime) override;

		// 視点を取得する関数
		DirectX::SimpleMath::Vector3 GetEyePosition() const override { return m_currentPosition; }

		// 注視点を取得する関数
		DirectX::SimpleMath::Vector3 GetTargetPosition() const override { return m_currentLookAt; }

		// カメラを設定
		void SetCamera(const DirectX::SimpleMath::Vector3& eye, const DirectX::SimpleMath::Vector3& target);

		// ターゲットを設定
		void SetTarget(const DirectX::SimpleMath::Vector3& eye, const DirectX::SimpleMath::Vector3& target);

		// カメラの追従速度を設定する
		// 値が大きいほど素早く目標に収束する（臨界減衰・オーバーシュートなし）
		// 収束時間の目安: 約 4 / freq 秒
		void SetFrequency(float freq);

	private:

		// バネによる位置の更新
		void UpdateSpring(
			const DirectX::SimpleMath::Vector3& target,
			DirectX::SimpleMath::Vector3& current,
			DirectX::SimpleMath::Vector3& velocity,
			float elapsedTime
		) const;

	};
}

