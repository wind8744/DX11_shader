/////////////
// GLOBALS //
/////////////

//일반적으로 단일 전역 변수인 경우에도 대부분의 전역 변수를 "씨버퍼"라는 버퍼 개체 유형에 넣습니다. 
//이러한 버퍼를 논리적으로 구성하는 것은 셰이더의 효율적인 실행뿐만 아니라 그래픽 카드가 버퍼를 저장하는 방법에도 중요합니다. 
//이 예에서는 각 프레임을 동시에 업데이트할 것이므로 동일한 버퍼에 3개의 행렬을 넣었습니다
/////////////
cbuffer MatrixBuffer
{
    matrix worldMatrix; // 월드
    matrix viewMatrix; // 뷰
    matrix projectionMatrix; // 투영
};

// x, y, z, w 위치 벡터와 빨간색, 녹색, 파란색, 알파 색상이 있는 유형을 만듭니다. 
//POSITION, COLOR 및 SV_POSITION은 변수 사용을 GPU에 전달하는 의미 체계입니다. 
//여기서는 구조가 동일하더라도 정점 셰이더와 픽셀 셰이더의 의미가 다르기 때문에 여기서 두 가지 다른 구조를 만들어야 합니다.
struct VSInput
{
    float4 position : POSITION;
    //float4 color : COLOR;
    float2 uv : TEXCOORD;
};

// 픽셀쉐이더로 넘어가는거
struct VSOutput
{
    float4 position : SV_POSITION;
    //float4 color : COLOR;
    float2 uv : TEXCOORD;
};

// 입력 정점의 위치를 가져와서 여기에 월드, 뷰, 투영 행렬을 곱합니다. 
// 그러면 우리의 관점에 따라 3D 공간에서 렌더링하기 위한 올바른 위치에 정점이 배치되고 2D 화면에 배치됩니다.
//그 후 출력 변수는 입력 색상의 복사본을 가져온 다음 픽셀 셰이더에 대한 입력으로 사용될 출력을 반환합니다. 
//또한 입력 위치의 W 값을 1.0으로 설정했습니다. 그렇지 않으면 위치에 대한 XYZ 벡터만 읽기 때문에 정의되지 않습니a다.
VSOutput main(VSInput vsInput)
{
    VSOutput vsOutput;
	vsInput.position.w = 1.0f;
    
    // 월드, 뷰, 투영행렬에 대한 정점 위치 계산
    vsOutput.position = mul(vsInput.position, worldMatrix); // 정점을 월드로 옮김
    vsOutput.position = mul(vsOutput.position, viewMatrix); // 월드좌표를 뷰행렬로
    vsOutput.position = mul(vsOutput.position, projectionMatrix); // 투영
    
    // 색 설정
    //vsOutput.color = vsInput.color;

    // 색상이 아닌 텍스쳐 좌표 복사본을 갖고온다
    vsOutput.uv = vsInput.uv;

    return vsOutput; // 픽셀로
}
