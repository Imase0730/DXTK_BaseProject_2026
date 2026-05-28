//--------------------------------------------------------------------------------------
// File: ModelTestScene.h
//
// モデルテスト用シーン
//
// Date: 2026.4.13
// Author: Hideyasu Imase
//--------------------------------------------------------------------------------------
#pragma once

#include "ImaseLib/SceneManager.h"
#include "GameContext.h"
#include "../SceneId.h"

#include "ImaseLib/DebugCamera.h"
#include "Imaselib/GridFloor.h"

#include "Camera/FixedCamera.h"
#include "Camera/MoveCamera.h"
#include "Camera/SpringCamera.h"

#include "Player.h"

class ModelTestScene : public Imase::SceneBase<SceneId, GameContext>
{
public:

	// コンストラクタ
    ModelTestScene();

	// 更新
	void Update(Imase::ISceneController<SceneId>& sceneController, GameContext& gameContext) override;

	// 描画
	void Render(GameContext& gameContext) override;

	// シーン切り替え時に呼び出される関数
	void OnEnter(GameContext& gameContext) override;

private:

	// ビュー行列
    DirectX::SimpleMath::Matrix m_view;

	// プロジェクション行列
    DirectX::SimpleMath::Matrix m_projection;

	// デバッグカメラ
    std::unique_ptr<Imase::DebugCamera> m_debugCamera;

	// グリッドフロア
    std::unique_ptr<Imase::GridFloor> m_gridFloor;

	// プロジェクション行列を作成する関数
    DirectX::SimpleMath::Matrix CreateProjectionMatrix(GameContext& gameContext);

	// ウインドウサイズ変更時に呼び出される関数
    void OnWindowSizeChanged(GameContext& gameContext) override;

	// モデルハンドル
    std::unique_ptr<DirectX::Model> m_model;

	// 固定カメラ
    Imase::FixedCamera m_fixedCamera;
	
	// 補間移動カメラ
    Imase::MoveCamera m_moveCamera;

	// バネカメラ
    Imase::SpringCamera m_springCamera;

	// プレイヤーへのユニークポインタ
    std::unique_ptr<Player> m_player;

	enum class CameraMode
	{
		Title,
		GamePlay,
	};

	// カメラモード
    CameraMode m_cameraMode = CameraMode::Title;

	// ゲームプレイ用カメラ
    void GamePlayCamera(float elapsedTime); 
	
	// タイトル用カメラ
    void TitleCamera(float elapsedTime);

	// カメラのY軸に対する回転角度（タイトル用）
    float m_titleAngleRad = 0.0f;

	// タイトル用カメラの回転角度（１秒間あたりの回転角度）
    static constexpr float TITLE_CAMERA_MOVE_ANGLE_DEG = 10.0f;

	// モデルハンドル（矢印）
    std::unique_ptr<DirectX::Model> m_arrowModel;

	// ---- オイラー角テスト用 ----- //
    float m_angleRad_X = 0.0f;
    float m_angleRad_Y = 0.0f;
    float m_angleRad_Z = 0.0f;

	// １秒間あたりの回転角度
    static constexpr float ROTATE_ANGLE_DEG = 90.0f;

	// クォータニオン
    DirectX::SimpleMath::Quaternion m_quaternion;

	// モデルハンドル（ターゲット）
    std::unique_ptr<DirectX::Model> m_targetModel;

	// ターゲットの位置
    DirectX::SimpleMath::Vector3 m_targetPosition = { 0.0f, 0.0f, -2.0f };

};

