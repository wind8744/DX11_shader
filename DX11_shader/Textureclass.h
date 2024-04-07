#pragma once
#ifndef _TEXTURECLASS_H_
#define _TEXTURECLASS_H_


//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <stdio.h>


////////////////////////////////////////////////////////////////////////////////
//  단일 텍스쳐 리소스 로드, 언로드 및 액세스를 캡슐화
//  각 텍스쳐에 대해 이 클래스의 객체를 인스턴트화 하여 사용.
////////////////////////////////////////////////////////////////////////////////
class Textureclass
{
private:
	// 텍스쳐 Targa 파일의 헤더 구조 정의
	struct TargaHeader
	{
		unsigned char data1[12];
		unsigned short width;
		unsigned short height;
		unsigned char bpp;
		unsigned char data2;
	};

public:
	Textureclass();
	Textureclass(const Textureclass&);
	~Textureclass();

	bool Initialize(ID3D11Device*, ID3D11DeviceContext*, char*);
	void Shutdown();

	ID3D11ShaderResourceView* GetTexture();

	int GetWidth();
	int GetHeight();

private:
	bool LoadTarga32Bit(char*);

private:
	unsigned char* m_targaData;// 파일에서 읽은 원래의 Targa 데이터를 보유
	ID3D11Texture2D* m_texture;
	ID3D11ShaderResourceView* m_textureView; // 쉐이더가 그림을 그릴 때 텍스쳐 데이터에 접근하는데 사용하는 리소스 뷰
	int m_width, m_height;
};

#endif