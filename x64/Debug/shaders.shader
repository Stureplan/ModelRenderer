#define MAX_LIGHTS 4

struct LIGHT
{
	float4 pos;
	float4 col;
	float ins;
};

struct VOUT
{
	float4 position : SV_POSITION;
	float4 worldPos	: POSITION;
	float2 texcoord : TEXCOORD;

	float3 tangent	: TANGENT;
	float3 bitangent: BITANGENT;
	float3 normal	: NORMAL;
};

cbuffer CBUFFER_MATRIX
{
	float4x4 wvp;
	float4x4 world;
};

cbuffer CBUFFER_LIGHTS
{
	LIGHT lights[MAX_LIGHTS];
	uint lightnumber;
};


Texture2D tex;
SamplerState smp;

VOUT VShader(float4 position : POSITION, float2 texcoord : TEXCOORD, float3 tangent : TANGENT, float3 bitangent : BITANGENT, float3 normal : NORMAL)
{
	VOUT output;

	output.position = mul(position, wvp);
	output.worldPos = mul(position, world);
	output.texcoord = texcoord;
	output.tangent	= tangent;
	output.bitangent = bitangent;
	output.normal	= mul(normal, world);

	return output;
}

float4 PShader(VOUT input) : SV_TARGET
{
	//return float4(1.0, 0.0, 0.4, 1.0);
	float4 pos = input.position;
	float2 uvs = input.texcoord;
	float3 tng = input.tangent;
	float3 btn = input.bitangent;
	float3 nrm = normalize(input.normal);

	float4 diffuse = tex.Sample(smp, input.texcoord);





	float3 lightToPixel = lights[0].pos - input.worldPos;
	float d = length(lightToPixel);
	float4 final = diffuse * 0.2;

	lightToPixel /= d;

	float lightAmt = dot(lightToPixel, nrm);

	if (lightAmt > 0)
	{
		final += lightAmt * 0.2 * lights[0].col;
	}

	final = saturate(final + (0.2*lights[0].col));




	//final += saturate(dot(float3(1, 0, 0), nrm) * lights[0].col * diffuse);

	return final;
}
