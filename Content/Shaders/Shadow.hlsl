#ifndef _SHADOW_HLSL_
#define _SHADOW_HLSL_

#include "Params.hlsl"

struct VS_IN
{
	float3 pos : POSITION;
};

struct VS_OUT
{
	float4 pos : SV_POSITION;
	float4 clipPos : POSITION;
};

VS_OUT VS_MAIN(VS_IN input)
{
	VS_OUT output = (VS_OUT)0.f;
	output.pos = mul(float4(input.pos, 1.f), matWVP);
	output.clipPos = output.pos;

	return output;
}

float4 PS_MAIN(VS_OUT input) : SV_Target
{
	return float4(input.clipPos.z / input.clipPos.w, 0.f, 0.f, 0.f);
}
#endif