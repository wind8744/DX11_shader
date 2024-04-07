#include "Colorshaderclass.h"

Colorshaderclass::Colorshaderclass()
{
	m_vertexShader = 0;
	m_pixelShader = 0;
	m_layout = 0;
	m_matrixBuffer = 0;
}
Colorshaderclass::Colorshaderclass(const Colorshaderclass& other)
{
}
Colorshaderclass::~Colorshaderclass()
{
}

// Hlsl 쉐이더 파일 전달
bool Colorshaderclass::Initialize(ID3D11Device* device, HWND hwnd)
{
	bool result;
	wchar_t vsFilename[128];
	wchar_t psFilename[128];
	int error;

	// Set the filename of the vertex shader.
	error = wcscpy_s(vsFilename, 128, L"vs.hlsl");
	if (error != 0)
	{
		return false;
	}

	// Set the filename of the pixel shader.
	error = wcscpy_s(psFilename, 128, L"ps.hlsl");
	if (error != 0)
	{
		return false;
	}

	// Initialize the vertex and pixel shaders.
	result = InitializeShader(device, hwnd, vsFilename, psFilename);
	if (!result)
	{
		return false;
	}

	return true;
}


void Colorshaderclass::Shutdown()
{
	// Shutdown the vertex and pixel shaders as well as the related objects.
	ShutdownShader();

	return;
}

// 렌더는 setshaderapram 함수로 쉐이더내부에 매개변수 설정
// 매개변수 설정 후 rendershader로 준비된 버퍼를 렌더링
bool Colorshaderclass::Render(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX worldMatrix, XMMATRIX viewMatrix,
	XMMATRIX projectionMatrix)
{
	bool result;


	// Set the shader parameters that it will use for rendering.
	result = SetShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix);
	if (!result)
	{
		return false;
	}

	// Now render the prepared buffers with the shader.
	RenderShader(deviceContext, indexCount);

	return true;
}

// 실제 쉐이더 파일을 로드하여 dx와 gpu에서 사용할수잇도록 함
// 레이아웃과 정점버퍼 데이터가 gpu의 그래픽 파이프라인에서 어떻게 보이는지 확인할 수잇음
//레이아웃은 modelclass.h 파일의 VertexType과 color.vs 파일에 정의된 VertexType과 일치해야 합니다.
bool Colorshaderclass::InitializeShader(ID3D11Device* device, HWND hwnd, WCHAR* vsFilename, WCHAR* psFilename)
{
	HRESULT result;
	ID3D10Blob* errorMessage;
	ID3D10Blob* vertexShaderBuffer;
	ID3D10Blob* pixelShaderBuffer;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
	unsigned int numElements;
	D3D11_BUFFER_DESC matrixBufferDesc;


	// Initialize the pointers this function will use to null.
	errorMessage = 0;
	vertexShaderBuffer = 0;
	pixelShaderBuffer = 0;

	// 여기가 셰이더 프로그램을 버퍼로 컴파일하는 곳입니다. 
	// 셰이더 파일 이름, 셰이더 이름, 셰이더 버전(DirectX 11의 경우 5.0),
	//  셰이더를 컴파일할 버퍼를 지정합니다. 
	// 셰이더 컴파일에 실패하면 오류를 기록하기 위해 다른 함수로 보내는 errorMessage 문자열 안에 
	// 오류 메시지를 넣습니다. 
	// 여전히 실패하고 errorMessage 문자열이 없으면 셰이더 파일을 찾을 수 없다는 의미이며, 
	// 이 경우 대화 상자가 팝업으로 표시됩니다.
	// Compile the vertex shader code.
	result = D3DCompileFromFile(vsFilename, NULL, NULL, "main", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, // ColorVertexShader 함수 이름 ㅠㅠ 같아야함
		&vertexShaderBuffer, &errorMessage);
	if (FAILED(result))
	{
		// If the shader failed to compile it should have written something to the error message.
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, vsFilename);
		}
		// If there was  nothing in the error message then it simply could not find the shader file itself.
		else
		{
			MessageBox(hwnd, vsFilename, L"Missing Shader File", MB_OK);
		}

		return false;
	}

	// Compile the pixel shader code.
	result = D3DCompileFromFile(psFilename, NULL, NULL, "main", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, // ColorPixelShader 함수 이름 ㅠㅠ 같아야함
		&pixelShaderBuffer, &errorMessage);
	if (FAILED(result))
	{
		// If the shader failed to compile it should have written something to the error message.
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, psFilename);
		}
		// If there was nothing in the error message then it simply could not find the file itself.
		else
		{
			MessageBox(hwnd, psFilename, L"Missing Shader File", MB_OK);
		}

		return false;
	}

	//정점 셰이더와 픽셀 셰이더 코드가 성공적으로 버퍼로 컴파일되면 
	// 해당 버퍼를 사용하여 셰이더 개체 자체를 생성합니다.
	//  우리는 이 시점부터 정점 및 픽셀 셰이더와 인터페이스하기 위해 이러한 포인터를
	//  사용할 것입니다.

	// Create the vertex shader from the buffer.
	result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &m_vertexShader);
	if (FAILED(result))
	{
		return false;
	}

	// Create the pixel shader from the buffer.
	result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &m_pixelShader);
	if (FAILED(result))
	{
		return false;
	}

	//다음 단계는 셰이더에서 처리할 정점 데이터의 레이아웃을 만드는 것입니다.
	// 이 셰이더는 위치 및 색상 벡터를 사용하므로 두 가지의 크기를 지정하는 레이아웃에서 두 가지를 모두 생성해야 합니다.
	// 의미 체계 이름은 레이아웃에서 가장 먼저 작성해야 하는 항목으로, 
	// 이를 통해 셰이더는 레이아웃에서 이 요소의 사용법을 결정할 수 있습니다. 
	// 두 개의 서로 다른 요소가 있으므로 첫 번째 요소에는 POSITION을 사용하고 두 번째 요소에는 COLOR를 사용합니다.
	// 레이아웃의 다음 중요한 부분은 형식입니다. 위치 벡터에는 DXGI_FORMAT_R32G32B32_FLOAT를 사용하고 
	// 색상에는 DXGI_FORMAT_R32G32B32A32_FLOAT를 사용합니다. 
	// 마지막으로 주의해야 할 사항은 버퍼에서 데이터의 간격을 나타내는 AlignedByteOffset입니다. 
	// 이 레이아웃의 경우 처음 12바이트는 위치이고 다음 16바이트는 색상이며 AlignedByteOffset은 각 요소가 시작되는 위치를 보여줍니다.
	// AlignedByteOffset에 고유한 값을 배치하는 대신 D3D11_APPEND_ALIGNED_ELEMENT를 사용하면 간격이 자동으로 파악됩니다.
	
	// Create the vertex input layout description.
	// This setup needs to match the VertexType structure in the ModelClass and in the shader.
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "COLOR";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT; // 데이터 간격, 간격이 자동으로 파악됨
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	//polygonLayout[2].SemanticName = "TEXCOORD";
	//polygonLayout[2].SemanticIndex = 0;
	//polygonLayout[2].Format = DXGI_FORMAT_R32G32_FLOAT;
	//polygonLayout[2].InputSlot = 0;
	//polygonLayout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	//polygonLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	//polygonLayout[2].InstanceDataStepRate = 0;

	// Get a count of the elements in the layout.
	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	// Create the vertex input layout.
	result = device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(),
		vertexShaderBuffer->GetBufferSize(), &m_layout);
	if (FAILED(result))
	{
		return false;
	}

	// Release the vertex shader buffer and pixel shader buffer since they are no longer needed.
	vertexShaderBuffer->Release();
	vertexShaderBuffer = 0;

	pixelShaderBuffer->Release();
	pixelShaderBuffer = 0;


	// 셰이더를 활용하기 위해 설정해야 하는 마지막 것은 상수 버퍼입니다.
	// 정점 셰이더에서 본 것처럼 현재 상수 버퍼가 하나만 있으므로 여기서 하나만 설정하면 셰이더와 인터페이스할 수 있습니다.
	// 버퍼 사용량은 매 프레임마다 업데이트되므로 동적으로 설정해야 합니다.
	// 바인드 플래그는 이 버퍼가 상수 버퍼가 될 것임을 나타냅니다.
	// CPU 액세스 플래그는 사용법과 일치해야 D3D11_CPU_ACCESS_WRITE로 설정됩니다.
	// 설명을 작성하고 나면 상수 버퍼 인터페이스를 생성한 다음 이를 사용하여 SetShaderParameters 함수를 사용하여 
	// 셰이더의 내부 변수에 액세스할 수 있습니다.
	// Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	result = device->CreateBuffer(&matrixBufferDesc, NULL, &m_matrixBuffer);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}


void Colorshaderclass::ShutdownShader()
{
	// Release the matrix constant buffer.
	if (m_matrixBuffer)
	{
		m_matrixBuffer->Release();
		m_matrixBuffer = 0;
	}

	// Release the layout.
	if (m_layout)
	{
		m_layout->Release();
		m_layout = 0;
	}

	// Release the pixel shader.
	if (m_pixelShader)
	{
		m_pixelShader->Release();
		m_pixelShader = 0;
	}

	// Release the vertex shader.
	if (m_vertexShader)
	{
		m_vertexShader->Release();
		m_vertexShader = 0;
	}

	return;
}


void Colorshaderclass::OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFilename)
{
	char* compileErrors;
	unsigned long long bufferSize, i;
	ofstream fout;


	// Get a pointer to the error message text buffer.
	compileErrors = (char*)(errorMessage->GetBufferPointer());

	// Get the length of the message.
	bufferSize = errorMessage->GetBufferSize();

	// Open a file to write the error message to.
	fout.open("shader-error.txt");

	// Write out the error message.
	for (i = 0; i < bufferSize; i++)
	{
		fout << compileErrors[i];
	}

	// Close the file.
	fout.close();

	// Release the error message.
	errorMessage->Release();
	errorMessage = 0;

	// Pop a message up on the screen to notify the user to check the text file for compile errors.
	MessageBox(hwnd, L"Error compiling shader.  Check shader-error.txt for message.", shaderFilename, MB_OK);

	return;
}

//SetShaderVariables 함수는 셰이더에서 전역 변수를 더 쉽게 설정할 수 있도록 하기 위해 존재합니다. 
// 이 함수에 사용된 행렬은 ApplicationClass 내부에서 생성된 후 Render 함수 호출 중에 정점 셰이더로 보내기 위해 이 함수가 호출됩니다.
bool Colorshaderclass::SetShaderParameters(ID3D11DeviceContext* deviceContext, XMMATRIX worldMatrix, XMMATRIX viewMatrix,
	XMMATRIX projectionMatrix)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;
	unsigned int bufferNumber;


	// Transpose the matrices to prepare them for the shader.
	// 행렬를 변환하여 쉐이더에 사용할 준비를 함
	worldMatrix = XMMatrixTranspose(worldMatrix);
	viewMatrix = XMMatrixTranspose(viewMatrix);
	projectionMatrix = XMMatrixTranspose(projectionMatrix);

	// Lock the constant buffer so it can be written to.
	// matrix버퍼를 잠그고 새 행렬을 설정한 후 잠금 해제, 매핑
	result = deviceContext->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}
	// 매핑된 리소스의 데이터에 대한 포인터를 가지고옴
	// Get a pointer to the data in the constant buffer.
	dataPtr = (MatrixBufferType*)mappedResource.pData;

	// 행렬 복사
	// Copy the matrices into the constant buffer.
	dataPtr->world = worldMatrix;
	dataPtr->view = viewMatrix;
	dataPtr->projection = projectionMatrix;

	// Unlock the constant buffer.
	deviceContext->Unmap(m_matrixBuffer, 0);

	// Set the position of the constant buffer in the vertex shader.
	bufferNumber = 0;

	// 업데이트 된 값으로 정점쉐이더의 상수 버퍼를 설정
	// Finanly set the constant buffer in the vertex shader with the updated values.
	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_matrixBuffer);

	return true;
}


//RenderShader는 Render 함수에서 호출되는 두 번째 함수입니다. 
// //SetShaderParameters는 셰이더 매개변수가 올바르게 설정되었는지 확인하기 위해 이 전에 호출됩니다.
// 이 함수의 첫 번째 단계는 입력 어셈블러에서 입력 레이아웃을 활성화하도록 설정하는 것입니다.
// 이를 통해 GPU는 정점 버퍼의 데이터 형식을 알 수 있습니다.
// 두 번째 단계는 이 정점 버퍼를 렌더링하는 데 사용할 정점 셰이더와 픽셀 셰이더를 설정하는 것입니다.
// 셰이더가 설정되면 D3D 장치 컨텍스트를 사용하여 DrawIndexed DirectX 11 함수를 호출하여 삼각형을 렌더링합니다.
// 이 함수가 호출되면 렌더링됩니다.
void Colorshaderclass::RenderShader(ID3D11DeviceContext* deviceContext, int indexCount)
{
	// Set the vertex input layout.
	deviceContext->IASetInputLayout(m_layout);

	// Set the vertex and pixel shaders that will be used to render this triangle.
	deviceContext->VSSetShader(m_vertexShader, NULL, 0);
	deviceContext->PSSetShader(m_pixelShader, NULL, 0);

	// Render the triangle.
	deviceContext->DrawIndexed(indexCount, 0, 0);

	return;
}