#pragma once
#include "../BaseDirectX/BaseDirectX.h"
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include "../Light/Light.h"
#include "../Shader/Shader.h"

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
};

template <typename T>
void CreateConstBuff0(T &each)
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
	BaseDirectX::result = BaseDirectX::dev->CreateCommittedResource(&heapprop, D3D12_HEAP_FLAG_NONE, &resdesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&each.constBuff0));
};
template <typename T>
void CreateConstBuff1(T &each)
{
	BaseDirectX::result = BaseDirectX::dev->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), D3D12_HEAP_FLAG_NONE, &CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataB1) + 0xff) & ~0xff), D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&each.constBuff1));
};
template <typename T>
void CreateConstBuff2(T& each)
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
	BaseDirectX::result = BaseDirectX::dev->CreateCommittedResource(&heapprop, D3D12_HEAP_FLAG_NONE, &resdesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&each.constBuff2));
};
template <typename T>
void ConstInit(T &each)
{
	CreateConstBuff0(each);
	CreateConstBuff1(each);
	CreateConstBuff2(each);
};

class Model
{
public:
	Model() = default;

	virtual ~Model();
	//virtual bool Initialize();

	static void SetLight(shared_ptr<Light> light);
	Mesh mesh;
	D3D12_CPU_DESCRIPTOR_HANDLE cpuDescHandleCBV;
	D3D12_GPU_DESCRIPTOR_HANDLE gpuDescHandleCBV;
	EachInfo each;
	XMMATRIX matWorld;
	static shared_ptr<Light> light;
	shared_ptr<BaseCollider> collider = nullptr;
	//----基本いるもの-----------
	float radi = 2.0f;
	bool billboard = false;
	//-----------------------
	// シェーダリソースビューのハンドル(CPU)
	CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV;
	// シェーダリソースビューのハンドル(CPU)
	CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV;
	// パイプラインステートオブジェクト
	ComPtr<ID3D12PipelineState> pipelinestate;
	// ルートシグネチャ
	ComPtr<ID3D12RootSignature> rootsignature;
	// テクスチャバッファ
	ComPtr<ID3D12Resource> texbuff;
	// デスクリプタサイズ
	UINT descriptorHandleIncrementSize;
	// デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> descHeap;
	//マテリアル
	Material material;
	//マテリアルの数
	int materialCount = 0;
	void CreateModel(const char *name, HLSLShader &shader, bool smoothing = false);
	//void Update();
	virtual void Update(EachInfo *each = nullptr, bool rCamera = false);
	void SendVertex();
	void CalcMatrix();
	void LoadFileContents(const char* name, bool smoothing = false);
	//スムージング
	unordered_map<unsigned short, vector<unsigned short>> smoothData;
	inline size_t GetVertexCount();
	//エッジ平滑化データの追加
	void AddAmoothData(unsigned short indexPosition, unsigned short indexVertex);
	void CalculateSmoothedVertexNormals();
	bool InitializeGraphicsPipeline(HLSLShader &shader);
	//bool LoadTexture(const wchar_t *texName = nullptr);
	bool LoadTexture(const string &directPath, const string &filename);
	bool InitializeDescriptorHeap();
	void LoadMaterial(const string &directoryPath, const string &filename);
	ComPtr<ID3DBlob> LoadShader(LPCWSTR VshaderName = L"", LPCSTR Vtarget = "", ComPtr<ID3DBlob> sBlob = nullptr /*, LPCWSTR PshaderName = L"", LPCSTR Ptarget = "", ComPtr<ID3DBlob> psBlob = nullptr, LPCWSTR GshaderName = L"", LPCSTR Gtarget = "", ComPtr<ID3DBlob> gsBlob = nullptr*/);
	const XMMATRIX &GetMatWorld() { return matWorld; }
	void SetCollider(BaseCollider *collider);
	inline const std::vector<Vertex> &GetVertices(){return mesh.vertices;}
	inline const std::vector<unsigned short> &GetIndices(){return mesh.indices;}
};
template <typename T, typename U>
void ConstBufferInit(T *model, U &eachInfo)
{
	if (model == nullptr) return;
	UINT sizeVB = static_cast<UINT>(sizeof(Vertex) * model->mesh.vertices.size());
	UINT sizeIB = static_cast<UINT>(sizeof(unsigned short) * model->mesh.indices.size());
	&CD3DX12_RESOURCE_DESC::Buffer(sizeVB);
	&CD3DX12_RESOURCE_DESC::Buffer(sizeIB);
	model->mesh.vbView.SizeInBytes = sizeVB;
	model->mesh.ibView.SizeInBytes = sizeIB;
	BaseDirectX::result = BaseDirectX::dev->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), D3D12_HEAP_FLAG_NONE, &CD3DX12_RESOURCE_DESC::Buffer(sizeVB), D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&model->mesh.vertBuff));
	Vertex* vertMap = nullptr;
	BaseDirectX::result = model->mesh.vertBuff->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(BaseDirectX::result))
	{
		copy(model->mesh.vertices.begin(), model->mesh.vertices.end(), vertMap);
		model->mesh.vertBuff->Unmap(0, nullptr);    // マップを解除
	}
	//インデックスバッファの生成
	BaseDirectX::result = BaseDirectX::dev->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), D3D12_HEAP_FLAG_NONE, &CD3DX12_RESOURCE_DESC::Buffer(sizeIB), D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&model->mesh.indexBuff));
	unsigned short* indexMap = nullptr;
	BaseDirectX::result = model->mesh.indexBuff->Map(0, nullptr, (void**)&indexMap);
	if (SUCCEEDED(BaseDirectX::result))
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

	ConstInit(eachInfo);
	
	UINT descHadleIncSize = BaseDirectX::dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	model->cpuDescHandleCBV = BaseDirectX::basicDescHeap->GetCPUDescriptorHandleForHeapStart();
	model->cpuDescHandleCBV.ptr += descHadleIncSize;

	model->gpuDescHandleCBV = BaseDirectX::basicDescHeap->GetGPUDescriptorHandleForHeapStart();
	model->gpuDescHandleCBV.ptr += descHadleIncSize;

	D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc{};
	cbvDesc.BufferLocation = eachInfo.constBuff0->GetGPUVirtualAddress();
	cbvDesc.SizeInBytes = (UINT)eachInfo.constBuff0->GetDesc().Width;
	
	BaseDirectX::dev->CreateConstantBufferView(&cbvDesc, model->cpuDescHandleCBV);
};
void Set3DDraw(const Model &model, bool triangle = true);
void Draw3DObject(const Model &model, int texNum = -1, bool triangle = true);
bool ObjectColition(Model& object1, Model& object2);
bool CiycleColition(const XMFLOAT3 &object1, const XMFLOAT3 &object2 , float radi1, float radi2);
