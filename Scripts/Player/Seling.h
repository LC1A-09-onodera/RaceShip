#pragma once
#include "../3DModel/Model.h"
#include "../BaseDirectX/DX12operator.h"
#include "../Rewired/Rewired.h"

class Camera;

class Seling
{
	//正面方向の取得
	XMFLOAT3 frontDirection = { 0.0f ,0.0f , 0.0f };
	//y軸に対しての角度
	float angle = 0;
	//動くかどうかのフラグ
	bool isMoveForce = false;
	
	//最大値を決めておく
	const XMFLOAT3 maxForce = { 0.7f ,0.7f , 0.7f};

	bool isGoal = false;
	bool isTitle = true;

	const float addForcePower = 0.02f;
	const float MaxForce = 1.0f;
	const float addSelingAngle = 1.0f;
	const float subForce = 50.0f;
public:
	//動く量
	XMFLOAT3 addForce = { 0.0f ,0.0f , 0.0f};
	Model selingModel;
	void AddForce(XMFLOAT3& force);
	XMFLOAT3 GetDirection()
	{
		return Normalize(frontDirection);
	}
	bool GetIsGoal() { return isGoal; }
	void ForceUpdate();
	void LoadModel(BaseDirectX &baseDirectX);
	void Init();
	void Update();
	void Update(bool isPouse);
	void Draw(BaseDirectX& baseDirectX, bool isRCamera = false);
	void Draw(BaseDirectX& baseDirectX, Camera &f_camera);

	void ForceAttach();
	void Move();

	void HitWall();
	void HitGoal();

	void LoadKeys();
	Rewired::RewiredKeys goFrontKey;
	Rewired::RewiredKeys goBackKey;
	Rewired::RewiredKeys lookToRightKey;
	Rewired::RewiredKeys lookToLeftKey;
};