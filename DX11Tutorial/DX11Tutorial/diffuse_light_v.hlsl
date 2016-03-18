cbuffer MatrixBuffer
{
	matrix world;
	matrix view;
	matrix proj;
}

struct Input
{
	float4 position : POSITION;
	float2 uv : TEXCOORD0;
	float3 normal : NORMAL;
};

struct Output
{
	float4 position : SV_POSITION;
	float2 uv : TEXCOORD0;
	float3 normal : NORMAL;
};

Output main (Input input)
{
	Output output;

	input.position.w = 1.0f;

	output.position = mul(input.position, world);
	output.position = mul(output.position, view);
	output.position = mul(output.position, proj);

	output.uv = input.uv;

	// translate to world space
	output.normal = normalize(mul(input.normal, (float3x3) world));

	return output;
}