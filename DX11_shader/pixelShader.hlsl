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

// �ȼ� ���̴��� ���� ���̴� ��¿��� �Է��� ����
float4 main(VSOutput vsOutput) : SV_TARGET
{
	float4 Texcolor;
	float3 lightDir;
	float lightIntensity;
	float4 color;
    Texcolor = baseColorTexture.Sample(baseColorSampler,vsOutput.uv);
    
	// ���� ���� ����
	lightDir = -lightDirection;

    // ���� ������ : �ﰢ���� Tex �� �������Ϳ� ���� ���� ����
	lightIntensity = saturate(dot(vsOutput.normal, lightDir));

    // ���� Ȯ�갪�� ���� ���� ����
	color = saturate(diffuseColor * lightIntensity);
	//color = color * Texcolor; // ���� �ؽ��Ŀ� ��
	
    return baseColorTexture.Sample(baseColorSampler, vsOutput.uv) * color;
}
