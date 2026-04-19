//--------------------------------------------------------------------------------------
// File: DebugCamera.cpp
//
// デバッグ用カメラクラス
//
// Date: 2018.4.15
// Author: Hideyasu Imase
//--------------------------------------------------------------------------------------
#include "pch.h"
#include "DebugCamera.h"

using namespace DirectX;

// コンストラクタ
Imase::DebugCamera::DebugCamera(int windowWidth, int windowHeight)
	: m_yAngle(0.0f)
	, m_yTmp(0.0f)
	, m_xAngle(0.0f)
	, m_xTmp(0.0f)
	, m_x(0)
	, m_y(0)
	, m_scrollWheelValue(0)
	, m_screenW(windowWidth)
	, m_screenH(windowHeight)
	, m_isActive(true)
{
	SetWindowSize(windowWidth, windowHeight);

	// マウスのフォイール値をリセット
	Mouse::Get().ResetScrollWheelValue();
}

// 更新
void Imase::DebugCamera::Update(float elapsedTime)
{
	elapsedTime;

	auto state = Mouse::Get().GetState();

	// 相対モードなら何もしない
	if (state.positionMode == Mouse::MODE_RELATIVE) return;

	m_tracker.Update(state);

	// マウスの左ボタンが押された
	if (m_tracker.leftButton == Mouse::ButtonStateTracker::ButtonState::PRESSED)
	{
		// マウスの座標を取得
		m_x = state.x;
		m_y = state.y;
	}
	else if (m_tracker.leftButton == Mouse::ButtonStateTracker::ButtonState::RELEASED)
	{
		// 現在の回転を保存
		m_xAngle = m_xTmp;
		m_yAngle = m_yTmp;
	}
	// マウスのボタンが押されていたらカメラを移動させる
	if (m_isActive && state.leftButton)
	{
		MoveCamera(state.x, state.y);
	}

	// マウスのフォイール値を取得
	m_scrollWheelValue = state.scrollWheelValue;
	if (m_scrollWheelValue > 0)
	{
		m_scrollWheelValue = 0;
		Mouse::Get().ResetScrollWheelValue();
	}

	// ビュー行列を算出する
	SimpleMath::Matrix rotY = SimpleMath::Matrix::CreateRotationY(m_yTmp);
	SimpleMath::Matrix rotX = SimpleMath::Matrix::CreateRotationX(m_xTmp);

	SimpleMath::Matrix rt = rotY * rotX;

	SimpleMath::Vector3 eye(0.0f, 1.0f, 1.0f);
	SimpleMath::Vector3 target(0.0f, 0.0f, 0.0f);
	SimpleMath::Vector3 up(0.0f, 1.0f, 0.0f);

	eye = SimpleMath::Vector3::Transform(eye, rt.Invert());
	eye *= (DEFAULT_CAMERA_DISTANCE - m_scrollWheelValue / 100);
	up = SimpleMath::Vector3::Transform(up, rt.Invert());

	m_eye = eye;
	m_target = target;

	m_view = SimpleMath::Matrix::CreateLookAt(eye, target, up);
}

// カメラ移動
void Imase::DebugCamera::MoveCamera(int x, int y)
{
	// マウスポインタの位置のドラッグ開始位置からの変位 (相対値)
	float dx = (x - m_x) * m_sx;
	float dy = (y - m_y) * m_sy;

	if (dx != 0.0f || dy != 0.0f)
	{
		// Ｙ軸の回転
		float yAngle = dx * XM_PI;
		// Ｘ軸の回転
		float xAngle = dy * XM_PI;

		m_xTmp = m_xAngle + xAngle;
		m_yTmp = m_yAngle + yAngle;
	}
}

// ウインドウサイズの設定
void Imase::DebugCamera::SetWindowSize(int windowWidth, int windowHeight)
{
	// 画面サイズに対する相対的なスケールに調整
	m_sx = 1.0f / float(windowWidth);
	m_sy = 1.0f / float(windowHeight);
}
