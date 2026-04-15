//--------------------------------------------------------------------------------------
// File: SpriteTestScene.cpp
//
// 新規シーン作成時の元にするファイル
//
// Date: 2026.4.13
// Author: Hideyasu Imase
//--------------------------------------------------------------------------------------
#include "pch.h"
#include "SpriteTestScene.h"

using namespace DirectX;

// 更新
void SpriteTestScene::Update(Imase::ISceneController<SceneId>& sceneController, GameContext& gameContext)
{
	Imase::DebugRenderer& debugRenderer = gameContext.debugRenderer;

	Keyboard::KeyboardStateTracker& tracker = gameContext.keyboardTracker;

	// スペースキーが押された
	if (tracker.pressed.Space)
	{
		// 次のシーンへ
		sceneController.RequestSwitch(SceneId::ShootingGameScene);
	}

	float elapsedTime = static_cast<float>(gameContext.timer.GetElapsedSeconds());
	float move = SPEED * elapsedTime;

	auto kb = Keyboard::Get().GetState();

	if (kb.Up)
	{
		m_position.y -= move;
	}
	if (kb.Down)
	{
		m_position.y += move;
	}
	if (kb.Left)
	{
		m_position.x -= move;
	}
	if (kb.Right)
	{
		m_position.x += move;
	}

	auto mouse = Mouse::Get().GetState();

	if (mouse.leftButton)
	{
		m_scale += 0.01f;
	}
	else
	{
		float force = (4.0f - m_scale) * K;
		m_speed += force;
		m_speed *= FRICTION;
		m_scale += m_speed;
	}
}

// 描画
void SpriteTestScene::Render(GameContext& gameContext)
{
	m_spriteBatch->Begin(SpriteSortMode_Deferred, gameContext.commonStates.NonPremultiplied(), gameContext.commonStates.PointWrap());

	RECT srcRect = { 0, 0, 32, 32 };
	m_spriteBatch->Draw(m_texture.Get(), m_position, &srcRect, Colors::White, 0.0f, SimpleMath::Vector2(16.0f,16.0f), m_scale);

	m_spriteBatch->End();
}

// シーン切り替え時に呼び出される関数
void SpriteTestScene::OnEnter(GameContext& gameContext)
{
	m_position = SimpleMath::Vector2(400, 400);
	auto device = gameContext.deviceResources.GetD3DDevice();
	auto context = gameContext.deviceResources.GetD3DDeviceContext();

	m_spriteBatch = std::make_unique<SpriteBatch>(context);
	DX::ThrowIfFailed(
		CreateDDSTextureFromFile(device, L"Resources/Textures/ShootingGame.dds",nullptr,m_texture.ReleaseAndGetAddressOf())
	);
}
