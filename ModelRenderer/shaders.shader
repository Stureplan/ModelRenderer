struct VOut
{
	float4 position : SV_POSITION;
};

VOut VShader(float4 position : POSITION)
{
	VOut output;

	output.position = position;

	return output;
}


float4 PShader(float4 position : SV_POSITION) : SV_TARGET
{
	return float4(1.0, 0.0, 0.4, 1.0);
	//return float4(position.x, position.y, position.z, 1.0);
}
