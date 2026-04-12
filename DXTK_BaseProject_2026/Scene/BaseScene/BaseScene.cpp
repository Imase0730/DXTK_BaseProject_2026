//--------------------------------------------------------------------------------------
// File: BaseScene.cpp
//
// 新規シーン作成時の元にするファイル
//
// Date: 2026.4.13
// Author: Hideyasu Imase
//--------------------------------------------------------------------------------------
#include "pch.h"
#include "BaseScene.h"
#include "../ShootingGameScene/ShootingGameScene.h"

using namespace DirectX;

// 更新
void BaseScene::Update(Imase::ISceneController<SceneId>& sceneController, GameContext& gameContext)
{
	float elapsedTime = float(gameContext.timer.GetElapsedSeconds());

	elapsedTime;

	Keyboard::KeyboardStateTracker& tracker = gameContext.keyboardTracker;

	// スペースキーが押された
	if (tracker.pressed.Space)
	{
		// 次のシーンへ
		sceneController.RequestSwitch(SceneId::ShootingGameScene);
	}

	auto& debugRenderer = gameContext.debugRenderer;

	debugRenderer.DrawText({ 0.0f, 0.0f }, L"BaseScene");
}

// 描画
void BaseScene::Render(GameContext& gameContext)
{
	gameContext;
}

// シーン切り替え時に呼び出される関数
void BaseScene::OnEnter(GameContext& gameContext)
{
	gameContext;
}
