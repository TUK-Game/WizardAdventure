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

#endif