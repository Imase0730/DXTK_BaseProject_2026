//--------------------------------------------------------------------------------------
// File: SpringCamera.cpp
//
// スプリングカメラベースクラス
//
// Date: 2026.4.19
// Author: Hideyasu Imase
//--------------------------------------------------------------------------------------
#include "pch.h"
#include "SpringCamera.h"

using namespace DirectX;

// コンストラクタ
Imase::SpringCamera::SpringCamera(
	const DirectX::SimpleMath::Vector3& eye,
	const DirectX::SimpleMath::Vector3& target
)
	: m_currentPosition(eye)
	, m_targetPosition(eye)
	, m_currentLookAt(target)
	, m_targetLookAt(target)
{
	SetFrequency(DEFAULT_FREQ);
}

// 更新
void Imase::SpringCamera::Update(float elapsedTime)
{
	// 視点の計算
	UpdateSpring(m_targetPosition, m_currentPosition, m_positionVelocity, elapsedTime);

	// 注視点の計算
	UpdateSpring(m_targetLookAt, m_currentLookAt, m_lookAtVelocity, elapsedTime);
}

// カメラを設定
void Imase::SpringCamera::SetCamera(
	const DirectX::SimpleMath::Vector3& eye,
	const DirectX::SimpleMath::Vector3& target
)
{
	m_currentPosition = eye;
	m_targetPosition = eye;
	m_positionVelocity = SimpleMath::Vector3::Zero;

	m_currentLookAt = target;
	m_targetLookAt = target;
	m_lookAtVelocity = SimpleMath::Vector3::Zero;
}

// ターゲットを設定
void Imase::SpringCamera::SetTarget(
	const DirectX::SimpleMath::Vector3& eye,
	const DirectX::SimpleMath::Vector3& target
)
{
	m_targetPosition = eye;
	m_targetLookAt = target;
}

// カメラの追従速度を設定する
// 値が大きいほど素早く目標に収束する（臨界減衰・オーバーシュートなし）
// 収束時間の目安: 約 4 / freq 秒
void Imase::SpringCamera::SetFrequency(float freq)
{
	m_stiffness = freq * freq;
	m_damping = 2.0f * freq;
}

// バネによる位置の更新
void Imase::SpringCamera::UpdateSpring(
	const DirectX::SimpleMath::Vector3& target,
	DirectX::SimpleMath::Vector3& current,
	DirectX::SimpleMath::Vector3& velocity,
	float elapsedTime
) const
{
	// 目標 - 現在
	SimpleMath::Vector3 delta = target - current;

	// 加速度を計算: a = [k * (目標 - 現在) - b * 速度] / 質量(1とする)
	SimpleMath::Vector3 accel = m_stiffness * delta - m_damping * velocity;

	// 速度と位置を更新
	velocity += accel * elapsedTime;
	current += velocity * elapsedTime;
}