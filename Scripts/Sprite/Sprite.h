#pragma once
//#include <Windows.h>
#include <vector>
#include <string>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#pragma warning(push)
#pragma warning(disable:26812)
#include <DirectXTex.h>
#pragma warning(pop)
#include "../BaseDirectX/Input.h"
#include <wrl.h>
#pragma warning(push)
#pragma warning(disable:6001)
#include <d3dx12.h>
#pragma warning(pop)
#include "../BaseDirectX/BaseDirectX.h"
#include "SpriteCommon.h"
#include "../Texture/Texture.h"
#include "../3DModel/Model.h"

using namespace Microsoft::WRL;
using namespace DirectX;

#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

struct VertexPosUv
{
	XMFLOAT3 pos;
	XMFLOAT2 uv;
};

class Sprite
{
public:
	static SpriteCommon common;
	Tex tex;
	ComPtr<ID3DBlob> vsBlob;	// ���_�V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> psBlob;	// �s�N�Z���V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> errorBlob;	// �G���[�I�u�W�F�N�g
	//���[�g�V�O�l�`��
	ComPtr<ID3D12RootSignature> spriteRootSignature;
	ComPtr<ID3DBlob> rootSigBlob;
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	// ���_�o�b�t�@�̐ݒ�
	ComPtr<ID3D12Resource> vertBuff;
	// ���_�o�b�t�@�r���[�̍쐬
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	//�r���[�̃A�h���X��ۑ�
	D3D12_GPU_DESCRIPTOR_HANDLE gpuDescHandleCBV;
	//�f�X�N���v�^�����W
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV;
	//�萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuff;
	//���[�g�p�����[�^�[
	CD3DX12_ROOT_PARAMETER rootParams[2];
	bool back;
	float rotation = 0.0f;
	int texNum;
	float pad1 = 0.0f;
	float pad2 = 0.0f;
	float pad3 = 0.0f;
	//�ˉe�s��
	XMVECTOR position{};
	XMMATRIX matProjection{};
	XMMATRIX matWorld{};
	XMFLOAT2 texLeftTop = {0,0};
	XMFLOAT2 texSize = {100,100};

	//�X�v���C�g�̐���
	void CreateSprite(BaseDirectX& baseDirectX, const wchar_t* graph, XMFLOAT3 f_position, ComPtr<ID3D12Resource> texBuff = nullptr, bool f_back = false, bool TexSize = true);
	void CreateSprite(BaseDirectX& baseDirectX, Tex f_tex, XMFLOAT3 f_position, ComPtr<ID3D12Resource> texBuff = nullptr, bool f_back = false, bool TexSize = true);
	//�X�v���C�g�p�p�C�v���C���̐ݒ�R�}���h
	
	//�X�v���C�g�p�`��R�}���h
	//�`�斈�t���[������
	void SpriteDraw(BaseDirectX& baseDirectX);
	//���W�ύX
	void Move(BaseDirectX& baseDirectX);
	//�\���T�C�Y�̕ύX
	void ChangeSize(BaseDirectX& baseDirectX, float wid,float hei);
	void ChangeSizeOther(BaseDirectX& baseDirectX, float wid, float hei);
	
	//���t���[������
	private:
	void SpritePipelineEdit(BaseDirectX& baseDirectX);
};