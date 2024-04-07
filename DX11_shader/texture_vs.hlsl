cbuffer MatrixBuffer
{
    matrix worldMatrix; // ����
    matrix viewMatrix; // ��
    matrix projectionMatrix; // ����
};

struct VSInput
{
    float4 position : POSITION;
    //float4 color : COLOR;
    float2 uv : TEXCOORD0;
};

struct VSOutput
{
    float4 position : SV_POSITION;
    //float4 color : COLOR;
    float2 uv : TEXCOORD0;
};

VSOutput main(VSInput vsInput)
{
    VSOutput vsOutput;
	vsInput.position.w = 1.0f;
    
    // ����, ��, ������Ŀ� ���� ���� ��ġ ���
    vsOutput.position = mul(vsInput.position, worldMatrix); // ������ ����� �ű�
    vsOutput.position = mul(vsOutput.position, viewMatrix); // ������ǥ�� ����ķ�
    vsOutput.position = mul(vsOutput.position, projectionMatrix); // ����
    
    // �ؽ��� ��ǥ ���纻�� ����´�
    vsOutput.uv = vsInput.uv;

    return vsOutput; // �ȼ���
}
