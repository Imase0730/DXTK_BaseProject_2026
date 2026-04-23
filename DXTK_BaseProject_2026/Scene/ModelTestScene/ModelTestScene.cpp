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

// 更新
void ModelTestScene::Update(Imase::ISceneController<SceneId>& sceneController, GameContext& gameContext)
{
	Imase::DebugRenderer& debugRenderer = gameContext.debugRenderer;

	// 経過時間を取得する
	float elapsedTime = static_cast<float>(gameContext.timer.GetElapsedSeconds());

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
    SimpleMath::Matrix view = m_debugCamera->GetCameraMatrix();

	// ------------------------------------------------------------- //
	
	// 視点の位置
    SimpleMath::Vector3 eye = { 0.0f, 2.0f, 4.0f};

	// 注視点の位置
    SimpleMath::Vector3 target = {0.0f, 0.0f, 0.0f};

	// ビュー行列を作成する
	view = SimpleMath::Matrix::CreateLookAt(eye, target, SimpleMath::Vector3::Up);

	// ------------------------------------------------------------- //

	// グリッドフロアの描画
	m_gridFloor->Render(context, view, m_projection);

	// モデルの描画
    SimpleMath::Matrix world;
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

	// モデルの読み込み
    EffectFactory fx(device);
    fx.SetDirectory(L"Resources/Models");	// <- ddsのフォルダ
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

