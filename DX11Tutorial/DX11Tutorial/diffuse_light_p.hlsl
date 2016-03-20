Texture2D tex2D;
SamplerState state;

cbuffer LightBuffer
{
	float4 diffCol;
	float3 lightDir;
	float padding;
}

struct Input
{
	float4 position : POSITION;
	float2 uv : TEXCOOR0;
	float3 normal : NORMAL;
};

float4 main (Input input) : SV_TARGET
{
	float4 texCol = tex2D.Sample(state, input.uv);

	// direction from pixel to the light
	float power = dot(-lightDir, input.normal);
	// power < 0 means the pixel is back to light
	power = saturate(power);
	
	// combine the tex
	float4 output = saturate(power * diffCol) * texCol;

	return output;
}