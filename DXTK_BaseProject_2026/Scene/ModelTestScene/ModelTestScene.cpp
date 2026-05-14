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
{
}

// 更新
void ModelTestScene::Update(Imase::ISceneController<SceneId>& sceneController, GameContext& gameContext)
{
	Imase::DebugRenderer& debugRenderer = gameContext.debugRenderer;

	// 経過時間を取得する
	float elapsedTime = static_cast<float>(gameContext.timer.GetElapsedSeconds());

	// Aキーが押された
    if (gameContext.keyboardTracker.pressed.A)
    {
        m_moveCamera.SetTarget(SimpleMath::Vector3(0.0f, 2.0f, 2.0f), SimpleMath::Vector3(0.0f, 0.0f, 0.0f), 1.0f);
    }

    // Sキーが押された
    if (gameContext.keyboardTracker.pressed.S)
    {
        m_moveCamera.SetTarget(SimpleMath::Vector3(0.0f, 2.0f, 10.0f), SimpleMath::Vector3(0.0f, 0.0f, 0.0f), 1.0f);
    }

	// 補間移動カメラの更新
	m_moveCamera.Update(elapsedTime);

	// デバッグカメラの更新
    m_debugCamera->Update(elapsedTime);

	debugRenderer.DrawText({ 0.0f, 0.0f }, L"ModelTestScene");
}

// 描画
void ModelTestScene::Render(GameContext& gameContext)
{
    // DirectX3Dのデバイスコンテキストを取得する
    auto context = gameContext.deviceResources.GetD3DDeviceContext();

	SimpleMath::Matrix world, view;

	// デバッグカメラからビュー行列を取得する
//    view = m_debugCamera->GetCameraMatrix();

	// 固定カメラからビュー行列を取得する
//    view = m_fixedCamera.GetViewMatrix();

	// 補間移動カメラからビュー行列を取得する
    view = m_moveCamera.GetViewMatrix();

    // グリッドフロアの描画
	m_gridFloor->Render(context, view, m_projection);

	// モデルの描画
    m_model->Draw(context, gameContext.commonStates, world, view, m_projection);
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

	// スザンヌの読み込み
	m_model = Model::CreateFromCMO(device, L"Resources/Models/Monkey.cmo", fx);
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

