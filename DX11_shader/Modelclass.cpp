#include "Modelclass.h"
Modelclass::Modelclass()
{
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
	m_Texture = nullptr;
	m_model = nullptr;
}
Modelclass::Modelclass(const Modelclass& other)
{
}
Modelclass::~Modelclass()
{
}

// ���� ���� �̸����� ���
// �ʱ�ȭ���� �ε� �𵨷� m model�� �ε�
bool Modelclass::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* modelFilename, char* textureFilename)
{
	bool result;
	// Load in the model data.
	//result = LoadObjModel(modelFilename);
	result = LoadModel(modelFilename);
	if (!result)
	{
		return false;
	}

	// Initialize the vertex and index buffers.
	result = InitializeBuffers(device);
	if (!result)
	{
		return false;
	}

	// Load the texture for this model.
	result = LoadTexture(device, deviceContext, textureFilename);
	if (!result)
	{
		return false;
	}

	return true;
}

void Modelclass::Shutdown()
{
	// Release the model texture.
	ReleaseTexture();

	// Shutdown the vertex and index buffers.
	ShutdownBuffers();

	// Release the model data.
	ReleaseModel();

	return;
}


ID3D11ShaderResourceView* Modelclass::GetTexture()
{
	return m_Texture->GetTexture();
}

// Render�� ApplicationClass::Render �Լ����� ȣ��˴ϴ�. 
// �� �Լ��� RenderBuffers�� ȣ���Ͽ� ���� �� �ε��� ���۸� �׷��� ���������ο� ��ġ�ϹǷ� ���� ���̴��� �̸� �������� �� �ֽ��ϴ�.
void Modelclass::Render(ID3D11DeviceContext* deviceContext)
{
	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	RenderBuffers(deviceContext);

	return;
}

//GetIndexCount�� ���� �ε��� ���� ��ȯ�մϴ�. �÷� ���̴��� �� ���� �׸����� �� ������ �ʿ��մϴ�.
int Modelclass::GetIndexCount()
{
	return m_indexCount;
}

bool Modelclass::InitializeBuffers(ID3D11Device* device)
{
	// ������ �ε��� �迭�� ���� !
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	// �ϴ� �������� ���� (�ﰢ��, �簢���� ���� ����)
	// Set the number of vertices in the vertex array.
	//m_vertexCount = 3;
	//// Set the number of indices in the index array.
	//m_indexCount = 3;

	// Create the vertex array.
	vertices = new VertexType[m_vertexCount];
	if (!vertices)
	{
		return false;
	}

	// Create the index array.
	indices = new unsigned long[m_indexCount];
	if (!indices)
	{
		return false;
	}

	// ������ �׻� �ð�������� �׸���. �ݴ�� �׸��� �޸� �ø����� �ﰢ���� �׷����� �ʴ´�.
	// Load the vertex array with data.
	//vertices[0].position = XMFLOAT3(-1.0f, -1.0f, 0.0f);  // Bottom left.
	//vertices[0].color = XMFLOAT4(1.0f, 0.5f, 0.0f, 1.f);
	//vertices[1].position = XMFLOAT3(-1.0f, 1.0f, 0.0f);  // Top middle.
	//vertices[1].color = XMFLOAT4(0.0f, 1.0f, 0.5f, 1.f);
	//vertices[2].position = XMFLOAT3(1.0f, 1.0f, 0.0f);  // Bottom right.
	//vertices[2].color = XMFLOAT4(0.5f, 0.0f, 1.0f, 1.0f);
	//vertices[3].position = XMFLOAT3(1.0f, -1.0f, 0.0f);  // Bottom right.
	//vertices[3].color = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);

	// ���� uv tex
	//vertices[0].position = XMFLOAT3(-1.0f, -1.0f, 0.0f);  // Bottom left.
	//vertices[0].texture = XMFLOAT2(0.0f, 1.0f);
	//vertices[0].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);

	//vertices[1].position = XMFLOAT3(0.0f, 1.0f, 0.0f);  // Top middle.
	//vertices[1].texture = XMFLOAT2(0.5f, 0.0f);
	//vertices[1].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);

	//vertices[2].position = XMFLOAT3(1.0f, -1.0f, 0.0f);  // Bottom right.
	//vertices[2].texture = XMFLOAT2(1.0f, 1.0f);
	//vertices[2].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);

	//// Load the index array with data.
	//indices[0] = 0;  // Bottom left.
	//indices[1] = 1;  // Top middle.
	//indices[2] = 2;  // Bottom right.

	// ���� �����͸� ���� �迭�� ,
	// �ε����� ��������
	for (int i = 0; i < m_vertexCount; i++)
	{
		vertices[i].position = XMFLOAT3(m_model[i].x, m_model[i].y, m_model[i].z);
		vertices[i].texture = XMFLOAT2(m_model[i].tu, m_model[i].tv);
		vertices[i].normal = XMFLOAT3(m_model[i].nx, m_model[i].ny, m_model[i].nz);

		indices[i] = i;
	}


	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// �����Ͱ� ���ۿ� ����Ǿ��� ������ ���̻� �ʿ����. ���� �� �ε��� �迭 ����
	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;

	return true;
}

#include <string>
#include <vector>
#include <sstream>
bool Modelclass::LoadObjModel(char* filename)
{
	ifstream* file = new ifstream(filename);
	if (!file->is_open())
	{
		return false;
	}

	char str[256];
	vector<string> tokens;
	vector<vector<float>> vertices;
	vector<vector<float>> normals;
	vector<vector<float>> uvs;
	vector<vector<vector<int>>> faces;
	while (file->getline(str, 256))//!file->eof())
	{
		stringstream stream(str);
		string ctn;
		vector<string> contents;
		tokens.push_back(str);

		// �� ���� ���ڿ��� ���� �������� ��� ������ contents�� ����.
		while (getline(stream, ctn, ' '))
		{
			contents.push_back(ctn);
		}

		for (const auto& sp : contents)
		{
			if (sp == "v")
			{
				vector<float> vertex(3, 0);
				vertex[0] = std::stof(contents[1]);
				vertex[1] = std::stof(contents[2]);
				vertex[2] = -std::stof(contents[3]);
				vertices.push_back(vertex);
			}
			else if (sp == "vn")
			{
				vector<float> normal(3, 0);
				normal[0] = std::stof(contents[1]);
				normal[1] = std::stof(contents[2]);
				normal[2] = -std::stof(contents[3]);
				normals.push_back(normal);
			}
			else if (sp == "vt")
			{
				vector<float> uv(2, 0);
				uv[0] = std::stof(contents[1]);
				uv[1] = 1 - std::stof(contents[2]);
				uvs.push_back(uv);
			}
			else if (sp == "f")
			{
				//int** face = new int* [3];
				vector<vector<int>> face(3, vector<int>(3, 0));
				for (int i = 0; i < 3; i++)
				{
					//face[i] = new int[3];
					std::stringstream ss(contents[i + 1]);
					std::string val;
					int cnt = 0;
					while (std::getline(ss, val, '/'))
					{
						if (val.empty()) val = '0';
						face[i][cnt] = std::stoi(val) - 1;
						cnt++;
					}
				}
				faces.push_back(face);
			}

		}
	}

	//struct VertexInput
	//{
	//	XMFLOAT3 Position;
	//	XMFLOAT3 Normal;
	//	XMFLOAT3 Tangent;
	//};

	//struct ModelType
	//{
	//	float x, y, z;
	//	float tu, tv;
	//	float nx, ny, nz;
	//};

	int i = 0;
	m_vertexCount = 312;
	m_indexCount = 312;
	m_model = new ModelType[m_vertexCount*3];
	for (const auto& f : faces)
	{
		ModelType NewModel;

		vector<int> info = f[0];
		m_model[i].x = vertices[info[0]][0];
		m_model[i].y = vertices[info[0]][1];
		m_model[i].z = vertices[info[0]][2];

		m_model[i].tu = uvs[info[1]][0];
		m_model[i].tv = uvs[info[1]][1];

		m_model[i].nx = normals[info[2]][0];
		m_model[i].ny = normals[info[2]][1];
		m_model[i].nz = normals[info[2]][2];
	}


	file->close();
	delete file;
	file = nullptr;

	return true;
}

// �ؽ�Ʈ ������ �� �����͸� �о�� ��Ÿ�� ����ü�� �迭�� �����Ͽ� �����͸� ����
bool Modelclass::LoadModel(char* filename)
{
	ifstream fin;
	char input;
	int i;


	// Open the model file.
	fin.open(filename);

	// If it could not open the file then exit.
	if (fin.fail())
	{
		return false;
	}

	// Read up to the value of vertex count.
	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}

	// Read in the vertex count.
	fin >> m_vertexCount;

	// Set the number of indices to be the same as the vertex count.
	m_indexCount = m_vertexCount;

	// Create the model using the vertex count that was read in.
	m_model = new ModelType[m_vertexCount];

	// Read up to the beginning of the data.
	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}
	fin.get(input);
	fin.get(input);

	// Read in the vertex data.
	for (i = 0; i < m_vertexCount; i++)
	{
		fin >> m_model[i].x >> m_model[i].y >> m_model[i].z;
		fin >> m_model[i].tu >> m_model[i].tv;
		fin >> m_model[i].nx >> m_model[i].ny >> m_model[i].nz;
	}

	// Close the model file.
	fin.close();

	return true;
}

void Modelclass::ReleaseModel()
{
	if (m_model)
	{
		delete[] m_model;
		m_model = 0;
	}

	return;
}

void Modelclass::ShutdownBuffers()
{
	// Release the index buffer.
	if (m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	// Release the vertex buffer.
	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}

	return;
}

//RenderBuffers�� Render �Լ����� ȣ��˴ϴ�. 
// �� �Լ��� ������ ���� ���ۿ� �ε��� ���۸� GPU�� �Է� ��������� Ȱ������ �����ϴ� ���Դϴ�. 
// GPU�� Ȱ�� ���� ���۰� ������ ���̴��� ����Ͽ� �ش� ���۸� �������� �� �ֽ��ϴ�. 
// ���� �� �Լ��� �ﰢ��, ��, �� ��� ���� ���۸� �׸��� ����� �����մϴ�. 
// �� Ʃ�丮�󿡼��� �Է� ��������� ���� ���ۿ� �ε��� ���۸� Ȱ������ �����ϰ� 
// IASetPrimitiveTopology DirectX �Լ��� ����Ͽ� ���۸� �ﰢ������ �׷��� ���� GPU�� �˸��ϴ�.
void Modelclass::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;


	// Set vertex buffer stride and offset.
	stride = sizeof(VertexType);
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}

bool Modelclass::LoadTexture(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* filename)
{
	bool result;


	// Create and initialize the texture object.
	m_Texture = new Textureclass;

	result = m_Texture->Initialize(device, deviceContext, filename);
	if (!result)
	{
		return false;
	}

	return true;
}

void Modelclass::ReleaseTexture()
{
	// Release the texture object.
	if (m_Texture)
	{
		m_Texture->Shutdown();
		delete m_Texture;
		m_Texture = 0;
	}

	return;
}