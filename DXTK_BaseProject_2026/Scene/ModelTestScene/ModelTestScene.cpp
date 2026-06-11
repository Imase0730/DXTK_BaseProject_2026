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

// 球とAABBの衝突判定関数
bool HitCheck_Sphere2AABB(const BoundingSphere& sphere, const BoundingBox& box)
{
    // AABBの『各軸の最大値、最小値』を求める
    SimpleMath::Vector3 min = SimpleMath::Vector3(box.Center) - SimpleMath::Vector3(box.Extents);
    SimpleMath::Vector3 max = SimpleMath::Vector3(box.Center) + SimpleMath::Vector3(box.Extents);

    // 各軸の『球の中心とAABBとの距離』を求める
    min = min - SimpleMath::Vector3::Min(sphere.Center, min);
    max = SimpleMath::Vector3::Max(sphere.Center, max) - max;

    // 『球の中心とAABBの距離の２乗』を内積を使ってもとめる
    float distSq = min.Dot(min) + max.Dot(max);

    // 『球の中心とAABBの距離の２乗』と『球の半径の２乗』を比較する
    return distSq <= sphere.Radius * sphere.Radius;
}

// AABBとAABBの衝突判定関数
bool HitCheck_AABB2AABB(const BoundingBox& box1, const BoundingBox& box2)
{
    if (fabsf(box1.Center.x - box2.Center.x) > (box1.Extents.x + box2.Extents.x))
        return false;
    if (fabsf(box1.Center.y - box2.Center.y) > (box1.Extents.y + box2.Extents.y))
        return false;
    if (fabsf(box1.Center.z - box2.Center.z) > (box1.Extents.z + box2.Extents.z))
        return false;
    return true;
}

// 球と球の衝突判定関数
bool HitCheck_Sphere2Sphere(const BoundingSphere& sphere1, const BoundingSphere& sphere2)
{
    // 2つの球の中心の間の距離の平方を計算
    SimpleMath::Vector3 d = SimpleMath::Vector3(sphere1.Center) - SimpleMath::Vector3(sphere2.Center);
    float distSq = d.Dot(d);
    // 平方した距離が平方した半径の合計よりも小さい場合に球は交差している
    float radiusSum = sphere1.Radius + sphere2.Radius;
    return distSq <= radiusSum * radiusSum;
}

// コンストラクタ
ModelTestScene::ModelTestScene()
    : m_fixedCamera(SimpleMath::Vector3(0.0f, 2.0f, 5.0f), SimpleMath::Vector3(0.0f, 0.0f, 0.0f))
	, m_moveCamera(SimpleMath::Vector3(0.0f, 2.0f, 5.0f), SimpleMath::Vector3(0.0f, 0.0f, 0.0f))
	, m_springCamera(SimpleMath::Vector3(0.0f, 2.0f, 5.0f), SimpleMath::Vector3(0.0f, 0.0f, 0.0f))
{
}

// 更新
void ModelTestScene::Update(Imase::ISceneController<SceneId>& sceneController, GameContext& gameContext)
{
	Imase::DebugRenderer& debugRenderer = gameContext.debugRenderer;

	// 経過時間を取得する
	float elapsedTime = static_cast<float>(gameContext.timer.GetElapsedSeconds());

    // スペースキーで操作対象を切り替える
    if (gameContext.keyboardTracker.pressed.Space)
    {
        if (m_control == ControlPlayer::P1)
        {
            m_control = ControlPlayer::P2;
        }
        else
        {
            m_control = ControlPlayer::P1;
        }
    }

	// プレイヤーの更新
    if (m_control == ControlPlayer::P1)
    {
        m_player1->Update(elapsedTime);
    }
    else
    {
        m_player2->Update(elapsedTime);
    }

    // デバッグカメラの更新
    m_debugCamera->Update(elapsedTime);

    // P1とP2の衝突判定（球と球）
    //if (HitCheck_Sphere2Sphere(m_player1->GetBoundingSphere(), m_player2->GetBoundingSphere()))
    //{
    //    debugRenderer.DrawText({0.0f, 50.0f}, L"Hit!");
    //}

    // P1とP2の衝突判定（AABBとAABB）
    //if (HitCheck_AABB2AABB(m_player1->GetBoundingBox(), m_player2->GetBoundingBox()))
    //{
    //    debugRenderer.DrawText({0.0f, 50.0f}, L"Hit!");
    //}

    // P1とP2の衝突判定（球とAABB）
    //if (HitCheck_Sphere2AABB(m_player1->GetBoundingSphere(), m_player2->GetBoundingBox()))
    //{
    //    debugRenderer.DrawText({0.0f, 50.0f}, L"Hit!");
    //}

    // P1とP2の衝突判定（モデルデータ同士）
    if (m_player1->GetModelCollision()->Intersects(m_player2->GetModelCollision()))
    {
        debugRenderer.DrawText({0.0f, 50.0f}, L"Hit!");
    }

	debugRenderer.DrawText({ 0.0f, 0.0f }, L"ModelTestScene");
}

// 描画
void ModelTestScene::Render(GameContext& gameContext)
{
    // DirectX3Dのデバイスコンテキストを取得する
    auto context = gameContext.deviceResources.GetD3DDeviceContext();

	SimpleMath::Matrix world;

	// デバッグカメラからビュー行列を取得する
    m_view = m_debugCamera->GetCameraMatrix();

	// グリッドフロアの描画
	m_gridFloor->Render(context, m_view, m_projection);

	// ----- P1 ----- //
    // プレイヤーの描画
    m_player1->Render();
    // プレイヤーのコリジョン情報の表示
    m_player1->GetModelCollision()->AddCollisionRenderer(m_collisionRenderer.get());

	// ----- P2 ----- //
    // プレイヤーの描画
    m_player2->Render();
    // プレイヤーのコリジョン情報の表示
    m_player2->GetModelCollision()->AddCollisionRenderer(m_collisionRenderer.get());

    // コリジョン情報の描画
    m_collisionRenderer->DrawCollision(context, gameContext.commonStates, m_view, m_projection);
}

// シーン切り替え時に呼び出される関数
void ModelTestScene::OnEnter(GameContext& gameContext)
{
	// プロジェクション行列を設定する
    m_projection = CreateProjectionMatrix(gameContext);

	// DirectX3Dのデバイスを取得する
    auto device = gameContext.deviceResources.GetD3DDevice();

	// DirectX3Dのデバイスコンテキストを取得する
    auto context = gameContext.deviceResources.GetD3DDeviceContext();

	// 画面のサイズを取得する
    RECT rect = gameContext.deviceResources.GetOutputSize();

	// デバッグカメラの作成
    m_debugCamera = std::make_unique<Imase::DebugCamera>(rect.right, rect.bottom);

	// グリッドフロアの作成
    m_gridFloor = std::make_unique<Imase::GridFloor>(
		device,	context, &gameContext.commonStates);

	// エフェクトを作成する工場
    EffectFactory fx(device);
    fx.SetDirectory(L"Resources/Models");	// <- ddsのフォルダ

	// モデルの読み込み
	m_model = Model::CreateFromCMO(device, L"Resources/Models/Pacman.cmo", fx);

	// プレイヤーの作成
    m_player = std::make_unique<Player>(gameContext, m_view, m_projection, m_model.get());

    // モデルの読み込み（矢印）
    m_arrowModel = Model::CreateFromCMO(device, L"Resources/Models/Arrow.cmo", fx);

    // モデルの読み込み（ターゲット）
    m_targetModel = Model::CreateFromCMO(device, L"Resources/Models/Target.cmo", fx);

	// コリジョン情報表示オブジェクトの作成
    m_collisionRenderer = std::make_unique<Imase::CollisionRenderer>(device, context);

	// プレイヤーの作成
    m_player1 = std::make_unique<Player>(gameContext, m_view, m_projection, m_model.get());
    m_player2 = std::make_unique<Player>(gameContext, m_view, m_projection, m_model.get());
}

// プロジェクション行列を作成する関数
DirectX::SimpleMath::Matrix ModelTestScene::CreateProjectionMatrix(GameContext& gameContext)
{
    SimpleMath::Matrix m;

	// 画面のサイズを取得する
    RECT rect = gameContext.deviceResources.GetOutputSize();

	// プロジェクション行列を作成
	m = SimpleMath::Matrix::CreatePerspectiveFieldOfView(
		XMConvertToRadians(45.0f),	// 画角
		static_cast<float>(rect.right) / static_cast<float>(rect.bottom),	// アスペクト比
		0.1f,	// Near Clip
		1000.0f	// Far Clip
	);

    return m;
}

// ウインドウサイズ変更時に呼び出される関数
void ModelTestScene::OnWindowSizeChanged(GameContext& gameContext)
{
    // プロジェクション行列を設定する
    m_projection = CreateProjectionMatrix(gameContext);
}

// ゲームプレイ用カメラ
void ModelTestScene::GamePlayCamera(float elapsedTime)
{
	// プレイヤーの位置からのカメラの相対位置
    SimpleMath::Vector3 cameraPosition(0.0f, 4.0f, 4.0f);

	// 回転行列を作成
    SimpleMath::Matrix rotY = SimpleMath::Matrix::CreateRotationY(m_player->GetFacingAngleRad());

	// カメラの相対位置をプレイヤーの向いている角度で回転させる
    SimpleMath::Vector3 v = SimpleMath::Vector3::Transform(cameraPosition, rotY);

	// バネカメラのターゲットを設定
    m_springCamera.SetTarget(m_player->GetPosition() + v, m_player->GetPosition(), 0.2f);

	// バネカメラの更新
    m_springCamera.Update(elapsedTime);
}

// タイトル用カメラ
void ModelTestScene::TitleCamera(float elapsedTime)
{
    // プレイヤーの位置からのカメラの相対位置
    SimpleMath::Vector3 cameraPosition(0.0f, 5.0f, 5.0f);

	// カメラを回す
	m_titleAngleRad += XMConvertToRadians(TITLE_CAMERA_MOVE_ANGLE_DEG) * elapsedTime;

	// 回転行列を作成
    SimpleMath::Matrix rotY = SimpleMath::Matrix::CreateRotationY(m_titleAngleRad);

	// カメラの相対位置を回転させる
    SimpleMath::Vector3 v = SimpleMath::Vector3::Transform(cameraPosition, rotY);

	// バネカメラのターゲットを設定
    m_springCamera.SetTarget(m_player->GetPosition() + v, m_player->GetPosition(), 1.0f);

    // バネカメラの更新
    m_springCamera.Update(elapsedTime);
}

