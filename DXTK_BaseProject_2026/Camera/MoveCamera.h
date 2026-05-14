//--------------------------------------------------------------------------------------
// File: MoveCamera.h
//
// 補間移動カメラ
//
// Date: 2026.5.14
// Author: Hideyasu Imase
//--------------------------------------------------------------------------------------
#pragma once

#include "CameraBase.h"

namespace Imase
{
    // 補間移動カメラクラス
    class MoveCamera : public CameraBase
    {
    public:

        // コンストラクタ
        MoveCamera(
            const DirectX::SimpleMath::Vector3& eye,
            const DirectX::SimpleMath::Vector3& target
        );

        // カメラを設定する関数
        void SetCamera(
            const DirectX::SimpleMath::Vector3& eye,
            const DirectX::SimpleMath::Vector3& target
        );

        // カメラの移動先を設定する関数
        void SetTarget(
            const DirectX::SimpleMath::Vector3& eye,
            const DirectX::SimpleMath::Vector3& target,
            float time
        );

        // 更新関数
        void Update(float elapsedTime) override;

    private:

        // 移動までにかかる時間
        float m_time = 0.0f;

        // 位置の割合
        float m_t = 0.0f;

        // ----- 視点 ----- //
        // 移動開始位置
        DirectX::SimpleMath::Vector3 m_eyeStart;
        // 移動終了位置
        DirectX::SimpleMath::Vector3 m_eyeEnd;

        // ----- 注視点 ----- //
        // 移動開始位置
        DirectX::SimpleMath::Vector3 m_targetStart;
        // 移動終了位置
        DirectX::SimpleMath::Vector3 m_targetEnd;

    };
}
