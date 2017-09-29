cbuffer CBUFFER
{
	float4x4 WVP;
};

struct VOut
{
	float4 position : SV_POSITION;
	float2 texcoord : TEXCOORD;

	float3 tangent	: TANGENT;
	float3 bitangent: BITANGENT;
	float3 normal	: NORMAL;
};

Texture2D tex;
SamplerState smp;

VOut VShader(float4 position : POSITION, float2 texcoord : TEXCOORD, float3 tangent : TANGENT, float3 bitangent : BITANGENT, float3 normal : NORMAL)
{
	VOut output;

	output.position = mul(position, WVP);
	output.texcoord = texcoord;
	output.tangent	= tangent;
	output.bitangent = bitangent;
	output.normal	= normal;


	return output;
}

float4 PShader(VOut input) : SV_TARGET
{
	//return float4(1.0, 0.0, 0.4, 1.0);
	float4 pos = input.position;
	float2 uv = input.texcoord;
	float3 tng = input.tangent;
	float3 btn = input.bitangent;
	float3 nrm = input.normal;

	//return float4(uv.x, uv.y, pos.z, 1.0);
	//return float4(pos.x, pos.y, pos.z, 1.0);

	//return tex.Sample(smp, input.texcoord);
	return float4(nrm, 1.0);
}
