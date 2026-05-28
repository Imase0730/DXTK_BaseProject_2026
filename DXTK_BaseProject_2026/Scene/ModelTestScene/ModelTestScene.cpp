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

	auto kb = Keyboard::Get().GetState();

    // ----- ターゲットの移動 ----- //
    if (kb.W)
    {
        m_targetPosition.z -= 0.05f;
    }
    if (kb.S)
    {
        m_targetPosition.z += 0.05f;
    }

    if (kb.A)
    {
        m_targetPosition.x -= 0.05f;
    }
    if (kb.D)
    {
        m_targetPosition.x += 0.05f;
    }

    if (kb.Q)
    {
        m_targetPosition.y += 0.05f;
    }
    if (kb.Z)
    {
        m_targetPosition.y -= 0.05f;
    }

	// 回転角度を算出する
	float rotateRangleRad = XMConvertToRadians(ROTATE_ANGLE_DEG) * elapsedTime;

	// 左キーでZ軸＋回転
    if (!kb.LeftShift && kb.Left)
    {
        m_angleRad_Z += rotateRangleRad;
    }
    // 右キーでZ軸－回転
    if (!kb.LeftShift && kb.Right)
    {
        m_angleRad_Z -= rotateRangleRad;
    }

	// 上キーでX軸＋回転
    if (kb.Up)
    {
        m_angleRad_X += rotateRangleRad;
    }
    // 下キーでX軸－回転
    if (kb.Down)
    {
        m_angleRad_X -= rotateRangleRad;
    }

    // 左シフトキー＋左キーでY軸＋回転
    if (kb.LeftShift && kb.Left)
    {
        m_angleRad_Y += rotateRangleRad;
    }
    // 左シフトキー＋右キーでY軸－回転
    if (kb.LeftShift && kb.Right)
    {
        m_angleRad_Y -= rotateRangleRad;
    }

    // -------------------------------------------------------------------------- //
    // 各軸の回転角からクォータニオンを作成する
    //m_quaternion = SimpleMath::Quaternion::CreateFromYawPitchRoll(m_angleRad_Y, m_angleRad_X, m_angleRad_Z);
    // -------------------------------------------------------------------------- //

    // -------------------------------------------------------------------------- //
    //// X軸回転
    //SimpleMath::Quaternion rotX =
    //    SimpleMath::Quaternion::CreateFromAxisAngle(SimpleMath::Vector3::UnitX, m_angleRad_X);
    //// Y軸回転
    //SimpleMath::Quaternion rotY =
    //    SimpleMath::Quaternion::CreateFromAxisAngle(SimpleMath::Vector3::UnitY, m_angleRad_Y);
    //// Z軸回転
    //SimpleMath::Quaternion rotZ =
    //    SimpleMath::Quaternion::CreateFromAxisAngle(SimpleMath::Vector3::UnitZ, m_angleRad_Z);
    //// クォータニオンを結合する
    //m_quaternion = rotZ * rotX * rotY;
    //  -------------------------------------------------------------------------- //

    //  -------------------------------------------------------------------------- //
    // ターゲット方向へ回転させるクォータニオンを作成する
    //m_quaternion = SimpleMath::Quaternion::FromToRotation(SimpleMath::Vector3::Forward, m_targetPosition);
    //  -------------------------------------------------------------------------- //

    //  -------------------------------------------------------------------------- //
    //// ターゲット方向へ回転させるクォータニオンを作成する（ねじれなし）
    //SimpleMath::Matrix m = 
    //    SimpleMath::Matrix::CreateWorld(SimpleMath::Vector3::Zero, m_targetPosition, SimpleMath::Vector3::Up);
    //m_quaternion = SimpleMath::Quaternion::CreateFromRotationMatrix(m);
    //   -------------------------------------------------------------------------- //

    // ターゲット方向へ回転させるクォータニオンを作成する
    SimpleMath::Quaternion q =
        SimpleMath::Quaternion::FromToRotation(SimpleMath::Vector3::Forward, m_targetPosition);
    // 球面線形補間を行う
    m_quaternion = SimpleMath::Quaternion::Lerp(m_quaternion, q, 0.1f);

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

	// デバッグカメラからビュー行列を取得する
    m_view = m_debugCamera->GetCameraMatrix();

    // グリッドフロアの描画
	m_gridFloor->Render(context, m_view, m_projection);

	// ワールド行列を作成する
    //world = SimpleMath::Matrix::CreateRotationZ(m_angleRad_Z)
    //      * SimpleMath::Matrix::CreateRotationX(m_angleRad_X)
    //      * SimpleMath::Matrix::CreateRotationY(m_angleRad_Y);

    // クォータニオンから回転行列を作成する
    world = SimpleMath::Matrix::CreateFromQuaternion(m_quaternion);

	// 矢印の描画
    m_arrowModel->Draw(context, gameContext.commonStates, world, m_view, m_projection);

    // ワールド行列を作成する
    world = SimpleMath::Matrix::CreateTranslation(m_targetPosition);

    // ターゲットの描画
    m_targetModel->Draw(context, gameContext.commonStates, world, m_view, m_projection);
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

    // モデルの読み込み（矢印）
    m_arrowModel = Model::CreateFromCMO(device, L"Resources/Models/Arrow.cmo", fx);

    // モデルの読み込み（ターゲット）
    m_targetModel = Model::CreateFromCMO(device, L"Resources/Models/Target.cmo", fx);
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

