#pragma once
#pragma warning(push)
#pragma warning(disable:6001)
#include <d3dx12.h>
#pragma warning(pop)
#include <DirectXMath.h>
#include "../BaseDirectX/BaseDirectX.h"
#include "../Rewired/Rewired.h"
using namespace Microsoft::WRL;
using namespace DirectX;

class ImguiEnumElement
{
public:
    int number;
    std::string name;
};

class ImguiEnum
{
public:
    ImguiEnum(int count, ...);
    ~ImguiEnum();
    std::list<ImguiEnumElement> enums;
    std::string nameString;
    int enumCount = 0;
};

class Imgui
{
private:
    static ComPtr<ID3D12DescriptorHeap> imguiDescHeap;//imgui保持用メンバ
    static ComPtr<ID3D12DescriptorHeap> heapForImgui;
    //ラジオボタン用
    static int radioMode;
    static void RewiredUpdate();
    static void ShowRewiredElement();
public:
    static ComPtr<ID3D12DescriptorHeap> CreateDescrriptorHeapForImgui(BaseDirectX& baseDirectX);
    static ComPtr<ID3D12DescriptorHeap> GetHeapForImgui();
    static void DrawImGui(BaseDirectX& baseDirectX);
    static void Init(BaseDirectX& baseDirectX);
    static void GetAndDrawInfo(const int &intOriginal, int& imguiInfo);
    static void GetAndDrawInfo(const float &floatOriginal, float& imguiInfo);
    static void GetAndDrawInfo(const XMFLOAT3 &xmfloat3Original, XMFLOAT3& imguiInfo);
    static void ChangeInfo(int &intOriginal, int& imguiInfo);
    static void ChangeInfo(float &floatOriginal, float& imguiInfo);
    static void ChangeInfo(XMFLOAT3 &xmfloat3Original, XMFLOAT3& imguiInfo);

    static void CreateMenuBar();
    static void EachInfo();
    static void DebugUpdate();

    static bool isActive;
    static void SetWindowActive(bool f_isActive);

    static void Update(BaseDirectX &baseDirectX);

    static int effectType;
    enum class ImguiType
    {
        Status,
        CameraInfo,
        Debug,
        PostEffect,
        Rewired,

        ImguiTypeEnd,
    };
    static ImguiType tab;

    enum class DebugType
    {
        Player,
        Water,

        DebugTypeEnd,
    };
    static DebugType debugType;

    static int sceneNum;
    static int oldSceneNum;
    static bool isSceneChange;
    static ImguiEnum iEnum;

    static float volume;


    static float CameraR;
    static float CameraRotation;
    static float CameraHigh;
    static bool CameraControl;

    static int useWaterNum;

    static int mouseWheel;

    static bool touchedImgui;

    static int exportStageNum;
    static int LoadStageNum;

    static bool isExport;
    static bool isLoadstage;

    static bool isDeleteObjects;

    static list<Rewired::RewiredKeys> keyList;
    static const char *fileName;
    static char buf[256];
};
