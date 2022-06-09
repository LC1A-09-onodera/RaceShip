#include "ImguiControl.h"
#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_dx12.h"
#include "../imgui/imgui_impl_win32.h"
#include "../BaseDirectX/BaseDirectX.h"
#include "../Camera/Camera.h"
#include <stdarg.h>

//#include "../BaseDirectX/GameScene.h"

ComPtr<ID3D12DescriptorHeap> Imgui::imguiDescHeap;
ComPtr<ID3D12DescriptorHeap> Imgui::heapForImgui;
int Imgui::effectType;
Imgui::ImguiType Imgui::tab;
Imgui::DebugType Imgui::debugType;
int Imgui::playerCombo = 0;
ImguiEnum Imgui::iEnum(5, "a", "sample2", "sample3", "sample4", "player");
std::string Imgui::test;
std::string Imgui::ipv4Name;
bool Imgui::isActive = true;
float Imgui::volume = 1.0f;

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
    }
    else if (tab == ImguiType::CameraInfo)
    {
        ImGui::Text("eye:%.2f, %.2f, %.2f", Cameras::camera.eye.v.x, Cameras::camera.eye.v.y, Cameras::camera.eye.v.z);
        ImGui::Text("target:%.2f, %.2f, %.2f", Cameras::camera.target.v.x, Cameras::camera.target.v.y, Cameras::camera.target.v.z);
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

const char* ImguiEnum::GetNames()
{
    if (enumCount == 1)
    {
        char str[11];
        auto itr = enums.begin();
        //文字数のカウント
        int charCount = 0;
        int enumCo = 1;
        //全体を文字列化
        for (; itr != enums.end(); ++itr)
        {
            //一個分のenumを元のにくっつける
            for (int i = 0; i < itr->name.length(); i++)
            {
                str[charCount] = itr->name.c_str()[i];
                charCount++;
            }
            //一個分のenumを元のにくっつける
            for (int i = itr->name.length(); i < 9; i++)
            {
                str[charCount] = ' ';
                charCount++;
            }
            enumCo++;
            str[charCount] = '\0';
            charCount++;
        }
        str[charCount] = '\0';
        return str;
    }
    else if (enumCount == 2)
    {
        char str[21];
        auto itr = enums.begin();
        //文字数のカウント
        int charCount = 0;
        int enumCo = 1;
        //全体を文字列化
        for (; itr != enums.end(); ++itr)
        {
            //一個分のenumを元のにくっつける
            for (int i = 0; i < itr->name.length(); i++)
            {
                str[charCount] = itr->name.c_str()[i];
                charCount++;
            }
            //一個分のenumを元のにくっつける
            for (int i = itr->name.length(); i < 9; i++)
            {
                str[charCount] = ' ';
                charCount++;
            }
            enumCo++;
            str[charCount] = '\0';
            charCount++;
        }
        str[charCount] = '\0';
        return str;
    }
    else if (enumCount == 3)
    {
        char str[31];
        auto itr = enums.begin();
        //文字数のカウント
        int charCount = 0;
        int enumCo = 1;
        //全体を文字列化
        for (; itr != enums.end(); ++itr)
        {
            //一個分のenumを元のにくっつける
            for (int i = 0; i < itr->name.length(); i++)
            {
                str[charCount] = itr->name.c_str()[i];
                charCount++;
            }
            //一個分のenumを元のにくっつける
            for (int i = itr->name.length(); i < 9; i++)
            {
                str[charCount] = ' ';
                charCount++;
            }
            enumCo++;
            str[charCount] = '\0';
            charCount++;
        }
        str[charCount] = '\0';
        return str;
    }
    else if (enumCount == 4)
    {
        char str[41];
        auto itr = enums.begin();
        //文字数のカウント
        int charCount = 0;
        int enumCo = 1;
        //全体を文字列化
        for (; itr != enums.end(); ++itr)
        {
            //一個分のenumを元のにくっつける
            for (int i = 0; i < itr->name.length(); i++)
            {
                str[charCount] = itr->name.c_str()[i];
                charCount++;
            }
            //一個分のenumを元のにくっつける
            for (int i = itr->name.length(); i < 9; i++)
            {
                str[charCount] = ' ';
                charCount++;
            }
            enumCo++;
            str[charCount] = '\0';
            charCount++;
        }
        str[charCount] = '\0';
        return str;
    }
    else if (enumCount == 5)
    {
        char str[51];
        auto itr = enums.begin();
        //文字数のカウント
        int charCount = 0;
        int enumCo = 1;
        //全体を文字列化
        for (; itr != enums.end(); ++itr)
        {
            //一個分のenumを元のにくっつける
            for (int i = 0; i < itr->name.length(); i++)
            {
                str[charCount] = itr->name.c_str()[i];
                charCount++;
            }
            //一個分のenumを元のにくっつける
            for (int i = itr->name.length(); i < 9; i++)
            {
                str[charCount] = ' ';
                charCount++;
            }
            enumCo++;
            str[charCount] = '\0';
            charCount++;
        }
        str[charCount] = '\0';
        return str;
    }
    else if (enumCount == 6)
    {
        char str[61];
        auto itr = enums.begin();
        //文字数のカウント
        int charCount = 0;
        int enumCo = 1;
        //全体を文字列化
        for (; itr != enums.end(); ++itr)
        {
            //一個分のenumを元のにくっつける
            for (int i = 0; i < itr->name.length(); i++)
            {
                str[charCount] = itr->name.c_str()[i];
                charCount++;
            }
            //一個分のenumを元のにくっつける
            for (int i = itr->name.length(); i < 9; i++)
            {
                str[charCount] = ' ';
                charCount++;
            }
            enumCo++;
            str[charCount] = '\0';
            charCount++;
        }
        str[charCount] = '\0';
        return str;
    }
    else if (enumCount == 7)
    {
        char str[70];
        auto itr = enums.begin();
        //文字数のカウント
        int charCount = 0;
        int enumCo = 1;
        //全体を文字列化
        for (; itr != enums.end(); ++itr)
        {
            //一個分のenumを元のにくっつける
            for (int i = 0; i < itr->name.length(); i++)
            {
                str[charCount] = itr->name.c_str()[i];
                charCount++;
            }
            //一個分のenumを元のにくっつける
            for (int i = itr->name.length(); i < 10 * enumCo; i++)
            {
                str[charCount] = ' ';
                charCount++;
            }
            enumCo++;
            str[charCount] = '\0';
            charCount++;
        }
        str[charCount] = '\0';
        return str;
    }
    else if (enumCount == 8)
    {
        char str[80];
        auto itr = enums.begin();
        //文字数のカウント
        int charCount = 0;
        int enumCo = 1;
        //全体を文字列化
        for (; itr != enums.end(); ++itr)
        {
            //一個分のenumを元のにくっつける
            for (int i = 0; i < itr->name.length(); i++)
            {
                str[charCount] = itr->name.c_str()[i];
                charCount++;
            }
            //一個分のenumを元のにくっつける
            for (int i = itr->name.length(); i < 10 * enumCo; i++)
            {
                str[charCount] = ' ';
                charCount++;
            }
            enumCo++;
            str[charCount] = '\0';
            charCount++;
        }
        str[charCount] = '\0';
        return str;
    }
    else if (enumCount == 9)
    {
        char str[90];
        auto itr = enums.begin();
        //文字数のカウント
        int charCount = 0;
        int enumCo = 1;
        //全体を文字列化
        for (; itr != enums.end(); ++itr)
        {
            //一個分のenumを元のにくっつける
            for (int i = 0; i < itr->name.length(); i++)
            {
                str[charCount] = itr->name.c_str()[i];
                charCount++;
            }
            //一個分のenumを元のにくっつける
            for (int i = itr->name.length(); i < 10 * enumCo; i++)
            {
                str[charCount] = ' ';
                charCount++;
            }
            enumCo++;
            str[charCount] = '\0';
            charCount++;
        }
        str[charCount] = '\0';
        return str;
    }
    else if (enumCount == 10)
    {
        char str[100];
        auto itr = enums.begin();
        //文字数のカウント
        int charCount = 0;
        int enumCo = 1;
        //全体を文字列化
        for (; itr != enums.end(); ++itr)
        {
            //一個分のenumを元のにくっつける
            for (int i = 0; i < itr->name.length(); i++)
            {
                str[charCount] = itr->name.c_str()[i];
                charCount++;
            }
            //一個分のenumを元のにくっつける
            for (int i = itr->name.length(); i < 10 * enumCo; i++)
            {
                str[charCount] = ' ';
                charCount++;
            }
            enumCo++;
            str[charCount] = '\0';
            charCount++;
        }
        str[charCount] = '\0';
        return str;
    }
}
