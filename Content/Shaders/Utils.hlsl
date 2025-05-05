#ifndef _UTILS_HLSL_
#define _UTILS_HLSL_

#include "Params.hlsl"

LightColor CalculateLightColor(int lightIndex, float3 viewNormal, float3 viewPos)
{
    LightColor color = (LightColor) 0.f;

    float3 viewLightDir = (float3) 0.f;

    float diffuseRatio = 0.f;
    float specularRatio = 0.f;
    float distanceRatio = 1.f;

    if (light[lightIndex].lightType == 0)
    {
        // Directional Light
        viewLightDir = normalize(mul(float4(light[lightIndex].direction.xyz, 0.f), matView).xyz);
        diffuseRatio = saturate(dot(-viewLightDir, viewNormal));
    }
    else if (light[lightIndex].lightType == 1)
    {
        // Point Light
        float3 viewLightPos = mul(float4(light[lightIndex].position.xyz, 1.f), matView).xyz;
        viewLightDir = normalize(viewPos - viewLightPos);
        diffuseRatio = saturate(dot(-viewLightDir, viewNormal));

        float dist = distance(viewPos, viewLightPos);
        if (light[lightIndex].range == 0.f)
            distanceRatio = 0.f;
        else
            distanceRatio = saturate(1.f - pow(dist / light[lightIndex].range, 2));
    }
    else
    {
        // Spot Light
        float3 viewLightPos = mul(float4(light[lightIndex].position.xyz, 1.f), matView).xyz;
        viewLightDir = normalize(viewPos - viewLightPos);
        diffuseRatio = saturate(dot(-viewLightDir, viewNormal));

        if (light[lightIndex].range == 0.f)
            distanceRatio = 0.f;
        else
        {
            float halfAngle = light[lightIndex].angle / 2;

            float3 viewLightVec = viewPos - viewLightPos;
            float3 viewCenterLightDir = normalize(mul(float4(light[lightIndex].direction.xyz, 0.f), matView).xyz);

            float centerDist = dot(viewLightVec, viewCenterLightDir);
            distanceRatio = saturate(1.f - centerDist / light[lightIndex].range);

            float lightAngle = acos(dot(normalize(viewLightVec), viewCenterLightDir));

            if (centerDist < 0.f || centerDist > light[lightIndex].range) // 최대 거리를 벗어났는지
                distanceRatio = 0.f;
            else if (lightAngle > halfAngle) // 최대 시야각을 벗어났는지
                distanceRatio = 0.f;
            else // 거리에 따라 적절히 세기를 조절
                distanceRatio = saturate(1.f - pow(centerDist / light[lightIndex].range, 2));
        }
    }

    float3 reflectionDir = normalize(viewLightDir + 2 * (saturate(dot(-viewLightDir, viewNormal)) * viewNormal));
    float3 eyeDir = normalize(viewPos);
    specularRatio = saturate(dot(-eyeDir, reflectionDir));
    specularRatio = pow(specularRatio, 2);

    color.diffuse = light[lightIndex].color.diffuse * diffuseRatio * distanceRatio;
    color.ambient = light[lightIndex].color.ambient * distanceRatio;
    color.specular = light[lightIndex].color.specular * specularRatio * distanceRatio;

    return color;
}

float Rand(float2 co)
{
    return 0.5 + (frac(sin(dot(co.xy, float2(12.9898, 78.233))) * 43758.5453)) * 0.5;
}

struct SkinningInfo
{
    float3 pos;
    float3 normal;
    float3 tangent;
};

void Skinning(inout float3 pos, inout float3 normal, inout float3 tangent,
    inout float4 weight, inout float4 indices)
{
    SkinningInfo info = (SkinningInfo) 0.f;

    for (int i = 0; i < 4; ++i)
    {
        if (weight[i] == 0.f)
            continue;

        int boneIdx = indices[i];
        matrix matBone = mat_bone[boneIdx];

        info.pos += (mul(float4(pos, 1.f), matBone) * weight[i]).xyz;
        info.normal += (mul(float4(normal, 0.f), matBone) * weight[i]).xyz;
        info.tangent += (mul(float4(tangent, 0.f), matBone) * weight[i]).xyz;
    }

    pos = info.pos;
    tangent = normalize(info.tangent);
    normal = normalize(info.normal);
}

float4 Fog(float4 cColor, float3 vPosition)
{
    float2 vCameraPosition = cameraPosition.xz;
    float2 vPositionToCamera = vCameraPosition - vPosition.xz;
    float fDistanceToCamera = length(vPositionToCamera);
    
    float fFogRange = fogRange;
    float fFogFactor = saturate((fDistanceToCamera - fogStart) / fFogRange);
    
    float4 cColorByFog = lerp(cColor, fogColor, fFogFactor);
    return cColorByFog;
}


#endif