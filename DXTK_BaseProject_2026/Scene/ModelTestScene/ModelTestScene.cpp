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

	float elapsedTime = static_cast<float>(gameContext.timer.GetElapsedSeconds());


	auto kb = Keyboard::Get().GetState();

	if (kb.Up)
	{
		m_position.z -= SPEED * elapsedTime;
	}
	if (kb.Down)
	{
		m_position.z += SPEED * elapsedTime;
	}
	if (kb.Left)
	{
		m_position.x -= SPEED * elapsedTime;
	}
	if (kb.Right)
	{
		m_position.x += SPEED * elapsedTime;
	}

	m_camera->SetTarget(XMFLOAT3(0, 2, 5), m_position);

	// カメラの更新
	m_camera->Update(elapsedTime);

	// デバッグカメラの更新
	m_debugCamera->Update(elapsedTime);

	debugRenderer.DrawText({ 0.0f, 0.0f }, L"ModelTestScene");
}

// 描画
void ModelTestScene::Render(GameContext& gameContext)
{
	auto context = gameContext.deviceResources.GetD3DDeviceContext();

	SimpleMath::Matrix world = SimpleMath::Matrix::CreateTranslation(m_position);
	SimpleMath::Matrix view = m_debugCamera->GetCameraMatrix();

	view = m_camera->GetViewMatrix();

	// グリッドフロアの描画
	m_gridFloor->Render(context, view, m_projection);

	// モデルの描画
	m_model->Draw(context, gameContext.commonStates, world, view, m_projection);
}

// シーン切り替え時に呼び出される関数
void ModelTestScene::OnEnter(GameContext& gameContext)
{
	auto device = gameContext.deviceResources.GetD3DDevice();
	auto context = gameContext.deviceResources.GetD3DDeviceContext();

	// プロジェクション行列の初期化
	InitProjectionMatrix(gameContext);

	// 画面の解像度を取得
	RECT rect = gameContext.deviceResources.GetOutputSize();

	// デバッグカメラの作成
	m_debugCamera = std::make_unique<Imase::DebugCamera>(rect.right, rect.bottom);

	// グリッドフロアの作成
	m_gridFloor = std::make_unique<Imase::GridFloor>(device, context, &gameContext.commonStates);

	// モデルの読み込み
	EffectFactory fx(device);
	fx.SetDirectory(L"Resources/Models");
	m_model = Model::CreateFromCMO(device, L"Resources/Models/Monkey.cmo", fx);

	// カメラ作成
	m_camera = std::make_unique<GameCamera>(
		SimpleMath::Vector3(0.0f, 2.0f, 5.0f),
		SimpleMath::Vector3(0.0f, 0.0f, 0.0f)
	);
}

// プロジェクション行列の初期化
void ModelTestScene::InitProjectionMatrix(GameContext& gameContext)
{
	// 画面の解像度を取得
	RECT rect = gameContext.deviceResources.GetOutputSize();

	// プロジェクション行列の作成
	m_projection = SimpleMath::Matrix::CreatePerspectiveFieldOfView(
		XMConvertToRadians(45.0f),	// 画角
		static_cast<float>(rect.right) / rect.bottom,	// アスペクト比
		0.1f,	// near Clip
		1000.0f	// far Clip
	);
}
