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
#include "../Sound/Sound.h"
#include "../LoadStage/LoadStage.h"
#include "../LoadStage/StageObject.h"
#include "../MapLayout/MapLayout.h"

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
	Imgui::Update(baseDirectX);
	if (Input::KeyTrigger(DIK_F1))
	{
		Imgui::sceneNum = TITLE;
		Imgui::CameraControl = true;
		Cameras::camera.isRCamera = false;
	}
	if (Input::KeyTrigger(DIK_F2))
	{
		Imgui::sceneNum = MAPEDIT;
		Cameras::camera.isRCamera = true;
		Imgui::CameraControl = false;
		Cameras::camera.Init(XMFLOAT3(0, 0, 20.0f), XMFLOAT3(0, 0, 0));
	}
	switch (Imgui::sceneNum)
	{
	case OP:
		OPUpdate();
		OPDraw();
		break;
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
	case MAPEDIT:
		MapEditUpdate();
		MapEditDraw();
		break;
	case RewiredEdit:
		
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
	//Cameras::camera.isRCamera = true;
	Cameras::camera.Init(XMFLOAT3(0, 0, 20.0f), XMFLOAT3(0, 0, 0));
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
	/*FbxLoader::GetInstance()->Initialize(baseDirectX.dev.Get());
	FBXObject::SetDevice(baseDirectX.dev.Get());
	FBXObject::CreateGraphicsPipeline();*/
	//ライト初期化
	light.reset(Light::Create());
	//モデルすべてにライトを適用
	Model::SetLight(light);
	//ポストエフェクトの初期化
	PostEffects::Init(baseDirectX);
	//Rewiredの要素初期化
	Rewired::KeyCodeString::KeyCodeStringInit();
	//ステージをテキストからロード
	LoadStage::LoadStages("Resource/TextData/Stage/stage1.txt");

	StageObjects::LoadModel(baseDirectX);

	MapEditorObjects::LoadModels(baseDirectX);

	mapFrameV.CreateModel(baseDirectX, "Map", ShaderManager::playerShader, false, false);
	mapFrameH.CreateModel(baseDirectX, "Map", ShaderManager::playerShader, false, false);

	mapFrameV.each.rotation.y = 90.0f;
	mapFrameH.each.rotation.x = 90.0f;
	mapFrameH.each.rotation.y = 0.0f;
	mapFrameH.each.rotation.z = 90.0f;

	seling.LoadModel(baseDirectX);
	rSeling.LoadModel(baseDirectX);

	//ボイスコマンドの通信受付スタート
	//送信側はまた違うアプリケーションで行う
	VoiceReciver::StartUp();

	waterFace.LoadModel(baseDirectX, ShaderManager::waterShader, PostEffects::postNormal);
	normalWater.LoadModel(baseDirectX, ShaderManager::normalPlaneShader, PostEffects::postNormal);
	const float worldSize = 40.0f;
	world.CreateModel(baseDirectX, "SphereW", ShaderManager::playerShader);
	world.each.scale = { worldSize, worldSize, worldSize };
	rWorld.CreateModel(baseDirectX, "SphereW", ShaderManager::playerShader);
	rWorld.each.scale = { worldSize, worldSize, worldSize };
	skyDome.CreateModel(baseDirectX, "sky", ShaderManager::playerShader, true);
	const float skyDomeSize = 60.0f;
	skyDome.each.scale = { skyDomeSize , skyDomeSize , skyDomeSize };

	spaceSp.CreateSprite(baseDirectX, L"Resource/Image/space.png", XMFLOAT3(static_cast<float>(window_width), 600.0f, 0));
	goalSp.CreateSprite(baseDirectX, L"Resource/Image/Goal.png", XMFLOAT3(static_cast<float>(window_width), window_height / 2.0f, 0));
	titleSp.CreateSprite(baseDirectX, L"Resource/Image/Title.png", XMFLOAT3(window_width / 2.0f - 128.0f, -128.0f, 0));
	for (int i = 0; i < 10; i++)
	{
		string path = "Resource/Image/Numbers/Num" + to_string(i) + ".png";
		numTex[i].LoadGraph(baseDirectX, ConvertStringToWchaer(path));
	}
	for (int i = 0; i < 10; i++)
	{
		resultNumSprite[i].CreateSprite(baseDirectX, numTex[i], XMFLOAT3(window_width / 2.0f - 100.0f, 600.0f, 0));
		resultNumSprite[i + 10].CreateSprite(baseDirectX, numTex[i], XMFLOAT3(window_width / 2.0f - 100.0f, 600.0f, 0));
		resultNumSprite[i + 20].CreateSprite(baseDirectX, numTex[i], XMFLOAT3(window_width / 2.0f - 100.0f, 600.0f, 0));
		resultNumSprite[i + 30].CreateSprite(baseDirectX, numTex[i], XMFLOAT3(window_width / 2.0f - 100.0f, 600.0f, 0));
	}
}

void GameScene::TitleUpdate()
{
	Cameras::camera.isRCamera = false;
	const float spriteSpeed = 0.15f;
	spaceSp.position = ConvertXMFLOAT3toXMVECTOR(Lerp(ConvertXMVECTORtoXMFLOAT3(spaceSp.position), XMFLOAT3(window_width / 2.0f - 80.0f, 600.0f, 0), spriteSpeed));
	titleSp.position = ConvertXMFLOAT3toXMVECTOR(Lerp(ConvertXMVECTORtoXMFLOAT3(titleSp.position), XMFLOAT3(window_width / 2.0f - 80.0f, 250.0f, 0), spriteSpeed));
	if (Input::KeyTrigger(DIK_SPACE))
	{
		Imgui::sceneNum = GAME;
	}

	Cameras::camera.target = ConvertXMVECTORtoXMFLOAT3(seling.selingModel.each.position);
	Cameras::camera.Update();
	Cameras::rCamera.eye = { 0.0f, -60.0f, -5.0f };
	Cameras::rCamera.up = { 0, 1, 0 };
	Cameras::rCamera.Update();

	VoiceReciver::VoiceUDPUpdate(baseDirectX);
	Sound::Updete(Imgui::volume);
}

void GameScene::SelectUpdate()
{
	if (Input::KeyTrigger(DIK_SPACE))
	{
		Imgui::sceneNum = GAME;
	}
}

void GameScene::GameUpdate()
{
	light->SetLightDir(XMFLOAT3(Cameras::camera.GetTargetDirection()));
	LightUpdate();
	Cameras::camera.isRCamera = false;
	seling.Update();
	rSeling.Update();
	waterFace.Update();
	normalWater.Update();

	//
	const float spriteSpeed = 0.15f;
	spaceSp.position = ConvertXMFLOAT3toXMVECTOR(Lerp(ConvertXMVECTORtoXMFLOAT3(spaceSp.position), XMFLOAT3(static_cast<float>(window_width), 600.0f, 0), spriteSpeed));
	titleSp.position = ConvertXMFLOAT3toXMVECTOR(Lerp(ConvertXMVECTORtoXMFLOAT3(titleSp.position), XMFLOAT3(window_width / 2.0f - 80.0f, -128.0f, 0), spriteSpeed));
	
	Cameras::camera.target = ConvertXMVECTORtoXMFLOAT3(seling.selingModel.each.position);
	Cameras::camera.Update();

	Cameras::rCamera.eye = { 0.0f, -65.0f, -5.0f };
	Cameras::rCamera.up = { 0, 1, 0 };
	Cameras::rCamera.Update();

	if (seling.GetIsGoal())
	{
		Imgui::sceneNum = RESULT;
	}

	VoiceReciver::VoiceUDPUpdate(baseDirectX);
	Sound::Updete(Imgui::volume);
}

void GameScene::ResultUpdate()
{
	//カメラのイージングを行う　
	Imgui::CameraRotation = ShlomonMath::EaseInOutQuad(XMFLOAT3(Imgui::CameraRotation, 0 ,0) , XMFLOAT3(350.0f, 0, 0), 0.1f).x;
	Cameras::camera.target = ConvertXMVECTORtoXMFLOAT3(seling.selingModel.each.position);
	Cameras::camera.Update();

	Cameras::rCamera.eye = { 0.0f, -60.0f, -5.0f };
	Cameras::rCamera.up = { 0, 1, 0 };
	Cameras::rCamera.Update();

	//スプライトのイージング
	const float spriteSpeed = 0.2f;
	goalSp.position = ConvertXMFLOAT3toXMVECTOR(ShlomonMath::EaseInQuad(ConvertXMVECTORtoXMFLOAT3(goalSp.position), XMFLOAT3(window_width / 2.0f - 64.0f, window_height / 2.0f, -0), spriteSpeed));
	spaceSp.position = ConvertXMFLOAT3toXMVECTOR(Lerp(ConvertXMVECTORtoXMFLOAT3(spaceSp.position), XMFLOAT3(window_width / 2.0f - 80.0f, 600.0f, 0), spriteSpeed));

	//タイトルに戻す
	if (Input::KeyTrigger(DIK_SPACE))
	{
		Imgui::sceneNum = TITLE;
		seling.Init();
		rSeling.Init();
		Imgui::CameraRotation = 270.0f;
		goalSp.position = { static_cast<float>(window_width), window_height / 2.0f, 0 , 1.0f};
	}

	VoiceReciver::VoiceUDPUpdate(baseDirectX);
	Sound::Updete(Imgui::volume);
}

void GameScene::OPUpdate()
{
	if (opAnimationTime > MaxOPAnimationTime)
	{
		Imgui::sceneNum = TITLE;
	}
	opAnimationTime++;
}

void GameScene::MapEditUpdate()
{
	light->SetLightDir(XMFLOAT3(Cameras::camera.GetTargetDirection()));
	mapFrameV.Update(baseDirectX, &mapFrameV.each);
	mapFrameH.Update(baseDirectX, &mapFrameH.each);
	//deleteした後のオブジェクトを描画しようとしてエラーを出さないための応急処置
	//Update段階でオブジェクトをなくしたい
	Imgui::DrawImGui(baseDirectX);
	XMFLOAT3 mousePos = Cameras::camera.MousePosition(baseDirectX, 0.0f);
	if (!Imgui::touchedImgui)
	{
		MapEditorObjects::Update(baseDirectX, XMFLOAT3(mousePos.x - Cameras::camera.mouseMoveAmount[0], mousePos.y - Cameras::camera.mouseMoveAmount[1], mousePos.z));
	}
	Imgui::touchedImgui = false;
	LightUpdate();
	Imgui::CameraControl = false;
	Cameras::camera.MouseWheelY();
	Cameras::camera.MouseRightPushMove(baseDirectX);
	Cameras::camera.Update();
}

void GameScene::EndUpdate()
{
	if (Input::KeyTrigger(DIK_SPACE) || Input::directInput->IsButtonPush(DirectInput::ButtonKind::Button01))
	{
		Imgui::sceneNum = TITLE;
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
}

void GameScene::DrawSprites()
{
	switch (SceneNum)
	{
	case OP:
		break;
	case TITLE:
		spaceSp.SpriteDraw(baseDirectX);
		titleSp.SpriteDraw(baseDirectX);
		break;
	case GAME:
		spaceSp.SpriteDraw(baseDirectX);
		titleSp.SpriteDraw(baseDirectX);
		break;
	case RESULT:
		spaceSp.SpriteDraw(baseDirectX);
		goalSp.SpriteDraw(baseDirectX);
		break;
	}
}

void GameScene::TitleDraw()
{
	//PostEffectのPreDraw
	PostEffects::PreDraw(baseDirectX);

	PreWaterFaceDraw();

	baseDirectX.UpdateFront();

	PostWaterFaceDraw();

	XMVECTOR waterFacePosition = { 0, -1.0f, 0.0f, 1.0 };
	//水面の切り替え
	if (Imgui::useWaterNum == 0)
	{
		waterFace.Draw(baseDirectX, PostEffects::postNormal, waterFacePosition);
	}
	else if (Imgui::useWaterNum == 1)
	{
		normalWater.Draw(baseDirectX, PostEffects::postNormal, waterFacePosition);
	}

	PostEffects::Draw(baseDirectX);

	DrawSprites();

	PostEffects::PostDraw(baseDirectX);
	Imgui::DrawImGui(baseDirectX);
	//描画コマンドここまで
	baseDirectX.UpdateBack();
}

void GameScene::SelectDraw()
{
	baseDirectX.UpdateFront();
	
	DrawSprites();
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

	XMVECTOR waterFacePosition = { 0, -1.0f, 0.0f, 1.0 };
	//水面の切り替え
	if (Imgui::useWaterNum == 0)
	{
		waterFace.Draw(baseDirectX, PostEffects::postNormal, waterFacePosition);
	}
	else if (Imgui::useWaterNum == 1)
	{
		normalWater.Draw(baseDirectX, PostEffects::postNormal, waterFacePosition);
	}

	PostEffects::Draw(baseDirectX);

	DrawSprites();

	PostEffects::PostDraw(baseDirectX);
	Imgui::DrawImGui(baseDirectX);
	//描画コマンドここまで
	baseDirectX.UpdateBack();
}

void GameScene::ResultDraw()
{
	//PostEffectのPreDraw
	PostEffects::PreDraw(baseDirectX);

	PreWaterFaceDraw();

	baseDirectX.UpdateFront();

	PostWaterFaceDraw();

	XMVECTOR waterFacePosition = { 0, -1.0f, 0.0f, 1.0 };
	//水面の切り替え
	if (Imgui::useWaterNum == 0)
	{
		waterFace.Draw(baseDirectX, PostEffects::postNormal, waterFacePosition);
	}
	else if (Imgui::useWaterNum == 1)
	{
		normalWater.Draw(baseDirectX, PostEffects::postNormal, waterFacePosition);
	}

	PostEffects::Draw(baseDirectX);

	DrawSprites();

	PostEffects::PostDraw(baseDirectX);

	//
	Imgui::DrawImGui(baseDirectX);
	//描画コマンドここまで
	baseDirectX.UpdateBack();
}

void GameScene::OPDraw()
{
}

void GameScene::MapEditDraw()
{
	baseDirectX.UpdateFront();
	Draw3DObject(baseDirectX, mapFrameV, -1, false);
	Draw3DObject(baseDirectX, mapFrameH, -1, false);
	MapEditorObjects::Draw(baseDirectX);
	Imgui::DrawImGui(baseDirectX);
	//描画コマンドここまで
	baseDirectX.UpdateBack();
}

void GameScene::RewiredEditUpdate()
{
	
}

void GameScene::RewiredEditDraw()
{
	
}

void GameScene::EndDraw()
{
	//PostEffectのPreDraw
	PostEffects::PreDraw(baseDirectX);

	PreWaterFaceDraw();

	baseDirectX.UpdateFront();

	PostWaterFaceDraw();

	XMVECTOR waterFacePosition = { 0, -0.5f, 0.0f, 1.0 };
	//水面の切り替え
	if (Imgui::useWaterNum == 0)
	{
		waterFace.Draw(baseDirectX, PostEffects::postNormal, waterFacePosition);
	}
	else if (Imgui::useWaterNum == 1)
	{
		normalWater.Draw(baseDirectX, PostEffects::postNormal, waterFacePosition);
	}

	PostEffects::Draw(baseDirectX);

	DrawSprites();

	PostEffects::PostDraw(baseDirectX);

	//
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
