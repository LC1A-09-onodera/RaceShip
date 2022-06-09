#include "WaterFace.h"
#include "../Shader/ShaderManager.h"

void WaterFace::LoadModel()
{
	waterModel.CreateModel("WaterFace", ShaderManager::waterShader);
}

void WaterFace::Init()
{
	each.position = {0, 0, 0, 1};
}

void WaterFace::Update()
{
}

void WaterFace::Draw()
{
	waterModel.Draw(each);
}

void WaterFaceModel::Update()
{
}

void WaterFaceModel::PreDraw()
{
}

void WaterFaceModel::PostDraw()
{
}

void WaterFaceModel::Draw(EachInfo& each)
{
	BaseDirectX::cmdList->IASetIndexBuffer(&mesh.ibView);
	BaseDirectX::cmdList->SetPipelineState(pipelinestate.Get());
	BaseDirectX::cmdList->SetGraphicsRootSignature(rootsignature.Get());
	BaseDirectX::cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	BaseDirectX::cmdList->IASetVertexBuffers(0, 1, &mesh.vbView);
	BaseDirectX::cmdList->IASetIndexBuffer(&mesh.ibView);
	ID3D12DescriptorHeap* ppHeap[] = { descHeap.Get() };
	BaseDirectX::cmdList->SetDescriptorHeaps(_countof(ppHeap), ppHeap);
	BaseDirectX::cmdList->SetGraphicsRootConstantBufferView(0, each.constBuff0->GetGPUVirtualAddress());
	Model::light->Draw(BaseDirectX::cmdList.Get(), 3);
	BaseDirectX::cmdList->SetGraphicsRootConstantBufferView(1, each.constBuff1->GetGPUVirtualAddress());
	BaseDirectX::cmdList->SetGraphicsRootDescriptorTable(2, gpuDescHandleSRV);
	BaseDirectX::cmdList->DrawIndexedInstanced((UINT)mesh.indices.size(), 1, 0, 0, 0);
}
