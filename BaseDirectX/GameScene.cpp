#include "../VoiceUDPRecive/VoiceUDPRecive.h"
#include"GameScene.h"
#include "BaseDirectX.h"
#include "../WindowsAPI/WinAPI.h"
#include "Input.h"
#include "viewport.h"
#include "../Particle/Particle3D.h"
#include "../imgui/ImguiControl.h"
#include "../FBXObject/FBXObject.h"
#include "../Shader/ShaderManager.h"
#include "../3DObjectParticle/3DObjectParticle.h"
#include "../Sound/Sound.h"
#include "../LoadStage/LoadStage.h"
#include "../LoadStage/StageObject.h"

GameScene::GameScene()
{
	SceneNum = TITLE;
}

GameScene::~GameScene()
{
	VoiceReciver::EndRecive();
	
}

void GameScene::SceneManageUpdateAndDraw()
{
	Input::Update();
	WindowsAPI::CheckMsg();
	light->Update();
	switch (SceneNum)
	{
	case TITLE:
		TitleUpdate();
		TitleDraw();
		break;
	case SELECT:
		SelectUpdate();
		SelectDraw();
		break;
	case GAME:
		GameUpdate();
		GameDraw();
		break;
	case RESULT:
		ResultUpdate();
		ResultDraw();
		break;
	case END:
		EndUpdate();
		EndDraw();
		break;
	default:
		break;
	}
}

void GameScene::Init()
{
	BaseDirectX::Set();
	//サウンド
	Sound::CreateVoice();
	//SRVのアドレス確保
	BaseDirectX::GetAdress();
	//カメラ初期化
	Cameras::camera.Init(XMFLOAT3(0, 10, -15.0f), XMFLOAT3(0, 0, 0));
	Cameras::rCamera.Init(XMFLOAT3(0, -10, -15.0f), XMFLOAT3(0, 0, 0));
	//Imguiの初期化
	Imgui::Init();
	//ライトの初期化
	Light::StaticInitialize(BaseDirectX::dev.Get());
	//シェーダーのロード
	ShaderManager::LoadShaders();
	// 3Dパーティクル静的初期化
	ParticleControl::Init();
	//インプット初期化
	Input::KeySet(WindowsAPI::w, WindowsAPI::hwnd);
	//FBX系
	FbxLoader::GetInstance()->Initialize(BaseDirectX::dev.Get());
	FBXObject::SetDevice(BaseDirectX::dev.Get());
	FBXObject::CreateGraphicsPipeline();
	//ライト初期化
	light.reset(Light::Create());
	//モデルすべてにライトを適用
	Model::SetLight(light);
	//ポストエフェクトの初期化
	PostEffects::Init();
	//3Dオブジェクトのパーティクルロード
	ObjectParticles::LoadModels();
	//Rewiredの要素初期化
	Rewired::KeyCodeString::KeyCodeStringInit();
	
	jumpKey.LoadKey("RewiredTest.txt");
	
	//ステージをテキストからロード
	LoadStage::LoadStages("test.txt");
	StageObjects::walls.wallModel.CreateModel("MapWall", ShaderManager::playerShader);
	StageObjects::walls.LoadPosition();
	StageObjects::goals.goalModel.CreateModel("goal", ShaderManager::playerShader);
	StageObjects::goals.LoadPosition();
	seling.LoadModel();
	seling.Init();
	rSeling.LoadModel();
	rSeling.Init();

	//ボイスコマンドの通信受付スタート
	//送信側はまた違うアプリケーションで行う
	VoiceReciver::StartUp();
	//EnemyModels::LoadModels();

	waterFace.LoadModel(ShaderManager::waterShader, PostEffects::postNormal);
	waterFace.Init();
	normalWater.LoadModel(ShaderManager::normalPlaneShader, PostEffects::postNormal);
	normalWater.Init();
	world.CreateModel("SphereW", ShaderManager::playerShader);
	world.each.scale = { 40.0f, 40.0f, 40.0f };
	rWorld.CreateModel("SphereW", ShaderManager::playerShader);
	rWorld.each.scale = { 40.0f, 40.0f, 40.0f };
	Cameras::rCamera.isRCamera = true;
}

void GameScene::TitleUpdate()
{
	Cameras::camera.Update();
	Cameras::rCamera.eye.x = Cameras::camera.eye.x;
	Cameras::rCamera.eye.y = -Cameras::camera.eye.y;
	Cameras::rCamera.eye.z = Cameras::camera.eye.z;
	Cameras::rCamera.target = Cameras::camera.target;
	Cameras::rCamera.Update();

	seling.Update();
	rSeling.Update();
	waterFace.Update();
	normalWater.Update();

	if (jumpKey.GetKeyDown())
	{
		int a = 0;
	}

	VoiceReciver::VoiceUDPUpdate();
	ObjectParticles::Update();
	Sound::Updete(Imgui::volume);
}

void GameScene::SelectUpdate()
{
	if (Input::KeyTrigger(DIK_SPACE))
	{
		SceneNum = GAME;
	}
}

void GameScene::GameUpdate()
{

}

void GameScene::ResultUpdate()
{

}

void GameScene::EndUpdate()
{
	if (Input::KeyTrigger(DIK_SPACE) || Input::directInput->IsButtonPush(DirectInput::ButtonKind::Button01))
	{
		SceneNum = TITLE;
		Cameras::camera.Init(XMFLOAT3(0, 10, -15.0f), XMFLOAT3(0, 0, 0));
	}
}

void GameScene::TitleDraw()
{
	//PostEffectのPreDraw
	PostEffects::PreDraw();

	light->SetLightDir(XMFLOAT3(Cameras::rCamera.GetTargetDirection()));
	LightUpdate();
	rSeling.Draw(true);
	rWorld.Update(&rWorld.each, true);
	Draw3DObject(rWorld);
	StageObjects::Draw(true);
	ObjectParticles::Draw();

	BaseDirectX::clearColor[0] = 0.0f;
	BaseDirectX::clearColor[1] = 0.0f;
	BaseDirectX::clearColor[2] = 0.0f;
	BaseDirectX::clearColor[3] = 0.0f;
	BaseDirectX::UpdateFront();

	//PostEffectのDraw
	//PostEffects::Draw();

	light->SetLightDir(XMFLOAT3(Cameras::camera.GetTargetDirection()));
	LightUpdate();

	seling.Draw();
	//world.each.rotation.y = 180;
	world.Update(&world.each, false);
	StageObjects::Draw(false);
	Draw3DObject(world);
	ObjectParticles::Draw();

	XMVECTOR sample = { 0, -3.0f, 0.0f, 1.0 };
	if (Imgui::useWaterNum == 0)
	{
		//waterFace.waterModel.each.rotation.y = 180;
		waterFace.Draw(PostEffects::postNormal, sample/*seling.seling.each.position*/);
	}
	else if (Imgui::useWaterNum == 1)
	{
		//normalWater.waterModel.each.rotation.y = 180;
		normalWater.Draw(PostEffects::postNormal, sample/*seling.seling.each.position*/);
	}
	PostEffects::PostDraw();

	Imgui::DrawImGui();
	//描画コマンドここまで
	BaseDirectX::UpdateBack();
}

void GameScene::SelectDraw()
{
	//PostEffectのPreDraw
	PostEffects::PreDraw();
	Draw3DObject(sample);
	BaseDirectX::clearColor[0] = 0.0f;
	BaseDirectX::clearColor[1] = 0.0f;
	BaseDirectX::clearColor[2] = 0.0f;
	BaseDirectX::clearColor[3] = 0.0f;
	BaseDirectX::UpdateFront();
	//PostEffectのDraw
	PostEffects::Draw();
	PostEffects::PostDraw();

	Imgui::DrawImGui();
	//描画コマンドここまで
	BaseDirectX::UpdateBack();
}

void GameScene::GameDraw()
{
	//PostEffectのPreDraw
	PostEffects::PreDraw();
	Draw3DObject(sample);
	BaseDirectX::clearColor[0] = 0.0f;
	BaseDirectX::clearColor[1] = 0.0f;
	BaseDirectX::clearColor[2] = 0.0f;
	BaseDirectX::clearColor[3] = 0.0f;
	BaseDirectX::UpdateFront();
	//PostEffectのDraw
	PostEffects::Draw();
	PostEffects::PostDraw();
	Imgui::DrawImGui();

	//描画コマンドここまで
	BaseDirectX::UpdateBack();
}

void GameScene::ResultDraw()
{
	//PostEffectのPreDraw
	PostEffects::PreDraw();
	Draw3DObject(sample);
	BaseDirectX::clearColor[0] = 0.0f;
	BaseDirectX::clearColor[1] = 0.0f;
	BaseDirectX::clearColor[2] = 0.0f;
	BaseDirectX::clearColor[3] = 0.0f;
	BaseDirectX::UpdateFront();
	//PostEffectのDraw
	PostEffects::Draw();
	PostEffects::PostDraw();
	Imgui::DrawImGui();

	//描画コマンドここまで
	BaseDirectX::UpdateBack();
}

void GameScene::EndDraw()
{
	//PostEffectのPreDraw
	PostEffects::PreDraw();
	Draw3DObject(sample);
	BaseDirectX::clearColor[0] = 0.0f;
	BaseDirectX::clearColor[1] = 0.0f;
	BaseDirectX::clearColor[2] = 0.0f;
	BaseDirectX::clearColor[3] = 0.0f;
	BaseDirectX::UpdateFront();
	//PostEffectのDraw
	PostEffects::Draw();
	PostEffects::PostDraw();
	Imgui::DrawImGui();

	//描画コマンドここまで
	BaseDirectX::UpdateBack();
}

void GameScene::LightUpdate()
{
	light->SetPointLightActive(0, false);
	light->SetSpotLightActive(0, false);
	//light->SetCircleShadowActive(0, false);
	light->SetPointLightPos(0, XMFLOAT3(pointLightPos));
	light->SetPointLightAtten(0, XMFLOAT3(pointLightAtten));
	light->SetPointLightColor(0, XMFLOAT3(pointLightColor));
	light->SetSpotLightDir(0, XMVECTOR({ spotLightDir[0], spotLightDir[1], spotLightDir[2], 0 }));
	light->SetSpotLightPos(0, XMFLOAT3(spotLightPos));
	light->SetSpotLightColor(0, XMFLOAT3(spotLightColor));
	light->SetSpotLightAtten(0, XMFLOAT3(spotLightAtten));
	light->SetSpotLightAngle(0, XMFLOAT2(spotLightAngle));
}
