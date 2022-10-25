#pragma once
#include "../3DModel/Model.h"
#include "../Camera/Camera.h"
#include "../Texture/Texture.h"
#include "../Sprite/Sprite.h"
#include "../Light/Light.h"
#include "../PostEffect/PostEffect.h"
#include "../FBX/FBXObject/FBXObject.h"
#include "../FBX/FbxLoader/FbxLoader.h"
#include "../Player/Seling.h"
#include "../Tools/Rewired/Rewired.h"
#include "../WaterFace/WaterFace.h"
#include "../MenuUI/MenuUI.h"
#include "../Tools/Particle/Particle3D.h"
#include "../Tools/LoadStage/SpringBoard.h"
#include "../Tools/KeyLog/KeyLog.h"

enum GameSceneName { TITLE, SELECT, GAME, END, RESULT, OP , MAPEDIT, RewiredEdit, ParticleEdit, Behavior};

class GameScene
{
private:
	//更新
	void GameUpdate();
	void TitleUpdate();
	void EndUpdate();
	void SelectUpdate();
	void ResultUpdate();

	void OPUpdate();
	void MapEditUpdate();
	void ParticleEditUpdate();
	//描画
	void GameDraw();
	void TitleDraw();
	void EndDraw();
	void SelectDraw();
	void ResultDraw();

	void OPDraw();
	void MapEditDraw();
	void ParticleEditDraw();


	void RewiredEditUpdate();
	void RewiredEditDraw();

	void PreWaterFaceDraw();
	void PostWaterFaceDraw();

	void DrawSprites();

	void LightUpdate();

	void PouseUpdate();
	void PouseDraw();

	void DrawPostEffect();

	int SceneNum = TITLE;
	//BaseDirectX baseDirectX;

	int opAnimationTime = 0;
	const int MaxOPAnimationTime = 120;

	bool isPouse = false;

	std::shared_ptr<Light> light;
	PostEffect post;
	float pointLightPos[3] = { 0, 1, 0 };
	float pointLightColor[3] = { 1, 1, 1 };
	float pointLightAtten[3] = { 0.3f, 0.1f, 0.1f };

	float spotLightDir[3] = { 0, -1, 0 };
	float spotLightPos[3] = { 0, 1, -100 };
	float spotLightColor[3] = { 0.9f, 0.9f, 0.9f };
	float spotLightAtten[3] = { 0.0f, 0.0f, 0.0f };
	float spotLightAngle[2] = { 1.0f, 500.0f };

	float circleShadowDir[3] = { 0, -1, 0 };
	float circleShadowAtten[3] = { 0.5f, 0.6f, 0.0f };
	float circleShadowFactorAngle[2] = { 0.0f, 0.0f };

	PostEffect postWater;
	PostEffect postMosaic;
	//動くプレイヤー
	Seling seling;
	Seling rSeling;
	//水面用
	WaterFace waterFace[2];
	WaterFace normalWater;
	WaterFace mosaicWater;
	WaterFace monoWater;
	WaterFace blurWater;
	//ワイヤーフレームのドーム
	Model world;
	Model rWorld;
	//外側のドーム
	Model skyDome;

	Sprite spaceSp;
	Tex numTex[10];
	Sprite resultNumSprite[40];
	Sprite goalSp;
	Sprite titleSp;

	//
	Model mapFrameV;
	Model mapFrameH;

	MenuUI pouseUI;

	Model particleAreaModel;
public:
	GameScene();
	~GameScene();
	//シーン切り替え
	void SceneManageUpdateAndDraw();
	//初期化
	void Init();
};