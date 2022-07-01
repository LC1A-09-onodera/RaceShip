#pragma once
#include <d3dcompiler.h>
#pragma warning(push)
#pragma warning(disable:6001)
#include <d3dx12.h>
#pragma warning(pop)

using namespace Microsoft::WRL;

class HLSLShader
{
public:
    ComPtr<ID3DBlob> vsBlob = nullptr; //頂点シェーダオブジェクト
    ComPtr<ID3DBlob> gsBlob = nullptr; //ジオメトリシェーダー
    ComPtr<ID3DBlob> psBlob = nullptr; //ピクセルシェーダオブジェクト
};
ComPtr<ID3DBlob> LoadShader(LPCWSTR VshaderName = L"", LPCSTR Vtarget = "");