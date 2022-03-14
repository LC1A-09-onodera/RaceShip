#pragma once
#include <d3dcompiler.h>
#include <d3dx12.h>

using namespace Microsoft::WRL;

class HLSLShader
{
public:
    ComPtr<ID3DBlob> vsBlob; // ���_�V�F�[�_�I�u�W�F�N�g
    ComPtr<ID3DBlob> gsBlob; //�W�I���g���V�F�[�_�[
    ComPtr<ID3DBlob> psBlob;	// �s�N�Z���V�F�[�_�I�u�W�F�N�g
    
};
ComPtr<ID3DBlob> LoadShader(LPCWSTR VshaderName = L"", LPCSTR Vtarget = "");