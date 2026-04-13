//--------------------------------------------------------------------------------------
// File: EnemyGeneratorTask.h
//
// 敵生成タスク
//
// Date: 2026.4.5
// Author: Hideyasu Imase
//--------------------------------------------------------------------------------------
#pragma once

#include "ImaseLib/Task.h"
#include "GameContext.h"

class EnemyGeneratorTask : public Imase::Task
{
private:

	// １秒間あたりの出現数
	static constexpr int SPAWN_COUNT_PERSECOND = 3;

	// 出現間隔
	static constexpr float SPAWN_INTERVAL = 1.0f / SPAWN_COUNT_PERSECOND;

public:

	// コンストラクタ
	EnemyGeneratorTask(
		const GameContext& gameContext,
		DirectX::SpriteBatch& spriteBatch,
		ID3D11ShaderResourceView* pTexture
	);

	// 更新
	bool Update(float elapsedTime) override;

private:

	// ゲームコンテキストへのポインタ
	const GameContext& m_gameContext;

	// スプライトバッチへのポインタ
	DirectX::SpriteBatch& m_spriteBatch;

	// テクスチャへのポインタ
	ID3D11ShaderResourceView* m_pTexture = nullptr;

	// 敵の出現用のタイマー
	float m_spawnTimer = 0.0f;

};