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
};

