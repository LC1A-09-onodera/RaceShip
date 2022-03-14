#pragma once
#include <d3dcompiler.h>
#include <d3dx12.h>

using namespace Microsoft::WRL;

class HLSLShader
{
public:
    ComPtr<ID3DBlob> vsBlob; // 頂点シェーダオブジェクト
    ComPtr<ID3DBlob> gsBlob; //ジオメトリシェーダー
    ComPtr<ID3DBlob> psBlob;	// ピクセルシェーダオブジェクト
    
};
ComPtr<ID3DBlob> LoadShader(LPCWSTR VshaderName = L"", LPCSTR Vtarget = "");