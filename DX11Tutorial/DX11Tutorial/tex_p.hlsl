// Gloabl
//Texture2D texture2D;
//SamplerState state; // How the shader sample the texture

struct Input
{
	float4 position : SV_POSITION;
	float4 tex : TEXCOOR0;
};

float4 main (Input input) : SV_TARGET
{
	float o = (1,1,1,1);
	
	// o = texture2D.Sample(state, input.tex);

	return o;
}