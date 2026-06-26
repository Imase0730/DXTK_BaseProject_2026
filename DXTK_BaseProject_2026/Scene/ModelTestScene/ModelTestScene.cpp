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

// コンストラクタ
ModelTestScene::ModelTestScene()
{
}

// 更新
void ModelTestScene::Update(Imase::ISceneController<SceneId>& sceneController, GameContext& gameContext)
{
	Imase::DebugRenderer& debugRenderer = gameContext.debugRenderer;

	// 経過時間を取得する
	float elapsedTime = static_cast<float>(gameContext.timer.GetElapsedSeconds());

    // ロボットの更新
    m_robot->Update(elapsedTime);

    // ロボットの位置に線分を表示する
    m_line[0].x = m_robot->GetPosition().x;
    m_line[1].x = m_robot->GetPosition().x;
    m_line[0].y = 2.0f;
    m_line[1].y = -2.0f;
    m_line[0].z = m_robot->GetPosition().z;
    m_line[1].z = m_robot->GetPosition().z;

    // 床のモデルと線分の交差判定
    SimpleMath::Vector3 hitPosition;
    SimpleMath::Vector3 normal;
    if (m_objCollision->IntersectLineSegment(m_line[0], m_line[1], &hitPosition, &normal))
    {
        // ロボットのY座標を設定する
        m_robot->SetPositionY(hitPosition.y);

        // 床の法線方向へ傾けるクォータニオンを作成する
        SimpleMath::Quaternion q = SimpleMath::Quaternion::FromToRotation(SimpleMath::Vector3::Up, normal);
        // ロボットを傾ける
        m_robot->SetTilt(q);

        debugRenderer.DrawText({ 0.0f, 50.0f }, L"Hit!");
    }

    // デバッグカメラの更新
    m_debugCamera->Update(elapsedTime);

	debugRenderer.DrawText({ 0.0f, 0.0f }, L"ModelTestScene");
}

// 描画
void ModelTestScene::Render(GameContext& gameContext)
{
    // DirectX3Dのデバイスコンテキストを取得する
    auto context = gameContext.deviceResources.GetD3DDeviceContext();

	// デバッグカメラからビュー行列を取得する
    m_view = m_debugCamera->GetCameraMatrix();

	// グリッドフロアの描画
	//m_gridFloor->Render(context, m_view, m_projection);

    // 床の描画
    SimpleMath::Matrix world;
    // m_modelFloor->Draw(context, gameContext.commonStates, world, m_view, m_projection);

    // 戦車の描画
    //m_tank->Render();

    // ロボットの描画
    m_robot->Render();

    // 衝突判定用のモデルデータ（床）の表示
    m_objCollision->AddCollisionRenderer(m_collisionRenderer.get());

    // 線分の表示
    m_collisionRenderer->AddLineSegment(m_line[0], m_line[1], Colors::Blue);

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
    m_gridFloor = std::make_unique<Imase::GridFloor>(device, context, &gameContext.commonStates);

    // エフェクトを作成する工場
    EffectFactory fx(device);
    fx.SetDirectory(L"Resources/Models"); // <- ddsのフォルダ

    // モデルの読み込み（床）
    m_modelFloor = Model::CreateFromCMO(device, L"Resources/Models/Floor.cmo", fx);

    // コリジョン情報表示オブジェクトの作成
    m_collisionRenderer = std::make_unique<Imase::CollisionRenderer>(device, context);

    // モデルデータから衝突判定用データを作成
    m_objCollision = std::make_unique<Imase::ObjCollision>("Resources/Models/Floor.obj");

    // 各パーツのモデルの読み込み（戦車）
    m_modelTanks[static_cast<int>(Tank::Parts::BODY)] =
        Model::CreateFromCMO(device, L"Resources/Models/TankBody.cmo", fx);
    m_modelTanks[static_cast<int>(Tank::Parts::HEAD)] =
        Model::CreateFromCMO(device, L"Resources/Models/TankHead.cmo", fx);
    m_modelTanks[static_cast<int>(Tank::Parts::BARREL)] =
        Model::CreateFromCMO(device, L"Resources/Models/TankBarrel.cmo", fx);

    // 戦車の作成
    m_tank = std::make_unique<Tank>(gameContext, m_view, m_projection, m_modelTanks);

    // 各パーツのモデルの読み込み（ロボット）
    m_modelRobots[static_cast<int>(Robot::Parts::LEG)] =
        Model::CreateFromCMO(device, L"Resources/Models/Leg.cmo", fx);
    m_modelRobots[static_cast<int>(Robot::Parts::BODY)] =
        Model::CreateFromCMO(device, L"Resources/Models/Body.cmo", fx);
    m_modelRobots[static_cast<int>(Robot::Parts::HEAD)] =
        Model::CreateFromCMO(device, L"Resources/Models/Head.cmo", fx);
    m_modelRobots[static_cast<int>(Robot::Parts::ARM_R)] =
        Model::CreateFromCMO(device, L"Resources/Models/Arm_R.cmo", fx);
    m_modelRobots[static_cast<int>(Robot::Parts::ARM_L)] =
        Model::CreateFromCMO(device, L"Resources/Models/Arm_L.cmo", fx);
    m_modelRobots[static_cast<int>(Robot::Parts::MISSILE)] =
        Model::CreateFromCMO(device, L"Resources/Models/Missile.cmo", fx);

    // ロボットの作成
    m_robot = std::make_unique<Robot>(gameContext, m_view, m_projection, m_modelRobots);
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
