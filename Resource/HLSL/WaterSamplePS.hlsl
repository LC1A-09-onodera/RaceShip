#include "OBJHeader.hlsli"

Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

#define  Iterations  8

float4 main(VSOutput input) : SV_TARGET
{
    //float2 smpUV;
    //smpUV.x = 1.0 - input.uv.x;
    //smpUV.y = input.uv.y;
    //return tex.Sample(smp, smpUV);
    //水面ぽいの
    float2 windowSize;
    windowSize.x = 1280 * 2;
    windowSize.y = 720 * 2;
    //水面の動きの速度
    float speed = 0.7f;
    //波の細かさ
    float ripple = 0.9f;
    float2 uv;
    uv.x = 1.0 - input.uv.x;
    uv.y = input.uv.y;
    //現在のフレームを取得
    uint frameCount = flash;
    float time = frameCount * 0.005;
    //波の一区画の大きさを変える
    float onePlot = 12.0f;
    //ズレ度の係数(小さすぎるとゆがまなくなる、大きいと全体が歪んで結果ゆがんでないように見える)
    //float coefficient = 15.0f;
    float coefficient = 13.0f;
    float2 pos = input.svpos.xy / windowSize * onePlot - coefficient;
    float2 tmp = pos;
    float speed2 = speed * 0.5;
    //波のデカさ
    float inten = 0.025f;
    float col = 0;
    //歪みのデカさ
    float distortion = 0.02f;
    for (int i = 0; i < Iterations; ++i)
    {
        float t = time * (1.0 - (3.2 / (float(i) + speed)));
        tmp = pos + float2(
		cos(t - tmp.x * ripple) + sin(t + tmp.y * ripple),
		sin(t - tmp.y * ripple) + cos(t + tmp.x * ripple));
        tmp += time;
        col += 1.0 / length(float2(pos.x / (sin(tmp.x + t * speed2) / inten), pos.y / (cos(tmp.y + t * speed2) / inten)));
    }
    col /= float(Iterations);
    col = saturate(1.5 - sqrt(col));
    uv += col * distortion;
    return (tex.Sample(smp, uv) + (col * 0.6f) + 0.15f) * 0.7;
}