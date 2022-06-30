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

}

GameScene::~GameScene()
{
	VoiceReciver::EndRecive();

}

void GameScene::SceneManageUpdateAndDraw()
{
	Input::Update(baseDirectX);
	WindowsAPI::CheckMsg();
	light->Update();
	switch (SceneNum)
	{
	case OP:
		OPUpdate();
		OPDraw();
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
	SceneNum = TITLE;
	baseDirectX.Set();
	//サウンド
	Sound::CreateVoice(baseDirectX);
	//SRVのアドレス確保
	baseDirectX.GetAdress();
	//カメラ初期化
	Cameras::camera.Init(XMFLOAT3(0, 10, -15.0f), XMFLOAT3(0, 0, 0));
	Cameras::rCamera.Init(XMFLOAT3(0, -10, -15.0f), XMFLOAT3(0, 0, 0));
	Cameras::rCamera.isRCamera = true;
	//Imguiの初期化
	Imgui::Init(baseDirectX);
	//ライトの初期化
	Light::StaticInitialize(baseDirectX.dev.Get());
	//シェーダーのロード
	ShaderManager::LoadShaders();
	// 3Dパーティクル静的初期化
	ParticleControl::Init(baseDirectX);
	//インプット初期化
	Input::KeySet(baseDirectX, WindowsAPI::w, WindowsAPI::hwnd);
	//FBX系
	FbxLoader::GetInstance()->Initialize(baseDirectX.dev.Get());
	FBXObject::SetDevice(baseDirectX.dev.Get());
	FBXObject::CreateGraphicsPipeline();
	//ライト初期化
	light.reset(Light::Create());
	//モデルすべてにライトを適用
	Model::SetLight(light);
	//ポストエフェクトの初期化
	PostEffects::Init(baseDirectX);
	//3Dオブジェクトのパーティクルロード
	ObjectParticles::LoadModels(baseDirectX);
	//Rewiredの要素初期化
	Rewired::KeyCodeString::KeyCodeStringInit();
	//ジャンプキーのリワイヤード決定
	jumpKey.LoadKey("Resource/TextData/Rewired/RewiredTest.txt");
	//ステージをテキストからロード
	LoadStage::LoadStages("Resource/TextData/Stage/test.txt");

	StageObjects::LoadModel(baseDirectX);

	seling.LoadModel(baseDirectX);
	rSeling.LoadModel(baseDirectX);

	//ボイスコマンドの通信受付スタート
	//送信側はまた違うアプリケーションで行う
	VoiceReciver::StartUp();
	//EnemyModels::LoadModels();

	waterFace.LoadModel(baseDirectX, ShaderManager::waterShader, PostEffects::postNormal);
	normalWater.LoadModel(baseDirectX, ShaderManager::normalPlaneShader, PostEffects::postNormal);
	const float worldSize = 40.0f;
	world.CreateModel(baseDirectX, "SphereW", ShaderManager::playerShader);
	world.each.scale = { worldSize, worldSize, worldSize };
	rWorld.CreateModel(baseDirectX, "SphereW", ShaderManager::playerShader);
	rWorld.each.scale = { worldSize, worldSize, worldSize };
	skyDome.CreateModel(baseDirectX, "sky", ShaderManager::playerShader);
	const float skyDomeSize = 60.0f;
	skyDome.each.scale = { skyDomeSize , skyDomeSize , skyDomeSize };
}

void GameScene::TitleUpdate()
{
	if (Input::KeyTrigger(DIK_SPACE))
	{
		SceneNum = GAME;
	}
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
	Cameras::camera.Update();
	Cameras::rCamera.eye.x = 0;
	Cameras::rCamera.eye.y = -60.0f;
	Cameras::rCamera.eye.z = -0.1f;
	Cameras::rCamera.target = Cameras::camera.target;
	Cameras::rCamera.up = { 0, 1, 0 };
	Cameras::rCamera.Update();

	seling.Update();
	rSeling.Update();
	waterFace.Update();
	normalWater.Update();

	if (jumpKey.GetKeyDown())
	{
		int a = 0;
	}

	if (seling.GetIsGoal())
	{
		SceneNum = RESULT;
	}

	VoiceReciver::VoiceUDPUpdate(baseDirectX);
	ObjectParticles::Update();
	Sound::Updete(Imgui::volume);
}

void GameScene::ResultUpdate()
{

}

void GameScene::OPUpdate()
{
	if (opAnimationTime > MaxOPAnimationTime)
	{
		SceneNum = TITLE;
	}
	opAnimationTime++;

}

void GameScene::EndUpdate()
{
	if (Input::KeyTrigger(DIK_SPACE) || Input::directInput->IsButtonPush(DirectInput::ButtonKind::Button01))
	{
		SceneNum = TITLE;
		Cameras::camera.Init(XMFLOAT3(0, 10, -15.0f), XMFLOAT3(0, 0, 0));
	}
}

void GameScene::PreWaterFaceDraw()
{
	bool isRDraw;
	if (Imgui::effectType < 0)
	{
		isRDraw = true;
	}
	else
	{
		isRDraw = false;
	}
	light->SetLightDir(XMFLOAT3(Cameras::rCamera.GetTargetDirection()));
	LightUpdate();
	rSeling.Draw(baseDirectX, isRDraw);
	rWorld.Update(baseDirectX, &rWorld.each, isRDraw);
	Draw3DObject(baseDirectX, rWorld);
	StageObjects::Draw(baseDirectX, isRDraw);
	//ObjectParticles::Draw(baseDirectX);

}

void GameScene::PostWaterFaceDraw()
{
	light->SetLightDir(XMFLOAT3(Cameras::camera.GetTargetDirection()));
	LightUpdate();
	seling.Draw(baseDirectX);
	world.Update(baseDirectX, &world.each, false);
	Draw3DObject(baseDirectX, world);
	skyDome.Update(baseDirectX, &skyDome.each);
	Draw3DObject(baseDirectX, skyDome);
	StageObjects::Draw(baseDirectX, false);
	//ObjectParticles::Draw(baseDirectX);
}

void GameScene::TitleDraw()
{
	//PostEffectのPreDraw
	PostEffects::PreDraw(baseDirectX);

	baseDirectX.UpdateFront();
	Imgui::DrawImGui(baseDirectX);
	//描画コマンドここまで
	baseDirectX.UpdateBack();
}

void GameScene::SelectDraw()
{
	//PostEffectのPreDraw
	PostEffects::PreDraw(baseDirectX);

	baseDirectX.UpdateFront();

	//PostEffectのDraw
	PostEffects::Draw(baseDirectX);

	Imgui::DrawImGui(baseDirectX);
	//描画コマンドここまで
	baseDirectX.UpdateBack();
}

void GameScene::GameDraw()
{
	//PostEffectのPreDraw
	PostEffects::PreDraw(baseDirectX);

	PreWaterFaceDraw();

	baseDirectX.UpdateFront();

	PostWaterFaceDraw();

	XMVECTOR sample = { 0, -0.5f, 0.0f, 1.0 };

	PostEffect waterFaceTarget;
	if (PostEffects::type == PostEffects::PostEffectType::Normal)
	{
		waterFaceTarget = PostEffects::postNormal;
	}
	else if (PostEffects::type == PostEffects::PostEffectType::Water)
	{
		waterFaceTarget = PostEffects::postWater;
	}
	else if (PostEffects::type == PostEffects::PostEffectType::Mosaic)
	{
		waterFaceTarget = PostEffects::postMosaic;
	}
	else if (PostEffects::type == PostEffects::PostEffectType::Blur)
	{
		waterFaceTarget = PostEffects::postBlur;
	}
	else
	{
		waterFaceTarget = PostEffects::postNormal;
	}

	if (Imgui::useWaterNum == 0)
	{
		waterFace.Draw(baseDirectX, waterFaceTarget, sample);
	}
	else if (Imgui::useWaterNum == 1)
	{
		normalWater.Draw(baseDirectX, waterFaceTarget, sample);
	}

	PostEffects::Draw(baseDirectX);

	Imgui::DrawImGui(baseDirectX);
	//描画コマンドここまで
	baseDirectX.UpdateBack();
}

void GameScene::ResultDraw()
{
	//PostEffectのPreDraw
	PostEffects::PreDraw(baseDirectX);
	Draw3DObject(baseDirectX, sample);
	baseDirectX.UpdateFront();
	//PostEffectのDraw
	PostEffects::Draw(baseDirectX);
	Imgui::DrawImGui(baseDirectX);

	//描画コマンドここまで
	baseDirectX.UpdateBack();
}

void GameScene::OPDraw()
{
}

void GameScene::EndDraw()
{
	//PostEffectのPreDraw
	PostEffects::PreDraw(baseDirectX);
	Draw3DObject(baseDirectX, sample);

	baseDirectX.UpdateFront();
	//PostEffectのDraw
	PostEffects::Draw(baseDirectX);
	PostEffects::PostDraw(baseDirectX);
	Imgui::DrawImGui(baseDirectX);

	//描画コマンドここまで
	baseDirectX.UpdateBack();
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
