#ifndef COLOR_H
#define COLOR_H
#include <Windows.h>
#include <gdiplus.h>
#include <gdipluspixelformats.h>
#include "CLSIDFields.h"
#include "Common.h"
#pragma comment(lib,"gdiplus.lib")




class Color
{
	
public:
	Color() = default;
	Color(byte r, byte g, byte b, byte a) : R(r), G(g), B(b), A(a) { };
	Color(byte r, byte g, byte b) : R(r), G(g), B(b), A(1) { };
	Color(Gdiplus::ARGB c)
	{
		SetColor(*this,c);
	}
	Color(Gdiplus::Color& c)
	{
		this->A = (float)c.GetA();
		this->R = (float)c.GetR();
		this->G = (float)c.GetG();
		this->B = (float)c.GetB();
	}
	float R;
	float G;
	float B;
	float A;
	void SetColor(Color& c, unsigned int n);
};

void Color::SetColor(Color& c, unsigned int n)
{
	union {
		struct { uint8_t B, G, R, A; };
		uint32_t BGRA;
	} color;

	color.BGRA = n;

	c.R = (float)color.R;
	c.G = (float)color.G;
	c.B = (float)color.B;
	c.A = (float)color.A;
}

#endif //..