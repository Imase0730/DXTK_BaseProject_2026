//--------------------------------------------------------------------------------------
// File: ModelTestScene.cpp
//
// モデルテスト用シーン
//
// Date: 2026.4.13
// Author: Hideyasu Imase
//--------------------------------------------------------------------------------------
#include "pch.h"
#include "ModelTestScene.h"

using namespace DirectX;

// 更新
void ModelTestScene::Update(Imase::ISceneController<SceneId>& sceneController, GameContext& gameContext)
{
	Imase::DebugRenderer& debugRenderer = gameContext.debugRenderer;

	debugRenderer.DrawText({ 0.0f, 0.0f }, L"ModelTestScene");
}

// 描画
void ModelTestScene::Render(GameContext& gameContext)
{
}

// シーン切り替え時に呼び出される関数
void ModelTestScene::OnEnter(GameContext& gameContext)
{
}

