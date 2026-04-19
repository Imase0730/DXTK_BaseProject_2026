//--------------------------------------------------------------------------------------
// File: DebugCamera.h
//
// デバッグ用カメラクラス
//
// Date: 2018.4.15
// Author: Hideyasu Imase
//--------------------------------------------------------------------------------------
#pragma once

#include "CameraBase.h"

namespace Imase
{

	// デバッグ用カメラクラス
	class DebugCamera : public CameraBase
	{
		// カメラの距離
		static constexpr float DEFAULT_CAMERA_DISTANCE = 5.0f;

		// 横回転
		float m_yAngle, m_yTmp;

		// 縦回転
		float m_xAngle, m_xTmp;

		// ドラッグされた座標
		int m_x, m_y;

		float m_sx, m_sy;

		// 生成されたビュー行列
		DirectX::SimpleMath::Matrix m_view;

		// スクロールフォイール値
		int m_scrollWheelValue;

		// 視点
		DirectX::SimpleMath::Vector3 m_eye;

		// 注視点
		DirectX::SimpleMath::Vector3 m_target;

		// マウストラッカー
		DirectX::Mouse::ButtonStateTracker m_tracker;

		// スクリーンサイズ
		int m_screenW, m_screenH;

		// アクティブ
		bool m_isActive;

	private:

		// カメラ移動
		void MoveCamera(int x, int y);

	public:

		// コンストラクタ
		DebugCamera(int windowWidth, int windowHeight);

		// 更新
		void Update(float elapsedTime) override;

		// 視点取得
		DirectX::SimpleMath::Vector3 GetEyePosition() const override { return m_eye; }

		// 注視点取得
		DirectX::SimpleMath::Vector3 GetTargetPosition() const override { return m_target; }

		// ビュー行列の取得関数
		DirectX::SimpleMath::Matrix GetCameraMatrix() const { return m_view; }

		// ウインドウサイズの設定
		void SetWindowSize(int windowWidth, int windowHeight);

		// アクティブ設定
		void SetActive(bool active) { m_isActive = active; }
	};

}
