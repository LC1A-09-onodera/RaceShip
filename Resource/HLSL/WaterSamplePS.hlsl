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
    //���ʂۂ���
    float2 windowSize;
    windowSize.x = 1280 * 2;
    windowSize.y = 720 * 2;
    //���ʂ̓����̑��x
    float speed = 0.7f;
    //�g�ׂ̍���
    float ripple = 0.9f;
    float2 uv;
    uv.x = 1.0 - input.uv.x;
    uv.y = input.uv.y;
    //���݂̃t���[�����擾
    uint frameCount = flash;
    float time = frameCount * 0.005;
    //�g�̈���̑傫����ς���
    float onePlot = 12.0f;
    //�Y���x�̌W��(����������Ƃ䂪�܂Ȃ��Ȃ�A�傫���ƑS�̂��c��Ō��ʂ䂪��łȂ��悤�Ɍ�����)
    //float coefficient = 15.0f;
    float coefficient = 13.0f;
    float2 pos = input.svpos.xy / windowSize * onePlot - coefficient;
    float2 tmp = pos;
    float speed2 = speed * 0.5;
    //�g�̃f�J��
    float inten = 0.025f;
    float col = 0;
    //�c�݂̃f�J��
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