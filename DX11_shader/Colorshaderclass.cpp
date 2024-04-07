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

// Hlsl ���̴� ���� ����
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

// ������ setshaderapram �Լ��� ���̴����ο� �Ű����� ����
// �Ű����� ���� �� rendershader�� �غ�� ���۸� ������
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

// ���� ���̴� ������ �ε��Ͽ� dx�� gpu���� ����Ҽ��յ��� ��
// ���̾ƿ��� �������� �����Ͱ� gpu�� �׷��� ���������ο��� ��� ���̴��� Ȯ���� ������
//���̾ƿ��� modelclass.h ������ VertexType�� color.vs ���Ͽ� ���ǵ� VertexType�� ��ġ�ؾ� �մϴ�.
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

	// ���Ⱑ ���̴� ���α׷��� ���۷� �������ϴ� ���Դϴ�. 
	// ���̴� ���� �̸�, ���̴� �̸�, ���̴� ����(DirectX 11�� ��� 5.0),
	//  ���̴��� �������� ���۸� �����մϴ�. 
	// ���̴� �����Ͽ� �����ϸ� ������ ����ϱ� ���� �ٸ� �Լ��� ������ errorMessage ���ڿ� �ȿ� 
	// ���� �޽����� �ֽ��ϴ�. 
	// ������ �����ϰ� errorMessage ���ڿ��� ������ ���̴� ������ ã�� �� ���ٴ� �ǹ��̸�, 
	// �� ��� ��ȭ ���ڰ� �˾����� ǥ�õ˴ϴ�.
	// Compile the vertex shader code.
	result = D3DCompileFromFile(vsFilename, NULL, NULL, "main", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, // ColorVertexShader �Լ� �̸� �Ф� ���ƾ���
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
	result = D3DCompileFromFile(psFilename, NULL, NULL, "main", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, // ColorPixelShader �Լ� �̸� �Ф� ���ƾ���
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

	//���� ���̴��� �ȼ� ���̴� �ڵ尡 ���������� ���۷� �����ϵǸ� 
	// �ش� ���۸� ����Ͽ� ���̴� ��ü ��ü�� �����մϴ�.
	//  �츮�� �� �������� ���� �� �ȼ� ���̴��� �������̽��ϱ� ���� �̷��� �����͸�
	//  ����� ���Դϴ�.

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

	//���� �ܰ�� ���̴����� ó���� ���� �������� ���̾ƿ��� ����� ���Դϴ�.
	// �� ���̴��� ��ġ �� ���� ���͸� ����ϹǷ� �� ������ ũ�⸦ �����ϴ� ���̾ƿ����� �� ������ ��� �����ؾ� �մϴ�.
	// �ǹ� ü�� �̸��� ���̾ƿ����� ���� ���� �ۼ��ؾ� �ϴ� �׸�����, 
	// �̸� ���� ���̴��� ���̾ƿ����� �� ����� ������ ������ �� �ֽ��ϴ�. 
	// �� ���� ���� �ٸ� ��Ұ� �����Ƿ� ù ��° ��ҿ��� POSITION�� ����ϰ� �� ��° ��ҿ��� COLOR�� ����մϴ�.
	// ���̾ƿ��� ���� �߿��� �κ��� �����Դϴ�. ��ġ ���Ϳ��� DXGI_FORMAT_R32G32B32_FLOAT�� ����ϰ� 
	// ���󿡴� DXGI_FORMAT_R32G32B32A32_FLOAT�� ����մϴ�. 
	// ���������� �����ؾ� �� ������ ���ۿ��� �������� ������ ��Ÿ���� AlignedByteOffset�Դϴ�. 
	// �� ���̾ƿ��� ��� ó�� 12����Ʈ�� ��ġ�̰� ���� 16����Ʈ�� �����̸� AlignedByteOffset�� �� ��Ұ� ���۵Ǵ� ��ġ�� �����ݴϴ�.
	// AlignedByteOffset�� ������ ���� ��ġ�ϴ� ��� D3D11_APPEND_ALIGNED_ELEMENT�� ����ϸ� ������ �ڵ����� �ľǵ˴ϴ�.
	
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
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT; // ������ ����, ������ �ڵ����� �ľǵ�
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


	// ���̴��� Ȱ���ϱ� ���� �����ؾ� �ϴ� ������ ���� ��� �����Դϴ�.
	// ���� ���̴����� �� ��ó�� ���� ��� ���۰� �ϳ��� �����Ƿ� ���⼭ �ϳ��� �����ϸ� ���̴��� �������̽��� �� �ֽ��ϴ�.
	// ���� ��뷮�� �� �����Ӹ��� ������Ʈ�ǹǷ� �������� �����ؾ� �մϴ�.
	// ���ε� �÷��״� �� ���۰� ��� ���۰� �� ������ ��Ÿ���ϴ�.
	// CPU �׼��� �÷��״� ������ ��ġ�ؾ� D3D11_CPU_ACCESS_WRITE�� �����˴ϴ�.
	// ������ �ۼ��ϰ� ���� ��� ���� �������̽��� ������ ���� �̸� ����Ͽ� SetShaderParameters �Լ��� ����Ͽ� 
	// ���̴��� ���� ������ �׼����� �� �ֽ��ϴ�.
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

//SetShaderVariables �Լ��� ���̴����� ���� ������ �� ���� ������ �� �ֵ��� �ϱ� ���� �����մϴ�. 
// �� �Լ��� ���� ����� ApplicationClass ���ο��� ������ �� Render �Լ� ȣ�� �߿� ���� ���̴��� ������ ���� �� �Լ��� ȣ��˴ϴ�.
bool Colorshaderclass::SetShaderParameters(ID3D11DeviceContext* deviceContext, XMMATRIX worldMatrix, XMMATRIX viewMatrix,
	XMMATRIX projectionMatrix)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;
	unsigned int bufferNumber;


	// Transpose the matrices to prepare them for the shader.
	// ��ĸ� ��ȯ�Ͽ� ���̴��� ����� �غ� ��
	worldMatrix = XMMatrixTranspose(worldMatrix);
	viewMatrix = XMMatrixTranspose(viewMatrix);
	projectionMatrix = XMMatrixTranspose(projectionMatrix);

	// Lock the constant buffer so it can be written to.
	// matrix���۸� ��װ� �� ����� ������ �� ��� ����, ����
	result = deviceContext->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}
	// ���ε� ���ҽ��� �����Ϳ� ���� �����͸� �������
	// Get a pointer to the data in the constant buffer.
	dataPtr = (MatrixBufferType*)mappedResource.pData;

	// ��� ����
	// Copy the matrices into the constant buffer.
	dataPtr->world = worldMatrix;
	dataPtr->view = viewMatrix;
	dataPtr->projection = projectionMatrix;

	// Unlock the constant buffer.
	deviceContext->Unmap(m_matrixBuffer, 0);

	// Set the position of the constant buffer in the vertex shader.
	bufferNumber = 0;

	// ������Ʈ �� ������ �������̴��� ��� ���۸� ����
	// Finanly set the constant buffer in the vertex shader with the updated values.
	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_matrixBuffer);

	return true;
}


//RenderShader�� Render �Լ����� ȣ��Ǵ� �� ��° �Լ��Դϴ�. 
// //SetShaderParameters�� ���̴� �Ű������� �ùٸ��� �����Ǿ����� Ȯ���ϱ� ���� �� ���� ȣ��˴ϴ�.
// �� �Լ��� ù ��° �ܰ�� �Է� ��������� �Է� ���̾ƿ��� Ȱ��ȭ�ϵ��� �����ϴ� ���Դϴ�.
// �̸� ���� GPU�� ���� ������ ������ ������ �� �� �ֽ��ϴ�.
// �� ��° �ܰ�� �� ���� ���۸� �������ϴ� �� ����� ���� ���̴��� �ȼ� ���̴��� �����ϴ� ���Դϴ�.
// ���̴��� �����Ǹ� D3D ��ġ ���ؽ�Ʈ�� ����Ͽ� DrawIndexed DirectX 11 �Լ��� ȣ���Ͽ� �ﰢ���� �������մϴ�.
// �� �Լ��� ȣ��Ǹ� �������˴ϴ�.
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