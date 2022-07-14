#include "ImguiControl.h"
#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_dx12.h"
#include "../imgui/imgui_impl_win32.h"
#include "../Camera/Camera.h"
#include "../LoadStage/StageObject.h"
#pragma warning(push)
#pragma warning(disable:4505)
#include "../BaseDirectX/DX12operator.h"
#pragma warning(pop)
#include "../MapLayout/MapLayout.h"
#include <sstream>
#include <fstream>
#include <stdarg.h>

ComPtr<ID3D12DescriptorHeap> Imgui::imguiDescHeap;
ComPtr<ID3D12DescriptorHeap> Imgui::heapForImgui;
int Imgui::effectType = -1;
Imgui::ImguiType Imgui::tab;
Imgui::DebugType Imgui::debugType;
int Imgui::sceneNum = 0;
ImguiEnum Imgui::iEnum(5, "a", "sample2", "sample3", "sample4", "player");
bool Imgui::isActive = true;
float Imgui::volume = 1.0f;
float Imgui::CameraR = 25.0f;
float Imgui::CameraRotation = 270.0f;
float Imgui::CameraHigh = 0.4f;
bool Imgui::CameraControl = true;
int Imgui::useWaterNum = 0;

int Imgui::mouseWheel;
bool Imgui::touchedImgui = false;

int Imgui::oldSceneNum;
bool Imgui::isSceneChange = false;

int Imgui::exportStageNum = 1;
int Imgui::LoadStageNum = 1;
bool Imgui::isExport = false;
bool Imgui::isLoadstage = false;
bool Imgui::isDeleteObjects = false;

int Imgui::radioMode = 0;

list<Rewired::RewiredKeys> Imgui::keyList;
char Imgui::buf[256] = {};
const char* Imgui::fileName = " ";
void Imgui::RewiredUpdate()
{
    //ラジオボタン用
    if (ImGui::Button("ReloadRewireds"))
    {
        Rewired::RewiredContainer::ReloadRewired();
    }
    ImGui::Text("Actions");
    int count = 0;
    for (auto itr = Rewired::RewiredContainer::rewiredsC.begin(); itr != Rewired::RewiredContainer::rewiredsC.end(); ++itr)
    {
        //ファイル名の記載
        ImGui::RadioButton(itr->GetFileName().c_str(), &radioMode, count);
        count++;
    }
    ImGui::InputText(fileName, buf, 256);
    if (ImGui::Button("AddFile"))
    {
        Rewired::RewiredContainer::CreateRewired(buf);
    }
    ShowRewiredElement();
}

void Imgui::ShowRewiredElement()
{
    auto itr = Rewired::RewiredContainer::rewiredsC.begin();
    for (int i = 0; i < radioMode; i++)
    {
        itr++;
    }
    //キーボードのの設定されている値
    if (itr->keys.size() > 0)
    {
        for (auto keyListItr = Rewired::KeyCodeString::mKeyboardKeys.begin(); keyListItr != Rewired::KeyCodeString::mKeyboardKeys.end(); ++keyListItr)
        {
            for (auto keyItr = itr->keys.begin(); keyItr != itr->keys.end(); ++keyItr)
            {
                if (*keyItr == std::get<1>(*keyListItr))
                {
                    string fileName = " " + std::get<0>(*keyListItr);
                    ImGui::Text(fileName.c_str());
                }
            }
        }
    }
    //XBoxPadの設定されている値
    if (itr->padKeys.size() > 0)
    {
        for (auto keyListItr = Rewired::KeyCodeString::mPadKeys.begin(); keyListItr != Rewired::KeyCodeString::mPadKeys.end(); ++keyListItr)
        {
            for (auto keyItr = itr->padKeys.begin(); keyItr != itr->padKeys.end(); ++keyItr)
            {
                if (*keyItr == std::get<1>(*keyListItr))
                {
                    string fileName = " " + std::get<0>(*keyListItr);
                    ImGui::Text(fileName.c_str());
                }
            }
        }
    }
    static int comboNum = 0;
    string keyList;
    for (auto keysItr = Rewired::KeyCodeString::mKeyboardKeys.begin(); keysItr != Rewired::KeyCodeString::mKeyboardKeys.end(); ++keysItr)
    {
        keyList = keyList + std::get<0>(*keysItr);
        keyList.resize(keyList.size() + 1);
    }
    for (auto padKeyItr = Rewired::KeyCodeString::mPadKeys.begin(); padKeyItr != Rewired::KeyCodeString::mPadKeys.end(); ++padKeyItr)
    {
        keyList = keyList + std::get<0>(*padKeyItr);
        keyList.resize(keyList.size() + 1);
    }
    ImGui::Combo("", &comboNum, keyList.c_str());
    if (ImGui::Button("AddKey"))
    {
        if (static_cast<int>(KeyCode::KeyCodeMax) - 1 > comboNum)
        {
            auto keyStringItr = Rewired::KeyCodeString::mKeyboardKeys.begin();
            if (Rewired::KeyCodeString::mKeyboardKeys.size() > 0)
            {
                for (int i = 0; i < comboNum; i++)
                {
                    keyStringItr++;
                }
            }
            bool isA = false;
            //既に使っているキーを再登録しない
            for (auto keyContainarItr = Rewired::RewiredContainer::rewiredsC.begin(); keyContainarItr != Rewired::RewiredContainer::rewiredsC.end(); ++keyContainarItr)
            {
                for (auto kItr = keyContainarItr->keys.begin(); kItr != keyContainarItr->keys.end(); ++kItr)
                {
                    if (*kItr == std::get<1>(*keyStringItr))
                    {
                        isA = true;
                    }
                }
            }
            if (!isA) itr->AddKey(std::get<1>(*keyStringItr));
        }
        else
        {
            auto padStringItr = Rewired::KeyCodeString::mPadKeys.begin();
            if (Rewired::KeyCodeString::mPadKeys.size() > 0)
            {
                for (int j = 0; j < comboNum - ((static_cast<int>(KeyCode::KeyCodeMax)) - 1); j++)
                {
                    padStringItr++;
                }
            }
            //既に使っているキーを再登録しない
            bool isA = false;
            for (auto keyContainarItr = Rewired::RewiredContainer::rewiredsC.begin(); keyContainarItr != Rewired::RewiredContainer::rewiredsC.end(); ++keyContainarItr)
            {
                for (auto kItr = keyContainarItr->padKeys.begin(); kItr != keyContainarItr->padKeys.end(); ++kItr)
                {
                    if (*kItr == std::get<1>(*padStringItr))
                    {
                        isA = true;
                    }
                }
            }
            if (!isA) itr->AddKey(std::get<1>(*padStringItr));
        }
    }
    if (ImGui::Button("SubKey"))
    {
        if (static_cast<int>(KeyCode::KeyCodeMax) - 1 > comboNum)
        {
            auto keyStringItr = Rewired::KeyCodeString::mKeyboardKeys.begin();
            if (Rewired::KeyCodeString::mKeyboardKeys.size() > 0)
            {
                for (int i = 0; i < comboNum; i++)
                {
                    keyStringItr++;
                }
            }
            itr->Subkey(std::get<1>(*keyStringItr));
        }
        else
        {
            auto padStringItr = Rewired::KeyCodeString::mPadKeys.begin();
            if (Rewired::KeyCodeString::mPadKeys.size() > 0)
            {
                for (int j = static_cast<int>(KeyCode::KeyCodeMax) - 1; j < comboNum - ((static_cast<int>(KeyCode::KeyCodeMax)) - 1); j++)
                {
                    padStringItr++;
                }
            }
            itr->SubKey(std::get<1>(*padStringItr));
        }
    }
    if (ImGui::Button("Save"))
    {
        itr->SaveKey();
    }
}

ComPtr<ID3D12DescriptorHeap> Imgui::CreateDescrriptorHeapForImgui(BaseDirectX& baseDirectX)
{
    ComPtr<ID3D12DescriptorHeap> ret;
    D3D12_DESCRIPTOR_HEAP_DESC desc = {};
    desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    desc.NodeMask = 0;
    desc.NumDescriptors = 1;
    desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    baseDirectX.dev->CreateDescriptorHeap(&desc, IID_PPV_ARGS(ret.ReleaseAndGetAddressOf()));
    return ret;
}

ComPtr<ID3D12DescriptorHeap> Imgui::GetHeapForImgui()
{
    return imguiDescHeap;
}

void Imgui::DrawImGui(BaseDirectX& baseDirectX)
{
    if (!isActive) return;
    ImGui_ImplDX12_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
    ImGui::Begin("InfomationAndEdit", nullptr, ImGuiWindowFlags_MenuBar);//ウィンドウの名前
    ImGui::SetWindowSize(ImVec2(400, 500), ImGuiCond_::ImGuiCond_FirstUseEver);
    CreateMenuBar();
    EachInfo(baseDirectX);
    ImGui::End();
    ImGui::Render();
    baseDirectX.cmdList->SetDescriptorHeaps(1, GetHeapForImgui().GetAddressOf());
    ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), baseDirectX.cmdList.Get());
}

void Imgui::Init(BaseDirectX& baseDirectX)
{
    //imgui
    imguiDescHeap = CreateDescrriptorHeapForImgui(baseDirectX);
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
    blnResult = ImGui_ImplDX12_Init(baseDirectX.dev.Get(), 3, DXGI_FORMAT_R8G8B8A8_UNORM, GetHeapForImgui().Get(), GetHeapForImgui()->GetCPUDescriptorHandleForHeapStart(), GetHeapForImgui()->GetGPUDescriptorHandleForHeapStart());
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
        /*if (ImGui::MenuItem("PostEffect"))
        {
            tab = ImguiType::PostEffect;
        }*/
        if (ImGui::MenuItem("Rewired"))
        {
            tab = ImguiType::Rewired;
        }
        ImGui::EndMenuBar();
    }
}

void Imgui::EachInfo(BaseDirectX& baseDirectX)
{
    if (tab == ImguiType::Status)
    {
        ImGui::Text("FPS:%.2f", WindowsAPI::rate);
        ImGui::InputInt("WaterFaceType", &useWaterNum, 1, 1);
        useWaterNum = ShlomonMath::Clamp(useWaterNum, 0, 2);
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

        ImGui::InputInt("LoadStageNum", &LoadStageNum, 1, 1);
        if (ImGui::Button("LoadStage"))
        {
            isLoadstage = true;
        }
        ImGui::InputInt("ExportNum", &exportStageNum, 1, 1);
        if (ImGui::Button("OutputFile"))
        {
            isExport = true;
            touchedImgui = true;
        }
        if (ImGui::Button("DeleteObject"))
        {
            isDeleteObjects = true;
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
        CameraR = ShlomonMath::Clamp(CameraR, 3.0f, 35.0f);
        ImGui::InputFloat("CameraRotation:", &CameraRotation, 1.0f, 10.0f);
        //CameraRotation = ShlomonMath::Clamp(CameraRotation, 200.0f, 340.0f);
        ImGui::InputFloat("CameraHigh:", &CameraHigh, 0.01f, 0.02f);
        CameraHigh = ShlomonMath::Clamp(CameraHigh, 0.0f, 1.0f);
    }
    else if (tab == ImguiType::Debug)
    {
        //DebugUpdate();
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
    else if (tab == ImguiType::Rewired)
    {
        RewiredUpdate();
    }
}
void Imgui::DebugUpdate()
{
    if (debugType == DebugType::Player)
    {
        isSceneChange = false;
        oldSceneNum = sceneNum;
        ImGui::Combo("sceneNum", &sceneNum, "TITLE\0SELECT\0GAME\0END\0RESULT\0OP\0MAPEDIT\0\0");
        if (oldSceneNum != sceneNum)
        {
            isSceneChange = true;
        }
    }
    else if (debugType == DebugType::Water)
    {

    }
}

void Imgui::SetWindowActive(bool isActive)
{
    Imgui::isActive = isActive;
}

void Imgui::Update(BaseDirectX& baseDirectX)
{
    if (isLoadstage)
    {
        string path = "Resource/TextData/Stage/stage" + to_string(LoadStageNum) + ".txt";
        StageObjects::LoadFile(baseDirectX, path.c_str());
        isLoadstage = false;
    }
    if (isExport)
    {
        string path = "Resource/TextData/Stage/stage" + to_string(exportStageNum) + ".txt";
        MapEditorObjects::OutputFile(path.c_str());
        isExport = false;
    }
    if (isDeleteObjects)
    {
        MapEditorObjects::DeleteObjects();
        isDeleteObjects = false;
    }
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
