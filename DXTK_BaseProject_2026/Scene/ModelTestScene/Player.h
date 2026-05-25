#pragma once

#include "GameContext.h"

class Player
{
public:

	// コンストラクタ
    Player(
        const GameContext& gameContext,
        const DirectX::SimpleMath::Matrix& view,
        const DirectX::SimpleMath::Matrix& projection,
        DirectX::Model* pModel
	);

    // 更新
    void Update(float elapsedTime);

    // 描画
    void Render();

    // 位置を取得する関数
    DirectX::SimpleMath::Vector3 GetPosition() const;

    // 向いている角度を取得する関数
    float GetFacingAngleRad() const;

private:

    // 回転の速さ（１秒間あたりの角度）
    static constexpr float ROTATE_SPEED_DEG = 90.0f;

    // 移動の速さ（１秒間あたりの移動量）
    static constexpr float MOVE_SPEED = 2.0f;

	// ゲームコンテキスト
    const GameContext& m_gameContext;

	// ビュー行列
    const DirectX::SimpleMath::Matrix& m_view;

	// プロジェクション行列
    const DirectX::SimpleMath::Matrix& m_projection;

	// モデルへのポインタ
    DirectX::Model* m_pModel = nullptr;

    // 位置
    DirectX::SimpleMath::Vector3 m_position;

    // 向いている角度（ラジアン）
    float m_facingAngleRad = 0.0f;

};
