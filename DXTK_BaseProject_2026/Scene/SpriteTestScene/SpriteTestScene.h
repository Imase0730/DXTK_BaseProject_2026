//--------------------------------------------------------------------------------------
// File: SpriteTestScene.h
//
// 新規シーン作成時の元にするファイル
//
// Date: 2026.4.13
// Author: Hideyasu Imase
//--------------------------------------------------------------------------------------
#pragma once

#include "ImaseLib/SceneManager.h"
#include "GameContext.h"
#include "../SceneId.h"

class SpriteTestScene : public Imase::SceneBase<SceneId, GameContext>
{
public:

	// 更新
	void Update(Imase::ISceneController<SceneId>& sceneController, GameContext& gameContext) override;

	// 描画
	void Render(GameContext& gameContext) override;

	// シーン切り替え時に呼び出される関数
	void OnEnter(GameContext& gameContext) override;

private:

	static constexpr float SPEED = 600.0f;

	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;

	DirectX::SimpleMath::Vector2 m_position = { 0.0f, 0.0f };

	static constexpr float K = 0.08f;
	static constexpr float FRICTION = 0.97f;

	float m_scale = 4.0f;
	float m_speed = 0.0f;
};

