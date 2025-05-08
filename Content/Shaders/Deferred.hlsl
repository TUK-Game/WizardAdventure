#ifndef _DEFERRED_HLSL_
#define _DEFERRED_HLSL_

#include "Params.hlsl"
#include "Utils.hlsl"

struct VS_IN
{
    float3 pos : POSITION;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float4 weight : WEIGHT;
    float4 indices : INDICES;

    row_major matrix Instance_matWorld : W;
    row_major matrix Instance_matWV : WV;
    row_major matrix Instance_matWVP : WVP;
    uint instanceID : SV_InstanceID;
};

struct VS_OUT
{
    float4 pos : SV_Position;
    float2 uv : TEXCOORD;
    float3 viewPos : POSITION;
    float3 viewNormal : NORMAL;
    float3 viewTangent : TANGENT;
    float3 viewBinormal : BINORMAL;
    float3 worldPos : POSITION2;
};

VS_OUT VS_Main(VS_IN input)
{
    VS_OUT output = (VS_OUT) 0;
    if (int_0 == 1)
    {
        if (int_1 == 1)
            Skinning(input.pos, input.normal, input.tangent, input.weight, input.indices);
        
        output.pos = mul(float4(input.pos, 1.f), input.Instance_matWVP);
        output.uv = input.uv;

        output.viewPos = mul(float4(input.pos, 1.f), input.Instance_matWV).xyz;
        output.viewNormal = normalize(mul(float4(input.normal, 0.f), input.Instance_matWV).xyz);
        output.viewTangent = normalize(mul(float4(input.tangent, 0.f), input.Instance_matWV).xyz);
        output.viewBinormal = normalize(cross(output.viewTangent, output.viewNormal));
        output.worldPos = mul(float4(input.pos, 1.f), input.Instance_matWorld).xyz;

    }
    else
    {
        if (int_1 == 1)
            Skinning(input.pos, input.normal, input.tangent, input.weight, input.indices);
        
        output.pos = mul(float4(input.pos, 1.f), matWVP);
        output.uv = input.uv;

        output.viewPos = mul(float4(input.pos, 1.f), matWV).xyz;
        output.viewNormal = normalize(mul(float4(input.normal, 0.f), matWV).xyz);
        output.viewTangent = normalize(mul(float4(input.tangent, 0.f), matWV).xyz);
        output.viewBinormal = normalize(cross(output.viewTangent, output.viewNormal));
        output.worldPos = mul(float4(input.pos, 1.f), matWorld).xyz;
    }
    
    return output;
}

struct PS_OUT
{
    float4 position : SV_Target0;
    float4 normal : SV_Target1;
    float4 color : SV_Target2;
};

PS_OUT PS_Main(VS_OUT input)
{
    PS_OUT output = (PS_OUT) 0;

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

    
    output.position = float4(input.viewPos.xyz, 0.f);
    output.normal = float4(viewNormal.xyz, 0.f);
    //if (input.worldPos.y < -10.f)
    color = Fog(color, input.worldPos.xyz);
    output.color = color;

    return output;
}

// dissolve

PS_OUT PS_Dissolve(VS_OUT input) 
{
    PS_OUT output = (PS_OUT)0;

    float4 baseColor = float4(1.f, 1.f, 1.f, 1.f);
    if (tex_on_0)
        baseColor = tex_0.Sample(sam_0, input.uv);

    // 1. noise
    float noise = tex_2.Sample(sam_0, input.uv).r; // 디졸브용 노이즈

    // 2. 디졸브 discard 처리
    if (noise < float_0)
        discard;

    // 3. 엣지 강조 효과
    float edge = step(noise, float_0 + 0.02) - step(noise, float_0);
    float4 finalColor = lerp(baseColor, vec4_0, edge);

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

    output.position = float4(input.viewPos.xyz, 0.f);
    output.normal = float4(viewNormal.xyz, 0.f);
    output.color = finalColor;

    return output;
}

struct PS_MAPOUT
{
    float4 color : SV_Target0;
};

PS_MAPOUT PS_Map(VS_OUT input)
{
    PS_MAPOUT output = (PS_MAPOUT) 0;

    float4 color = float4(1.f, 1.f, 1.f, 1.f);
    if (tex_on_0)   
        color = tex_0.Sample(sam_0, input.uv);



    output.color = color;
    output.color.a = 0.9;
    return output;
}

PS_MAPOUT PS_MapBack(VS_OUT input)
{
    PS_MAPOUT output = (PS_MAPOUT) 0;

    float4 color = float4(1.f, 1.f, 1.f, 0.3f);
    if (tex_on_0)   
        color = tex_0.Sample(sam_0, input.uv);

    output.color = color;
    output.color.a = 0.3;
    return output;
}
#endif

