#include "../VoiceUDPRecive/VoiceUDPRecive.h"
#include"GameScene.h"
#include "BaseDirectX.h"
#include "../WindowsAPI/WinAPI.h"
#include "Input.h"
#include "viewport.h"
#include "../imgui/ImguiControl.h"
#include "../FBX/FBXObject/FBXObject.h"
#include "../Shader/ShaderManager.h"
#include "../Sound/Sound.h"
#include "../Tools/LoadStage/LoadStage.h"
#include "../Tools/LoadStage/StageObject.h"
#include "../Tools/MapLayout/MapLayout.h"
#include "../Tools/MapLayout/SceneMapLayout.h"
#include "../BehaviorTree/BehaviorTree.h"

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
	Imgui::GetInstance()->Update(seling);
	Input::Update();
	if (Imgui::GetInstance()->isKeyRec == Imgui::GetInstance()->KeyRec::PlayBack)
	{
		KeyLog::Playback();
	}
	else if (Imgui::GetInstance()->isKeyRec == Imgui::GetInstance()->KeyRec::Rec)
	{
		KeyLog::Recording();
	}
	if (Imgui::GetInstance()->isPosRec == Imgui::GetInstance()->KeyRec::PlayBack)
	{
		PlayerPositionLog::Playback(seling.selingModel.each);
	}
	else if (Imgui::GetInstance()->isPosRec == Imgui::GetInstance()->KeyRec::Rec)
	{
		XMFLOAT3 pos = { seling.selingModel.each.position.m128_f32[0], seling.selingModel.each.position.m128_f32[1], seling.selingModel.each.position.m128_f32[2] };
		PlayerPositionLog::Recording(pos);
	}
	WindowsAPI::CheckMsg();
	light->Update();
	if (Input::KeyTrigger(DIK_F1))
	{
		Imgui::GetInstance()->sceneNum = TITLE;
		Imgui::GetInstance()->CameraControl = true;
		Cameras::camera.isRCamera = false;
	}
	if (Input::KeyTrigger(DIK_F2))
	{
		Imgui::GetInstance()->sceneNum = MAPEDIT;
		Cameras::camera.isRCamera = true;
		Imgui::GetInstance()->GetInstance()->CameraControl = false;
		Cameras::camera.mouseMoveAmount[0] = 0.0f;
		Cameras::camera.mouseMoveAmount[1] = 0.0f;
		XMFLOAT3 cameraEeyReset(0, 0, 20.0f);
		XMFLOAT3 cameraTargetReset(0, 0, 0);
		Cameras::camera.Init(cameraEeyReset, cameraTargetReset);
	}
	if (Input::KeyTrigger(DIK_F3))
	{
		Imgui::GetInstance()->sceneNum = ParticleEdit;
	}
	switch (Imgui::GetInstance()->sceneNum)
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
	case Behavior:
		BaseDirectX::GetInstance()->UpdateFront();
		Cameras::camera.EditorMouseControl();

		Imgui::GetInstance()->DrawImGui(seling.selingModel.each);
		//描画コマンドここまで
		BaseDirectX::GetInstance()->UpdateBack();
	default:
		break;
	}
}

void GameScene::Init()
{
	SceneNum = TITLE;
	BaseDirectX::GetInstance()->Set();
	//サウンド
	Sound::CreateVoice();
	//SRVのアドレス確保
	BaseDirectX::GetInstance()->GetAdress();
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
	Imgui::GetInstance()->Init();
	//シェーダーのロード
	ShaderManager::LoadShaders();
	// 3Dパーティクル静的初期化
	ParticleControl::Init();
	//インプット初期化
	Input::KeySet(WindowsAPI::w, WindowsAPI::hwnd);
	//FBX系
	/*FbxLoader::GetInstance()->Initialize();
	FBXObject::SetDevice(.dev.Get());
	FBXObject::CreateGraphicsPipeline();*/
	//ライト初期化
	light.reset(Light::Create());
	//モデルすべてにライトを適用
	Model::SetLight(light);
	//inspector初期化
	EachManager::eahcs.clear();
	//ポストエフェクトの初期化
	PostEffects::Init();

	StageObjects::LoadModel();

	MapEditorObjects::LoadModels();

	//Rewiredの要素初期化
	Rewired::KeyCodeString::KeyCodeStringInit();
	Rewired::RewiredContainer::GetFilesName();

	mapFrameV.CreateModel( "Map", ShaderManager::playerShader, false, false);
	mapFrameV.each.m_eachName = "MapV";
	EachManager::eahcs.push_back(&mapFrameV.each);
	mapFrameH.CreateModel("Map", ShaderManager::playerShader, false, false);
	mapFrameH.each.m_eachName = "MapH";
	EachManager::eahcs.push_back(&mapFrameH.each);
	mapFrameV.each.rotation.y = 90.0f;
	mapFrameH.each.rotation.x = 90.0f;
	mapFrameH.each.rotation.y = 0.0f;
	mapFrameH.each.rotation.z = 90.0f;

	particleAreaModel.CreateModel("ParticleArea", ShaderManager::playerShader, false, false);

	seling.LoadModel();
	EachManager::eahcs.push_back(&seling.selingModel.each);

	rSeling.LoadModel();
	//ボイスコマンドの通信受付スタート
	//送信側はまた違うアプリケーションで行う
	VoiceReciver::StartUp();
	XMFLOAT3 cameraPos = { 0.0f, -67.0f, -10.1f };
	XMFLOAT3 targetPos = { 0, 0, 0 };
	XMFLOAT3 cameraPos2 = { 0.0f, -67.0f + 89.0f, -10.1f };
	XMFLOAT3 targetPos2 = { 0, 0 + 89.0f, 0 };
	waterFace[0].LoadModel( ShaderManager::waterShader, cameraPos, targetPos);
	waterFace[0].waterModel.eachData.m_eachName = "water0";
	EachManager::eahcs.push_back((&waterFace[0].waterModel.eachData));
	waterFace[1].LoadModel( ShaderManager::waterShader, cameraPos2, targetPos2);
	waterFace[1].waterModel.eachData.position.m128_f32[2] = 20.0f;
	normalWater.LoadModel( ShaderManager::normalPlaneShader, cameraPos, targetPos);
	mosaicWater.LoadModel( ShaderManager::mosaicPlaneShader, cameraPos, targetPos);
	monoWater.LoadModel( ShaderManager::monoPlaneShader, cameraPos, targetPos);
	blurWater.LoadModel( ShaderManager::blurPlaneShader, cameraPos, targetPos);

	const float worldSize = 40.0f;
	world.CreateModel( "SphereW", ShaderManager::playerShader);
	world.each.scale = { worldSize, worldSize, worldSize };
	rWorld.CreateModel( "SphereW", ShaderManager::playerShader);
	rWorld.each.scale = { worldSize, worldSize, worldSize };
	skyDome.CreateModel( "sky", ShaderManager::playerShader, true);
	const float skyDomeSize = 60.0f;
	skyDome.each.scale = { skyDomeSize , skyDomeSize , skyDomeSize };

	spaceSp.CreateSprite( L"Resource/Image/space.png", XMFLOAT3(static_cast<float>(window_width), 600.0f, 0));
	goalSp.CreateSprite( L"Resource/Image/Goal.png", XMFLOAT3(static_cast<float>(window_width), window_height / 2.0f, 0));
	titleSp.CreateSprite( L"Resource/Image/Title.png", XMFLOAT3(window_width / 2.0f - 128.0f, -128.0f, 0));
	for (int i = 0; i < 10; i++)
	{
		string path = "Resource/Image/Numbers/Num" + to_string(i) + ".png";
		numTex[i].LoadGraph( ConvertStringToWchaer(path));
	}
	for (int i = 0; i < 10; i++)
	{
		resultNumSprite[i].CreateSprite( numTex[i], XMFLOAT3(window_width / 2.0f - 100.0f, 600.0f, 0));
		resultNumSprite[i + 10].CreateSprite( numTex[i], XMFLOAT3(window_width / 2.0f - 100.0f, 600.0f, 0));
		resultNumSprite[i + 20].CreateSprite( numTex[i], XMFLOAT3(window_width / 2.0f - 100.0f, 600.0f, 0));
		resultNumSprite[i + 30].CreateSprite( numTex[i], XMFLOAT3(window_width / 2.0f - 100.0f, 600.0f, 0));
	}
	pouseUI.LoadFile();
	Rewired::RewiredContainer::LoadAllRewired();

	seling.Init();
	rSeling.Init();
	//ステージをテキストからロード
	//LoadStage::LoadStages("Resource/TextData/Stage/stage1.txt");
	StageObjects::LoadFile( seling, "Resource/TextData/Stage/stage1.txt");

	KeyLog::SetFileName("log");

	SceneMapLayout::LoadModel();
	SceneMapLayout::Init();

	//BehaviorTree::ExBehavior::Init(BehaviorTree::ExBehavior::hoge);
	BehaviorTree::Enemy::Init("hoge");
}

void GameScene::TitleUpdate()
{
	BehaviorTree::Enemy::Update();
	/*if (BehaviorTree::ExBehavior::IsFunc())
	{
		int a = 0;
	}*/
	if (Imgui::GetInstance()->isMulchthled)
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
	if (Imgui::GetInstance()->isParticleEditActive)
	{
		ParticleEditUpdate();
	}


	spaceSp.position = ConvertXMFLOAT3toXMVECTOR(Lerp(ConvertXMVECTORtoXMFLOAT3(spaceSp.position), spaceEndPos, spriteSpeed));
	titleSp.position = ConvertXMFLOAT3toXMVECTOR(Lerp(ConvertXMVECTORtoXMFLOAT3(titleSp.position), titleEndPos, spriteSpeed));
	if (Input::KeyTrigger(DIK_SPACE))
	{
		Imgui::GetInstance()->sceneNum = GAME;
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

	Sound::Updete(Imgui::GetInstance()->volume);

	PouseUpdate();
}

void GameScene::SelectUpdate()
{
	if (Input::KeyTrigger(DIK_SPACE))
	{
		Imgui::GetInstance()->sceneNum = GAME;
	}
}

void GameScene::GameUpdate()
{
	if (Imgui::GetInstance()->isMulchthled)
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

	if (Imgui::GetInstance()->isParticleEditActive)
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
	//Cameras::rCamera.up = { 0, 0, -1 };
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
		Imgui::GetInstance()->sceneNum = RESULT;
	}


	//ParticleControl::Update();
	Sound::Updete(Imgui::GetInstance()->volume);
	PouseUpdate();
}

void GameScene::ResultUpdate()
{
	if (Imgui::GetInstance()->isMulchthled)
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
	/*XMFLOAT3 cameraStart(Imgui::GetInstance()->CameraRotation, 0, 0);
	XMFLOAT3 cameraEnd(350.0f, 0, 0);
	Imgui::GetInstance()->CameraRotation = ShlomonMath::EaseInOutQuad(cameraStart, cameraEnd, 0.1f).x;*/
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
		Imgui::GetInstance()->sceneNum = TITLE;
		seling.Init();
		rSeling.Init();
		string path = "Resource/TextData/Stage/stage" + to_string(Imgui::GetInstance()->LoadStageNum) + ".txt";
		StageObjects::LoadFile( seling, path.c_str());
		Imgui::GetInstance()->CameraRotation = 270.0f;
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

	Sound::Updete(Imgui::GetInstance()->volume);
	PouseUpdate();
}

void GameScene::OPUpdate()
{
	if (opAnimationTime > MaxOPAnimationTime)
	{
		Imgui::GetInstance()->sceneNum = TITLE;
	}
	opAnimationTime++;
}

void GameScene::MapEditUpdate()
{
	light->SetLightDir(XMFLOAT3(Cameras::camera.GetTargetDirection()));
	mapFrameV.Update( &mapFrameV.each);
	mapFrameH.Update( &mapFrameH.each);
	//deleteした後のオブジェクトを描画しようとしてエラーを出さないための応急処置
	//Update段階でオブジェクトをなくしたい
	Imgui::GetInstance()->DrawImGui(seling.selingModel.each);
	XMFLOAT3 nowMousePos = Cameras::camera.MousePosition( 0.0f);

	XMFLOAT3 mousePosi(nowMousePos.x - Cameras::camera.mouseMoveAmount[0], nowMousePos.y - Cameras::camera.mouseMoveAmount[1], nowMousePos.z);
	MapEditorObjects::Update( mousePosi);
	
	Imgui::GetInstance()->touchedImgui = false;
	LightUpdate();
	Imgui::GetInstance()->CameraControl = false;
	Cameras::camera.MouseWheelY();
	Cameras::camera.MouseRightPushMove();
	Cameras::camera.Update();
}

void GameScene::ParticleEditUpdate()
{
	//if (Imgui::GetInstance()->emitterPlayTimer > 0)
	//{
	ParticleControl::editorParticle->EditorParticle();
	//}
	//Imgui::GetInstance()->emitterPlayTimer--;

	//ParticleControl::Update();
	particleAreaModel.each.position = { static_cast<float>(Imgui::GetInstance()->emitterPosition[0]), static_cast<float>(Imgui::GetInstance()->emitterPosition[1]), static_cast<float>(Imgui::GetInstance()->emitterPosition[2]), 1 };
	particleAreaModel.each.scale = { static_cast<float>(Imgui::GetInstance()->particleSpornArea[0]), static_cast<float>(Imgui::GetInstance()->particleSpornArea[1]) , static_cast<float>(Imgui::GetInstance()->particleSpornArea[2]) };
	particleAreaModel.Update( &particleAreaModel.each, Cameras::camera);
	light->SetLightDir(XMFLOAT3(Cameras::camera.GetTargetDirection()));
	LightUpdate();
}

void GameScene::EndUpdate()
{
	if (Input::KeyTrigger(DIK_SPACE) || Input::directInput->IsButtonPush(DirectInput::ButtonKind::Button01))
	{
		
		Imgui::GetInstance()->sceneNum = TITLE;
		XMFLOAT3 eye(0, 10, -15.0f);
		XMFLOAT3 target(0, 0, 0);
		Cameras::camera.Init(eye, target);
	}
}

void GameScene::PreWaterFaceDraw()
{
	bool isRDraw;
	if (Imgui::GetInstance()->effectType < 0)
	{
		isRDraw = true;
	}
	else
	{
		isRDraw = false;
	}

	XMVECTOR waterFacePosition = { 0, -0.8f, 0.0f, 1.0 };
	XMVECTOR waterFacePosition2 = { waterFacePosition.m128_f32[0], -0.8f, waterFacePosition.m128_f32[2] + 89.0f, 1.0 };

	waterFace[0].waterModel.m_renderTarget.PreDraw();

	light->SetLightDir(XMFLOAT3(Cameras::rCamera.GetTargetDirection()));
	LightUpdate();
	rSeling.Draw(waterFace[0].m_camera.get());
	//rWorld.Update( &rWorld.each, waterFace[0].m_camera.get());
	Draw3DObject(rWorld);
	SceneMapLayout::WaterFaceDraw();
	StageObjects::Draw(waterFace[0].m_camera.get());

	ParticleManager::InitializeCamera(window_width, window_height, Cameras::rCamera.eye, Cameras::rCamera.target, Cameras::rCamera.up);
	
	waterFace[0].waterModel.m_renderTarget.PostDraw();
}

void GameScene::PostWaterFaceDraw()
{
	light->SetLightDir(XMFLOAT3(Cameras::camera.GetTargetDirection()));
	LightUpdate();
	seling.Draw();
	//world.Update( &world.each, false);
	Draw3DObject( world);

	SceneMapLayout::MainDraw();
	ParticleManager::InitializeCamera(window_width, window_height, Cameras::camera.eye, Cameras::camera.target, Cameras::camera.up);
	if (Imgui::GetInstance()->isParticleEditActive)
	{
		Draw3DObject( particleAreaModel, true);
	}
	ParticleControl::Draw();
	/*skyDome.Update( &skyDome.each);
	Draw3DObject( skyDome);*/
	StageObjects::Draw( false);
}

void GameScene::DrawSprites()
{
	switch (SceneNum)
	{
	case OP:
		break;
	case TITLE:
		spaceSp.SpriteDraw();
		titleSp.SpriteDraw();
		break;
	case GAME:
		spaceSp.SpriteDraw();
		titleSp.SpriteDraw();
		break;
	case RESULT:
		spaceSp.SpriteDraw();
		goalSp.SpriteDraw();
		break;
	}
}

void GameScene::TitleDraw()
{
	//PostEffectのPreDraw
	PostEffects::PreDraw();

	PreWaterFaceDraw();

	BaseDirectX::GetInstance()->UpdateFront();

	PostWaterFaceDraw();

	DrawPostEffect();

	PostEffects::Draw();

	//PostEffects::PostDraw();

	DrawSprites();
	PouseDraw();

	PostEffects::PostDraw();
	Imgui::GetInstance()->DrawImGui(seling.selingModel.each);
	//描画コマンドここまで
	BaseDirectX::GetInstance()->UpdateBack();
}

void GameScene::SelectDraw()
{
	BaseDirectX::GetInstance()->UpdateFront();

	DrawSprites();
	Imgui::GetInstance()->DrawImGui(seling.selingModel.each);
	//描画コマンドここまで
	BaseDirectX::GetInstance()->UpdateBack();
}

void GameScene::GameDraw()
{
	//PostEffectのPreDraw
	PostEffects::PreDraw();

	PreWaterFaceDraw();

	BaseDirectX::GetInstance()->UpdateFront();

	PostWaterFaceDraw();

	DrawPostEffect();

	PostEffects::Draw();

	DrawSprites();
	PouseDraw();

	PostEffects::PostDraw();
	Imgui::GetInstance()->DrawImGui(seling.selingModel.each);
	//描画コマンドここまで
	BaseDirectX::GetInstance()->UpdateBack();
}

void GameScene::ResultDraw()
{
	//PostEffectのPreDraw
	PostEffects::PreDraw();

	PreWaterFaceDraw();

	BaseDirectX::GetInstance()->UpdateFront();

	PostWaterFaceDraw();

	DrawPostEffect();

	PostEffects::Draw();

	DrawSprites();
	PouseDraw();

	PostEffects::PostDraw();

	//
	Imgui::GetInstance()->DrawImGui(seling.selingModel.each);
	//描画コマンドここまで
	BaseDirectX::GetInstance()->UpdateBack();
}

void GameScene::OPDraw()
{
}

void GameScene::MapEditDraw()
{
	BaseDirectX::GetInstance()->UpdateFront();
	Draw3DObject( mapFrameV, false);
	Draw3DObject( mapFrameH, false);
	MapEditorObjects::Draw();
	Imgui::GetInstance()->DrawImGui(seling.selingModel.each);
	//描画コマンドここまで
	BaseDirectX::GetInstance()->UpdateBack();
}

void GameScene::ParticleEditDraw()
{
	BaseDirectX::GetInstance()->UpdateFront();

	Draw3DObject( particleAreaModel, true);
	ParticleControl::Draw();
	Cameras::camera.EditorMouseControl();
	Imgui::GetInstance()->DrawImGui(seling.selingModel.each);
	//描画コマンドここまで
	BaseDirectX::GetInstance()->UpdateBack();
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
	PostEffects::PreDraw();

	PreWaterFaceDraw();

	BaseDirectX::GetInstance()->UpdateFront();

	PostWaterFaceDraw();

	XMVECTOR waterFacePosition = { 0, -0.5f, 0.0f, 1.0 };
	//waterFace[0].waterModel.eachData.rotation.x = Imgui::GetInstance()->CameraRotation;
	//水面の切り替え
	if (Imgui::GetInstance()->useWaterNum == 0)
	{
		waterFace[0].Draw( waterFacePosition);
		waterFace[1].Draw( waterFacePosition);
	}
	else if (Imgui::GetInstance()->useWaterNum == 1)
	{
		normalWater.Draw( waterFacePosition);
	}

	PostEffects::Draw();

	DrawSprites();

	PostEffects::PostDraw();

	//
	Imgui::GetInstance()->DrawImGui(seling.selingModel.each);
	//描画コマンドここまで
	BaseDirectX::GetInstance()->UpdateBack();
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
	pouseUI.DrawUI();
}

void GameScene::DrawPostEffect()
{
	XMVECTOR waterFacePosition = { seling.selingModel.each.position.m128_f32[0], -0.2f, seling.selingModel.each.position.m128_f32[2], 1.0 };
	XMVECTOR waterFacePosition2 = { waterFacePosition.m128_f32[0], -0.2f, waterFacePosition.m128_f32[2] + 89.0f, 1.0 };
	//水面の切り替え
	if (Imgui::GetInstance()->useWaterNum == 0)
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
		waterFace[0].Draw( waterFacePosition);
	}
	else if (Imgui::GetInstance()->useWaterNum == 1)
	{
		normalWater.Draw( waterFacePosition);
	}
	else if (Imgui::GetInstance()->useWaterNum == 2)
	{
		mosaicWater.Draw( waterFacePosition);
	}
	else if (Imgui::GetInstance()->useWaterNum == 3)
	{
		monoWater.Draw( waterFacePosition);
	}
	else if (Imgui::GetInstance()->useWaterNum == 4)
	{
		blurWater.Draw( waterFacePosition);
	}
}
