#ifndef _PARAMS_HLSL_
#define _PARAMS_HLSL_

struct LightColor
{
    float4 diffuse;
    float4 ambient;
    float4 specular;
};

struct LightInfo
{
    LightColor color;
    float4 position;
    float4 direction;
    int lightType;
    float range;
    float angle;
    int padding;
};

//cbuffer GLOBAL_PARAMS : register(b0)
//{
//    int lightCount;
//    float3 lightPadding;
//    LightInfo light[50];
//}

cbuffer TRANSFORM_PARAMS : register(b0)
{
    row_major matrix matWorld;
    row_major matrix matView;
    row_major matrix matProjection;
    row_major matrix matWV;
    row_major matrix matWVP;
};

cbuffer MATERIAL_PARAMS : register(b1)
{
    int int_0;
    int int_1;
    int int_2;
    int int_3;
    float float_0;
    float float_1;
    float float_2;
    float float_3;
    int tex_on_0;
    int tex_on_1;
    int tex_on_2;
    int tex_on_3;
    float2 vec2_0;
    float2 vec2_1;
    float2 vec2_2;
    float2 vec2_3;
};

Texture2D tex_0 : register(t0);
Texture2D tex_1 : register(t1);
Texture2D tex_2 : register(t2);
Texture2D tex_3 : register(t3);
Texture2D tex_4 : register(t4);

SamplerState sam_0 : register(s0);

#endif