//--------------------------------------------------------------------------------------
// File: MoveCamera.cpp
//
// 補間移動カメラ
//
// Date: 2026.5.14
// Author: Hideyasu Imase
//--------------------------------------------------------------------------------------
#include "pch.h"
#include "MoveCamera.h"

using namespace DirectX;

// コンストラクタ
Imase::MoveCamera::MoveCamera(
    const DirectX::SimpleMath::Vector3& eye,
    const DirectX::SimpleMath::Vector3& target
)
    : CameraBase(eye, target)
{
    // カメラを初期化する
    SetCamera(eye, target);
}

// カメラを設定する関数
void Imase::MoveCamera::SetCamera(
	const DirectX::SimpleMath::Vector3& eye,
	const DirectX::SimpleMath::Vector3& target
)
{
    m_t = 1.0f;

    SetEyePosition(eye);
    m_eyeStart = eye;
    m_eyeEnd = eye;
    
    SetTargetPosition(target);
    m_targetStart = target;
    m_targetEnd = target;
}

// カメラの移動先を設定する関数
void Imase::MoveCamera::SetTarget(
    const DirectX::SimpleMath::Vector3& eye,
    const DirectX::SimpleMath::Vector3& target,
    float time
)
{
    m_t = 0.0f;

    m_eyeStart = GetEyePosition();
    m_eyeEnd = eye;

    m_targetStart = GetTargetPosition();
    m_targetEnd = target;

    m_time = time;
}

// 更新関数
void Imase::MoveCamera::Update(float elapsedTime)
{
    if ((m_time > 0.0f) && (m_t < 1.0f))
    {
        // 割合を変えていく
        m_t += 1.0f / m_time * elapsedTime;
        if (m_t > 1.0f)
        {
            m_t = 1.0f;
        }
    }

    // 視点の計算
    SetEyePosition(SimpleMath::Vector3::Lerp(m_eyeStart, m_eyeEnd, m_t));

    // 注視点の計算
    SetTargetPosition(SimpleMath::Vector3::Lerp(m_targetStart, m_targetEnd, m_t));
}

