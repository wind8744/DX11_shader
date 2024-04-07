/////////////
// GLOBALS //
/////////////

//�Ϲ������� ���� ���� ������ ��쿡�� ��κ��� ���� ������ "������"��� ���� ��ü ������ �ֽ��ϴ�. 
//�̷��� ���۸� �������� �����ϴ� ���� ���̴��� ȿ������ ����Ӹ� �ƴ϶� �׷��� ī�尡 ���۸� �����ϴ� ������� �߿��մϴ�. 
//�� �������� �� �������� ���ÿ� ������Ʈ�� ���̹Ƿ� ������ ���ۿ� 3���� ����� �־����ϴ�
/////////////
cbuffer MatrixBuffer
{
    matrix worldMatrix; // ����
    matrix viewMatrix; // ��
    matrix projectionMatrix; // ����
};

// x, y, z, w ��ġ ���Ϳ� ������, ���, �Ķ���, ���� ������ �ִ� ������ ����ϴ�. 
//POSITION, COLOR �� SV_POSITION�� ���� ����� GPU�� �����ϴ� �ǹ� ü���Դϴ�. 
//���⼭�� ������ �����ϴ��� ���� ���̴��� �ȼ� ���̴��� �ǹ̰� �ٸ��� ������ ���⼭ �� ���� �ٸ� ������ ������ �մϴ�.
struct VSInput
{
    float4 position : POSITION;
    //float4 color : COLOR;
    float2 uv : TEXCOORD;
};

// �ȼ����̴��� �Ѿ�°�
struct VSOutput
{
    float4 position : SV_POSITION;
    //float4 color : COLOR;
    float2 uv : TEXCOORD;
};

// �Է� ������ ��ġ�� �����ͼ� ���⿡ ����, ��, ���� ����� ���մϴ�. 
// �׷��� �츮�� ������ ���� 3D �������� �������ϱ� ���� �ùٸ� ��ġ�� ������ ��ġ�ǰ� 2D ȭ�鿡 ��ġ�˴ϴ�.
//�� �� ��� ������ �Է� ������ ���纻�� ������ ���� �ȼ� ���̴��� ���� �Է����� ���� ����� ��ȯ�մϴ�. 
//���� �Է� ��ġ�� W ���� 1.0���� �����߽��ϴ�. �׷��� ������ ��ġ�� ���� XYZ ���͸� �б� ������ ���ǵ��� �ʽ���a��.
VSOutput main(VSInput vsInput)
{
    VSOutput vsOutput;
	vsInput.position.w = 1.0f;
    
    // ����, ��, ������Ŀ� ���� ���� ��ġ ���
    vsOutput.position = mul(vsInput.position, worldMatrix); // ������ ����� �ű�
    vsOutput.position = mul(vsOutput.position, viewMatrix); // ������ǥ�� ����ķ�
    vsOutput.position = mul(vsOutput.position, projectionMatrix); // ����
    
    // �� ����
    //vsOutput.color = vsInput.color;

    // ������ �ƴ� �ؽ��� ��ǥ ���纻�� ����´�
    vsOutput.uv = vsInput.uv;

    return vsOutput; // �ȼ���
}
