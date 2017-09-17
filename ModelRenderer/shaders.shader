cbuffer CBUFFER
{
	float4x4 WVP;
};

struct VOut
{
	float4 position : SV_POSITION;
};

VOut VShader(float4 position : POSITION)
{
	VOut output;

	output.position = mul(position, WVP);

	return output;
}


float4 PShader(VOut input) : SV_TARGET
{
	//return float4(1.0, 0.0, 0.4, 1.0);
	float4 pos = input.position;
	return float4(pos.x, pos.y, pos.z, 1.0);
}
