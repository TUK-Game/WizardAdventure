#ifndef _SKYBOX_HLSLI_
#define _SKYBOX_HLSLI_

//#include "params.hlsli"

cbuffer TRANSFORM_PARAMS : register(b1)
{
    row_major matrix matWorld;
    row_major matrix matView;
    row_major matrix matProjection;
    row_major matrix matWV;
    row_major matrix matWVP;
};

cbuffer MATERIAL_PARAMS : register(b2)
{
    int int_0;
    int int_1;
    int int_2;
    int int_3;
    int int_4;
    float float_0;
    float float_1;
    float float_2;
    float float_3;
    float float_4;
};

TextureCube cubmap : register(t0);

SamplerState sam_0 : register(s0);

struct VS_IN
{
    float3 localPos : POSITION;
    float2 uv : TEXCOORD;
};

struct VS_OUT
{
    float4 pos : SV_Position;
    float3 position : Position;
    float2 uv : TEXCOORD;
};

VS_OUT VS_Main(VS_IN input)
{
    VS_OUT output = (VS_OUT) 0;

    // Translation은 하지 않고 Rotation만 적용한다
    float4 viewPos = mul(float4(input.localPos, 0), matView);
    float4 clipSpacePos = mul(viewPos, matProjection);

    // w/w=1이기 때문에 항상 깊이가 1로 유지된다
    output.pos = clipSpacePos.xyww;
    output.position = input.localPos;
    output.uv = input.uv;

    return output;
}

float4 PS_Main(VS_OUT input) : SV_Target
{
    float4 color = cubmap.Sample(sam_0, input.position);
    return color;
}

#endif