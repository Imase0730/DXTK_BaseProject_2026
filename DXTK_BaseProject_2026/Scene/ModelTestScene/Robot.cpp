#include "pch.h"
#include "Robot.h"

using namespace DirectX;

// コンストラクタ
Robot::Robot(
    const GameContext& gameContext,
    const DirectX::SimpleMath::Matrix& view,
    const DirectX::SimpleMath::Matrix& projection,
    const std::array<std::unique_ptr<DirectX::Model>, PARTS_CNT>& model
)
    : m_gameContext(gameContext)
    , m_view(view)
    , m_projection(projection)
{
    // 各モデルへのポインタを取得
    for (size_t i = 0; i < PARTS_CNT; i++)
    {
        m_pModels[i] = model[i].get();
    }

    // 親のインデックスを設定する
    m_parentIndexes[static_cast<int>(Parts::LEG)] = -1;                                 // 親はいない
    m_parentIndexes[static_cast<int>(Parts::BODY)] = static_cast<int>(Parts::LEG);      // 親はLEG
    m_parentIndexes[static_cast<int>(Parts::HEAD)] = static_cast<int>(Parts::BODY);     // 親はBODY
    m_parentIndexes[static_cast<int>(Parts::ARM_R)] = static_cast<int>(Parts::BODY);    // 親はBODY
    m_parentIndexes[static_cast<int>(Parts::ARM_L)] = static_cast<int>(Parts::BODY);    // 親はBODY
    m_parentIndexes[static_cast<int>(Parts::MISSILE)] = static_cast<int>(Parts::ARM_L); // 親はARM_L

    // 初期化行列を作成
    m_initializeMatrices[static_cast<int>(Parts::LEG)] =
        SimpleMath::Matrix::CreateTranslation(0.0f, 0.0f, 0.0f);
    m_initializeMatrices[static_cast<int>(Parts::BODY)] =
        SimpleMath::Matrix::CreateTranslation(0.0f, 0.32f, 0.0f);       // LEGからの相対距離
    m_initializeMatrices[static_cast<int>(Parts::HEAD)] =
        SimpleMath::Matrix::CreateTranslation(0.0f, 0.43f, 0.0f);       // BODYからの相対距離
    m_initializeMatrices[static_cast<int>(Parts::ARM_R)] =
        SimpleMath::Matrix::CreateTranslation(0.18f, 0.4f, 0.0f);       // BODYからの相対距離
    m_initializeMatrices[static_cast<int>(Parts::ARM_L)] =
        SimpleMath::Matrix::CreateTranslation(-0.18f, 0.4f, 0.0f);      // BODYからの相対距離
    m_initializeMatrices[static_cast<int>(Parts::MISSILE)] =
        SimpleMath::Matrix::CreateTranslation(-0.1f, -0.2f, -0.39f);    // ARM_Lからの相対距離
}

// 更新
void Robot::Update(float elapsedTime)
{
    auto kb = Keyboard::Get().GetState();

    // 左キーで左回転
    if (!kb.LeftShift && kb.Left)
    {
        m_facingAngleRad += XMConvertToRadians(ROTATE_SPEED_DEG) * elapsedTime;
    }
    // 右キーで右回転
    if (!kb.LeftShift && kb.Right)
    {
        m_facingAngleRad -= XMConvertToRadians(ROTATE_SPEED_DEG) * elapsedTime;
    }

    // 回転クォータニオンを作成する
    m_rotates[static_cast<int>(Parts::LEG)] =
        SimpleMath::Quaternion::CreateFromYawPitchRoll(m_facingAngleRad, 0.0f, 0.0f);

    // Forward(0,0,-1)を回転クォータニオンで回転させる
    SimpleMath::Vector3 v = 
        SimpleMath::Vector3::Transform(SimpleMath::Vector3::Forward, m_rotates[static_cast<int>(Parts::LEG)]);

    // 上キーで前進
    if (!kb.LeftShift && kb.Up)
    {
        m_positions[static_cast<int>(Parts::LEG)] += v * MOVE_SPEED * elapsedTime;
    }
    // 下キーで後進
    if (!kb.LeftShift && kb.Down)
    {
        m_positions[static_cast<int>(Parts::LEG)] -= v * MOVE_SPEED * elapsedTime;
    }

    // 左シフト＋左キーで体を回転
    if (kb.LeftShift && kb.Left)
    {
        m_rotates[static_cast<int>(Parts::BODY)] *= SimpleMath::Quaternion::CreateFromAxisAngle(
            SimpleMath::Vector3::UnitY, XMConvertToRadians(ROTATE_SPEED_DEG) * elapsedTime);
    }
    // 左シフト＋右キーで体を回転
    if (kb.LeftShift && kb.Right)
    {
        m_rotates[static_cast<int>(Parts::BODY)] *= SimpleMath::Quaternion::CreateFromAxisAngle(
            SimpleMath::Vector3::UnitY, -XMConvertToRadians(ROTATE_SPEED_DEG) * elapsedTime);
    }
    
    // 左シフト＋上キーで左腕を回転
    if (kb.LeftShift && kb.Up)
    {
        m_rotates[static_cast<int>(Parts::ARM_L)] *= SimpleMath::Quaternion::CreateFromAxisAngle(
            SimpleMath::Vector3::UnitX, XMConvertToRadians(ROTATE_SPEED_DEG) * elapsedTime);
    }
    // 左シフト＋下キーで左腕を回転
    if (kb.LeftShift && kb.Down)
    {
        m_rotates[static_cast<int>(Parts::ARM_L)] *= SimpleMath::Quaternion::CreateFromAxisAngle(
            SimpleMath::Vector3::UnitX, -XMConvertToRadians(ROTATE_SPEED_DEG) * elapsedTime);
    }

    // スペースキーでミサイルを発射
    if (kb.Space)
    {
        // 発射フラグ（ON）
        m_fireFlag = true;

        // ミサイルを切り離す
        m_parentIndexes[static_cast<int>(Parts::MISSILE)] = -1;

        // ミサイルの位置をワールド行列から取得
        m_positions[static_cast<int>(Parts::MISSILE)] = 
            m_worldMatrices[static_cast<int>(Parts::MISSILE)].Translation();

        // ミサイルの回転をワールド行列から取得
        m_rotates[static_cast<int>(Parts::MISSILE)] =
            SimpleMath::Quaternion::CreateFromRotationMatrix(m_worldMatrices[static_cast<int>(Parts::MISSILE)]);

    }

}

// 描画
void Robot::Render()
{
    // 各パーツの変換行列を作成
    for (size_t i = 0; i < PARTS_CNT; i++)
    {
        m_transformMatrices[i] = SimpleMath::Matrix::CreateFromQuaternion(m_rotates[i])
                               * SimpleMath::Matrix::CreateTranslation(m_positions[i]);
    }

    // 各パーツのワールド行列を作成
    for (size_t i = 0; i < PARTS_CNT; i++)
    {
        // パーツの変換行列をコピー
        m_worldMatrices[i] = m_transformMatrices[i];

        // 親がいれば
        if (m_parentIndexes[i] != -1)
        {
            // 初期化行列をかける
            m_worldMatrices[i] *= m_initializeMatrices[i];
            // 親のワールド行列をかける
            m_worldMatrices[i] *= m_worldMatrices[m_parentIndexes[i]];
        }
    }

    // 各パーツの描画
    for (size_t i = 0; i < PARTS_CNT; i++)
    {
        m_pModels[i]->Draw( m_gameContext.deviceResources.GetD3DDeviceContext(), m_gameContext.commonStates
                          , m_worldMatrices[i], m_view, m_projection );

    }
}

// 位置を取得する関数
DirectX::SimpleMath::Vector3 Robot::GetPosition() const
{
    return m_positions[static_cast<int>(Parts::LEG)];
}
