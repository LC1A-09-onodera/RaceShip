cbuffer cbuff0 : register(b0)
{
	matrix mat;
	matrix matBillboard; // ３Ｄ変換行列
};

struct VSOutput
{
	float4 pos : SV_POSITION; // システム用頂点座標
	float scale : SCALE;//scale値
};

struct GSOutput
{
	float4 svpos : SV_POSITION; // システム用頂点座標
	float2 uv  :TEXCOORD; // uv値
};