// Gloabl
Texture2D tex2D;
SamplerState state; // How the shader sample the texture

struct Input
{
	float4 position : SV_POSITION;
	float4 uv : TEXCOOR0;
};

float4 main (Input input) : SV_TARGET
{
	float4 output = tex2D.Sample(state, input.uv);

	return output;
}