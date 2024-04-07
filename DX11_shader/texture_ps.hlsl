Texture2D baseColorTexture : register(t0);
SamplerState baseColorSampler : register(s0);

struct VSOutput
{
    float4 position : SV_POSITION;
    //float4 color : COLOR;
    float2 uv : TEXCOORD;
};

// �ȼ� ���̴��� ���� ���̴� ��¿��� �Է��� ����
float4 main(VSOutput vsOutput) : SV_TARGET
{
    float4 Texcolor;
    Texcolor = baseColorTexture.Sample(baseColorSampler,vsOutput.uv);
    return Texcolor; //baseColorTexture.Sample(baseColorSampler, vsOutput.uv);
}
