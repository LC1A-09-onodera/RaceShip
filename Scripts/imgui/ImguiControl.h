#pragma once
#pragma warning(push)
#pragma warning(disable:6001)
#include <d3dx12.h>
#pragma warning(pop)
#include <DirectXMath.h>
#include "../BaseDirectX/BaseDirectX.h"
#include "../Rewired/Rewired.h"
#include "../Player/Seling.h"
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
    static bool isActive;
    //ラジオボタン用
    static int radioMode;
    static void RewiredUpdate();
    static void ShowRewiredElement();
    static void CreateMenuBar();
    static void InspectorView();
    static void FileFalse();
    static void EachInfo();
    static ComPtr<ID3D12DescriptorHeap> CreateDescrriptorHeapForImgui(BaseDirectX& baseDirectX);
    static ComPtr<ID3D12DescriptorHeap> GetHeapForImgui();
public:
    
    static void DrawImGui(BaseDirectX& baseDirectX);
    static void Init(BaseDirectX& baseDirectX);
    static void DebugUpdate();
    static void SetWindowActive(bool f_isActive);
    static void Update(BaseDirectX &baseDirectX, Seling& player);
    static bool isMulchthled;
    static int effectType;
    static bool isFileOutputFalse;
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
