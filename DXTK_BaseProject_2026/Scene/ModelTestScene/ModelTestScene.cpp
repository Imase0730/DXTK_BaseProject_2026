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

	SimpleMath::Matrix world, view;

	// デバッグカメラからビュー行列を取得する
    view = m_debugCamera->GetCameraMatrix();

	// ------------------------------------------------------------- //

	// ----- ビューポートの設定（左側） ----- //

	D3D11_VIEWPORT viewport_Left[1] = {
        0.0f,   0.0f,   // 左上の座標
        640.0f, 720.0f, // 幅と高さ
        0.0f,   1.0f    // 奥行の最小値と最大値
    };
    context->RSSetViewports(1, viewport_Left);

	// -------------------------------------- //

	// スカイドームの描画
    m_skydome->Draw(context, gameContext.commonStates, world, view, m_projection);

	// グリッドフロアの描画
	m_gridFloor->Render(context, view, m_projection);

	// モデルの描画
    m_model->Draw(context, gameContext.commonStates, world, view, m_projection);

	// ----- ビューポートの設定（右側） ----- //

	D3D11_VIEWPORT viewport_Right[1] = {
        640.0f,   0.0f, // 左上の座標
        640.0f, 720.0f,	// 幅と高さ
        0.0f,   1.0f    // 奥行の最小値と最大値
    };
    context->RSSetViewports(1, viewport_Right);

	// -------------------------------------- //

	// スカイドームの描画
    m_skydome->Draw(context, gameContext.commonStates, world, view, m_projection);

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

	// スカイドームの読み込み
	m_skydome = Model::CreateFromCMO(device, L"Resources/Models/Skydome.cmo", fx);

	// スカイドームのエフェクトを設定する（ライト影響は受けない）
    m_skydome->UpdateEffects([&](IEffect* effect)
		{
			BasicEffect* basicEffect = dynamic_cast<BasicEffect*>(effect);
			if (basicEffect)
			{
                basicEffect->SetAmbientLightColor(Colors::Black);
                basicEffect->SetLightEnabled(0, false);
                basicEffect->SetLightEnabled(1, false);
                basicEffect->SetLightEnabled(2, false);
                basicEffect->SetEmissiveColor(Colors::White);
            }
		}
	);
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
		//static_cast<float>(rect.right) / static_cast<float>(rect.bottom),	// アスペクト比
		640.0f / 720.0f,	// アスペクト比
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

