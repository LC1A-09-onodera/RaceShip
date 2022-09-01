#include <DirectXMath.h>
#include <array>
#include "../BaseDirectX/BaseDirectX.h"
#include "../BaseDirectX/DX12operator.h"
using namespace DirectX;

class Camera
{
	float shakePower = 0.0f;
	bool isShake = false;
	float rotationXZ = 0.0f;
	XMFLOAT3 rotation = {0, 0.2f, -1.0f};
	float targetR = 10.0f;

public:
	//位置
	XMFLOAT3 eye = {0, 0, 0};
	XMFLOAT3 eyeStartPosition = {0, 0, 0};
	XMFLOAT3 eyeGoalPosition = {0, 0, 0};
	
	//どこを注視点とするか
	XMFLOAT3 target = { 0, 0, 0 };
	XMFLOAT3 targetStartPosition = { 0, 0, 0 };
	XMFLOAT3 targetGoalPosition = { 0, 0, 0 };
	//カメラの向き？
	XMFLOAT3 up = { 0, 0, 0 };
	//ビュー行列の作成
	XMMATRIX matView = {};
	//ビルボード
	XMMATRIX matBillboard = {};
	//Y軸ビルボード
	XMMATRIX matBillboardY = {};
	//Imguiからの操作を受け付ける
	bool isControl = true;
	//反対方向のカメラかどうか
	bool isRCamera = false;

	std::array<float, 2> mouseClickPos = { 0, 0 };
	std::array<float, 2> mouseMoveAmount = { 0, 0 };
	
	void CameraTargetRot();
	void Init(XMFLOAT3 &f_eye, XMFLOAT3 &f_target);
	void Update();
	void EyeAndTargetMove(XMFLOAT3 moveAmount);
	float TargetLength();
	void ShakeUpdate();
	void SetShake(float f_shakePower);
	void SetTarget(XMFLOAT3 &f_target);
	XMFLOAT3 GetTargetDirection();
	XMFLOAT3 GetMousePosition(BaseDirectX &baseDirectX);
	XMFLOAT3 EyeToMouseVec(BaseDirectX& baseDirectX);
	XMFLOAT3 MousePosition(BaseDirectX& baseDirectX, float z);
	void MouseWheelY();
	void MouseRightPushMove(BaseDirectX& baseDirectX);
};

class Cameras
{
public:
	static Camera camera;
	static Camera rCamera;
};