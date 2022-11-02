#include "ImguiControl.h"
#include "../imgui/imgui.h"
#pragma warning(push)
#pragma warning(disable:26451)
#include "../imgui/imgui_impl_dx12.h"
#pragma warning(pop)
#include "../imgui/imgui_impl_win32.h"
#include "../Camera/Camera.h"
#include "../Tools/LoadStage/StageObject.h"
#pragma warning(push)
#pragma warning(disable:4505)
#include "../BaseDirectX/DX12operator.h"
#pragma warning(pop)
#include "../Tools/MapLayout/MapLayout.h"
#include <sstream>
#include <fstream>
#include <stdarg.h>
#include "../Player/Seling.h"
#include "../Tools/Particle/ParticleEdit.h"
#include "../Tools/KeyLog/KeyLog.h"
#include "../BihaviorTree/BehaviorTree.h"
//#define _DEBUG

using namespace Editors;
Imgui::Imgui()
{
}
Imgui::~Imgui()
{
}

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

ComPtr<ID3D12DescriptorHeap> Imgui::CreateDescrriptorHeapForImgui()
{
    ComPtr<ID3D12DescriptorHeap> ret;
    D3D12_DESCRIPTOR_HEAP_DESC desc = {};
    desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    desc.NodeMask = 0;
    desc.NumDescriptors = 1;
    desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    BaseDirectX::GetInstance()->dev->CreateDescriptorHeap(&desc, IID_PPV_ARGS(ret.ReleaseAndGetAddressOf()));
    return ret;
}

ComPtr<ID3D12DescriptorHeap> Imgui::GetHeapForImgui()
{
    return imguiDescHeap;
}

Imgui* Imgui::GetInstance()
{
    static Imgui imgui;
    return &imgui;
}

void Imgui::DrawImGui(EachInfo& each)
{
    if (!isActive) return;
    ImGui_ImplDX12_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    //GizmoDraw
    if (isUseGizmo)
    {
        //ウィンドウサイズを画面全体に
        ImGui::SetNextWindowSize(ImVec2(static_cast<float>(window_width), static_cast<float>(window_height)), ImGuiCond_Appearing);
        //座標を左上に
        ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f), ImGuiCond_Appearing);
        //背景色を透明に
        ImGui::PushStyleColor(ImGuiCol_WindowBg, (ImVec4)ImColor(0.0f, 0.0f, 0.0f, 0.0f));
        //
        gizmoWindowFlags |= ImGuiWindowFlags_NoBackground;
        gizmoWindowFlags |= ImGuiWindowFlags_NoCollapse;
        gizmoWindowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
        ImGui::Begin("Gizmo", 0, gizmoWindowFlags);//ウィンドウの名前
        //ImGui::SetWindowSize(ImVec2(400, 500), ImGuiCond::ImGuiCond_FirstUseEver);
        //これ必要
        ImGuizmo::SetDrawlist();
        float windowWidth = (float)ImGui::GetWindowWidth();
        float windowHeight = (float)ImGui::GetWindowHeight();
        ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);
        //実際の処理
        GizmoUpdate();
        ImGui::End();
        //背景色設定を削除
        ImGui::PopStyleColor();
    }
    //MenuBar
    {
        //ウィンドウサイズを画面全体に
        ImGui::SetNextWindowSize(ImVec2(static_cast<float>(window_width), static_cast<float>(0.0f)), ImGuiCond_Appearing);
        //座標を左上に
        ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f), ImGuiCond_Appearing);
        //ウィンドウの背景と外枠を描画しない
        menuBarWindowFlags |= ImGuiWindowFlags_NoBackground;
        //タイトルバーを付けない
        menuBarWindowFlags |= ImGuiWindowFlags_NoTitleBar;
        //メニューバーをつける
        menuBarWindowFlags |= ImGuiWindowFlags_MenuBar;
        //右下のサイズ変更を出来なくする
        menuBarWindowFlags |= ImGuiWindowFlags_NoResize;
        //動かないようにする
        menuBarWindowFlags |= ImGuiWindowFlags_NoMove;
        //最前面に来ないように
        menuBarWindowFlags |= ImGuiWindowFlags_NoFocusOnAppearing;
        //iniを読み込み書き込みをしない
        menuBarWindowFlags |= ImGuiWindowFlags_NoSavedSettings;

        ImGui::Begin("MenuBar", nullptr, menuBarWindowFlags);//ウィンドウの名前
        ImGui::SetWindowSize(ImVec2(static_cast<float>(window_width), 0.0f), ImGuiCond_::ImGuiCond_FirstUseEver);
        //guiのウィンドウ取得
        ImGuiWindow* window = ImGui::GetCurrentWindow();
        //Gizmo触ってもウィンドウを動かないように
        menuBarWindowFlags = ImGui::IsWindowHovered() && ImGui::IsMouseHoveringRect(window->InnerRect.Min, window->InnerRect.Max) ? ImGuiWindowFlags_NoMove : 0;
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::MenuItem("Main Game"))
            {
                sceneNum = 0;
                Imgui::CameraControl = true;
                Cameras::camera.isRCamera = false;
                isParticleSystemWindow = false;
                isUseGizmo = false;
                isBehavior = false;
            }
            if (ImGui::MenuItem("Map Editor"))
            {
                sceneNum = 6;
                Cameras::camera.isRCamera = true;
                Imgui::CameraControl = false;
                Cameras::camera.mouseMoveAmount[0] = 0.0f;
                Cameras::camera.mouseMoveAmount[1] = 0.0f;
                XMFLOAT3 cameraEeyReset = { 0, 0, 20.0f };
                //XMFLOAT3 cameraTargetReset = {0, 0, 0};
                Cameras::camera.Init(cameraEeyReset, XMFLOAT3(0, 0, 0));
                isParticleSystemWindow = false;
                isBehavior = false;
            }
            if (ImGui::MenuItem("Particle System"))
            {
                Imgui::sceneNum = 8;
                isParticleSystemWindow = true;
                isBehavior = false;
            }
            if (ImGui::MenuItem("Key Recording"))
            {
                isKeyRecWindow = true;
            }
            if (ImGui::MenuItem("Inspector"))
            {
                isInspectorWindow = true;

            }
            if (ImGui::MenuItem("Behavior"))
            {
                Imgui::sceneNum = 9;
                Cameras::camera.isRCamera = true;
                Imgui::CameraControl = false;
                Cameras::camera.mouseMoveAmount[0] = 0.0f;
                Cameras::camera.mouseMoveAmount[1] = 0.0f;
                XMFLOAT3 cameraEeyReset = { 0, 0, 20.0f };
                //XMFLOAT3 cameraTargetReset = {0, 0, 0};
                Cameras::camera.Init(cameraEeyReset, XMFLOAT3(0, 0, 0));
                isBehavior = true;
                isInspectorWindow = false;
                isParticleSystemWindow = false;
            }
            ImGui::EndMenuBar();
        }
        ImGui::End();
    }
    //Gizmo用バー
    if (sceneNum == 6)
    {
        //ウィンドウサイズを画面全体に
        ImGui::SetNextWindowSize(ImVec2(static_cast<float>(window_width), static_cast<float>(0.0f)), ImGuiCond_Appearing);
        //座標を左上に
        ImGui::SetNextWindowPos(ImVec2(0.0f, 18.0f), ImGuiCond_Appearing);
        //ウィンドウの背景と外枠を描画しない
        menuBarWindowFlags |= ImGuiWindowFlags_NoBackground;
        //タイトルバーを付けない
        menuBarWindowFlags |= ImGuiWindowFlags_NoTitleBar;
        //メニューバーをつける
        menuBarWindowFlags |= ImGuiWindowFlags_MenuBar;
        //右下のサイズ変更を出来なくする
        menuBarWindowFlags |= ImGuiWindowFlags_NoResize;
        //動かないようにする
        menuBarWindowFlags |= ImGuiWindowFlags_NoMove;
        //最前面に来ないように
        menuBarWindowFlags |= ImGuiWindowFlags_NoFocusOnAppearing;
        //iniを読み込み書き込みをしない
        menuBarWindowFlags |= ImGuiWindowFlags_NoSavedSettings;

        ImGui::Begin("GizmoMenu", nullptr, menuBarWindowFlags);//ウィンドウの名前
        ImGui::SetWindowSize(ImVec2(static_cast<float>(window_width), 0.0f), ImGuiCond_::ImGuiCond_FirstUseEver);
        //guiのウィンドウ取得
        ImGuiWindow* window = ImGui::GetCurrentWindow();
        //Gizmo触ってもウィンドウを動かないように
        menuBarWindowFlags = ImGui::IsWindowHovered() && ImGui::IsMouseHoveringRect(window->InnerRect.Min, window->InnerRect.Max) ? ImGuiWindowFlags_NoMove : 0;
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::MenuItem("SetObjects"))
            {
                isUseGizmo = false;
            }
            if (ImGui::MenuItem("EditObjects"))
            {
                isUseGizmo = true;
                XMFLOAT3 gizmoPos = { gizmoTaget.r[3].m128_f32[0], gizmoTaget.r[3].m128_f32[1], gizmoTaget.r[3].m128_f32[2] };
                MapEditorObjects::ObjectCollision(gizmoPos);
            }
            ImGui::EndMenuBar();
        }
        ImGui::End();
    }
    //インスペクターウィンドウ
    if (isInspectorWindow)
    {
        ImGui::Begin("InfomationAndEdit", &isInspectorWindow, ImGuiWindowFlags_MenuBar);//ウィンドウの名前
        ImGui::SetWindowSize(ImVec2(400, 500), ImGuiCond_::ImGuiCond_FirstUseEver);

        CreateMenuBar();
        EachInfos();

        ImGui::End();
    }
    //キーレコーディング
    if (isKeyRecWindow)
    {
        ImGui::Begin("Recording", &isKeyRecWindow, ImGuiWindowFlags_MenuBar);//ウィンドウの名前
        ImGui::SetWindowSize(ImVec2(150, 100), ImGuiCond_::ImGuiCond_FirstUseEver);

        if (isKeyRec == KeyRec::None)
        {
            if (ImGui::Button("KeyRecording"))
            {
                isKeyRec = KeyRec::Rec;
                KeyLog::RecordingInit();
            }
            if (ImGui::Button("KeyPlayback"))
            {
                isKeyRec = KeyRec::PlayBack;
                KeyLog::PlaybackInit();
            }
        }
        else if (isKeyRec == KeyRec::Rec)
        {
            if (ImGui::Button("KeyStop"))
            {
                isKeyRec = KeyRec::None;
                KeyLog::SaveLog();
            }
        }
        else if (isKeyRec == KeyRec::PlayBack)
        {
            if (ImGui::Button("Stop"))
            {
                isKeyRec = KeyRec::None;
            }
        }
        if (isPosRec == KeyRec::None)
        {
            if (ImGui::Button("PosRecording"))
            {
                isPosRec = KeyRec::Rec;
                PlayerPositionLog::RecordingInit();
            }
            if (ImGui::Button("PosPlayback"))
            {
                string fileName = "posLog";
                PlayerPositionLog::SetFileName(fileName);
                isPosRec = KeyRec::PlayBack;
                PlayerPositionLog::PlaybackInit();
            }
        }
        else if (isPosRec == KeyRec::Rec)
        {
            if (ImGui::Button("PosStop"))
            {
                isPosRec = KeyRec::None;
                string fileName = "posLog";
                PlayerPositionLog::SetFileName(fileName);
                PlayerPositionLog::SaveLog();
            }
        }
        else if (isPosRec == KeyRec::PlayBack)
        {
            if (ImGui::Button("PosStop"))
            {
                isPosRec = KeyRec::None;
            }
        }
        ImGui::End();
    }
    //マップの出力失敗
    if (isFileOutputFalse)
    {
        ImGui::Begin("Error", &isFileOutputFalse, ImGuiWindowFlags_MenuBar);//ウィンドウの名前
        ImGui::SetWindowSize(ImVec2(400, 500), ImGuiCond_::ImGuiCond_FirstUseEver);

        FileFalse();

        ImGui::End();
    }
    //パーティクルの設定ウィンドウ
    if (isParticleSystemWindow)
    {
        ImGui::Begin("ParticleSystem", nullptr, ImGuiWindowFlags_MenuBar);//ウィンドウの名前
        ImGui::SetWindowSize(ImVec2(400, 500), ImGuiCond_::ImGuiCond_FirstUseEver);

        ParticleEdit();

        ImGui::End();
    }
    //ビヘイビアのドロー
    if (isBehavior)
    {
        BehaviorTree::BehavierImGui::DrawImGui();
    }
    ImGui::Render();
    BaseDirectX::GetInstance()->cmdList->SetDescriptorHeaps(1, GetHeapForImgui().GetAddressOf());
    ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), BaseDirectX::GetInstance()->cmdList.Get());
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
    blnResult = ImGui_ImplDX12_Init(BaseDirectX::GetInstance()->dev.Get(), 3, DXGI_FORMAT_R8G8B8A8_UNORM, GetHeapForImgui().Get(), GetHeapForImgui()->GetCPUDescriptorHandleForHeapStart(), GetHeapForImgui()->GetGPUDescriptorHandleForHeapStart());
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
    ImGui::Checkbox("Active", &isParticleEditActive);
    ImGui::Combo("", &particleType, "Normal\0Easeeing\0Lerp\0\0");
    ImGui::InputText(particleFileName, particleBuf, 256);
    if (ImGui::Button("AddFile"))
    {
        //Rewired::RewiredContainer::CreateRewired(buf, false);
        ParticleExport::CreatePartileFile(particleBuf);
    }
    if (ImGui::TreeNode("EmitterLife"))
    {
        ImGui::InputInt("EmitterLife", &emitterLife);
        ImGui::TreePop();
    }
    if (ImGui::Button("Play"))
    {
        emitterPlayTimer = emitterLife;
    }
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

void Imgui::GizmoUpdate()
{
    //guiのウィンドウ取得
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    //Gizmo触ってもウィンドウを動かないように
    gizmoWindowFlags = ImGui::IsWindowHovered() && ImGui::IsMouseHoveringRect(window->InnerRect.Min, window->InnerRect.Max) ? ImGuiWindowFlags_NoMove : 0;
    //平行移動·回転·scaleの変更
    static ImGuizmo::OPERATION mCurrentGizmoOperation(ImGuizmo::TRANSLATE);
    //座標系の決定
    static ImGuizmo::MODE mCurrentGizmoMode(ImGuizmo::WORLD);
    //わからんとりま必要
    static bool useSnap = true;
    static float snap[3] = { 0.1f, 0.1f, 0.1f };
    //わからんとりま必要
    float viewManipulateRight = static_cast<float>(window_width);
    float viewManipulateTop = 0;
    //対象オブジェクトのワールド行列を取得
    isTuchiGizmo = ImGuizmo::Manipulate(Cameras::camera.matView.r->m128_f32, BaseDirectX::GetInstance()->matProjection.r->m128_f32,
        mCurrentGizmoOperation, mCurrentGizmoMode, gizmoTaget.r->m128_f32, NULL, useSnap ? &snap[0] : NULL);
    ImGuizmo::ViewManipulate(Cameras::camera.matView.r->m128_f32, Imgui::CameraR, ImVec2(viewManipulateRight, viewManipulateTop), ImVec2(128, 128), 0x10101010);
    //平行移動をワールド行列から自身の座標系に移行
    if (gizmoTargetObject == nullptr) return;
    gizmoTargetObject->position.m128_f32[0] = gizmoTaget.r[3].m128_f32[0];
    gizmoTargetObject->position.m128_f32[1] = gizmoTaget.r[3].m128_f32[1];
    gizmoTargetObject->position.m128_f32[2] = gizmoTaget.r[3].m128_f32[2];
    gizmoTargetObject->position.m128_f32[3] = gizmoTaget.r[3].m128_f32[3];
}

void Imgui::DrawBehavior()
{
}

void Imgui::SetGizmoObject(EachInfo& each)
{
    gizmoTaget = each.matWorld;
    gizmoTargetObject = &each;
}

void Imgui::FileFalse()
{
    ImGui::Text("File Export Is Failed");

    if (ImGui::Button("Close"))
    {
        isFileOutputFalse = false;
    }
}

void Imgui::EachInfos()
{
    if (tab == ImguiType::Status)
    {
        //ImGui::Image()
        //ImGui::Text("FPS:%.2f", WindowsAPI::rate);
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

void Imgui::Update(Seling& player)
{
    if (isLoadstage)
    {
        string path = "Resource/TextData/Stage/stage" + to_string(LoadStageNum) + ".txt";
        StageObjects::LoadFile(player, path.c_str());
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
