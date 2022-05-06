#include "PostEffectTest.hlsli"

Texture2D<float4> tex0 : register(t0);
//Texture2D<float4> tex1 : register(t1);
SamplerState smp : register(s0);

#define  Iterations  8

float4 main(VSOutput input) : SV_TARGET
{
	return tex0.Sample(smp, input.uv);
}