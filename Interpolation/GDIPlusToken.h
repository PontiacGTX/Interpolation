#ifndef GDI_PLUS_TOKEN_H
#define GDI_PLUS_TOKEN_H
#include "Color.h"

class Color;
class GDIPlusToken
{
private:
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	std::wstring filePath;
	Gdiplus::Image *image;
	Gdiplus::PixelFormat format;
public:

	

	GDIPlusToken(std::string path)
	{
		Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
		filePath = std::wstring(path.begin(), path.end());
		if (CLSIDCode::GetCLSID(path.substr(path.find_last_of('.') + 1, path.size() - (path.find_last_of('.') + 1))) == "")
		{
			std::string  error(path + " does not contain a valid file type supported by GDI+");
			throw std::exception(error.c_str());
		}
		image = Gdiplus::Image::FromFile(filePath.c_str());
		format = image->GetPixelFormat();

	}
	inline Gdiplus::Image* GetImage()
	{
		return image;
	}
	inline Gdiplus::PixelFormat& GetFormat()
	{
		return format;
	}



};

#endif
