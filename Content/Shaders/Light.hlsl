#ifndef _LIGHTING_HLSL_
#define _LIGHTING_HLSL_

#define CASCADE_COUNT 4

#include "Params.hlsl"
#include "Utils.hlsl"

cbuffer CascadeShadowData : register(b4)
{
    matrix matCascadeVP[CASCADE_COUNT];
    float splitDistances[CASCADE_COUNT];
}

int GetCascadeIndex(float viewDepth)
{
    for (int i = 0; i < 4; ++i)
    {
        if (viewDepth < splitDistances[i])
            return i;
    }
    return 3;
}

Texture2D tex_ShadowMaps[CASCADE_COUNT] : register(t8);

struct VS_IN
{
    float3 pos : POSITION;
    float2 uv : TEXCOORD;
};

struct VS_OUT
{
    float4 pos : SV_Position;
    float2 uv : TEXCOORD;
};

struct PS_OUT
{
    float4 diffuse : SV_Target0;
    float4 specular : SV_Target1;
};

// [Directional Light]
// int_0 : Light index
// tex_0 : Position RT
// tex_1 : Normal RT
// tex_2 : Shadow RT
// mat_0 : ShadowCamera VP
// Mesh : Rectangle

VS_OUT VS_DirLight(VS_IN input)
{
    VS_OUT output = (VS_OUT) 0;

    output.pos = float4(input.pos * 2.f, 1.f);
    output.uv = input.uv;

    return output;
}

PS_OUT PS_DirLight(VS_OUT input)
{
    PS_OUT output = (PS_OUT) 0;

    //float3 viewPos = tex_0.Sample(sam_0, input.uv).xyz;

    //if (viewPos.z <= 0.f)
    //    clip(-1);

    //float3 viewNormal = tex_1.Sample(sam_0, input.uv).xyz;

    //LightColor color = CalculateLightColor(int_0, viewNormal, viewPos);

    //if (length(color.diffuse) != 0)
    //{
    //    int cascadeIdx = GetCascadeIndex(viewPos.z);
    //    matrix shadowVP = matCascadeVP[cascadeIdx];

    //    float4 worldPos = mul(float4(viewPos.xyz, 1.f), matViewInv);
    //    float4 shadowPos = mul(worldPos, shadowVP);
    //    shadowPos.xyz /= shadowPos.w;

    //    float2 shadowUV = shadowPos.xy * 0.5f + 0.5f;

    //    if (all(shadowUV > 0.0f) && all(shadowUV < 1.0f))
    //    {
    //        float shadowDepth = tex_ShadowMaps[cascadeIdx].Sample(sam_0, shadowUV).r;
    //        float currentDepth = shadowPos.z;

    //        if (currentDepth > shadowDepth + 0.00001f)
    //        {
    //            color.diffuse *= 0.4f;
    //            color.specular = 0;
    //        }
    //    }
    //}

    //output.diffuse = color.diffuse + color.ambient;
    //output.specular = color.specular;

    //return output;



    float3 viewPos = tex_0.Sample(sam_0, input.uv).xyz;
    if (viewPos.z <= 0.f)
        clip(-1);

    float viewDepth = viewPos.z;

    // splitDistances¿¡ µû¶ó »ö»ó Ãâ·Â
    float4 debugColor;
    if (viewDepth < splitDistances[0])
        debugColor = float4(1, 1, 1, 1); // ¿¡·¯ ¶Ç´Â ºñÁ¤»ó
    else if (viewDepth < splitDistances[1])
        debugColor = float4(1, 0, 0, 1); // Cascade 0 (»¡°­)
    else if (viewDepth < splitDistances[2])
        debugColor = float4(0, 1, 0, 1); // Cascade 1 (ÃÊ·Ï)
    else if (viewDepth < splitDistances[3])
        debugColor = float4(0, 0, 1, 1); // Cascade 2 (ÆÄ¶û)
    else
        debugColor = float4(1, 1, 0, 1); // Cascade 3 (³ë¶û)

    output.diffuse = debugColor;
    output.specular = float4(0, 0, 0, 0); // ½ºÆäÅ§·¯´Â ¾È ¾¸
    return output;
}

// [Point Light]
// int_0 : Light index
// tex_0 : Position RT
// tex_1 : Normal RT
// vec2_0 : RenderTarget Resolution
// Mesh : Sphere

VS_OUT VS_PointLight(VS_IN input)
{
    VS_OUT output = (VS_OUT) 0;

    output.pos = mul(float4(input.pos, 1.f), matWVP);
    output.uv = input.uv;

    return output;
}

PS_OUT PS_PointLight(VS_OUT input)
{
    PS_OUT output = (PS_OUT) 0;

    // input.pos = SV_Position = Screen ÁÂÇ¥
    float2 uv = float2(input.pos.x / vec2_0.x, input.pos.y / vec2_0.y);
    float3 viewPos = tex_0.Sample(sam_0, uv).xyz;
    if (viewPos.z <= 0.f)
        clip(-1);

    int lightIndex = int_0;
    float3 viewLightPos = mul(float4(light[lightIndex].position.xyz, 1.f), matView).xyz;
    float distance = length(viewPos - viewLightPos);
    if (distance > light[lightIndex].range)
        clip(-1);

    float3 viewNormal = tex_1.Sample(sam_0, uv).xyz;

    LightColor color = CalculateLightColor(int_0, viewNormal, viewPos);

    output.diffuse = color.diffuse + color.ambient;
    output.specular = color.specular;

    return output;
}

// [Final]
// tex_0 : Diffuse Color Target
// tex_1 : Diffuse Light Target
// tex_2 : Specular Light Target
// Mesh : Rectangle

VS_OUT VS_Final(VS_IN input)
{
    VS_OUT output = (VS_OUT) 0;

    output.pos = float4(input.pos * 2.f, 1.f);
    output.uv = input.uv;

    return output;
}

float4 PS_Final(VS_OUT input) : SV_Target
{
    float4 output = (float4) 0;

    float4 lightPower = tex_1.Sample(sam_0, input.uv);
    if (lightPower.x == 0.f && lightPower.y == 0.f && lightPower.z == 0.f)
        clip(-1);

    float4 color = tex_0.Sample(sam_0, input.uv);
    float4 specular = tex_2.Sample(sam_0, input.uv);

    output = (color * lightPower) + specular;
    return output;
}



#endif