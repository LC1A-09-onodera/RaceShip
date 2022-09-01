#include "OBJHeader.hlsli"

Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

#define  Iterations  8

float4 main(VSOutput input) : SV_TARGET
{
    //�e�N�X�`���}�b�s���O
    float4 texcolor = tex.Sample(smp, input.uv);
    //texcolor.x += 0.4f;
    return texcolor + 0.15;
}
