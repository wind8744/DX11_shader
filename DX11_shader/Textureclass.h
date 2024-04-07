#pragma once
#ifndef _TEXTURECLASS_H_
#define _TEXTURECLASS_H_


//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <stdio.h>


////////////////////////////////////////////////////////////////////////////////
//  ���� �ؽ��� ���ҽ� �ε�, ��ε� �� �׼����� ĸ��ȭ
//  �� �ؽ��Ŀ� ���� �� Ŭ������ ��ü�� �ν���Ʈȭ �Ͽ� ���.
////////////////////////////////////////////////////////////////////////////////
class Textureclass
{
private:
	// �ؽ��� Targa ������ ��� ���� ����
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
	unsigned char* m_targaData;// ���Ͽ��� ���� ������ Targa �����͸� ����
	ID3D11Texture2D* m_texture;
	ID3D11ShaderResourceView* m_textureView; // ���̴��� �׸��� �׸� �� �ؽ��� �����Ϳ� �����ϴµ� ����ϴ� ���ҽ� ��
	int m_width, m_height;
};

#endif