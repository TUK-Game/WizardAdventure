#ifndef _FORWARD_HLSL_
#define _FORWARD_HLSL_

#include "Params.hlsl"
#include "Utils.hlsl"

struct VS_IN
{
    float3 pos : POSITION;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
};

struct VS_OUT
{
    float4 pos : SV_Position;
    float2 uv : TEXCOORD;
    float3 viewPos : POSITION;
    float3 viewNormal : NORMAL;
    float3 viewTangent : TANGENT;
    float3 viewBinormal : BINORMAL;
};

VS_OUT VS_Main(VS_IN input)
{
    VS_OUT output = (VS_OUT) 0;

    output.pos = mul(float4(input.pos, 1.f), matWVP);
    output.uv = input.uv;

    output.viewPos = mul(float4(input.pos, 1.f), matWV).xyz;
    output.viewNormal = normalize(mul(float4(input.normal, 0.f), matWV).xyz);
    output.viewTangent = normalize(mul(float4(input.tangent, 0.f), matWV).xyz);
    output.viewBinormal = normalize(cross(output.viewTangent, output.viewNormal));

    return output;
}

float4 PS_Main(VS_OUT input) : SV_Target
{
    float4 color = float4(1.f, 1.f, 1.f, 1.f);
    if (tex_on_0)
        color = tex_0.Sample(sam_0, input.uv);

    float3 viewNormal = input.viewNormal;
    if (tex_on_1)
    {
        // [0,255] 범위에서 [0,1]로 변환
        float3 tangentSpaceNormal = tex_1.Sample(sam_0, input.uv).xyz;
        // [0,1] 범위에서 [-1,1]로 변환
        tangentSpaceNormal = (tangentSpaceNormal - 0.5f) * 2.f;
        float3x3 matTBN = { input.viewTangent, input.viewBinormal, input.viewNormal };
        viewNormal = normalize(mul(tangentSpaceNormal, matTBN));
    }

    LightColor totalColor = (LightColor) 0.f;

    for (int i = 0; i < lightCount; ++i)
    {
        LightColor color = CalculateLightColor(i, viewNormal, input.viewPos);
        totalColor.diffuse += color.diffuse;
        totalColor.ambient += color.ambient;
        totalColor.specular += color.specular;
    }

    color.xyz = (totalColor.diffuse.xyz * color.xyz)
        + totalColor.ambient.xyz * color.xyz
        + totalColor.specular.xyz;
    color = Fog(color, input.pos.xyz);

    return color;
}

// [Texture Shader]
// g_tex_0 : Output Texture
// AlphaBlend : true
struct VS_TEX_IN
{
    float3 pos : POSITION;
    float2 uv : TEXCOORD;
};

struct VS_TEX_OUT
{
    float4 pos : SV_Position;
    float2 uv : TEXCOORD;
};

struct VS_TEX_OUT_WORLD
{
    float4 pos : SV_Position;
    float2 uv : TEXCOORD;
};

VS_TEX_OUT VS_Tex(VS_TEX_IN input)
{
    VS_TEX_OUT output = (VS_TEX_OUT) 0;

    output.pos = mul(float4(input.pos, 1.f), matWorld);
    //output.uv = input.uv;

    // 정규화된 Screen Space 좌표 설정 (-1 ~ 1 범위)
    //output.pos = float4(input.pos.xy, 0.0f, 1.0f);
    output.uv = input.uv;

    return output;
}

float4 PS_Tex(VS_TEX_OUT input) : SV_Target
{
    float4 color = float4(1.f, 1.f, 1.f, 1.f);
    if (tex_on_0)
        color = tex_0.Sample(sam_0, input.uv);

    return color;
}


float4 PS_TexSkill(VS_TEX_OUT input) : SV_Target
{
    float4 skillColor = tex_0.Sample(sam_0, input.uv);

    float2 delta = input.uv - float2(0.5f, 0.5f);
    float angle = atan2(delta.x, delta.y);
    float normalizedAngle = (angle + PI) / (2.0f * PI);

    if (normalizedAngle <= (float_0))
    {
        float4 overlayColor = float4(0.0f, 0.0f, 0.0f, 0.5f);
        return lerp(skillColor, overlayColor, overlayColor.a);
    }
    
    return skillColor;
}

VS_TEX_OUT_WORLD VS_TexWorld(VS_TEX_IN input)
{
    VS_TEX_OUT_WORLD output = (VS_TEX_OUT_WORLD) 0;

    float4 worldPos = mul(float4(input.pos, 1.f), matWorld);
    float4 viewPos = mul(worldPos, matView);
    output.pos = mul(viewPos, matProjection);

    output.uv = input.uv;
    return output;
}

float4 PS_TexWorld(VS_TEX_OUT_WORLD input) : SV_Target
{
    float4 color = float4(1.f, 1.f, 1.f, 1.f);

    if (tex_on_0)
        color = tex_0.Sample(sam_0, input.uv);

    return color;
}


// [BillboardAnimated Shader]
// Topology: POINTLIST
// g_tex_0 : Output Texture
// AlphaBlend : true
// int_1 : spriteX 
// int_2 : spriteY 
// int_3 : currentFrame
// float_0 : start scale
// float_1 : end scale
// float_2 : current frame (for scale)
// float_3 : total frame count
// vec2_2 = (startAlpha, endAlpha)

struct VS_BILLBOARD_IN
{
    float3 pos : POSITION;
};

struct VS_BILLBOARD_OUT
{
    float3 worldPos : TEXCOORD0;
};

struct PS_IN
{
    float4 pos : SV_Position;
    float2 uv : TEXCOORD;
};

VS_BILLBOARD_OUT VS_BillboardAnimated(VS_BILLBOARD_IN input)
{
    VS_BILLBOARD_OUT output = (VS_BILLBOARD_OUT) 0;
    output.worldPos = mul(float4(input.pos, 1.f), matWorld).xyz;
    return output;
}

[maxvertexcount(6)]
void GS_BillboardAnimated(point VS_BILLBOARD_OUT input[1], inout TriangleStream<PS_IN> triStream)
{
    float3 center = input[0].worldPos;

    float3 look = normalize(center - cameraPosition);
    float3 up = float3(0, 1, 0);
    float3 right = normalize(cross(up, look));
    float3 trueUp = cross(look, right);

    float scale = lerp(float_0, float_1, float(float_2) / float(float_3));

    float3 corners[4] =
    {
        center + (-right - trueUp) * scale, // 0: bottom left
        center + (-right + trueUp) * scale, // 1: top left
        center + (right + trueUp) * scale, // 2: top right
        center + (right - trueUp) * scale // 3: bottom right
    };

    float2 uvs[4] = { float2(0, 1), float2(0, 0), float2(1, 0), float2(1, 1) };
    int indices[6] = { 0, 1, 2, 0, 2, 3 };

    for (int i = 0; i < 6; ++i)
    {
        int idx = indices[i];
        PS_IN o;
        o.pos = mul(float4(corners[idx], 1.0f), mul(matView, matProjection));
        o.uv = uvs[idx];
        triStream.Append(o);
    }
}

float4 PS_BillboardAnimated(PS_IN input) : SV_Target
{
    int spriteX = int_1;
    int spriteY = int_2;
    int currentFrame = int_3;

    float2 frameUV = input.uv / float2(spriteX, spriteY);
    int x = currentFrame % spriteX;
    int y = currentFrame / spriteX;
    float2 offset = float2(x, y) / float2(spriteX, spriteY);
    float2 finalUV = frameUV + offset;

    float4 color = tex_0.Sample(sam_0, finalUV);

    // 알파 보간
    float alpha = lerp(vec2_2.x, vec2_2.y, float(float_2) / float(float_3));
    color.a *= alpha;

    return color;
}
#endif
