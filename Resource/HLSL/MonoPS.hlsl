#include "OBJHeader.hlsli"

Texture2D<float4> tex0 : register(t0);
SamplerState smp : register(s0);

#define  Iterations  8

float4 main(VSOutput input) : SV_TARGET
{
    float4 col = tex0.Sample(smp, input.uv);
    float colAmount = col.x + col.y + col.z;
    colAmount = colAmount / 3;
    return float4(colAmount, colAmount, colAmount, 1);
}