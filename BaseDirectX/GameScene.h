#pragma once
#include "../3DModel/Model.h"
#include "../Camera/Camera.h"
#include "../Sprite/Sprite.h"
#include "../Light/Light.h"
#include "../PostEffect/PostEffect.h"
#include "../FBXObject/FBXObject.h"
#include "../FbxLoader/FbxLoader.h"
#include "../Player/Seling.h"
#include "../Rewired/Rewired.h"
#include "../WaterFace/WaterFace.h"

enum GameSceneName { TITLE, SELECT, GAME, END, RESULT, OP };

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
	//描画
	void GameDraw();
	void TitleDraw();
	void EndDraw();
	void SelectDraw();
	void ResultDraw();

	void OPDraw();

	void PreWaterFaceDraw();
	void PostWaterFaceDraw();
	int SceneNum = TITLE;
	BaseDirectX baseDirectX;

	int opAnimationTime;
	const int MaxOPAnimationTime = 120;

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

	EachInfo playerPos;
	EachInfo rPlayerPos;

	Seling seling;
	Seling rSeling;
	WaterFace waterFace;
	WaterFace normalWater;

	Model world;
	Model rWorld;
	Model skyDome;

public:
	GameScene();
	~GameScene();
	//シーン切り替え
	void SceneManageUpdateAndDraw();
	//初期化
	void Init();

	void LightUpdate();

public://シーンで使うもの
	
};