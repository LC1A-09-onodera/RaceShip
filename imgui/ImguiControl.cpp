#include "ImguiControl.h"
#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_dx12.h"
#include "../imgui/imgui_impl_win32.h"
#include "../BaseDirectX/BaseDirectX.h"
#include "../Camera/Camera.h"
#include "../LoadStage/StageObject.h"
#include <stdarg.h>

ComPtr<ID3D12DescriptorHeap> Imgui::imguiDescHeap;
ComPtr<ID3D12DescriptorHeap> Imgui::heapForImgui;
int Imgui::effectType = -1;
Imgui::ImguiType Imgui::tab;
Imgui::DebugType Imgui::debugType;
int Imgui::playerCombo = 0;
ImguiEnum Imgui::iEnum(5, "a", "sample2", "sample3", "sample4", "player");
std::string Imgui::test;
std::string Imgui::ipv4Name;
bool Imgui::isActive = true;
float Imgui::volume = 1.0f;
float Imgui::CameraR = 25.0f;
float Imgui::CameraRotation = 270.0f;
float Imgui::CameraHigh = 0.2f;
bool Imgui::CameraControl = true;
int Imgui::useWaterNum = 0;
ComPtr<ID3D12DescriptorHeap> Imgui::CreateDescrriptorHeapForImgui()
{
    ComPtr<ID3D12DescriptorHeap> ret;
    D3D12_DESCRIPTOR_HEAP_DESC desc = {};
    desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    desc.NodeMask = 0;
    desc.NumDescriptors = 1;
    desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    BaseDirectX::dev->CreateDescriptorHeap(&desc, IID_PPV_ARGS(ret.ReleaseAndGetAddressOf()));
    return ret;
}

ComPtr<ID3D12DescriptorHeap> Imgui::GetHeapForImgui()
{
    return imguiDescHeap;
}

void Imgui::DrawImGui()
{
    if (!isActive) return;
    ImGui_ImplDX12_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
    ImGui::Begin("InfomationAndEdit", nullptr, ImGuiWindowFlags_MenuBar);//ウィンドウの名前
    ImGui::SetWindowSize(ImVec2(400, 500), ImGuiCond_::ImGuiCond_FirstUseEver);
    CreateMenuBar();
    EachInfo();
    ImGui::End();
    ImGui::Render();
    BaseDirectX::cmdList->SetDescriptorHeaps(1, GetHeapForImgui().GetAddressOf());
    ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), BaseDirectX::cmdList.Get());
}

void Imgui::Init()
{
    //imgui
    imguiDescHeap = CreateDescrriptorHeapForImgui();
    if (imguiDescHeap == nullptr)
    {
        return;
    }
    if (ImGui::CreateContext() == nullptr)
    {
        assert(0);
        return;
    }
    //[ImGui::]
    bool blnResult = ImGui_ImplWin32_Init(WindowsAPI::hwnd);
    if (!blnResult)
    {
        assert(0);
        return;
    }
    blnResult = ImGui_ImplDX12_Init(BaseDirectX::dev.Get(), 3, DXGI_FORMAT_R8G8B8A8_UNORM, GetHeapForImgui().Get(), GetHeapForImgui()->GetCPUDescriptorHandleForHeapStart(), GetHeapForImgui()->GetGPUDescriptorHandleForHeapStart());
}

void Imgui::ChangeInfo(int& intOriginal, int& imguiInfo)
{
    intOriginal = imguiInfo;
    return;
}

void Imgui::ChangeInfo(float& floatOriginal, float& imguiInfo)
{
    floatOriginal = imguiInfo;
    return;
}

void Imgui::ChangeInfo(XMFLOAT3& xmfloat3Original, XMFLOAT3& imguiInfo)
{
    xmfloat3Original = imguiInfo;
    return;
}

void Imgui::GetAndDrawInfo(const int& intOriginal, int& imguiInfo)
{
    imguiInfo = intOriginal;
    return;
}

void Imgui::GetAndDrawInfo(const float& floatOriginal, float& imguiInfo)
{
    imguiInfo = floatOriginal;
    return;
}

void Imgui::GetAndDrawInfo(const XMFLOAT3& xmfloat3Original, XMFLOAT3& imguiInfo)
{
    imguiInfo = xmfloat3Original;
    return;
}

void Imgui::CreateMenuBar()
{
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::MenuItem("Status"))
        {
            tab = ImguiType::Status;
        }
        if (ImGui::MenuItem("Camera"))
        {
            tab = ImguiType::CameraInfo;
        }
        if (ImGui::MenuItem("Debug"))
        {
            tab = ImguiType::Debug;
        }
        if (ImGui::MenuItem("PostEffect"))
        {
            tab = ImguiType::PostEffect;
        }
        ImGui::EndMenuBar();
    }
}

void Imgui::EachInfo()
{
    if (tab == ImguiType::Status)
    {
        ImGui::Text("FPS:%.2f", WindowsAPI::rate);
        ImGui::Text(test.c_str());
        ImGui::Text(ipv4Name.c_str());
        ImGui::SliderFloat("volume", &volume, 0, 100.0f);
        ImGui::InputInt("WaterFaceType", &useWaterNum, 1, 1);
        if (useWaterNum == 0)
        {
            ImGui::Text("water");
        }
        else if (useWaterNum == 1)
        {
            ImGui::Text("Normal");
        }
        else if (useWaterNum == 2)
        {
            ImGui::Text("notiong");
        }
        if (ImGui::Button("LoadStage"))
        {
            StageObjects::LoadFile();
        }
    }
    else if (tab == ImguiType::CameraInfo)
    {
        ImGui::Text("eye:%.2f, %.2f, %.2f", Cameras::camera.eye.x, Cameras::camera.eye.y, Cameras::camera.eye.z);
        ImGui::Text("target:%.2f, %.2f, %.2f", Cameras::camera.target.x, Cameras::camera.target.y, Cameras::camera.target.z);
        ImGui::Text("Reye:%.2f, %.2f, %.2f", Cameras::rCamera.eye.x, Cameras::rCamera.eye.y, Cameras::rCamera.eye.z);
        ImGui::Text("Rtarget:%.2f, %.2f, %.2f", Cameras::rCamera.target.x, Cameras::rCamera.target.y, Cameras::rCamera.target.z);
        ImGui::Checkbox("ImGuiCameraControl", &CameraControl);
        ImGui::InputFloat("CameraLength:", &CameraR, 1.0f, 10.0f);
        ImGui::InputFloat("CameraRotation:", &CameraRotation, 1.0f, 10.0f);
        ImGui::InputFloat("CameraHigh:", &CameraHigh, 0.01f, 0.02f);
    }
    else if (tab == ImguiType::Debug)
    {
        DebugUpdate();
    }
    else if (tab == ImguiType::PostEffect)
    {
        ImGui::InputInt("EffectType", &effectType, 1, 10);
        if (effectType == 0)
        {
            ImGui::Text("Normal");
        }
        else if (effectType == 1)
        {
            ImGui::Text("Water");
        }
        else if (effectType == 2)
        {
            ImGui::Text("Mosaic");
        }
        else if (effectType == 3)
        {
            ImGui::Text("Blur");
        }
        else if (effectType >= 4)
        {
            ImGui::Text("NoSetting");
        }
    }
}
void Imgui::DebugUpdate()
{
    if (debugType == DebugType::Player)
    {
        ImGui::Combo("playerCombo", &playerCombo, "aaaa\0bbbb\0cccc\0dddd\0eeee\0\0");
        //ImGui::Combo("playerCombo", &playerCombo, iEnum.GetNames());
    }
    else if (debugType == DebugType::Water)
    {

    }
}

void Imgui::SetWindowActive(bool isActive)
{
    Imgui::isActive = isActive;
};

ImguiEnum::ImguiEnum(int count, ...)
{
    va_list ap;
    va_start(ap, count);
    if (count <= 0)
    {
        va_end(ap);
        return;
    }
    for (int i = 0; i < count; i++)
    {
        std::string str = va_arg(ap, const char*);
        nameString += str;
        ImguiEnumElement elemnt = { i, str };
        enums.push_back(elemnt);
    }
    enumCount = count;
    va_end(ap);
}

ImguiEnum::~ImguiEnum()
{
    enums.clear();
}
