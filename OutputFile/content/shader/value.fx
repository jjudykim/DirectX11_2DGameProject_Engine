#ifndef _VALUE
#define _VALUE

#include "struct.fx"

#define PI 3.1415926535f

// 4096
cbuffer OBJECT_POS : register(b0)
{
    row_major matrix matWorld;
    row_major matrix matView;
    row_major matrix matProj;
    
    row_major matrix matWV;
    row_major matrix matWVP;
};

cbuffer MATERIAL : register(b1)
{
    int g_int_0;
    int g_int_1;
    int g_int_2;
    int g_int_3;                         // 16
    
    float g_float_0;
    float g_float_1;
    float g_float_2;
    float g_float_3;                     // 16 (32)
    
    float2 g_vec2_0;
    float2 g_vec2_1;
    float2 g_vec2_2;
    float2 g_vec2_3;                     // 32 (64)
    
    float4 g_vec4_0;
    float4 g_vec4_1;
    float4 g_vec4_2;
    float4 g_vec4_3;                     // 64 (128)
    
    row_major matrix g_mat_0;
    row_major matrix g_mat_1;
    row_major matrix g_mat_2;
    row_major matrix g_mat_3;            // 256 (384)
    
    int g_btex_0;
    int g_btex_1;
    int g_btex_2;
    int g_btex_3;
    
    int g_btex_4;
    int g_btex_5;
    int g_btex_6;
    int g_btex_7;
    
    int g_btex_8;
    int g_btex_9;
    int g_btex_10;
    int g_btex_11;
    
    int g_btex_12;
    int g_btex_13;
    int g_btex_14;
    int g_btex_15;                       // 64 (448)
    
    int g_btexcube_0;                    
    int g_btexcube_1;
    int g_btexarr_0;
    int g_btexarr_1;                     // 16 (464)
}

cbuffer SPRITE_INFO : register(b2)
{
    float2  LeftTopUV;
    float2  SliceUV;
    float2  BackGroundUV;
    float2  OffsetUV;
    int     UseFlipbook;
    int3    SpriteInfoPadding;
}

cbuffer GLOBAL_DATA : register(b3)
{
    // �ð� ���� ����
    float g_DT;
    float g_EngineDT;
    float g_Time;
    float g_EngineTime;
    
    // ����Ÿ���� �ػ� ����
    float2 g_Resolution;
    
    // ���ε� �� ����ȭ���ۿ� ������ �� �� ����ִ���
    int g_Light2DCount;
    int g_Light3DCount;
}

SamplerState g_sam_0   : register(s0);      // �̹漺 ���� ���÷�
SamplerState g_sam_1   : register(s1);      // ����Ʈ ���� ���÷�

Texture2D          g_tex_0  : register(t0);
Texture2D          g_tex_1  : register(t1);
Texture2D          g_tex_2  : register(t2);
Texture2D          g_tex_3  : register(t3);
Texture2D          g_tex_4  : register(t4);
Texture2D          g_tex_5  : register(t5);
Texture2D          g_tex_6  : register(t6);
Texture2D          g_tex_7  : register(t7);
Texture2D          g_tex_8  : register(t8);
Texture2D          g_tex_9  : register(t9);
Texture2D          g_tex_10 : register(t10);
Texture2D          g_tex_11 : register(t11);
Texture2D          g_tex_12 : register(t12);
Texture2D          g_tex_13 : register(t13);
Texture2D          g_tex_14 : register(t14);
Texture2D          g_tex_15 : register(t15);

TextureCube        g_texcube_0 : register(t16);
TextureCube        g_texcube_1 : register(t17);

Texture2DArray     g_texarr_0  : register(t18);
Texture2DArray     g_texarr_1  : register(t19);

Texture2D          g_AtalsTex : register(t20);

StructuredBuffer<tLightInfo> g_Light2D : register(t21);
StructuredBuffer<tLightInfo> g_Light3D : register(t22);

#endif