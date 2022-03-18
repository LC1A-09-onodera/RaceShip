#pragma once
#include <d3dx12.h>
#include <DirectXMath.h>
using namespace Microsoft::WRL;
using namespace DirectX;

class Imgui
{
private:
    static ComPtr<ID3D12DescriptorHeap> imguiDescHeap;//imguiï€éùópÉÅÉìÉo
    static ComPtr<ID3D12DescriptorHeap> heapForImgui;
public:
    
    static ComPtr<ID3D12DescriptorHeap> CreateDescrriptorHeapForImgui();
    static ComPtr<ID3D12DescriptorHeap> GetHeapForImgui();
    static void DrawImGui();
    static void Init();
    static void GetAndDrawInfo(const int &intOriginal, int& imguiInfo);
    static void GetAndDrawInfo(const float &floatOriginal, float& imguiInfo);
    static void GetAndDrawInfo(const XMFLOAT3 &xmfloat3Original, XMFLOAT3& imguiInfo);
    static void ChangeInfo(int &intOriginal, int& imguiInfo);
    static void ChangeInfo(float &floatOriginal, float& imguiInfo);
    static void ChangeInfo(XMFLOAT3 &xmfloat3Original, XMFLOAT3& imguiInfo);

    static bool spotLight1;
    static float lightColor[3];

    static float dethLine;
};
