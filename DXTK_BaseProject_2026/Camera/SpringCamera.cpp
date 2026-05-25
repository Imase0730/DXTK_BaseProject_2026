//--------------------------------------------------------------------------------------
// File: SpringCamera.cpp
//
// スプリングカメラ
//
// Date: 2026.5.17
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
    : CameraBase(eye, target)
	, m_eyeTarget(eye)
	, m_targetTarget(target)
{
    // カメラを初期化する
    SetCamera(eye, target);
}

// 更新
void Imase::SpringCamera::Update(float elapsedTime)
{
	// 視点の計算
    SimpleMath::Vector3 eye = GetEyePosition();
    UpdateSpring(m_eyeTarget, eye, m_eyeVelocity, elapsedTime);
	SetEyePosition(eye);

	// 注視点の計算
    SimpleMath::Vector3 target = GetTargetPosition();
    UpdateSpring(m_targetTarget, target, m_targetVelocity, elapsedTime);
	SetTargetPosition(target);
}

// カメラを設定する関数
void Imase::SpringCamera::SetCamera(
	const DirectX::SimpleMath::Vector3& eye,
	const DirectX::SimpleMath::Vector3& target
)
{
    SetEyePosition(eye);
    m_eyeTarget = GetEyePosition();
    m_eyeVelocity = SimpleMath::Vector3::Zero;

	SetTargetPosition(target);
    m_targetTarget = GetTargetPosition();
	m_targetVelocity = SimpleMath::Vector3::Zero;
}

// ターゲットを設定する関数
void Imase::SpringCamera::SetTarget(
	const DirectX::SimpleMath::Vector3& eye,
	const DirectX::SimpleMath::Vector3& target,
	float time
)
{
	m_eyeTarget = eye;
	m_targetTarget = target;
    SetMoveTime(time);
}

// 移動完了までのおおよその時間を設定する関数
void Imase::SpringCamera::SetMoveTime(float time)
{
    SetFrequency(4.0f / time);
}

// 移動終了か調べる関数
bool Imase::SpringCamera::IsArrived() const
{
    // 位置の許容誤差
    constexpr float POSITION_EPSILON = 0.01f;

    // 速度の許容誤差
    constexpr float VELOCITY_EPSILON = 0.01f;

    // 視点
    bool eyeArrived = ((m_eyeTarget - GetEyePosition()).LengthSquared() < POSITION_EPSILON * POSITION_EPSILON) &&
                      (m_eyeVelocity.LengthSquared() < VELOCITY_EPSILON * VELOCITY_EPSILON);

    // 注視点
    bool targetArrived = ((m_targetTarget - GetTargetPosition()).LengthSquared() < POSITION_EPSILON * POSITION_EPSILON) &&
						 (m_targetVelocity.LengthSquared() < VELOCITY_EPSILON * VELOCITY_EPSILON);

    return (eyeArrived && targetArrived);
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

// カメラの追従速度を設定する
// 値が大きいほど素早く目標に収束する（臨界減衰・オーバーシュートなし）
// 収束時間の目安: 約 4 / freq 秒
void Imase::SpringCamera::SetFrequency(float freq)
{
    m_stiffness = freq * freq;
    m_damping = 2.0f * freq;
}
