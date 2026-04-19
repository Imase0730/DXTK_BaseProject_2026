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
#include "ImaseLib/GridFloor.h"
#include "GameCamera.h"

class ModelTestScene : public Imase::SceneBase<SceneId, GameContext>
{
public:

	// 更新
	void Update(Imase::ISceneController<SceneId>& sceneController, GameContext& gameContext) override;

	// 描画
	void Render(GameContext& gameContext) override;

	// シーン切り替え時に呼び出される関数
	void OnEnter(GameContext& gameContext) override;

private:

	// 移動の速さ(m/s)
	static constexpr float SPEED = 5.0f;

	// デバッグ用カメラ
	std::unique_ptr<Imase::DebugCamera> m_debugCamera;

	// グリッドフロア
	std::unique_ptr<Imase::GridFloor> m_gridFloor;

	// モデル
	std::unique_ptr<DirectX::Model> m_model;

	// プロジェクション行列
	DirectX::SimpleMath::Matrix m_projection;

	// プロジェクション行列の初期化
	void InitProjectionMatrix(GameContext& gameContext);

	// 位置
	DirectX::SimpleMath::Vector3 m_position;

	// ゲームカメラ
	std::unique_ptr<GameCamera> m_camera;

};

