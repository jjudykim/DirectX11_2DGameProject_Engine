#ifndef _POSTPROCESS
#define POSTPROCESS

#include "value.fx"

// ==========================
//     GrayFilter Shader 
// --------------------------
// Mesh    : RectMesh
// DSTYPE  : NO_TEST_NO_WRITE
// g_tex_0 : TargetCopyTex
// ==========================
struct VS_IN
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
};

struct VS_OUT
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
};

VS_OUT VS_GrayFilter(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    output.vPosition = float4(_in.vPos.xy * 2.f, 0.f, 1.f);
    output.vUV = _in.vUV;
    
    return output;
}

float4 PS_GrayFilter(VS_OUT _in) : SV_Target
{
     // GrayFilter
    float4 vColor = g_tex_0.Sample(g_sam_0, _in.vUV);
    float Average = (vColor.x + vColor.y + vColor.z) / 3.f;
    vColor = float4(Average, Average, Average, 1.f);
    
    return vColor;
}



// ==========================
//     Distortion Shader 
// --------------------------
// Mesh    : RectMesh
// DSTYPE  : NO_TEST_NO_WRITE
// g_tex_0 : TargetCopyTex
// g_Tex_1 : NoiseTexture1
// g_Tex_2 : NoiseTexture2
// g_Tex_3 : NoiseTexture3
// ==========================
VS_OUT VS_Distortion(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    output.vPosition = mul(float4(_in.vPos, 1.f), matWVP);
    output.vUV = _in.vUV;
    
    return output;
}

float4 PS_Distortion(VS_OUT _in) : SV_Target
{
    // 1. Render Target 해상도 정보 -> g_Resolution 활용
    // 2. Pixel Shader가 호출된 Pixel 본인의 현재 좌표 -> SV_Position 활용
    float2 vScreenUV = _in.vPosition.xy / g_Resolution;
    float2 vNoiseUV = vScreenUV;
    
    vNoiseUV.x += g_EngineTime * 0.1;
    float4 vNoise = g_tex_3.Sample(g_sam_0, vNoiseUV);
    
    vNoise = (vNoise * 2.f - 1.f) * 0.01f;
    vScreenUV = vScreenUV + vNoise.xy;
    float4 vColor = g_tex_0.Sample(g_sam_0, vScreenUV);
    
    return vColor;
}


// ==========================
//       Ripple Shader 
// --------------------------
// Mesh    : RectMesh
// DSTYPE  : NO_TEST_NO_WRITE
// g_tex_0 : TargetCopyTex
// ==========================
VS_OUT VS_Ripple(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    output.vPosition = mul(float4(_in.vPos, 1.f), matWVP);
    output.vUV = _in.vUV;
    
    return output;
}

float4 PS_Ripple(VS_OUT _in) : SV_Target
{
    float2 vScreenUV = _in.vPosition.xy / g_Resolution;
    float2 delta = (g_Resolution / 2.f) - _in.vPosition.xy;
    
    const float duration = 3.0;
    float t = fmod(g_EngineTime, duration) / duration;
    float fade_out = smoothstep(t, 1.0, 0.95);
    float ring = t * 1000.0;
    
    float dist = length(delta) - ring;
    float ring_dist = fmod(dist, 250.0f);

    const float THICKNESS = 40.0;
    float thickness = THICKNESS * pow(length(delta), 0.2);
    
    const float FALLOFF = 20.0;
    float in_ring = smoothstep(length(delta) - FALLOFF, length(delta) + FALLOFF, ring);
    float ring_color = cos(6.2831 * min(1.0, ring_dist / thickness));
    float wave = ring_color * in_ring * fade_out;
    

    float4 vColor = g_tex_0.Sample(g_sam_0, vScreenUV + normalize(delta) * wave * 0.01);
    vColor.xyz -= 0.1 * wave;
    
    return vColor;
}


// ==========================
//      Melting Shader 
// --------------------------
// Mesh    : RectMesh
// DSTYPE  : NO_TEST_NO_WRITE
// g_tex_0 : TargetCopyTex
// ==========================


float rand(float2 p)
{
    float3 p3 = frac(float3(p.xyx) * .1031);
    p3 += dot(p3, p3.yzx + 33.33);
    
    return frac((p3.x + p3.y) * p3.z);
}

VS_OUT VS_Melting(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    output.vPosition = mul(float4(_in.vPos, 1.f), matWVP);
    output.vUV = _in.vUV;
    
    return output;
}

float4 PS_Melting(VS_OUT _in) : SV_Target
{
    float2 vScreenYUV = _in.vPosition.xy / g_Resolution.y;
    float2 vScreenUV = _in.vPosition.xy / g_Resolution;
    
    float4 vColor = g_tex_0.Sample(g_sam_0, vScreenUV);
 
    float m = 0.;
    float t = g_EngineTime * 2.2 + 100.;
    
    for (float i = 0.; i <= 1.0; i += 0.25)
    {
        float2 iuv = vScreenYUV + float2(i * .05, 0.);
        float2 ruv = iuv * 50.f;                           // size
        float2 id = ceil(ruv);
        
        for (float y = -1.; y <= 1.; y += 1.)
        {
            for (float x = -1.; x <= 1.; x += 1.0)
            {
                float2 nuv = ruv + float2(x, y);
                float2 nid = id + float2(x, y);

                nuv.y += t * 2. * (rand(float2(nid.x, 0.f)) * .75 + .5) * i * (-1);
                nuv.y += ceil(fmod(nid.x, 3.)) * 0.3 * t;

                float2 guv = frac(nuv);

                nuv = floor(nuv);
                float g = length(guv - float2(x, y));

                float v = rand(nuv);
                v *= step(0.9, v);
                m += smoothstep(v, v - .8, g);
            }
        }
    }
      
    m = m * .2 + step(1.25, m) * .2 + step(2.0, m) * .1;
    
    float3 col = g_tex_0.Sample(g_sam_0, vScreenUV + m * .125).rgb;

    vColor = float4(col, 1.f);
    
    return vColor;
}


// ==========================
//        Analog TV 
// --------------------------
// Mesh    : RectMesh
// DSTYPE  : NO_TEST_NO_WRITE
// g_tex_0 : TargetCopyTex
// ==========================

float TVrand(float2 co)
{
    return frac(sin(dot(co.xy, float2(12.9898, 78.233))) * 43758.5453);
}

VS_OUT VS_AnalogTV(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    output.vPosition = mul(float4(_in.vPos, 1.f), matWVP);
    output.vUV = _in.vUV;
    
    return output;
}

float4 PS_AnalogTV(VS_OUT _in) : SV_Target
{
    float2 uv = (_in.vPosition.xy - g_Resolution * 0.5) / g_Resolution.y;
        
    // Make better TV size
    uv *= .32;
    uv.x *= .56;
    
    // Fish eye
    float fovTheta = 7.55;
    float z = sqrt(0.2 - uv.x * uv.x - uv.y * uv.y);
    float a = 1. / (z * tan(fovTheta * 0.5));
    uv = uv * a;
    
    // Take video pixel
    float3 vColor = g_tex_0.Sample(g_sam_0, (uv + 0.5)).rgb;
        
    // Glitch color
    float2 ruv = uv;
    ruv.x += 0.02;
    vColor.r += g_tex_0.Sample(g_sam_0, (ruv + 0.5)).r * 0.4;
    
    // Color noise    
    vColor += rand(frac(floor((ruv + g_EngineTime) * g_Resolution.y) * 0.7)) * 0.2;
    
    // Make small lines               
    vColor *= clamp(frac(uv.y * 100. + g_EngineTime * 8.), 0.8, 1.);
    
    // Make big lines
    float bf = frac(uv.y * 3. + g_EngineTime * 26.);
    float ff = min(bf, 1. - bf) + 0.35;
    vColor *= clamp(ff, 0.5, 0.75) + 0.75;
    
    // Make low Hz
    vColor *= (sin(g_EngineTime * 120.) * 0.5 + 0.5) * 0.1 + 0.9;
    
    // Make borders
    vColor *= smoothstep(-0.51, -0.50, uv.x) * smoothstep(0.51, 0.50, uv.x);
    vColor *= smoothstep(-0.51, -0.50, uv.y) * smoothstep(0.51, 0.50, uv.y);
    
    float4 vColor4 = float4(vColor, 1.);
    
    return vColor4;
}


// ====================================
// Blur Shader
// Mesh     : RectMesh
// DSTYPE   : NO_TEST_NO_WRITE
// g_tex_0  : Target
// ====================================

static float GaussianFilter[5][5] =
{
    0.003f, 0.0133f, 0.0219f, 0.0133f, 0.003f,
    0.0133f, 0.0596f, 0.0983f, 0.0596f, 0.0133f,
    0.0219f, 0.0983f, 0.1621f, 0.0983f, 0.0219f,
    0.0133f, 0.0596f, 0.0983f, 0.0596f, 0.0133f,
    0.003f, 0.0133f, 0.0219f, 0.0133f, 0.003f,
};

static float CrossFilter[13] = { 0.0561f, 0.1353f, 0.278f, 0.4868f, 0.7261f, 0.9231f, 1.f, 0.9231f, 0.7261f, 0.4868f, 0.278f, 0.1353f, 0.0561f };
static float Total = 6.2108f;

struct VS_OUT_BLUR
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
    float InstID : FOG;
};

VS_OUT_BLUR VS_Blur(VS_IN _in, uint _InstID : SV_InstanceID)
{
    VS_OUT_BLUR output = (VS_OUT_BLUR) 0.f;
    
    output.vPosition = float4(_in.vPos * 2.f, 1.f);
    output.vUV = _in.vUV;
    output.InstID = _InstID;
    
    return output;
}

float4 PS_Blur(VS_OUT_BLUR _in) : SV_Target
{
    float4 vColor = float4(0.f, 0.f, 0.f, 0.f);
    
    //float2 vUVStep = 1.f / g_Resolution;
    //vUVStep *= 3.f;
    //
    //if (_in.InstID == 0)
    //{
    //    for (int i = 0; i < 13; ++i)
    //    {
    //        float2 vUV = _in.vUV + float2(vUVStep.x * (-6 + i), 0.f);
    //        vColor += g_tex_0.Sample(g_sam_2, vUV) * CrossFilter[i];
    //    }
    //}
    //else if (_in.InstID == 1)
    //{
    //    for (int j = 0; j < 13; ++j)
    //    {
    //        float2 vUV = _in.vUV + float2(0.f, vUVStep.y * (-6 + j));
    //        vColor += g_tex_0.Sample(g_sam_2, vUV) * CrossFilter[j];
    //    }
    //}
    //vColor /= Total;
    
     // UV 스텝 크기 설정
    float2 vUVStep = 1.f / g_Resolution;
    vUVStep *= 1.f; // Gaussian 필터에서는 3배 확대 대신 1배로 설정
    
    // Gaussian 필터 적용 (5x5 필터)
    for (int i = -2; i <= 2; ++i)
    {
        for (int j = -2; j <= 2; ++j)
        {
            // 필터의 현재 위치에 따른 UV 좌표 계산
            float2 vUV = _in.vUV + float2(vUVStep.x * i, vUVStep.y * j);
            
            // 텍스처 샘플링 및 Gaussian 필터 값 적용
            vColor += g_tex_0.Sample(g_sam_2, vUV) * GaussianFilter[i + 2][j + 2];
        }
    }
    
    return vColor;
}

VS_OUT VS_EffectMerge(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    output.vPosition = float4(_in.vPos * 2.f, 1.f);
    output.vUV = _in.vUV;
    
    return output;
}

float4 PS_EffectMerge(VS_OUT _in) : SV_Target
{
    float4 vColor = float4(0.f, 0.f, 0.f, 0.f);
    
    float4 vEffect = g_tex_0.Sample(g_sam_0, _in.vUV);
    float4 vEffectBlur = g_tex_1.Sample(g_sam_0, _in.vUV);
    
    float4 vBloom = pow(abs(pow(vEffect, 2.2f)) + abs(pow(vEffectBlur, 2.2f)), 1.f / 2.2f);
    
    return vBloom;
}

#endif