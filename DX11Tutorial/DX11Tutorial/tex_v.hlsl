// Global
cbuffer MatrixBuffer
{
	matrix world;
	matrix view;
	matrix proj;
};

// Input struct
struct Input
{
	float4 position : POSITION;
	float2 tex : TEXCOOR0;
};

// Output struct
struct Output
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOOR0;
};

Output main (Input input)
{
	Output output;

	input.position.w = 1.0f;

	output.position = mul(input.position, world);
	output.position = mul(output.position, view);
	output.position = mul(output.position, proj);

	output.tex = input.tex;
	return output;

}