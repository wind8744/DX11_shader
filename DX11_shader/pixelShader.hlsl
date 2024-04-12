Texture2D baseColorTexture : register(t0);
SamplerState baseColorSampler : register(s0);

// Lightshader class
cbuffer LightBuffer
{
	float4 diffuseColor;
	float3 lightDirection;
	float padding;
};

// Modelclass
struct VSOutput
{
    float4 position : SV_POSITION;
    //float4 color : COLOR;
    float2 uv : TEXCOORD;
	float3 normal : NORMAL;
};

// 픽셀 쉐이더는 정점 쉐이더 출력에서 입력을 얻음
float4 main(VSOutput vsOutput) : SV_TARGET
{
	float4 Texcolor;
	float3 lightDir;
	float lightIntensity;
	float4 color;
    Texcolor = baseColorTexture.Sample(baseColorSampler,vsOutput.uv);
    
	// 빛의 방향 반전
	lightDir = -lightDirection;

    // 빛의 강도값 : 삼각형의 Tex 의 법선벡터와 빛의 방향 내적
	lightIntensity = saturate(dot(vsOutput.normal, lightDir));

    // 빛의 확산값과 빛의 세기 결합
	color = saturate(diffuseColor * lightIntensity);
	//color = color * Texcolor; // 원본 텍스쳐와 곱
	
    return baseColorTexture.Sample(baseColorSampler, vsOutput.uv) * color;
}
