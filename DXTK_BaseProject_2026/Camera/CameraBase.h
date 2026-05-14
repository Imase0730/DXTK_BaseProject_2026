//--------------------------------------------------------------------------------------
// File: CameraBase.h
//
// カメラの基底クラス
//
// Date: 2026.5.14
// Author: Hideyasu Imase
//--------------------------------------------------------------------------------------
#pragma once

#include "SimpleMath.h"

namespace Imase
{
	class CameraBase
	{
    public:

        // コンストラクタ
        CameraBase(
            const DirectX::SimpleMath::Vector3& eye,
            const DirectX::SimpleMath::Vector3& target
        )
            : m_eye(eye)
            , m_target(target)
        {
            // ビュー行列を初期化する
            UpdateViewMatrix();
        }

        // デストラクタ
        virtual ~CameraBase() = default;

        // 更新関数
        virtual void Update(float elapsedTime)
        {
            UNREFERENCED_PARAMETER(elapsedTime);
        }

		// 視点を取得する関数
        DirectX::SimpleMath::Vector3 GetEyePosition() const
        {
            return m_eye;
        }

		// 注視点を取得する関数
        DirectX::SimpleMath::Vector3 GetTargetPosition() const
        {
            return m_target;
        }

        // ビュー行列を取得する関数
        DirectX::SimpleMath::Matrix GetViewMatrix() const
        {
            return m_view;
        }

    protected:
        
        // 視点の設定関数
        void SetEyePosition(DirectX::SimpleMath::Vector3 eye)
        {
            m_eye = eye;
            UpdateViewMatrix();
        }

        // 注視点の設定関数
        void SetTargetPosition(DirectX::SimpleMath::Vector3 target)
        {
            m_target = target;
            UpdateViewMatrix();
        }

        // カメラの上方向を取得する関数
        virtual DirectX::SimpleMath::Vector3 GetUp() const
        {
            return DirectX::SimpleMath::Vector3::Up;
        }

    private:

		// 視点
        DirectX::SimpleMath::Vector3 m_eye;

		// 注視点
        DirectX::SimpleMath::Vector3 m_target;

		// ビュー行列
        DirectX::SimpleMath::Matrix m_view;

        // ビュー行列を更新する関数
        void UpdateViewMatrix()
        {
            m_view = DirectX::SimpleMath::Matrix::CreateLookAt(m_eye, m_target, GetUp());
        }

	};
}
