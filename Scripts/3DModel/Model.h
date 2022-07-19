#pragma once

#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include "../Light/Light.h"
#include "../Shader/Shader.h"
#include "../BaseDirectX/BaseDirectX.h"

using namespace std;
class BaseCollider;

struct ConstBufferData
{
	XMMATRIX mat;
	XMFLOAT4 weight0;
	XMFLOAT4 weight1;
	UINT frameTime;
};

struct ConstBufferDataSP
{
	XMFLOAT4 color;
	XMMATRIX mat;
};

class ConstBufferDataB0
{
public:
	XMMATRIX viewproj;//ビュープロジェクション行列
	XMMATRIX world;//ワールド行列
	XMFLOAT3 cameraPos;//カメラ座標
};

struct ConstBufferDataB1
{
	XMFLOAT3 ambient;
	float pad1;
	XMFLOAT3 diffuse;
	float pad2;
	XMFLOAT3 specular;
	float alpha;
};

struct ConstBufferDataB3
{
public:
	XMMATRIX rViewproj;//ビュープロジェクション行列
	XMMATRIX rWorld;//ワールド行列
	XMFLOAT3 rCameraPos;//カメラ座標
};

struct Material
{
	string name;
	XMFLOAT3 ambient;
	XMFLOAT3 diffuse;
	XMFLOAT3 specular;

	float alpha;
	string texFilename;

	Material()
	{
		ambient = {0.3f, 0.3f, 0.3f};
		diffuse = {0.0f, 0.0f, 0.0f};
		specular = {0.0f, 0.0f, 0.0f};
		alpha = 1.0;
	}
};

struct Vertex
{
	XMFLOAT3 pos;
	XMFLOAT3 normal;
	XMFLOAT2 uv;
};

class Mesh
{
public:
	// 頂点バッファ
	ComPtr<ID3D12Resource> vertBuff;
	// インデックスバッファ
	ComPtr<ID3D12Resource> indexBuff;
	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView = {};
	// インデックスバッファビュー
	D3D12_INDEX_BUFFER_VIEW ibView = {};
	// 頂点データ配列
	std::vector<Vertex> vertices;
	// 頂点インデックス配列
	std::vector<unsigned short> indices;
};

class EachInfo
{
public:
	ComPtr<ID3D12Resource> constBuff0;
	ComPtr<ID3D12Resource> constBuff1;
	ComPtr<ID3D12Resource> constBuff2;
	XMFLOAT3 scale = { 1,1,1 };
	XMFLOAT3 rotation = { 0,0,0 };
	XMVECTOR position = { 0,0,0 };
	std::string m_eachName;
};

class EachManager
{
public:
	static list<EachInfo *> eahcs;
};

template <typename T>
void CreateConstBuff0(T &each, ComPtr<ID3D12Device> &dev)
{
	D3D12_HEAP_PROPERTIES heapprop{};
	heapprop.Type = D3D12_HEAP_TYPE_UPLOAD;
	//リソース設定
	D3D12_RESOURCE_DESC resdesc{};
	resdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resdesc.Width = (sizeof(typeid(each)) + 0xff) & ~0xff;
	resdesc.Height = 1;
	resdesc.DepthOrArraySize = 1;
	resdesc.MipLevels = 1;
	resdesc.SampleDesc.Count = 1;
	resdesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	HRESULT result;
	result = dev->CreateCommittedResource(&heapprop, D3D12_HEAP_FLAG_NONE, &resdesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&each.constBuff0));
};
template <typename T>
void CreateConstBuff1(T &each, ComPtr<ID3D12Device>& dev)
{
	HRESULT result;
	CD3DX12_HEAP_PROPERTIES heapProp(D3D12_HEAP_TYPE_UPLOAD);
	CD3DX12_RESOURCE_DESC resourceDesc = CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataB1) + 0xff) & ~0xff);
	result = dev->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&each.constBuff1));
};
template <typename T>
void CreateConstBuff2(T& each, ComPtr<ID3D12Device>& dev)
{
	D3D12_HEAP_PROPERTIES heapprop{};
	heapprop.Type = D3D12_HEAP_TYPE_UPLOAD;
	//リソース設定
	D3D12_RESOURCE_DESC resdesc{};
	resdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resdesc.Width = (sizeof(typeid(each)) + 0xff) & ~0xff;
	resdesc.Height = 1;
	resdesc.DepthOrArraySize = 1;
	resdesc.MipLevels = 1;
	resdesc.SampleDesc.Count = 1;
	resdesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	HRESULT result;
	result = dev->CreateCommittedResource(&heapprop, D3D12_HEAP_FLAG_NONE, &resdesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&each.constBuff2));
};
template <typename T>
void ConstInit(T &each, ComPtr<ID3D12Device>& dev)
{
	CreateConstBuff0(each , dev);
	CreateConstBuff1(each , dev);
	CreateConstBuff2(each , dev);
};

class Model
{
public:
	Model() = default;

	virtual ~Model();
	static void SetLight(shared_ptr<Light> Light);
	Mesh mesh;
	D3D12_CPU_DESCRIPTOR_HANDLE cpuDescHandleCBV = D3D12_CPU_DESCRIPTOR_HANDLE();
	D3D12_GPU_DESCRIPTOR_HANDLE gpuDescHandleCBV = D3D12_GPU_DESCRIPTOR_HANDLE();
	EachInfo each;
	XMMATRIX matWorld = XMMatrixIdentity();
	static shared_ptr<Light> light;
	shared_ptr<BaseCollider> collider = nullptr;
	string m_modelName;
	//----基本いるもの-----------
	float radi = 2.0f;
	//-----------------------
	// シェーダリソースビューのハンドル(CPU)
	CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV = CD3DX12_CPU_DESCRIPTOR_HANDLE();
	// シェーダリソースビューのハンドル(CPU)
	CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV = CD3DX12_GPU_DESCRIPTOR_HANDLE();
	// パイプラインステートオブジェクト
	ComPtr<ID3D12PipelineState> pipelinestate;
	// ルートシグネチャ
	ComPtr<ID3D12RootSignature> rootsignature;
	// テクスチャバッファ
	ComPtr<ID3D12Resource> texbuff;
	// デスクリプタサイズ
	UINT descriptorHandleIncrementSize = 0;
	// デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> descHeap;
	//マテリアル
	Material material;
	//マテリアルの数
	int materialCount = 0;
	void CreateModel(BaseDirectX& baseDirectX, const char *name, HLSLShader &shader, bool smoothing = false, bool isTriangle = true);
	//void Update();
	virtual void Update(BaseDirectX& baseDirectX, EachInfo *f_each, bool rCamera = false);
	void SendVertex(BaseDirectX& baseDirectX);
	
	void LoadFileContents(BaseDirectX& baseDirectX, const char* name, bool smoothing = false);
	//スムージング
	unordered_map<unsigned short, vector<unsigned short>> smoothData;
	inline size_t GetVertexCount();
	//エッジ平滑化データの追加
	void AddAmoothData(unsigned short indexPosition, unsigned short indexVertex);
	void CalculateSmoothedVertexNormals();
	bool InitializeGraphicsPipeline(BaseDirectX &baseDirectX, HLSLShader &shader, bool isTriangle = true);
	//bool LoadTexture(const wchar_t *texName = nullptr);
	bool LoadTexture(BaseDirectX& baseDirectX, const string &directPath, const string &filename);
	bool InitializeDescriptorHeap(BaseDirectX& baseDirectX);
	void LoadMaterial(BaseDirectX& baseDirectX ,const string &directoryPath, const string &filename);
	const XMMATRIX &GetMatWorld() { return matWorld; }
	inline const std::vector<Vertex> &GetVertices(){ return mesh.vertices; }
	inline const std::vector<unsigned short> &GetIndices(){ return mesh.indices; }
};
template <typename T, typename U>
void ConstBufferInit(BaseDirectX &baseDirectX, T *model, U &eachInfo)
{
	if (model == nullptr) return;
	UINT sizeVB = static_cast<UINT>(sizeof(Vertex) * model->mesh.vertices.size());
	UINT sizeIB = static_cast<UINT>(sizeof(unsigned short) * model->mesh.indices.size());
	/*CD3DX12_RESOURCE_DESC::Buffer(sizeVB);
	CD3DX12_RESOURCE_DESC::Buffer(sizeIB);*/
	model->mesh.vbView.SizeInBytes = sizeVB;
	model->mesh.ibView.SizeInBytes = sizeIB;
	CD3DX12_HEAP_PROPERTIES heapProp(D3D12_HEAP_TYPE_UPLOAD);
	CD3DX12_RESOURCE_DESC resourceDescVB = CD3DX12_RESOURCE_DESC::Buffer(sizeVB);
	baseDirectX.result = baseDirectX.dev->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_NONE, &resourceDescVB, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&model->mesh.vertBuff));
	Vertex* vertMap = nullptr;
	baseDirectX.result = model->mesh.vertBuff->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(baseDirectX.result))
	{
		copy(model->mesh.vertices.begin(), model->mesh.vertices.end(), vertMap);
		model->mesh.vertBuff->Unmap(0, nullptr);    // マップを解除
	}
	//インデックスバッファの生成
	CD3DX12_RESOURCE_DESC resourceDescIB = CD3DX12_RESOURCE_DESC::Buffer(sizeIB);
	baseDirectX.result = baseDirectX.dev->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_NONE, &resourceDescIB, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&model->mesh.indexBuff));
	unsigned short* indexMap = nullptr;
	baseDirectX.result = model->mesh.indexBuff->Map(0, nullptr, (void**)&indexMap);
	if (SUCCEEDED(baseDirectX.result))
	{
		copy(model->mesh.indices.begin(), model->mesh.indices.end(), indexMap);
		model->mesh.indexBuff->Unmap(0, nullptr);
	}

	// GPU上のバッファに対応した仮想メモリを取得
	model->mesh.vbView.BufferLocation = model->mesh.vertBuff->GetGPUVirtualAddress();
	//vbView.SizeInBytes = sizeVB;
	model->mesh.vbView.StrideInBytes = sizeof(Vertex);
	model->mesh.ibView.BufferLocation = model->mesh.indexBuff->GetGPUVirtualAddress();
	model->mesh.ibView.Format = DXGI_FORMAT_R16_UINT;
	//ibView.SizeInBytes = sizeIB;

	ConstInit(eachInfo, baseDirectX.dev);
	
	UINT descHadleIncSize = baseDirectX.dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	model->cpuDescHandleCBV = baseDirectX.basicDescHeap->GetCPUDescriptorHandleForHeapStart();
	model->cpuDescHandleCBV.ptr += descHadleIncSize;

	model->gpuDescHandleCBV = baseDirectX.basicDescHeap->GetGPUDescriptorHandleForHeapStart();
	model->gpuDescHandleCBV.ptr += descHadleIncSize;

	D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc{};
	cbvDesc.BufferLocation = eachInfo.constBuff0->GetGPUVirtualAddress();
	cbvDesc.SizeInBytes = static_cast<UINT>(eachInfo.constBuff0->GetDesc().Width);
	
	baseDirectX.dev->CreateConstantBufferView(&cbvDesc, model->cpuDescHandleCBV);
}
template <typename T, typename U>
void CalcMatrix(T *model, U *eachInfo)
{
	if (model == nullptr) return;
	if (eachInfo == nullptr) return;
	XMMATRIX matScale, matRot, matTrans;
	matScale = XMMatrixScaling(eachInfo->scale.x, eachInfo->scale.y, eachInfo->scale.z);
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(XMConvertToRadians(eachInfo->rotation.x));
	matRot *= XMMatrixRotationX(XMConvertToRadians(eachInfo->rotation.y));
	matRot *= XMMatrixRotationY(XMConvertToRadians(eachInfo->rotation.z));
	matTrans = XMMatrixTranslation(eachInfo->position.m128_f32[0], eachInfo->position.m128_f32[1], eachInfo->position.m128_f32[2]);
	model->matWorld = XMMatrixIdentity();

	//ビルボード
	//if (billboard)
	//{
	//    matWorld *= BaseDirectX::matBillboard;//ビルボードをかける
	//}
	//ビルボードY
	//if (billboard)
	//{
	//    matWorld *= Camera::matBillboardY;//ビルボードをかける
	//}
	model->matWorld *= matScale;
	model->matWorld *= matRot;
	model->matWorld *= matTrans;
}
void Set3DDraw(BaseDirectX& baseDirectX, const Model &model, bool triangle = true);
void Draw3DObject(BaseDirectX& baseDirectX, const Model &model, bool triangle = true);
bool ObjectColition(Model& object1, Model& object2);
bool CiycleColition(const XMFLOAT3 &object1, const XMFLOAT3 &object2 , float radi1, float radi2);

class ModelElement
{
public:
	ModelElement(std::string name, Model *model)
	{
		m_model = model;
		m_modelName = name;
	}
	std::string m_modelName;
	Model *m_model;
};

class ModelManager
{
public:
	static std::list<ModelElement *> m_models;
};
