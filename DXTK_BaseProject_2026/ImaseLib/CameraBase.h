//--------------------------------------------------------------------------------------
// File:  CameraBase.h
//
// カメラベースクラス
//
// Date: 2026.4.20
// Author: Hideyasu Imase
//--------------------------------------------------------------------------------------
#pragma once

#include "SimpleMath.h"

namespace Imase
{
    class CameraBase
    {
    public:
        // デストラクタ
        virtual ~CameraBase() = default;

        // 更新
        virtual void Update(float elapsedTime) = 0;

        // 視点取得
        virtual DirectX::SimpleMath::Vector3 GetEyePosition() const = 0;

        // 注視点取得
        virtual DirectX::SimpleMath::Vector3 GetTargetPosition() const = 0;
    };
}
