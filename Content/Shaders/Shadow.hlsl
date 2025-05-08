#ifndef _SHADOW_HLSL_
#define _SHADOW_HLSL_

#include "Params.hlsl"
#include "Utils.hlsl"

struct VS_IN
{
	float3 pos : POSITION;
    float2 uv : TEXCOORD;

    row_major matrix Instance_matWorld : W;
    row_major matrix Instance_matWV : WV;
    row_major matrix Instance_matWVP : WVP;
};

struct VS_AIN
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
};

struct VS_OUT
{
	float4 pos : SV_POSITION;
    float2 uv : TEXCOORD;
	float4 clipPos : POSITION;
};

VS_OUT VS_MAIN(VS_IN input)
{
	VS_OUT output = (VS_OUT)0.f;

    output.uv = input.uv; 

    if (int_0 == 1)
    {
        output.pos = mul(float4(input.pos, 1.f), input.Instance_matWVP);
        output.clipPos = output.pos;
    }
    else
    {
        output.pos = mul(float4(input.pos, 1.f), matWVP);
        output.clipPos = output.pos;
    }

	return output;
}

///////////////////////// ANIMATION //////////////////////////
VS_OUT VS_ANIM(VS_AIN input)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    Skinning(input.pos, input.normal, input.tangent, input.weight, input.indices);
    output.pos = mul(float4(input.pos, 1.f), matWVP);
    output.clipPos = output.pos;
    output.uv = input.uv;
    
    return output;
}


float4 PS_MAIN(VS_OUT input) : SV_Target
{
    if (int_1) {    // dissolve ?
        float noise = tex_1.Sample(sam_0, input.uv).r;
        if (noise < float_0)
           discard;
    }

	return float4(input.clipPos.z / input.clipPos.w, 0.f, 0.f, 0.f);
}
#endif