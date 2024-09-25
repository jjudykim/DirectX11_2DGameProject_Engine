#ifndef _FUNC
#define _FUNC

#include "value.fx"
#include "struct.fx"

int IsBinding(Texture2D _tex)
{
    int width = 0, height = 0;
    _tex.GetDimensions(width, height);
    
    if (0 == width || height == 0)
        return false;
    
    return true;
}

void CalculateLight2D(int _LightIdx, float3 _WorldPos, inout tLight _Light)
{
    tLightInfo Info = g_Light2D[_LightIdx];
    
    // Directional Light
    if (Info.Type == 0)
    {
        _Light.Color.rgb += Info.light.Color.rgb;
        _Light.Ambient.rgb += Info.light.Ambient.rgb;
    }
    
    // Point Light
    else if (Info.Type == 1)
    {
        float fDist = distance(Info.WorldPos.xy, _WorldPos.xy);
        float fPow = saturate(cos(saturate(fDist / Info.Radius) * (PI / 2.f)));
        
        _Light.Color.rgb += Info.light.Color.rgb * fPow;
        _Light.Ambient.rgb += Info.light.Ambient.rgb;
    }
    
    // Spot Light
    else
    {
        float fDist = distance(Info.WorldPos.xy, _WorldPos.xy);
        float2 lightDir = normalize(_WorldPos.xy - Info.WorldPos.xy);  // ������ ���� ���� ����ȭ
       
        float fAngleToCos = dot(lightDir.xy, Info.WorldDir.xy);        // �� ���� ���� ����
       
        if (fAngleToCos > cos(Info.Angle / 2.f))
        {
            float fPow = saturate(cos(saturate(fDist / Info.Radius) * (PI / 2.f)));
            _Light.Color.rgb += Info.light.Color.rgb * fPow;
            _Light.Ambient.rgb += Info.light.Ambient.rgb;
        }
    }
}

void BlinkEvent(inout float _BlinkAlpha, float _BlinkSpeed, inout float4 _vColor)
{   
    float cycle = g_Time * _BlinkSpeed;
    _BlinkAlpha = 0.7f * abs(sin(cycle * PI)) + 0.3f;
    
    _vColor.a *= _BlinkAlpha;
}

void DeadEvent(float _ElapsedTime, float _Duration, inout float4 _vColor)
{
    float _DeadAlpha = max(0.0f, 1.0f - (_ElapsedTime / _Duration));
    
    if (_DeadAlpha < 0.0f)
    {
        _DeadAlpha = 0.0f;
    }
    
    _vColor.a *= _DeadAlpha;
}

float4 SelectTexture(int _g_int_0, SamplerState _sampler, float2 _UV)
{
    float4 vColor = float4(0.f, 0.f, 0.f, 1.f);
    
    if (_g_int_0 == 0) 
        return vColor = g_tex_0.Sample(g_sam_0, _UV);
    else if (_g_int_0 == 1)
        return vColor = g_tex_1.Sample(g_sam_0, _UV);
    else if (_g_int_0 == 2)
        return vColor = g_tex_2.Sample(g_sam_0, _UV);
    else if (_g_int_0 == 3)
        return vColor = g_tex_3.Sample(g_sam_0, _UV);
    else if (_g_int_0 == 4)
        return vColor = g_tex_4.Sample(g_sam_0, _UV);
    else if (_g_int_0 == 5)
        return vColor = g_tex_5.Sample(g_sam_0, _UV);
    else if (_g_int_0 == 6)
        return vColor = g_tex_6.Sample(g_sam_0, _UV);
    else if (_g_int_0 == 7)
        return vColor = g_tex_7.Sample(g_sam_0, _UV);
    else if (_g_int_0 == 8)
        return vColor = g_tex_8.Sample(g_sam_0, _UV);
    else if (_g_int_0 == 9)
        return vColor = g_tex_9.Sample(g_sam_0, _UV);
    else if (_g_int_0 == 10)
        return vColor = g_tex_10.Sample(g_sam_0, _UV);
    else if (_g_int_0 == 11)
        return vColor = g_tex_11.Sample(g_sam_0, _UV);
    else if (_g_int_0 == 12)
        return vColor = g_tex_12.Sample(g_sam_0, _UV);
    else if (_g_int_0 == 13)
        return vColor = g_tex_13.Sample(g_sam_0, _UV);
    else if (_g_int_0 == 14)
        return vColor = g_tex_14.Sample(g_sam_0, _UV);
    else if (_g_int_0 == 15)
        return vColor = g_tex_15.Sample(g_sam_0, _UV);
    else
        return vColor = float4(1.f, 0.f, 0.f, 1.f);
}

float3 GetRandom(in Texture2D _NoiseTexture, uint _ID, uint _maxId)
{
    float2 vUV = (float2) 0.f;
    vUV.x = ((float) _ID / (float) (_maxId - 1)) + g_EngineTime * 0.5f;
    vUV.y = sin(vUV.x * 20 * PI) * 0.5f + g_EngineTime * 0.1f;
    float3 vRandom = _NoiseTexture.SampleLevel(g_sam_1, vUV, 0).xyz;
    
    return vRandom;
}

#endif