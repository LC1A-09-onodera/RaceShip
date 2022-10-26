#pragma once
#pragma warning(push)
#pragma warning(disable:6001)
#include <d3dx12.h>
#pragma warning(pop)
#include <DirectXMath.h>
#include "../BaseDirectX/BaseDirectX.h"
#include "../Tools/Rewired/Rewired.h"
#include "../Player/Seling.h"
#include "ImGuizmo.h"
#include ".././3DModel/Model.h"

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
    Imgui();
    ~Imgui();
    ComPtr<ID3D12DescriptorHeap> imguiDescHeap;//imguiï€éùópÉÅÉìÉo
    bool isActive = true;
    void RewiredUpdate();
    void ShowRewiredElement();
    void CreateMenuBar();
    void InspectorView();
    void ParticleEdit();
    void FileFalse();
    void EachInfos();
    void GizmoUpdate();

    void DrawBehavior();

    ComPtr<ID3D12DescriptorHeap> CreateDescrriptorHeapForImgui();
    ComPtr<ID3D12DescriptorHeap> GetHeapForImgui();

    ImGuiWindowFlags gizmoWindowFlags = 0;
    ImGuiWindowFlags menuBarWindowFlags = 0;
    bool isBehavior = false;
public:
    static Imgui *GetInstance();
    Imgui(const Imgui& obj) = delete;
    Imgui& operator=(const Imgui& obj) = delete;
    void DrawImGui(EachInfo &each);
    void Init();
    void DebugUpdate();
    void SetWindowActive(bool f_isActive);
    void Update(Seling& player);
    void SetGizmoObject(EachInfo &each);
    enum class ImguiType
    {
        Status,
        CameraInfo,
        Debug,
        PostEffect,
        Rewired,

        ImguiTypeEnd,
    };
    enum class DebugType
    {
        Player,
        Water,

        DebugTypeEnd,
    };
    enum ParticleType
    {
        Normal,
        Easeeing,
        Lerp,
    };
    enum ParticleEaseType
    {
        InQuad,
        OutQuad,
        InOutQuad,
        InBack,
        OutBack,
    };
    enum KeyRec
    {
        None,
        Rec,
        PlayBack,
    };
    int effectType = -1;
    ImguiType tab;
    DebugType debugType;
    int sceneNum = 0;
    float volume = 1.0f;
    float CameraR = 25.0f;
    float CameraRotation = 270.0f;
    float CameraHigh = 0.4f;
    bool CameraControl = true;
    int useWaterNum = 0;

    int mouseWheel;
    bool touchedImgui = false;

    int oldSceneNum;
    bool isSceneChange = false;

    int exportStageNum = 1;
    int LoadStageNum = 1;
    bool isExport = false;
    bool isLoadstage = false;
    bool isDeleteObjects = false;
    bool isFileOutputFalse = false;
    int radioMode = 0;
    bool isMulchthled = false;

    list<Rewired::RewiredKeys> keyList;
    char buf[256] = {};
    const char* fileName = " ";

    const char* particleFileName = " ";
    char particleBuf[256] = {};

    int particleType = ParticleType::Normal;

    int particleCount = 1;
    float particleSpeed[3] = { 0.0f ,1.5f , 0 };
    int particleSpeedDiff[3] = { 7, 0 ,0 };
    float particleAcc[3] = { 0.0f ,-0.1f , 0 };
    float particleStartPosition[3] = { 0 ,0 , 0 };
    float particleEndPosition[3] = { 0 ,0 , 0 };
    float particleEaseSpeed[3] = { 0 ,0 , 0 };
    float particleHalfwayPoint[3] = { 0 ,0 , 0 };
    float particleStartSize = 2.0f;
    float particleEndSize = 0;
    int particleLife = 60;
    int particleSpornArea[3] = { 1, 1, 1 };
    int particleSpornSpan;
    int particleEaseType = ParticleEaseType::InQuad;
    int emitterLife = 60;
    int emitterPlayTimer = 0;

    float emitterPosition[3] = { 0, 0, 0 };
    bool isParticleEditActive = false;
    int isKeyRec = KeyRec::None;

    XMMATRIX gizmoTaget;
    EachInfo* gizmoTargetObject;
    bool isUseGizmo = false;
    bool isTuchiGizmo = false;
    bool isParticleSystemWindow = false;

    bool isKeyRecWindow = false;
    bool isInspectorWindow = true;

    int isPosRec = KeyRec::None;

};
