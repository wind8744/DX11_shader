#pragma once
////////////////////////////////////////////////////////////////////////////////
// Filename: inputclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _INPUTCLASS_H_
#define _INPUTCLASS_H_


////////////////////////////////////////////////////////////////////////////////
// Class name: InputClass
////////////////////////////////////////////////////////////////////////////////
class Inputclass
{
public:
	Inputclass();
	Inputclass(const Inputclass&);
	~Inputclass();

	void Initialize();

	void KeyDown(unsigned int);
	void KeyUp(unsigned int);

	bool IsKeyDown(unsigned int);

private:
	bool m_keys[256];
};

#endif