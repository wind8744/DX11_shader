Texture2D baseColorTexture : register(t0);
SamplerState baseColorSampler : register(s0);

struct VSOutput
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
    //float2 uv : TEXCOORD;
};

// 픽셀 쉐이더는 정점 쉐이더 출력에서 입력을 얻음
float4 main(VSOutput vsOutput) : SV_TARGET
{
    return vsOutput.color; //baseColorTexture.Sample(baseColorSampler, vsOutput.uv);
}
