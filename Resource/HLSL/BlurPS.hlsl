#include "OBJHeader.hlsli"

Texture2D<float4> tex0 : register(t0);
SamplerState smp : register(s0);

#define  Iterations  8

float Gaussian(float2 drawUV, float2 pickUV, float sigma)
{
    float d = distance(drawUV, pickUV);
    return exp(-(d * d) / (2 * sigma * sigma));
}

float4 main(VSOutput input) : SV_TARGET
{
    float totalWeight = 0, _Sigma = 0.002, _SetpWidth = 0.001;
    float4 col = float4(0, 0, 0, 0);
    for (float py = -_Sigma * 2; py <= _Sigma; py += _SetpWidth)
    {
        for (float px = -_Sigma * 2; px <= _Sigma; px += _SetpWidth)
        {
            float2 pickUV = input.uv + float2(px, py);
            float weight = Gaussian(input.uv, pickUV, _Sigma);

            col += tex0.Sample(smp, pickUV) * weight;
            totalWeight += weight;
        }
    }
    col.rgb = col.rgb / totalWeight;
    return col;
}