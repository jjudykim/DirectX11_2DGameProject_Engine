#ifndef _STRUCT
#define _STRUCT

struct tLight
{
    float4 Color;
    float4 Ambient;
};

struct tLightInfo
{
    tLight    light;
    float3    WorldPos;
    float3    WorldDir;
    float     Radius;
    float     Angle;
    uint      Type;
    int3      padding;
};


#endif