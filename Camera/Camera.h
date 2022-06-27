#include <DirectXMath.h>
#include "../BaseDirectX/DX12operator.h"
using namespace DirectX;

class Camera
{
	float shakePower;
	bool isShake;
	float rotationXZ = 0.0f;
	XMFLOAT3 rotation = {0, 0.2f, -1.0f};
	float targetR = 10.0f;

public:
	//位置
	XMFLOAT3 eye;
	XMFLOAT3 eyeStartPosition;
	XMFLOAT3 eyeGoalPosition;
	
	//どこを注視点とするか
	XMFLOAT3 target;
	XMFLOAT3 targetStartPosition;
	XMFLOAT3 targetGoalPosition;
	//カメラの向き？
	XMFLOAT3 up;
	//ビュー行列の作成
	XMMATRIX matView;
	//ビルボード
	XMMATRIX matBillboard;
	//Y軸ビルボード
	XMMATRIX matBillboardY;
	//Imguiからの操作を受け付ける
	bool isControl = true;
	//反対方向のカメラかどうか
	bool isRCamera = false;
	
	void CameraTargetRot();
	void Init(XMFLOAT3 &eye, XMFLOAT3 &target);
	void Update();
	void EyeAndTargetMove(XMFLOAT3 moveAmount);
	float TargetLength();
	void ShakeUpdate();
	void SetShake(float shakePower);
	void SetTarget(XMFLOAT3 &target);
	XMFLOAT3 GetTargetDirection();
	XMFLOAT3 GetMousePosition();
	XMFLOAT3 EyeToMouseVec();
	XMFLOAT3 MousePosition(float z);
};

class Cameras
{
public:
	static Camera camera;
	static Camera rCamera;
};