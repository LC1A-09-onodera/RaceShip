#include "ImguiControl.h"
#include "../imgui/imgui.h"
#pragma warning(push)
#pragma warning(disable:26451)
#include "../imgui/imgui_impl_dx12.h"
#pragma warning(pop)
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
#include "../3DModel/Model.h"
#include "../Player/Seling.h"
#include "../Particle/ParticleEdit.h"

//#define _DEBUG

ComPtr<ID3D12DescriptorHeap> Imgui::imguiDescHeap;
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
bool Imgui::isFileOutputFalse = false;
int Imgui::radioMode = 0;
bool Imgui::isMulchthled = true;

list<Rewired::RewiredKeys> Imgui::keyList;
char Imgui::buf[256] = {};
const char* Imgui::fileName = " ";

int Imgui::particleType = ParticleType::Normal;

int Imgui::particleCount = 1;
float Imgui::particleSpeed[3] = { 1.0f ,0 , 0 };
int Imgui::particleSpeedDiff[3] = { 0, 0 ,0 };
float Imgui::particleAcc[3] = { -0.1f ,0 , 0 };
float Imgui::particleStartPosition[3] = { 0 ,0 , 0 };
float Imgui::particleEndPosition[3] = { 0 ,0 , 0 };
float Imgui::particleEaseSpeed[3] = { 0 ,0 , 0 };
float Imgui::particleHalfwayPoint[3] = { 0 ,0 , 0 };
float Imgui::particleStartSize = 1.0f;
float Imgui::particleEndSize = 0;
int Imgui::particleLife = 60;
int Imgui::particleSpornArea[3] = { 1, 1, 1 };
int Imgui::particleSpornSpan;
int Imgui::particleEaseType = ParticleEaseType::InQuad;

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
        Rewired::RewiredContainer::CreateRewired(buf, false);
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
                if (*keyItr == keyListItr->keyCode)
                {
                    string keyFileName = " " + keyListItr->keyName;
                    ImGui::Text(keyFileName.c_str());
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
                if (*keyItr == keyListItr->keyCode)
                {
                    string keyFileName = " " + keyListItr->keyName;
                    ImGui::Text(keyFileName.c_str());
                }
            }
        }
    }
    static int comboNum = 0;
    string sKeyList;
    for (auto keysItr = Rewired::KeyCodeString::mKeyboardKeys.begin(); keysItr != Rewired::KeyCodeString::mKeyboardKeys.end(); ++keysItr)
    {
        sKeyList = sKeyList + keysItr->keyName;
        sKeyList.resize(sKeyList.size() + 1);
    }
    for (auto padKeyItr = Rewired::KeyCodeString::mPadKeys.begin(); padKeyItr != Rewired::KeyCodeString::mPadKeys.end(); ++padKeyItr)
    {
        sKeyList = sKeyList + padKeyItr->keyName;
        sKeyList.resize(sKeyList.size() + 1);
    }
    ImGui::Combo("", &comboNum, sKeyList.c_str());
    if (ImGui::Button("AddKey"))
    {
        Rewired::RewiredContainer::AddKey(itr, comboNum);
    }
    if (ImGui::Button("SubKey"))
    {
        Rewired::RewiredContainer::SubKey(itr, comboNum);
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
    //#ifdef DEBUG
    if (!isActive) return;

    ImGui_ImplDX12_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("InfomationAndEdit", nullptr, ImGuiWindowFlags_MenuBar);//ウィンドウの名前
    ImGui::SetWindowSize(ImVec2(400, 500), ImGuiCond_::ImGuiCond_FirstUseEver);

    CreateMenuBar();
    EachInfo();

    ImGui::End();

    ImGui::Begin("Inspector", nullptr, ImGuiWindowFlags_MenuBar);//ウィンドウの名前
    ImGui::SetWindowSize(ImVec2(400, 500), ImGuiCond_::ImGuiCond_FirstUseEver);

    InspectorView();

    ImGui::End();

    if (isFileOutputFalse)
    {
        ImGui::Begin("Error", nullptr, ImGuiWindowFlags_MenuBar);//ウィンドウの名前
        ImGui::SetWindowSize(ImVec2(400, 500), ImGuiCond_::ImGuiCond_FirstUseEver);

        FileFalse();

        ImGui::End();
    }

    ImGui::Begin("ParticleSystem", nullptr, ImGuiWindowFlags_MenuBar);//ウィンドウの名前
    ImGui::SetWindowSize(ImVec2(400, 500), ImGuiCond_::ImGuiCond_FirstUseEver);

    ParticleEdit();

    ImGui::End();

    ImGui::Render();
    baseDirectX.cmdList->SetDescriptorHeaps(1, GetHeapForImgui().GetAddressOf());
    ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), baseDirectX.cmdList.Get());
    //#endif//DEBUG
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
        if (ImGui::MenuItem("Rewired"))
        {
            tab = ImguiType::Rewired;
        }
        ImGui::EndMenuBar();
    }
}

void Imgui::InspectorView()
{
    DebugUpdate();
}

void Imgui::ParticleEdit()
{
    ImGui::Combo("", &particleType, "Normal\0Easeeing\0Lerp\0\0");
    if (particleType == 1)
    {
        ImGui::InputInt("particleEaseType", &particleEaseType);
        particleEaseType = ShlomonMath::Clamp(particleEaseType, 0, 4);
    }

    ImGui::DragInt("count", &particleCount, 1, 1, 100);
    ImGui::DragInt("span", &particleSpornSpan, 1, 1, 120);
    if (particleType == 0)
    {
        if (ImGui::TreeNode("speed"))
        {
            ImGui::Text("Speed");
            ImGui::DragFloat3("x y z", particleSpeed, 0.1f, -100.0f, 100.0f, "%.2f");
            ImGui::TreePop();
        }
        if (ImGui::TreeNode("speedDiff"))
        {
            ImGui::Text("SpeedDiff");
            ImGui::DragInt3("x y z", particleSpeedDiff, 1, 0, 100, "%.2f");
            ImGui::TreePop();
        }
        if (ImGui::TreeNode("Acc"))
        {

            ImGui::Text("Acc");
            ImGui::DragFloat3("x y z", particleAcc, 0.1f, -100.0f, 100.0f, "%.2f");
            ImGui::TreePop();
        }
    }

    else if (particleType == 1 || particleType == 2)
    {
        if (ImGui::TreeNode("StartPosistion"))
        {
            ImGui::Text("StartPosistion");
            ImGui::DragFloat3("x y z", particleStartPosition, 0.1f, -100.0f, 100.0f, "%.2f");
            ImGui::TreePop();
        }
        if (ImGui::TreeNode("EndPosistion"))
        {
            ImGui::Text("EndPosistion");
            ImGui::DragFloat3("x y z", particleEndPosition, 0.1f, -100.0f, 100.0f, "%.2f");
            ImGui::TreePop();
        }

        ImGui::Text("EaseSpeed");
        ImGui::DragFloat("speed", particleEaseSpeed, 0.01f, 0.0f, 1.0f, "%.2f");
    }

    if (particleType == 2)
    {
        ImGui::Text("HalfwayPoint");
        ImGui::InputFloat3("x y z", particleHalfwayPoint, "%.2f");
    }

    ImGui::Text("StartSize");
    ImGui::DragFloat("StartSize", &particleStartSize, 0.01f, 0.0f, 100.0f, "%.2f");
    ImGui::Text("EndSize");
    ImGui::DragFloat("EndSize", &particleEndSize, 0.01f, 0.0f, 100.0f, "%.2f");

    ImGui::Text("Life");
    ImGui::DragInt("life", &particleLife, 1, 0, 1000);
    if (ImGui::TreeNode("Area"))
    {
        ImGui::Text("Area");
        ImGui::DragInt3("x y z", particleSpornArea, 1, 1, 100);
        ImGui::TreePop();
    }
}
void Imgui::FileFalse()
{
    ImGui::Text("File Export Is Failed");
    if (ImGui::Button("Close"))
    {
        isFileOutputFalse = false;
    }
}

void Imgui::EachInfo()
{
    if (tab == ImguiType::Status)
    {
        //ImGui::Image()
        ImGui::Text("FPS:%.2f", WindowsAPI::rate);
        ImGui::InputInt("WaterFaceType", &useWaterNum, 1, 1);
        useWaterNum = ShlomonMath::Clamp(useWaterNum, 0, 4);
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
            ImGui::Text("mosaic");
        }
        else if (useWaterNum == 3)
        {
            ImGui::Text("mono");
        }
        else if (useWaterNum == 4)
        {
            ImGui::Text("blur");
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
        ImGui::Checkbox("mulchThled", &isMulchthled);
    }
    else if (tab == ImguiType::CameraInfo)
    {
        ImGui::Text("angle[0] : %.2f, angle[1] : %.2f", Camera::angle[0], Camera::angle[1]);
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
    static int inspectorConbo = 1;
    int count = 0;
    auto itr = EachManager::eahcs.begin();
    for (; itr != EachManager::eahcs.end(); ++itr)
    {
        count++;
        ImGui::RadioButton((*itr)->m_eachName.c_str(), &inspectorConbo, count);
    }
    itr = EachManager::eahcs.begin();
    for (int i = 1; i < inspectorConbo; i++)
    {
        itr++;
    }

    XMFLOAT3 posXM = ConvertXMVECTORtoXMFLOAT3((*itr)->position);
    float pos[3] = { posXM.x, posXM.y , posXM.z };
    float rot[3] = { (*itr)->rotation.x, (*itr)->rotation.y ,(*itr)->rotation.z };
    float scale[3] = { (*itr)->scale.x, (*itr)->scale.y, (*itr)->scale.z };
    ImGui::DragFloat3("position", pos, 0.005f);
    ImGui::DragFloat3("rotation", rot, 0.005f);
    ImGui::DragFloat3("scale", scale, 0.005f);
    (*itr)->position = { pos[0], pos[1], pos[2], 1.0f };
    (*itr)->rotation = { rot[0], rot[1], rot[2] };
    (*itr)->scale = { scale[0], scale[1], scale[2] };

    ImGui::Text(" ");
    for (auto modelItr = ModelManager::m_models.begin(); modelItr != ModelManager::m_models.end(); ++modelItr)
    {
        ImGui::Text((*modelItr)->m_modelName.c_str());
    }
}

void Imgui::SetWindowActive(bool f_isActive)
{
    Imgui::isActive = f_isActive;
}

void Imgui::Update(BaseDirectX& baseDirectX, Seling& player)
{
    if (isLoadstage)
    {
        string path = "Resource/TextData/Stage/stage" + to_string(LoadStageNum) + ".txt";
        StageObjects::LoadFile(baseDirectX, player, path.c_str());
        isLoadstage = false;
    }
    if (isExport)
    {
        string path = "Resource/TextData/Stage/stage" + to_string(exportStageNum) + ".txt";
        if (MapEditorObjects::OutputFile(path.c_str()) == false)
        {
            isFileOutputFalse = true;
        }
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
