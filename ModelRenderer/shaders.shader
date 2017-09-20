cbuffer CBUFFER
{
	float4x4 WVP;
};

struct VOut
{
	float4 position : SV_POSITION;
	float2 texcoord : TEXCOORD;
};

VOut VShader(float4 position : POSITION, float2 texcoord : TEXCOORD)
{
	VOut output;

	output.position = mul(position, WVP);
	output.texcoord = texcoord;

	return output;
}

float4 PShader(VOut input) : SV_TARGET
{
	//return float4(1.0, 0.0, 0.4, 1.0);
	float4 pos = input.position;
	float2 uv = input.texcoord;
	//return float4(uv.x, uv.y, pos.z, 1.0);
	return float4(pos.x, pos.y, pos.z, 1.0);
}
