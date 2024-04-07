cbuffer MatrixBuffer
{
    matrix worldMatrix; // 월드
    matrix viewMatrix; // 뷰
    matrix projectionMatrix; // 투영
};

struct VSInput
{
    float4 position : POSITION;
    float2 uv : TEXCOORD0;
	float3 normal : NORMAL;
};

struct VSOutput
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD0;
	float3 normal : NORMAL;
};

VSOutput main(VSInput vsInput)
{
    VSOutput vsOutput;
	vsInput.position.w = 1.0f;
    
    // 월드, 뷰, 투영행렬에 대한 정점 위치 계산
    vsOutput.position = mul(vsInput.position, worldMatrix); // 정점을 월드로 옮김
    vsOutput.position = mul(vsOutput.position, viewMatrix); // 월드좌표를 뷰행렬로
    vsOutput.position = mul(vsOutput.position, projectionMatrix); // 투영
    
    // 텍스쳐 좌표 복사본을 갖고온다
    vsOutput.uv = vsInput.uv;
    
    // 월드좌표에 대해서 법선 벡터 계산
	vsOutput.normal = mul(vsInput.normal, (float3x3)worldMatrix);
    // 법선 벡터 정규화
	vsOutput.normal = normalize(vsOutput.normal);

    return vsOutput; // 픽셀로
}
