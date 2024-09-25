#ifndef _STD2D
#define _STD2D

#include "value.fx"
#include "func.fx"

static float BlinkAlpha = 1.0f; // 깜빡임 이벤트를 위한 알파 값
//static float DeadAlpha = 1.0f;

// Vertex Shader
struct VTX_IN
{
    float3 vPos       : POSITION;
    float4 vColor     : COLOR;
    float2 vUV        : TEXCOORD;
};

struct VTX_OUT
{
    float4 vPosition  : SV_Position;
    float4 vColor     : COLOR;
    float2 vUV        : TEXCOORD;
    
    float3 vWorldPos  : POSITION;
};

VTX_OUT VS_Std2D(VTX_IN _in)
{
    VTX_OUT output = (VTX_OUT) 0.f;
    
    // Local Space -> World Space : float3 vWorldPos = mul(float4(_in.vPos, 1.f), matWorld).xyz;
    // World Space -> View Space  : float4 vViewPos = mul(float4(vWorldPos, 1.f), matView);
    // View Space -> Projection Space : float4 vProjPos = mul(vViewPos, matProj);
    
    output.vPosition = mul(float4(_in.vPos, 1.f), matWVP);
    output.vColor = _in.vColor;
    output.vUV = _in.vUV;
    
    output.vWorldPos = mul(float4(_in.vPos, 1.f), matWorld).xyz;
    
    return output;
}

float4 PS_Std2D(VTX_OUT _in) : SV_Target
{
    float4 vColor = float4(0.f, 0.f, 0.f, 1.f);
    
    // FlipBook을 사용하는 경우
    if (UseFlipbook)
    {
        // _in.vUV : 스프라이트를 참조할 위치를 비율로 환산한 값
        float2 BackGroundLeftTop = LeftTopUV - ((BackGroundUV - SliceUV) / 2.f);
        float2 vSpriteUV = BackGroundLeftTop + (_in.vUV * BackGroundUV);
        vSpriteUV -= OffsetUV;
        
        if (LeftTopUV.x <= vSpriteUV.x && vSpriteUV.x <= LeftTopUV.x + SliceUV.x
            && LeftTopUV.y <= vSpriteUV.y && vSpriteUV.y <= LeftTopUV.y + SliceUV.y)
        {
            if (g_int_0 > -1)
            {
                vColor = SelectTexture(g_int_0, g_sam_0, vSpriteUV);
            }
            else
            {
                vColor = g_AtalsTex.Sample(g_sam_0, vSpriteUV);
            }
        }
        else
        {
            discard;
        }
    }
    // FlipBook을 사용하지 않는 경우
    else
    {
        if (g_btex_0)
        {
            vColor = SelectTexture(g_int_0, g_sam_0, _in.vUV);
        }
        else
        {
            vColor = float4(1.f, 0.f, 1.f, 1.f);
        }
    }
    
    if (vColor.a < 0.2f)
    {
        discard;
    }
   
    
    // Light2D 적용
    tLight Light = (tLight) 0.f;
    
    for (int i = 0; i < g_Light2DCount; ++i)
    {
        CalculateLight2D(i, _in.vWorldPos, Light);
    }
    
    vColor.rgb = vColor.rgb * Light.Color.rgb
                + vColor.rgb * Light.Ambient.rgb;
    
    return vColor;
}

float4 PS_Std2D_Alphablend(VTX_OUT _in) : SV_Target
{   
    float4 vColor = float4(0.f, 0.f, 0.f, 1.f);
    
    // FlipBook을 사용하는 경우
    if (UseFlipbook)
    {
        // _in.vUV : 스프라이트를 참조할 위치를 비율로 환산한 값
        float2 BackGroundLeftTop = LeftTopUV - ((BackGroundUV - SliceUV) / 2.f);
        float2 vSpriteUV = BackGroundLeftTop + (_in.vUV * BackGroundUV);
        vSpriteUV -= OffsetUV;
        
        if (LeftTopUV.x <= vSpriteUV.x && vSpriteUV.x <= LeftTopUV.x + SliceUV.x
            && LeftTopUV.y <= vSpriteUV.y && vSpriteUV.y <= LeftTopUV.y + SliceUV.y)
        {
            if (g_int_0 > -1)
            {
                vColor = SelectTexture(g_int_0, g_sam_0, vSpriteUV);
            }
            else
            {
                vColor = g_AtalsTex.Sample(g_sam_0, vSpriteUV);
            }
        }
        else
        {
            discard;
        }
    }
    // FlipBook을 사용하지 않는 경우
    else
    {
        if (g_btex_0)
        {
            vColor = SelectTexture(g_int_0, g_sam_0, _in.vUV);
        }
        else
        {
            vColor = float4(1.f, 0.f, 1.f, 1.f);
        }
    }
    
    if (g_int_3)
    { 
        BlinkEvent(BlinkAlpha, 1.5f, vColor);
    }
    else
    {   
        BlinkAlpha = 1.0f;
    }
    
    if (g_int_2)
    {
        DeadEvent(g_float_2, 2.0f, vColor);
    }
    
    // Light2D 적용
    tLight Light = (tLight) 0.f;
    
    for (int i = 0; i < g_Light2DCount; ++i)
    {
        CalculateLight2D(i, _in.vWorldPos, Light);
    }
    
    vColor.rgb = vColor.rgb * Light.Color.rgb
                + vColor.rgb * Light.Ambient.rgb;
     
    return vColor;
}



// =========================
//       Effect Shader
// =========================
VTX_OUT VS_Effect(VTX_IN _in)
{
    VTX_OUT output = (VTX_OUT) 0.f;
    
    // LocalSpace -> WorldSpace
    // float3 x float 4x4(matrix)
    // flaot3를 float4로 차수를 맞춰주기
    // 동차좌표를 1로 설정, 상태행렬 4행에 들어있는 이동을 적용
}
#endif