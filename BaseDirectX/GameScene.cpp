#include "../VoiceUDPRecive/VoiceUDPRecive.h"
#include"GameScene.h"
#include "BaseDirectX.h"
#include "../WindowsAPI/WinAPI.h"
#include "Input.h"
#include "viewport.h"
#include "../Particle/Particle3D.h"
#include "../imgui/ImguiControl.h"
#include "vec3.h"
#include "../FBXObject/FBXObject.h"
#include "../Shader/ShaderManager.h"
#include "../3DObjectParticle/3DObjectParticle.h"
#include "../Sound/Sound.h"

GameScene::GameScene()
{
	SceneNum = TITLE;
}

GameScene::~GameScene()
{
	VoiceReciver::EndRecive();
	delete(light);
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
	Cameras::camera.Init();
	Cameras::camera.eye = { 0, 10, -15.0f };
	Cameras::camera.target = { 0, 0, 0 };
	Cameras::camera.Update();

	Cameras::rCamera.Init();
	Cameras::rCamera.eye = {0, 0, -15.0f};
	Cameras::rCamera.target = {0, 0, 0};
	Cameras::rCamera.Update();
	//Imguiの初期化
	Imgui::Init();
	//ライトの初期化
	Light::StaticInitialize(BaseDirectX::dev.Get());
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
	light = Light::Create();
	//モデルすべてにライトを適用
	Model::SetLight(light);
	//ポストエフェクトの初期化
	PostEffects::Init();
	ObjectParticles::LoadModels();

	seling.LoadModel();
	seling.Init();

	VoiceReciver::StartUp();
	EnemyModels::LoadModels();
}

void GameScene::TitleUpdate()
{
	seling.Update();
	VoiceReciver::VoiceUDPUpdate();
	ObjectParticles::Update();
	LightUpdate();
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
	if (Input::KeyTrigger(DIK_SPACE) || directInput->IsButtonPush(DirectInput::ButtonKind::Button01))
	{
		SceneNum = TITLE;
		Cameras::camera.Init();
	}
}

void GameScene::TitleDraw()
{
	//PostEffectのPreDraw
	PostEffects::PreDraw();

	seling.Draw();
	ObjectParticles::Draw();

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
