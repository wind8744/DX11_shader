#include "Modelclass.h"

struct VertexType
{
	float x, y, z;
};

struct FaceType
{
	int vIndex1, vIndex2, vIndex3;
	int tIndex1, tIndex2, tIndex3;
	int nIndex1, nIndex2, nIndex3;
};

bool ReadFileCounts(char*, int&, int&, int&, int&);
bool LoadDataStructures(char*, int, int, int, int);
bool Modelclass::ReadObjModel(char* filename)
{
	bool result;
	int vertexCount, textureCount, normalCount, faceCount;

	// 버텍스 개수를 읽어옴
	result = ReadFileCounts(filename, vertexCount, textureCount, normalCount, faceCount);
	if (!result) { return false; }

	// 읽어온 값으로 데이터 생성
	result = LoadDataStructures(filename, vertexCount, textureCount, normalCount, faceCount);
	if (!result){ return false; }

	return true;
}

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

// 모델을 파일 이름으로 사용
// 초기화에서 로드 모델로 m model에 로드
bool Modelclass::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* modelFilename, char* textureFilename)
{
	bool result;

	// Load in the model data.
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

// Render는 ApplicationClass::Render 함수에서 호출됩니다. 
// 이 함수는 RenderBuffers를 호출하여 정점 및 인덱스 버퍼를 그래픽 파이프라인에 배치하므로 색상 셰이더가 이를 렌더링할 수 있습니다.
void Modelclass::Render(ID3D11DeviceContext* deviceContext)
{
	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	RenderBuffers(deviceContext);

	return;
}

//GetIndexCount는 모델의 인덱스 수를 반환합니다. 컬러 셰이더는 이 모델을 그리려면 이 정보가 필요합니다.
int Modelclass::GetIndexCount()
{
	return m_indexCount;
}

bool Modelclass::InitializeBuffers(ID3D11Device* device)
{
	// 정점과 인덱스 배열을 생성 !
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	// 일단 수동으로 생성 (삼각형, 사각형을 띄우기 위함)
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

	// 정점은 항상 시계방향으로 그린다. 반대로 그릴시 뒷면 컬링으로 삼각형이 그려지지 않는다.
	// Load the vertex array with data.
	//vertices[0].position = XMFLOAT3(-1.0f, -1.0f, 0.0f);  // Bottom left.
	//vertices[0].color = XMFLOAT4(1.0f, 0.5f, 0.0f, 1.f);
	//vertices[1].position = XMFLOAT3(-1.0f, 1.0f, 0.0f);  // Top middle.
	//vertices[1].color = XMFLOAT4(0.0f, 1.0f, 0.5f, 1.f);
	//vertices[2].position = XMFLOAT3(1.0f, 1.0f, 0.0f);  // Bottom right.
	//vertices[2].color = XMFLOAT4(0.5f, 0.0f, 1.0f, 1.0f);
	//vertices[3].position = XMFLOAT3(1.0f, -1.0f, 0.0f);  // Bottom right.
	//vertices[3].color = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);

	// 적용 uv tex
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

	// 받은 데이터를 정점 배열로 ,
	// 인덱스도 마찬가지
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

	// 데이터가 버퍼에 복사되었기 때문에 더이상 필요없음. 정점 및 인덱스 배열 삭제
	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;

	return true;
}


// 텍스트 파일의 모델 데이터를 읽어와 모델타입 구조체의 배열을 생성하여 데이터를 삽입
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

//RenderBuffers는 Render 함수에서 호출됩니다. 
// 이 함수의 목적은 정점 버퍼와 인덱스 버퍼를 GPU의 입력 어셈블러에서 활성으로 설정하는 것입니다. 
// GPU에 활성 정점 버퍼가 있으면 셰이더를 사용하여 해당 버퍼를 렌더링할 수 있습니다. 
// 또한 이 함수는 삼각형, 선, 팬 등과 같은 버퍼를 그리는 방법을 정의합니다. 
// 이 튜토리얼에서는 입력 어셈블러에서 정점 버퍼와 인덱스 버퍼를 활성으로 설정하고 
// IASetPrimitiveTopology DirectX 함수를 사용하여 버퍼를 삼각형으로 그려야 함을 GPU에 알립니다.
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

bool ReadFileCounts(char* filename, int& vertexCount, int& textureCount, int& normalCount, int& faceCount)
{
	ifstream fin;
	char input;

	// Initialize the counts.
	vertexCount = 0;
	textureCount = 0;
	normalCount = 0;
	faceCount = 0;

	// Open the file.
	fin.open(filename);

	// Check if it was successful in opening the file.
	if (fin.fail() == true)
	{
		return false;
	}

	// Read from the file and continue to read until the end of the file is reached.
	fin.get(input);
	while (!fin.eof())
	{
		// If the line starts with 'v' then count either the vertex, the texture coordinates, or the normal vector.
		if (input == 'v')
		{
			fin.get(input);
			if (input == ' ') { vertexCount++; }
			if (input == 't') { textureCount++; }
			if (input == 'n') { normalCount++; }
		}

		// If the line starts with 'f' then increment the face count.
		if (input == 'f')
		{
			fin.get(input);
			if (input == ' ') { faceCount++; }
		}

		// Otherwise read in the remainder of the line.
		while (input != '\n')
		{
			fin.get(input);
		}

		// Start reading the beginning of the next line.
		fin.get(input);
	}

	// Close the file.
	fin.close();

	return true;
}


bool LoadDataStructures(char* filename, int vertexCount, int textureCount, int normalCount, int faceCount)
{
	VertexType* vertices, * texcoords, * normals;
	FaceType* faces;
	ifstream fin;
	int vertexIndex, texcoordIndex, normalIndex, faceIndex, vIndex, tIndex, nIndex;
	char input, input2;
	ofstream fout;

	// Initialize the four data structures.
	vertices = new VertexType[vertexCount];
	if (!vertices)
	{
		return false;
	}

	texcoords = new VertexType[textureCount];
	if (!texcoords)
	{
		return false;
	}

	normals = new VertexType[normalCount];
	if (!normals)
	{
		return false;
	}

	faces = new FaceType[faceCount];
	if (!faces)
	{
		return false;
	}

	// Initialize the indexes.
	vertexIndex = 0;
	texcoordIndex = 0;
	normalIndex = 0;
	faceIndex = 0;

	// Open the file.
	fin.open(filename);

	// Check if it was successful in opening the file.
	if (fin.fail() == true)
	{
		return false;
	}

	// Read in the vertices, texture coordinates, and normals into the data structures.
	// Important: Also convert to left hand coordinate system since Maya uses right hand coordinate system.
	fin.get(input);
	while (!fin.eof())
	{
		if (input == 'v')
		{
			fin.get(input);

			// Read in the vertices.
			if (input == ' ')
			{
				fin >> vertices[vertexIndex].x >> vertices[vertexIndex].y >> vertices[vertexIndex].z;

				// Invert the Z vertex to change to left hand system.
				vertices[vertexIndex].z = vertices[vertexIndex].z * -1.0f;
				vertexIndex++;
			}

			// Read in the texture uv coordinates.
			if (input == 't')
			{
				fin >> texcoords[texcoordIndex].x >> texcoords[texcoordIndex].y;

				// Invert the V texture coordinates to left hand system.
				texcoords[texcoordIndex].y = 1.0f - texcoords[texcoordIndex].y;
				texcoordIndex++;
			}

			// Read in the normals.
			if (input == 'n')
			{
				fin >> normals[normalIndex].x >> normals[normalIndex].y >> normals[normalIndex].z;

				// Invert the Z normal to change to left hand system.
				normals[normalIndex].z = normals[normalIndex].z * -1.0f;
				normalIndex++;
			}
		}

		// Read in the faces.
		if (input == 'f')
		{
			fin.get(input);
			if (input == ' ')
			{
				// Read the face data in backwards to convert it to a left hand system from right hand system.
				fin >> faces[faceIndex].vIndex3 >> input2 >> faces[faceIndex].tIndex3 >> input2 >> faces[faceIndex].nIndex3
					>> faces[faceIndex].vIndex2 >> input2 >> faces[faceIndex].tIndex2 >> input2 >> faces[faceIndex].nIndex2
					>> faces[faceIndex].vIndex1 >> input2 >> faces[faceIndex].tIndex1 >> input2 >> faces[faceIndex].nIndex1;
				faceIndex++;
			}
		}

		// Read in the remainder of the line.
		//while (input != '\n')
		//{
		//	fin.get(input);
		//}

		// Start reading the beginning of the next line.
		fin.get(input);
	}

	// Close the file.
	fin.close();

	// Open the output file.
	char OpenFilename[128];
	strcpy_s(OpenFilename, "../resource/objTest.txt");

	//OpenFilename
	
	fout.open(OpenFilename);
	if (!fout.is_open())
	{
		int a = 1;
		return false;
	}

	// Write out the file header that our model format uses.
	fout << "Vertex Count: " << (faceCount * 3) << endl;
	fout << endl;
	fout << "Data:" << endl;
	fout << endl;

	// Now loop through all the faces and output the three vertices for each face.
	for (int i = 0; i < faceIndex; i++)
	{
		vIndex = faces[i].vIndex1 - 1;
		tIndex = faces[i].tIndex1 - 1;
		nIndex = faces[i].nIndex1 - 1;

		fout << vertices[vIndex].x << ' ' << vertices[vIndex].y << ' ' << vertices[vIndex].z << ' '
			<< texcoords[tIndex].x << ' ' << texcoords[tIndex].y << ' '
			<< normals[nIndex].x << ' ' << normals[nIndex].y << ' ' << normals[nIndex].z << endl;

		vIndex = faces[i].vIndex2 - 1;
		tIndex = faces[i].tIndex2 - 1;
		nIndex = faces[i].nIndex2 - 1;

		fout << vertices[vIndex].x << ' ' << vertices[vIndex].y << ' ' << vertices[vIndex].z << ' '
			<< texcoords[tIndex].x << ' ' << texcoords[tIndex].y << ' '
			<< normals[nIndex].x << ' ' << normals[nIndex].y << ' ' << normals[nIndex].z << endl;

		vIndex = faces[i].vIndex3 - 1;
		tIndex = faces[i].tIndex3 - 1;
		nIndex = faces[i].nIndex3 - 1;

		fout << vertices[vIndex].x << ' ' << vertices[vIndex].y << ' ' << vertices[vIndex].z << ' '
			<< texcoords[tIndex].x << ' ' << texcoords[tIndex].y << ' '
			<< normals[nIndex].x << ' ' << normals[nIndex].y << ' ' << normals[nIndex].z << endl;
	}

	// Close the output file.
	fout.close();

	// Release the four data structures.
	if (vertices)
	{
		delete[] vertices;
		vertices = 0;
	}
	if (texcoords)
	{
		delete[] texcoords;
		texcoords = 0;
	}
	if (normals)
	{
		delete[] normals;
		normals = 0;
	}
	if (faces)
	{
		delete[] faces;
		faces = 0;
	}
	/*
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

	// 한 줄의 문자열을 공백 기준으로 모두 나눠서 contents에 저장.
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
m_model = new ModelType[m_vertexCount * 3];
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
*/

	return true;
}