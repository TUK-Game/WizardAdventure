#ifndef _LIGHTING_HLSL_
#define _LIGHTING_HLSL_

#include "Params.hlsl"
#include "Utils.hlsl"

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

    float3 viewPos = tex_0.Sample(sam_0, input.uv).xyz;
    if (viewPos.z <= 0.f)
        clip(-1);

    float3 viewNormal = tex_1.Sample(sam_0, input.uv).xyz;

    LightColor color = CalculateLightColor(int_0, viewNormal, viewPos);

    // �׸���
    if (length(color.diffuse) != 0)
    {
        matrix shadowCameraVP = mat_0;

        float4 worldPos = mul(float4(viewPos.xyz, 1.f), matViewInv);
        float4 shadowClipPos = mul(worldPos, shadowCameraVP);
        float depth = shadowClipPos.z / shadowClipPos.w;

        // x [-1 ~ 1] -> u [0 ~ 1]
        // y [1 ~ -1] -> v [0 ~ 1]
        float2 uv = shadowClipPos.xy / shadowClipPos.w;
        uv.y = -uv.y;
        uv = uv * 0.5 + 0.5;

        if (0 < uv.x && uv.x < 1 && 0 < uv.y && uv.y < 1)
        {
            float shadowDepth = tex_2.Sample(sam_0, uv).x;
            if (shadowDepth > 0 && depth > shadowDepth + 0.0005f)
            {
                color.diffuse *= 0.5f;
                color.specular = (float4) 0.f;
            }
        }
    }

    output.diffuse = color.diffuse + color.ambient;
    output.specular = color.specular;

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

    // input.pos = SV_Position = Screen ��ǥ
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