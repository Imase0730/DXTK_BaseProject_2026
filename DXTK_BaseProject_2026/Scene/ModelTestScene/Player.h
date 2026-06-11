#pragma once

#include "GameContext.h"
#include "ImaseLib/ModelCollision.h"

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

    // 境界球を取得する関数
    const DirectX::BoundingSphere& GetBoundingSphere() const
    {
        return m_boundingSphere;
    }

    // AABBを取得する関数
    const DirectX::BoundingBox& GetBoundingBox() const
    {
        return m_boundingBox;
    }

    // モデルデータの衝突判定を取得する関数
    const const Imase::ModelCollision* GetModelCollision() const
    {
        return m_modelCollision.get();
    }

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

    // 回転クォータニオン
    DirectX::SimpleMath::Quaternion m_rotate;

    // 境界球（衝突判定用）
    DirectX::BoundingSphere m_boundingSphere = {DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f), 1.0f};

    // AABB（衝突判定用）
    DirectX::BoundingBox m_boundingBox = 
                { DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f)   // 中心,
                , DirectX::SimpleMath::Vector3(1.0f, 0.5f, 1.0f)   // 中心から各軸の辺までの距離
                };

   	// モデルデータの衝突判定
    std::unique_ptr<Imase::ModelCollision> m_modelCollision;
};
