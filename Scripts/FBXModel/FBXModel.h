#pragma once

#pragma warning(push)
#pragma warning(disable:26812)
#pragma warning(disable:26495)
#pragma warning(disable:26451)
#include <fbxsdk.h>
#include <DirectXTex.h>
#pragma warning(pop)

#include <string>
#include <vector>
#include <DirectXMath.h>

#include <wrl.h>
#include <d3d12.h>
#pragma warning(push)
#pragma warning(disable:6001)
#include <d3dx12.h>
#pragma warning(pop)

struct Node
{
	//���O
	std::string name;
	/*float pad1 = 0.0f;
	float pad2 = 0.0f;*/
	//�X�P�[��
	DirectX::XMVECTOR scaling;
	//��]�p
	DirectX::XMVECTOR rotation ;
	//���[�J���ړ�
	DirectX::XMVECTOR translation ;
	//���[�J���ό`�s��
	DirectX::XMMATRIX transform;
	//�O���[�o���ό`�s��
	DirectX::XMMATRIX globalTransform;
	//�e�m�[�h
	Node *parent = nullptr;
};

struct Bone
{
	std::string name;
	DirectX::XMMATRIX invInitialPose;
	FbxCluster *fbxCuster;
	Bone(const std::string& name)
	{
		this->name = name;
	}
};

class FBXModel
{
public:
	static const int MAX_BONE_INDICES = 4;
	struct VertexPosNormalUvSkin
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT3 normal;
		DirectX::XMFLOAT2 uv;
		UINT boneIndex[MAX_BONE_INDICES];//�{�[���ԍ�
		float boneWeight[MAX_BONE_INDICES];//�{�[���d��
	};

	friend class FbxLoader;

	void CreateBuffers(ID3D12Device *dev);
	void Draw(ID3D12GraphicsCommandList *cmdList);
	std::vector<Bone>& GetBornes() { return bones; }
	const DirectX::XMMATRIX &GetModelTransform(){return meshNode->globalTransform;}
	FbxScene *GetFbxScene(){return fbxScene;}
	~FBXModel();
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using TexMetadata = DirectX::TexMetadata;
	using ScratchImage = DirectX::ScratchImage;
	using string = std::string;
	template <class T> using vector = std::vector<T>;

	string name;
	vector<Node> nodes;
	std::shared_ptr<Node> meshNode;
	vector<VertexPosNormalUvSkin> vertices;
	vector<unsigned short> indices;
	XMFLOAT3 ambient = {1, 1, 1};
	XMFLOAT3 diffuse = {1, 1, 1};
	TexMetadata metadata = {};
	ScratchImage scratchImg = {};

	ComPtr<ID3D12Resource> vertBuff;
	ComPtr<ID3D12Resource> indexBuff;
	ComPtr<ID3D12Resource> texbuff;
	D3D12_VERTEX_BUFFER_VIEW vbView = {};
	D3D12_INDEX_BUFFER_VIEW ibView = {};
	ComPtr<ID3D12DescriptorHeap> descHeapSRV;

	std::vector<Bone> bones;
	FbxScene *fbxScene = nullptr;
};