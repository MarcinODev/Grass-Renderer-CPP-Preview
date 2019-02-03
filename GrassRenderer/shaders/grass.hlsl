//PROPERTIES
//_Time;FLOAT;
//_TessOutside;FLOAT;
//_TessInside;FLOAT;
//_GrassHeight;FLOAT;
//_GrassWidth;FLOAT;
//_GrassShrinkLinear;FLOAT;
//_GrassShrinkPow;FLOAT;
//_GrassDarknessPow;FLOAT;
//_GrassBendParams;FLOAT4;
//_WindParams;FLOAT4;
//_WindDir;FLOAT4;
//_CameraPos;FLOAT4;
//_LightColor;FLOAT4;
//_LightDir;FLOAT4;
//_GrassColor;FLOAT4;
//END_PROPERTIES

cbuffer MatrixBuffer : register(b0)
{
    float4x4 _ModelViewProjMatrix;
    float4x4 _ModelMatrix;
    float4x4 _ModelToMVPMatrix;
}

cbuffer DataBuffer : register(b1)
{
    float _Time;
    float _TessOutside;
    float _TessInside;
    float _GrassHeight;
    float _GrassWidth;
    float _GrassShrinkLinear;
    float _GrassShrinkPow;
    float _GrassDarknessPow; 
    float4 _GrassBendParams;//xy - randomisation over [x,z], z - bendLinear, w - bendPow
    float4 _WindParams;//xy - direction of waves (unnormalized as it is used also for waves length), z - frequency, w - wind max power
    float4 _WindDir;//normalized wind direction
    float4 _CameraPos; 
    float4 _LightColor;// w - fake ao intensity
    float4 _LightDir;//x,y,z normalized
    float4 _GrassColor;//a - variation difference
}

struct VertexInput
{
    float3 position : POSITION;
};

struct PatchConstantOutput
{
    float edges[3] : SV_TessFactor;
    float inside : SV_InsideTessFactor;
};

struct HullInput
{
    float3 position : POSITION;
};

struct DomainInput
{
    float3 position : POSITION;
};

struct GeometryInput
{
    float3 position : POSITION;
};

struct PixelInput
{
    float4 position : SV_POSITION;
    float heightProgress : TEXCOORD0;
    float3 wNormal : TEXCOORD1;
    float randomVal : TEXCOORD2;
};

HullInput VertMain(VertexInput input)
{
    HullInput output;
    output.position = input.position;
	return output;
}

PatchConstantOutput PatchConstant(InputPatch<HullInput, 3> inputPatch, uint patchId : SV_PrimitiveID)
{
    PatchConstantOutput output;
    output.edges[0] = _TessOutside;
    output.edges[1] = _TessOutside;
    output.edges[2] = _TessOutside;
    
    output.inside = _TessInside;

    return output;
}

[domain("tri")]
[partitioning("fractional_odd")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(3)]
[patchconstantfunc("PatchConstant")]
DomainInput HullMain(InputPatch<HullInput, 3> patch, uint pointId : SV_OutputControlPointID, uint patchId : SV_PrimitiveID)
{
    DomainInput output;
    
    output.position = patch[pointId].position;
    
    return output;
}

float rand(float2 uv)
{
    float2 noise = frac(sin(dot(uv, float2(12.9898, 78.233) * 2.0)) * 43758.5453);
    return abs(noise.x + noise.y) * 0.5;
}

[domain("tri")]
GeometryInput DomainMain(PatchConstantOutput input, float3 blend : SV_DomainLocation, const OutputPatch<DomainInput, 3> patch)
{
    GeometryInput output;
    
    float3 vertexPos = blend.x * patch[0].position + blend.y * patch[1].position + blend.z * patch[2].position;
    output.position = vertexPos;
    output.position.xz += rand(vertexPos.xz + blend.y) * 10;//randomise position a bit
    return output;
}

float2 posBasedRand(float2 pos, float2 randFactor)
{
    return sin(pos * randFactor);
}

float3 calculateBendVector(float yProgress, float windPower, float3 bendDir)
{
    float bendPower = saturate(pow(yProgress * _GrassBendParams.z, _GrassBendParams.w));
    float maxOffset = _GrassHeight * yProgress;
    bendDir.xz *= bendPower * maxOffset;
    bendDir.xz += _GrassBendParams.z * windPower * yProgress * -_WindDir.xy;
    float bendLength = length(bendDir.xz);
    if(bendLength > maxOffset)
    {
        bendDir.xz = bendDir.xz * (maxOffset / bendLength);
        bendLength = maxOffset;
    }
    bendDir.y -= bendLength * 2;
    return bendDir;
}

[maxvertexcount(28 /*7 steps of 4 vertices*/)]
void GeometryMain(point GeometryInput input[1], inout TriangleStream<PixelInput> triStream)
{
    float3 basePos = input[0].position;
    float3 baseWPos = mul(_ModelMatrix, float4(basePos, 1)).xyz;
    
    float3 up = float3(0, 1, 0);
    float3 look = _LightDir.xyz;/*_CameraPos - baseWPos*///in FPS would be better use of cam pos but in 3rd view it's better to use light dir
    look.y = 0;
    look = normalize(look);
    float3 right = -cross(up, look);

    float grassHeight = _GrassHeight;
    float3 rightV = right * _GrassWidth;
    float3 rightVHalf = rightV * 0.5;

    float isWind = sin(_Time * _WindParams.z + _WindParams.x * baseWPos.x + _WindParams.y * baseWPos.z) * 0.5 + 0.5;
    float windPower = _WindParams.w * isWind;
    float2 bendRandom = posBasedRand(baseWPos.xz, _GrassBendParams.xy);
    float3 bendDir = normalize(float3(bendRandom.x, 0, bendRandom.y));

    const int steps = 7;
	float yProgressStep = 1.0 / steps;
	float yProgress = 0.0;
    
    PixelInput output;
    output.wNormal = look;
    output.randomVal = (bendRandom.x + bendRandom.y) * 0.5;
    for(int i = 0; i < steps; i++)
    {
        for(int j = 0; j < 2; j++)
        {
            float shrink = saturate(yProgress * _GrassShrinkLinear + pow(yProgress, _GrassShrinkPow));
            float3 rightVHalfShrinked = lerp(rightVHalf, float3(0, 0, 0), shrink);
            float3 vertexUp = float3(0, grassHeight * yProgress, 0);
            float3 bendVector = calculateBendVector(yProgress, windPower, bendDir);
            float4 vertex = float4(baseWPos + rightVHalfShrinked + vertexUp + bendVector, 1.0);
            output.position = mul(_ModelToMVPMatrix, vertex);
            output.heightProgress = yProgress;
            triStream.Append(output);
        
            float currentYProgress = yProgress + yProgressStep;
            shrink = saturate(currentYProgress * _GrassShrinkLinear + pow(currentYProgress, _GrassShrinkPow));
            rightVHalfShrinked = lerp(rightVHalf, float3(0, 0, 0), shrink);
            vertexUp = float3(0, grassHeight * currentYProgress, 0);
            bendVector = calculateBendVector(currentYProgress, windPower, bendDir);
            vertex = float4(baseWPos + rightVHalfShrinked + vertexUp + bendVector, 1.0);
            output.position = mul(_ModelToMVPMatrix, vertex);
            output.heightProgress = currentYProgress;
            triStream.Append(output);

            rightVHalf = -rightVHalf;
        }
        
		yProgress += yProgressStep;
        triStream.RestartStrip();
    }
    

}

float4 PixelMain(PixelInput input) : SV_TARGET
{
    float3 color = _GrassColor.rgb * (1.0 + input.randomVal * _GrassColor.a);

    float dotLight = dot(input.wNormal, _LightDir.xyz) * pow(input.heightProgress * _LightColor.w, _GrassDarknessPow);
    color *= saturate(dotLight) * _LightColor.rgb;
    

    return float4(color, 1);
}