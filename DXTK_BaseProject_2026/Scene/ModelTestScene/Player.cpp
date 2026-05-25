#include "pch.h"
#include "Player.h"

using namespace DirectX;

// コンストラクタ
Player::Player(
    const GameContext& gameContext,
    const DirectX::SimpleMath::Matrix& view,
    const DirectX::SimpleMath::Matrix& projection,
    DirectX::Model* pModel
)
    : m_gameContext(gameContext)
    , m_view(view)
    , m_projection(projection)
    , m_pModel(pModel)
{
}

// 更新
void Player::Update(float elapsedTime)
{
    auto kb = Keyboard::Get().GetState();

    // 左キーで左回転
    if (kb.Left)
    {
        m_facingAngleRad += XMConvertToRadians(ROTATE_SPEED_DEG) * elapsedTime;
    }
    // 右キーで右回転
    if (kb.Right)
    {
        m_facingAngleRad -= XMConvertToRadians(ROTATE_SPEED_DEG) * elapsedTime;
    }

    // 回転行列を作成する
    SimpleMath::Matrix rotY = SimpleMath::Matrix::CreateRotationY(m_facingAngleRad);

    // Forward(0,0,-1)を回転行列で回転させる
    SimpleMath::Vector3 v = SimpleMath::Vector3::Transform(SimpleMath::Vector3::Forward, rotY);

    // 上キーで前進
    if (kb.Up)
    {
        m_position += v * MOVE_SPEED * elapsedTime;
    }
    // 下キーで後進
    if (kb.Down)
    {
        m_position -= v * MOVE_SPEED * elapsedTime;
    }
}

// 描画
void Player::Render()
{
    SimpleMath::Matrix world;

    // ワールド行列を作成する
    world = SimpleMath::Matrix::CreateRotationY(m_facingAngleRad)
          * SimpleMath::Matrix::CreateTranslation(m_position);

    // モデルの描画
    m_pModel->Draw(
        m_gameContext.deviceResources.GetD3DDeviceContext(),
        m_gameContext.commonStates,
        world,
        m_view,
        m_projection
    );
}

// 位置を取得する関数
DirectX::SimpleMath::Vector3 Player::GetPosition() const
{
    return m_position;
}

// 向いている角度を取得する関数
float Player::GetFacingAngleRad() const
{
    return m_facingAngleRad;
}
