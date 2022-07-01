#pragma once
#pragma warning(push)
#pragma warning(disable:6001)
#include <d3dx12.h>
#pragma warning(pop)
#include <DirectXMath.h>
#include "../BaseDirectX/BaseDirectX.h"
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
    static ComPtr<ID3D12DescriptorHeap> imguiDescHeap;//imguiï€éùópÉÅÉìÉo
    static ComPtr<ID3D12DescriptorHeap> heapForImgui;
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
    static void EachInfo(BaseDirectX& baseDirectX);
    static void DebugUpdate();

    static bool isActive;
    static void SetWindowActive(bool isActive);

    static int effectType;
    enum class ImguiType
    {
        Status,
        CameraInfo,
        Debug,
        PostEffect,

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

    static int playerCombo;
    static ImguiEnum iEnum;

    static std::string test;
    static std::string ipv4Name;
    static void GetTest(const char *st) {test = st;}
    static void GetIp(const char *st) {ipv4Name = st;}

    static float volume;


    static float CameraR;
    static float CameraRotation;
    static float CameraHigh;
    static bool CameraControl;

    static int useWaterNum;
};
