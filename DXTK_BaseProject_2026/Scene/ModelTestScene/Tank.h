#pragma once

#include "GameContext.h"

class Tank
{
public:

    // パーツ
    enum class Parts
    {
        BODY,       // 車体
        HEAD,       // 砲塔
        BARREL,     // 砲身

        PARTS_CNT
    };

    // パーツ数
    static constexpr int PARTS_CNT = static_cast<int>(Parts::PARTS_CNT);

	// コンストラクタ
    Tank(
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

    // 戦車のY座標を設定する関数
    void SetPositionY(float y)
    {
        m_position.y = y;
    }

    // 戦車の傾きを設定する関数
    void SetTilt(DirectX::SimpleMath::Quaternion tilt)
    {
        m_rotate *= tilt;
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

};
