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
        float2 lightDir = normalize(_WorldPos.xy - Info.WorldPos.xy);  // 광원의 방향 벡터 정규화
       
        float fAngleToCos = dot(lightDir.xy, Info.WorldDir.xy);        // 두 벡터 간의 내적
       
        if (fAngleToCos > cos(Info.Angle / 2.f))
        {
            float fPow = saturate(cos(saturate(fDist / Info.Radius) * (PI / 2.f)));
            _Light.Color.rgb += Info.light.Color.rgb * fPow;
            _Light.Ambient.rgb += Info.light.Ambient.rgb;
        }
    }
}

#endif