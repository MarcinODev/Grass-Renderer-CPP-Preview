
cbuffer MatrixBuffer : register(b0)
{
    float4x4 _ModelViewProjMatrix;
}

struct VertexInput
{
    float3 position : POSITION;
};

struct PixelInput
{
    float4 position : SV_POSITION;
};

PixelInput VertMain(float4 position : POSITION)
{
	PixelInput output;
    output.position = mul(_ModelViewProjMatrix, position);
	return output;
}

float4 PixelMain(PixelInput input) : SV_TARGET
{
	return float4(1,0,0,1);
}