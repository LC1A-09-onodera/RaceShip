#include "../VoiceUDPRecive/VoiceUDPRecive.h"
#include"GameScene.h"
#include "BaseDirectX.h"
#include "../WindowsAPI/WinAPI.h"
#include "Input.h"
#include "viewport.h"
#include "../imgui/ImguiControl.h"
#include "../FBXObject/FBXObject.h"
#include "../Shader/ShaderManager.h"
#include "../Sound/Sound.h"
#include "../LoadStage/LoadStage.h"
#include "../LoadStage/StageObject.h"
#include "../MapLayout/MapLayout.h"

#include <thread>

GameScene::GameScene()
{

}

GameScene::~GameScene()
{
	VoiceReciver::EndRecive();
}

void GameScene::SceneManageUpdateAndDraw()
{
	Imgui::Update(baseDirectX, seling);
	Input::Update(baseDirectX);
	if (Imgui::isKeyRec == Imgui::KeyRec::PlayBack)
	{
		KeyLog::Playback();
	}
	else if (Imgui::isKeyRec == Imgui::KeyRec::Rec)
	{
		KeyLog::Recording();
	}
	WindowsAPI::CheckMsg();
	light->Update();
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
		Cameras::camera.mouseMoveAmount[0] = 0.0f;
		Cameras::camera.mouseMoveAmount[1] = 0.0f;
		XMFLOAT3 cameraEeyReset(0, 0, 20.0f);
		XMFLOAT3 cameraTargetReset(0, 0, 0);
		Cameras::camera.Init(cameraEeyReset, cameraTargetReset);
	}
	if (Input::KeyTrigger(DIK_F3))
	{
		Imgui::sceneNum = ParticleEdit;

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
		break;
	case ParticleEdit:
		ParticleEditUpdate();
		ParticleControl::Update();
		light->SetLightDir(XMFLOAT3(Cameras::camera.GetTargetDirection()));
		LightUpdate();
		ParticleEditDraw();
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
	//Cameras::camera.isRCamera = true;
	XMFLOAT3 cameraEye(0, 0, 20.0f);
	XMFLOAT3 cameraTarget(0, 0, 0);
	Cameras::camera.Init(cameraEye, cameraTarget);
	XMFLOAT3 rCameraEye(0, -10, -15.0f);
	XMFLOAT3 rCameraTarget(0, 0, 0);
	Cameras::rCamera.Init(rCameraEye, rCameraTarget);
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
	//inspector初期化
	EachManager::eahcs.clear();
	//ポストエフェクトの初期化
	PostEffects::Init(baseDirectX);

	StageObjects::LoadModel(baseDirectX);

	MapEditorObjects::LoadModels(baseDirectX);

	//Rewiredの要素初期化
	Rewired::KeyCodeString::KeyCodeStringInit(baseDirectX);
	Rewired::RewiredContainer::GetFilesName();

	mapFrameV.CreateModel(baseDirectX, "Map", ShaderManager::playerShader, false, false);
	mapFrameV.each.m_eachName = "MapV";
	EachManager::eahcs.push_back(&mapFrameV.each);
	mapFrameH.CreateModel(baseDirectX, "Map", ShaderManager::playerShader, false, false);
	mapFrameH.each.m_eachName = "MapH";
	EachManager::eahcs.push_back(&mapFrameH.each);
	mapFrameV.each.rotation.y = 90.0f;
	mapFrameH.each.rotation.x = 90.0f;
	mapFrameH.each.rotation.y = 0.0f;
	mapFrameH.each.rotation.z = 90.0f;

	particleAreaModel.CreateModel(baseDirectX, "ParticleArea", ShaderManager::playerShader, false, false);

	seling.LoadModel(baseDirectX);
	EachManager::eahcs.push_back(&seling.selingModel.each);
	rSeling.LoadModel(baseDirectX);
	//ボイスコマンドの通信受付スタート
	//送信側はまた違うアプリケーションで行う
	VoiceReciver::StartUp();
	XMFLOAT3 cameraPos = { 0.0f, -67.0f, -10.1f };
	XMFLOAT3 targetPos = { 0, 0, 0 };
	XMFLOAT3 cameraPos2 = { 0.0f, -67.0f + 89.0f, -10.1f };
	XMFLOAT3 targetPos2 = { 0, 0 + 89.0f, 0 };
	waterFace[0].LoadModel(baseDirectX, ShaderManager::waterShader, cameraPos, targetPos);
	waterFace[0].waterModel.eachData.m_eachName = "water0";
	EachManager::eahcs.push_back((&waterFace[0].waterModel.eachData));
	waterFace[1].LoadModel(baseDirectX, ShaderManager::waterShader, cameraPos2, targetPos2);
	waterFace[1].waterModel.eachData.position.m128_f32[2] = 20.0f;
	normalWater.LoadModel(baseDirectX, ShaderManager::normalPlaneShader, cameraPos, targetPos);
	mosaicWater.LoadModel(baseDirectX, ShaderManager::mosaicPlaneShader, cameraPos, targetPos);
	monoWater.LoadModel(baseDirectX, ShaderManager::monoPlaneShader, cameraPos, targetPos);
	blurWater.LoadModel(baseDirectX, ShaderManager::blurPlaneShader, cameraPos, targetPos);

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
	pouseUI.LoadFile(baseDirectX);
	Rewired::RewiredContainer::LoadAllRewired();

	seling.Init(baseDirectX);
	rSeling.Init(baseDirectX);
	//ステージをテキストからロード
	LoadStage::LoadStages("Resource/TextData/Stage/stage1.txt");
	StageObjects::LoadFile(baseDirectX, seling, "Resource/TextData/Stage/stage1.txt");

	ParticleData par;
	ParticleLoader::ParticleLoad("sample", par);

	KeyLog::SetFileName("log");
	
}

void GameScene::TitleUpdate()
{
	if (Imgui::isMulchthled)
	{
		thread th_a(ParticleControl::Update);
		th_a.join();
		thread th_b(VoiceReciver::VoiceUDPUpdate);
		th_b.join();
	}
	else
	{
		ParticleControl::Update();
		VoiceReciver::VoiceUDPUpdate();
	}

	Cameras::camera.isRCamera = false;
	const float spriteSpeed = 0.15f;
	XMFLOAT3 spaceEndPos = { window_width / 2.0f - 80.0f, 600.0f, 0 };
	XMFLOAT3 titleEndPos = { window_width / 2.0f - 80.0f, 250.0f, 0 };

	//seling.Update();
	rSeling.selingModel.each.position = seling.selingModel.each.position;
	//rSeling.Update();
	waterFace[0].Update();
	waterFace[1].Update();
	normalWater.Update();
	mosaicWater.Update();

	XMFLOAT3 po = { 0, 0, 0 };
	if (Imgui::isParticleEditActive)
	{
		ParticleEditUpdate();
	}


	spaceSp.position = ConvertXMFLOAT3toXMVECTOR(Lerp(ConvertXMVECTORtoXMFLOAT3(spaceSp.position), spaceEndPos, spriteSpeed));
	titleSp.position = ConvertXMFLOAT3toXMVECTOR(Lerp(ConvertXMVECTORtoXMFLOAT3(titleSp.position), titleEndPos, spriteSpeed));
	if (Input::KeyTrigger(DIK_SPACE))
	{
		Imgui::sceneNum = GAME;
	}

	XMFLOAT3 selingPos = ConvertXMVECTORtoXMFLOAT3(seling.selingModel.each.position);
	selingPos.y -= 0.2f;
	ParticleControl::sheetOfSpray->SheetOfSprayParticle(selingPos, seling.selingModel.each.rotation, seling.addForce, 0.5f, 0.0f);

	static int particleTimer = 0;
	particleTimer++;
	if (particleTimer > 1)
	{
		XMFLOAT3 emitpos = ConvertXMVECTORtoXMFLOAT3(seling.selingModel.each.position);
		ParticleControl::sheetOfSpray->cubeParticle(emitpos, { 50.0f, 50.0f, 50.0f });
		//ParticleControl::elementEffect->LifeParticle(emitpos, 11.0f, 0.5f, 0.0f, 250);
		particleTimer = 0;
	}

	Cameras::camera.target = ConvertXMVECTORtoXMFLOAT3(seling.selingModel.each.position);
	Cameras::camera.Update();

	const float lengY = 6.0f;
	const float lengXY = 1.0f;
	Cameras::rCamera.eye = { Cameras::camera.eye.x * lengXY, Cameras::camera.eye.y * lengY, Cameras::camera.eye.z * lengXY };
	Cameras::rCamera.up = { 0, -1, 0 };
	Cameras::rCamera.target = Cameras::camera.target;
	Cameras::rCamera.Update();

	Sound::Updete(Imgui::volume);

	PouseUpdate();
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
	if (Imgui::isMulchthled)
	{
		std::thread th_a(ParticleControl::Update);
		th_a.join();
		std::thread th_b(VoiceReciver::VoiceUDPUpdate);
		th_b.join();
	}
	else
	{
		ParticleControl::Update();
		VoiceReciver::VoiceUDPUpdate();
	}
	light->SetLightDir(XMFLOAT3(Cameras::camera.GetTargetDirection()));
	LightUpdate();
	Cameras::camera.isRCamera = false;
	seling.Update(isPouse);
	rSeling.selingModel.each.position = seling.selingModel.each.position;
	rSeling.Update(isPouse);
	waterFace[0].Update();
	waterFace[1].Update();
	normalWater.Update();
	mosaicWater.Update();

	if (Imgui::isParticleEditActive)
	{
		ParticleEditUpdate();
	}

	XMFLOAT3 selingPos = ConvertXMVECTORtoXMFLOAT3(seling.selingModel.each.position);
	selingPos.y -= 0.2f;
	ParticleControl::sheetOfSpray->SheetOfSprayParticle(selingPos, seling.selingModel.each.rotation, seling.addForce, 0.5f, 0.0f);

	//
	const float spriteSpeed = 0.15f;
	XMFLOAT3 spaceSpGoal(static_cast<float>(window_width), 600.0f, 0);
	XMFLOAT3 titleSpGoal(window_width / 2.0f - 80.0f, -128.0f, 0);
	spaceSp.position = ConvertXMFLOAT3toXMVECTOR(Lerp(ConvertXMVECTORtoXMFLOAT3(spaceSp.position), spaceSpGoal, spriteSpeed));
	titleSp.position = ConvertXMFLOAT3toXMVECTOR(Lerp(ConvertXMVECTORtoXMFLOAT3(titleSp.position), titleSpGoal, spriteSpeed));

	Cameras::camera.target = ConvertXMVECTORtoXMFLOAT3(seling.selingModel.each.position);
	Cameras::camera.Update();

	const float lengY = 6.0f;
	const float lengXY = 1.0f;
	Cameras::rCamera.eye = { Cameras::camera.eye.x * lengXY, Cameras::camera.eye.y * lengY, Cameras::camera.eye.z * lengXY };
	Cameras::rCamera.up = { 0, -1, 0 };
	Cameras::rCamera.target = Cameras::camera.target;
	Cameras::rCamera.Update();

	static int particleTimer = 0;
	particleTimer++;
	if (particleTimer > 1)
	{
		XMFLOAT3 emitpos = ConvertXMVECTORtoXMFLOAT3(seling.selingModel.each.position);
		ParticleControl::sheetOfSpray->cubeParticle(emitpos, { 50.0f, 50.0f, 50.0f });
		particleTimer = 0;
	}

	if (seling.GetIsGoal())
	{
		Imgui::sceneNum = RESULT;
	}


	//ParticleControl::Update();
	Sound::Updete(Imgui::volume);
	PouseUpdate();
}

void GameScene::ResultUpdate()
{
	if (Imgui::isMulchthled)
	{
		std::thread th_a(ParticleControl::Update);
		th_a.join();
		std::thread th_b(VoiceReciver::VoiceUDPUpdate);
		th_b.join();
	}
	else
	{
		ParticleControl::Update();
		VoiceReciver::VoiceUDPUpdate();
	}
	//カメラのイージングを行う　
	/*XMFLOAT3 cameraStart(Imgui::CameraRotation, 0, 0);
	XMFLOAT3 cameraEnd(350.0f, 0, 0);
	Imgui::CameraRotation = ShlomonMath::EaseInOutQuad(cameraStart, cameraEnd, 0.1f).x;*/
	Cameras::camera.target = ConvertXMVECTORtoXMFLOAT3(seling.selingModel.each.position);
	Cameras::camera.Update();

	const float lengY = 6.0f;
	const float lengXY = 1.0f;
	Cameras::rCamera.eye = { Cameras::camera.eye.x * lengXY, Cameras::camera.eye.y * lengY, Cameras::camera.eye.z * lengXY };
	Cameras::rCamera.up = { 0, -1, 0 };
	Cameras::rCamera.target = Cameras::camera.target;
	Cameras::rCamera.Update();

	//スプライトのイージング
	const float spriteSpeed = 0.2f;

	XMFLOAT3 goalSpGoal(window_width / 2.0f - 64.0f, window_height / 2.0f, -0);
	XMFLOAT3 spaceSpGoal(window_width / 2.0f - 80.0f, 600.0f, 0);
	goalSp.position = ConvertXMFLOAT3toXMVECTOR(ShlomonMath::EaseInQuad(ConvertXMVECTORtoXMFLOAT3(goalSp.position), goalSpGoal, spriteSpeed));
	spaceSp.position = ConvertXMFLOAT3toXMVECTOR(Lerp(ConvertXMVECTORtoXMFLOAT3(spaceSp.position), spaceSpGoal, spriteSpeed));

	//タイトルに戻す
	if (Input::KeyTrigger(DIK_SPACE))
	{
		Imgui::sceneNum = TITLE;
		seling.Init(baseDirectX);
		rSeling.Init(baseDirectX);
		string path = "Resource/TextData/Stage/stage" + to_string(Imgui::LoadStageNum) + ".txt";
		StageObjects::LoadFile(baseDirectX, seling, path.c_str());
		Imgui::CameraRotation = 270.0f;
		goalSp.position = { static_cast<float>(window_width), window_height / 2.0f, 0 , 1.0f };
	}

	static int particleTimer = 0;
	particleTimer++;
	if (particleTimer > 1)
	{
		XMFLOAT3 emitpos = ConvertXMVECTORtoXMFLOAT3(seling.selingModel.each.position);
		ParticleControl::sheetOfSpray->cubeParticle(emitpos, { 50.0f, 50.0f, 50.0f });
		particleTimer = 0;
	}

	Sound::Updete(Imgui::volume);
	PouseUpdate();
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
	XMFLOAT3 nowMousePos = Cameras::camera.MousePosition(baseDirectX, 0.0f);
	if (!Imgui::touchedImgui)
	{
		XMFLOAT3 mousePosi(nowMousePos.x - Cameras::camera.mouseMoveAmount[0], nowMousePos.y - Cameras::camera.mouseMoveAmount[1], nowMousePos.z);
		MapEditorObjects::Update(baseDirectX, mousePosi);
	}
	Imgui::touchedImgui = false;
	LightUpdate();
	Imgui::CameraControl = false;
	Cameras::camera.MouseWheelY();
	Cameras::camera.MouseRightPushMove(baseDirectX);
	Cameras::camera.Update();
}

void GameScene::ParticleEditUpdate()
{
	//if (Imgui::emitterPlayTimer > 0)
	//{
	ParticleControl::editorParticle->EditorParticle();
	//}
	//Imgui::emitterPlayTimer--;

	//ParticleControl::Update();
	particleAreaModel.each.position = { static_cast<float>(Imgui::emitterPosition[0]), static_cast<float>(Imgui::emitterPosition[1]), static_cast<float>(Imgui::emitterPosition[2]), 1 };
	particleAreaModel.each.scale = { static_cast<float>(Imgui::particleSpornArea[0]), static_cast<float>(Imgui::particleSpornArea[1]) , static_cast<float>(Imgui::particleSpornArea[2]) };
	particleAreaModel.Update(baseDirectX, &particleAreaModel.each, Cameras::camera);
	light->SetLightDir(XMFLOAT3(Cameras::camera.GetTargetDirection()));
	LightUpdate();
}

void GameScene::EndUpdate()
{
	if (Input::KeyTrigger(DIK_SPACE) || Input::directInput->IsButtonPush(DirectInput::ButtonKind::Button01))
	{
		Imgui::sceneNum = TITLE;
		XMFLOAT3 eye(0, 10, -15.0f);
		XMFLOAT3 target(0, 0, 0);
		Cameras::camera.Init(eye, target);
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

	XMVECTOR waterFacePosition = { 0, -0.8f, 0.0f, 1.0 };
	XMVECTOR waterFacePosition2 = { waterFacePosition.m128_f32[0], -0.8f, waterFacePosition.m128_f32[2] + 89.0f, 1.0 };

	waterFace[0].waterModel.m_renderTarget.PreDraw(baseDirectX);
	light->SetLightDir(XMFLOAT3(Cameras::rCamera.GetTargetDirection()));
	LightUpdate();
	rSeling.Draw(baseDirectX, waterFace[0].m_camera.get());
	//rWorld.Update(baseDirectX, &rWorld.each, waterFace[0].m_camera.get());
	Draw3DObject(baseDirectX, rWorld);
	StageObjects::Draw(baseDirectX, waterFace[0].m_camera.get());
	ParticleManager::InitializeCamera(window_width, window_height, Cameras::rCamera.eye, Cameras::rCamera.target, Cameras::rCamera.up);
	waterFace[0].waterModel.m_renderTarget.PostDraw(baseDirectX);
	//ParticleControl::Draw(baseDirectX);
	//waterFace[0].Draw(baseDirectX, waterFacePosition);
	//waterFace[0].waterModel.m_renderTarget.PostDraw(baseDirectX);
}

void GameScene::PostWaterFaceDraw()
{
	light->SetLightDir(XMFLOAT3(Cameras::camera.GetTargetDirection()));
	LightUpdate();
	seling.Draw(baseDirectX);
	//world.Update(baseDirectX, &world.each, false);
	Draw3DObject(baseDirectX, world);
	ParticleManager::InitializeCamera(window_width, window_height, Cameras::camera.eye, Cameras::camera.target, Cameras::camera.up);
	if (Imgui::isParticleEditActive)
	{
		Draw3DObject(baseDirectX, particleAreaModel, true);
	}
	ParticleControl::Draw(baseDirectX);
	/*skyDome.Update(baseDirectX, &skyDome.each);
	Draw3DObject(baseDirectX, skyDome);*/
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

	DrawPostEffect();

	PostEffects::Draw(baseDirectX);

	//PostEffects::PostDraw(baseDirectX);

	DrawSprites();
	PouseDraw();

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

	DrawPostEffect();

	PostEffects::Draw(baseDirectX);

	DrawSprites();
	PouseDraw();

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

	DrawPostEffect();

	PostEffects::Draw(baseDirectX);

	DrawSprites();
	PouseDraw();

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
	Draw3DObject(baseDirectX, mapFrameV, false);
	Draw3DObject(baseDirectX, mapFrameH, false);
	MapEditorObjects::Draw(baseDirectX);
	Imgui::DrawImGui(baseDirectX);
	//描画コマンドここまで
	baseDirectX.UpdateBack();
}

void GameScene::ParticleEditDraw()
{
	baseDirectX.UpdateFront();

	Draw3DObject(baseDirectX, particleAreaModel, true);
	ParticleControl::Draw(baseDirectX);
	Cameras::camera.EditorMouseControl(baseDirectX);
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
	//waterFace[0].waterModel.eachData.rotation.x = Imgui::CameraRotation;
	//水面の切り替え
	if (Imgui::useWaterNum == 0)
	{
		waterFace[0].Draw(baseDirectX, waterFacePosition);
		waterFace[1].Draw(baseDirectX, waterFacePosition);
	}
	else if (Imgui::useWaterNum == 1)
	{
		normalWater.Draw(baseDirectX, waterFacePosition);
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

void GameScene::PouseUpdate()
{
	if (Input::KeyTrigger(DIK_ESCAPE) && !isPouse)
	{
		isPouse = true;
	}
	else if (Input::KeyTrigger(DIK_ESCAPE) && isPouse)
	{
		isPouse = false;
	}
}

void GameScene::PouseDraw()
{
	if (!isPouse)return;
	pouseUI.DrawUI(baseDirectX);
}

void GameScene::DrawPostEffect()
{
	XMVECTOR waterFacePosition = { seling.selingModel.each.position.m128_f32[0], -0.2f, seling.selingModel.each.position.m128_f32[2], 1.0 };
	XMVECTOR waterFacePosition2 = { waterFacePosition.m128_f32[0], -0.2f, waterFacePosition.m128_f32[2] + 89.0f, 1.0 };
	//水面の切り替え
	if (Imgui::useWaterNum == 0)
	{
		const float rotAngle = 0.25f;
		/*if (seling.lookToLeftKey.GetKey())
		{
			waterFace[0].waterModel.eachData.rotation.z -= rotAngle;
		}
		if (seling.lookToRightKey.GetKey())
		{
			waterFace[0].waterModel.eachData.rotation.z += rotAngle;
		}*/
		waterFace[0].waterModel.eachData.rotation.z = seling.selingModel.each.rotation.z;
		waterFace[0].Draw(baseDirectX, waterFacePosition);
	}
	else if (Imgui::useWaterNum == 1)
	{
		normalWater.Draw(baseDirectX, waterFacePosition);
	}
	else if (Imgui::useWaterNum == 2)
	{
		mosaicWater.Draw(baseDirectX, waterFacePosition);
	}
	else if (Imgui::useWaterNum == 3)
	{
		monoWater.Draw(baseDirectX, waterFacePosition);
	}
	else if (Imgui::useWaterNum == 4)
	{
		blurWater.Draw(baseDirectX, waterFacePosition);
	}
}
