#include"ShaderManager.h"

HLSLShader ShaderManager::playerShader;
HLSLShader ShaderManager::thunderShader;
HLSLShader ShaderManager::othelloShader;
HLSLShader ShaderManager::postWater;
HLSLShader ShaderManager::postMosaic;
HLSLShader ShaderManager::postBlur;
HLSLShader ShaderManager::postNormal;
//HLSLShader ShaderManager::translucentShader;
void ShaderManager::LoadShaders()
{
	playerShader.vsBlob = LoadShader(L"Resource/HLSL/OBJVertexShader.hlsl", "vs_5_0");
	playerShader.psBlob = LoadShader(L"Resource/HLSL/OBJPixelShader.hlsl", "ps_5_0");
	/*playerShader.hsBlob = LoadShader(L"Resource/HLSL/ObjectHS.hlsl", "hs_5_0");
	playerShader.dsBlob = LoadShader(L"Resource/HLSL/ObjectDS.hlsl", "ds_5_0");
	thunderShader.vsBlob = LoadShader(L"Resource/HLSL/ThunderVS.hlsl", "vs_5_0");
	thunderShader.psBlob = LoadShader(L"Resource/HLSL/ThunderPS.hlsl", "ps_5_0");
	othelloShader.vsBlob = LoadShader(L"Resource/HLSL/OthelloVS.hlsl", "vs_5_0");
	othelloShader.psBlob = LoadShader(L"Resource/HLSL/OthelloPS.hlsl", "ps_5_0");*/

	/*postWater.vsBlob = LoadShader(L"Resource/HLSL/PostEffectTestVS.hlsl", "vs_5_0");
	postWater.psBlob = LoadShader(L"Resource/HLSL/PostWaterPS.hlsl", "ps_5_0");
	postMosaic.vsBlob = LoadShader(L"Resource/HLSL/PostEffectTestVS.hlsl", "vs_5_0");
	postMosaic.psBlob = LoadShader(L"Resource/HLSL/PostMosaicPS.hlsl", "ps_5_0");
	postBlur.vsBlob = LoadShader(L"Resource/HLSL/PostEffectTestVS.hlsl", "vs_5_0");
	postBlur.psBlob = LoadShader(L"Resource/HLSL/PostBlurPS.hlsl", "ps_5_0");*/
	postNormal.vsBlob = LoadShader(L"Resource/HLSL/PostEffectTestVS.hlsl", "vs_5_0");
	postNormal.psBlob = LoadShader(L"Resource/HLSL/PostEffectTestPS.hlsl", "ps_5_0");
}
