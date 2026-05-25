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

	// プレイヤーの後進
    m_player->Update(elapsedTime);

	// カメラモードによるカメラの設定
	switch (m_cameraMode)
    {
        case ModelTestScene::CameraMode::Title:		// タイトル
            break;
        
		case ModelTestScene::CameraMode::GamePlay:	// ゲームプレイ中
            GamePlayCamera(elapsedTime);
            break;

		default:
            break;
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

	SimpleMath::Matrix world;

	// バネカメラからビュー行列を取得する
    m_view = m_springCamera.GetViewMatrix();

    // グリッドフロアの描画
	m_gridFloor->Render(context, m_view, m_projection);

	// プレイヤーの描画
    m_player->Render();
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
	m_model = Model::CreateFromCMO(device, L"Resources/Models/Player.cmo", fx);

	// プレイヤーの作成
    m_player = std::make_unique<Player>(gameContext, m_view, m_projection, m_model.get());
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

