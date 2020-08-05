#ifndef IMAGE_H
#define IMAGE_H
#include "Color.h"
class Color;
class Image
{

#define MATCH 0
	void Average(Color c1, Color c2, Color c3, Color c4, Color* res);
	void Average(Color c1, Color c2, Color c3, Color* res);
	void Average(Color c1, Color c2, Color* res);

	DWORD _OutputFormat;
	DWORD _InputFormat;
	DWORD _DefaultFormat;
	

	

	void GetColor(Color& c, unsigned int n)
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

	void SetPixels(Gdiplus::Bitmap*& output,UInt64 width,UInt64 height)
	{
		UInt64 x = 0;
		UInt64 y = 0;
		/*Gdiplus::Color  alpha;*/
		for (x = 0; x < width; x++)
		{
			for (y = 0; y < height; y++)
			{
				output->SetPixel(x, y,
										GetRGBA(
												(processedPixels->data() + (x * height + y))->R,
												(processedPixels->data() + (x * height + y))->G,
												(processedPixels->data() + (x * height + y))->B,
												(processedPixels->data() + (x * height + y))->A)
																							   );
			}
		}
	}

	
	void GetPixelsInt(Gdiplus::PixelFormat Flag)
	{
		const Gdiplus::Rect rectangle(0, 0, originalWidth, originalHeight);
		Gdiplus::BitmapData bitmapData;
		bmp->LockBits(&rectangle, Gdiplus::ImageLockMode::ImageLockModeRead, Flag, &bitmapData);
		register UInt64 Index = 0;
		register UInt64 x = 0;
		register UInt64 y = 0;
		for (x = 0; x < originalWidth; x++)
		{
			for (y = 0; y < originalHeight; y++)
			{
				GetColor(*(originalPixels->data() + Index), ((unsigned*)bitmapData.Scan0)[y * bitmapData.Stride / 4 + x]);
				Index++;
			}
		}

		processedPixels->reserve((originalHeight * Ratio) * (originalWidth * Ratio));


	}

	void SetTargetColor(Color* src, Color avg, Color* target, INT64 targetHeight, INT64 targetWidth, INT64 ratio)
	{
		if (targetWidth + (ratio - 1) <= originalWidth * ratio && targetHeight + (ratio -1) <= originalHeight+ratio)
		{
			INT64 height = (originalHeight * Ratio);

			//set pixels on the corners
			target[(targetWidth)*height + (targetHeight)] = src[0];
			target[(targetWidth + (ratio - 1)) * height + (targetHeight)] = src[1];
			target[(targetWidth)*height + (targetHeight + (ratio - 1))] = src[2];
			target[(targetWidth + (ratio - 1)) * height + (targetHeight + (ratio - 1))] = src[3];

			Color buffer;
			int64_t beginindex = 0;
			int64_t endIndex = ratio - 2;

			for (int64_t x = 0; x < (ratio); x++)
			{
				for (int64_t y = 0; y < (ratio) ; y++)
				{
					if(x > beginindex && x < endIndex && y > beginindex && y < endIndex)
					target[(targetWidth + x) * height + (targetWidth + y)] = avg;
					else if (y == beginindex && x > 0 && x < endIndex || y == (ratio - 1) && x > beginindex && x < endIndex)
					{
						//Avg between left and right pixels
						Average(target[(targetWidth + x - 1) * height + (targetWidth + y)], target[(targetWidth + x + (ratio - 1)) * height + (targetWidth + y)], &buffer);
						
						for (int64_t i = 0; i < (ratio - 2); i++)
						{
							target[(targetWidth + x + i) * height + (targetWidth + y)] = buffer;
						}

						x=ratio-1;
					}
					else if (x == beginindex && y > 0 && y < endIndex || x == (ratio - 1) && y > beginindex && y < endIndex)
					{
						Average(target[(targetWidth + x ) * height + (targetWidth + y - 1)], target[(targetWidth + x) * height + (targetWidth + y + (ratio - 1))], &buffer);
						for (int64_t i = 0; i < (ratio - 2); i++)
						{
							target[(targetWidth + x + i) * height + (targetWidth + y)] = buffer;
						}
						x = ratio - 1;
						//y=ratio-1;
					}


					/*target[(targetWidth - x) * height + (targetHeight + y)] = avg;
					target[(targetWidth + x) * height + (targetHeight + y)] = avg;
					target[(targetWidth - x) * height + (targetHeight - y)] = avg;
					target[(targetWidth + x) * height + (targetHeight - y)] = avg;


					target[(targetWidth)*height + (targetHeight - y)] = avg;
					target[(targetWidth)*height + (targetHeight + y)] = avg;
					target[(targetWidth - x) * height + (targetHeight)] = avg;
					target[(targetWidth + x) * height + (targetHeight)] = avg;*/

				}
			}
		}
	}

	
	inline const bool& IsValidIndexX(INT64 tWIndex, INT64 tHIndex)
	{
		return tWIndex <  Ratio * originalWidth;
	}

	inline const bool& IsValidIndexY(INT64 tWIndex, INT64 tHIndex)
	{
		return tHIndex < Ratio * originalHeight;
	}

	inline const bool& IsValidIndexXY(INT64 CONST tWIndex, INT64 CONST tHIndex, INT64 CONST widthLimit, INT64 CONST heightLimit)
	{
		return ((tWIndex < widthLimit) && tHIndex < heightLimit);
	}

	inline const bool& IsValidIndexXY(INT64 tWIndex, INT64 tHIndex)
	{
		return IsValidIndexXY(tWIndex, tHIndex, Ratio * originalWidth, Ratio * originalHeight);
	}

	void InterpolationByTiles(Color* source, Color* target, int64_t sh, int64_t  sw, int64_t Tile)
	{
		std::vector<Color> vc;
		register int64_t wIdx = 0;
		register int64_t hIdx = 0;
		register int64_t Index = 0;
		Color* clr = new Color[4];
		int64_t index = 0;
		Color res((Gdiplus::Color(Gdiplus::Color::White)).GetValue());

		for (wIdx = 0; wIdx < sw; wIdx++)
		{
			for (hIdx = 0; hIdx < sh; hIdx++)
			{
				if (wIdx + 1 < sw && hIdx + 1 < sh)
				{
					if (IsValidIndexXY((wIdx * Tile) - 1, (hIdx * Tile) - 1))
					{
						clr[0] = source[wIdx * sh + hIdx];
						clr[1] = source[(wIdx + 1) * sh + hIdx];
						clr[2] = source[wIdx * sh + (hIdx + 1)];
						clr[3] = source[(wIdx + 1) * sh + (hIdx + 1)];

						index = (wIdx * Tile - 1 >= 0) && (hIdx + Tile - 1 >= 0) ?
							((wIdx * Tile) - 1) * (sw * Tile) + ((hIdx * Tile) - 1) : ((wIdx * Tile)) * (sw * Tile) + ((hIdx * Tile));

					
							Average(clr[0], clr[1], clr[2], clr[3], &res);
							SetTargetColor(clr, res, target, 
											(hIdx * Tile) - 1 < 0 ? (hIdx * Tile) : (hIdx * Tile) - 1, 
											(wIdx * Tile) - 1 < 0 ? (wIdx * Tile) : (wIdx * Tile) - 1,
														Tile);
					}
					else
					{

					}
				}
			}
		}


	}


	

	void Interpolation(Color* source, Color* target, INT64 width, INT64 height, INT64 ratio)
	{

		INT64 wIdx = 0;
		INT64 hIdx = 0;
		INT64 Index = 0;
		INT64 limit = (width / ratio) * (height / ratio);
		Gdiplus::Color black = Gdiplus::Color::White;

		for (wIdx = 0; wIdx < width; wIdx++)
		{
			for (hIdx = 0; hIdx < height; hIdx++)
			{
				target[wIdx * height + hIdx].R = black.GetR();
				target[wIdx * height + hIdx].G = black.GetG();
				target[wIdx * height + hIdx].B = black.GetB();
				target[wIdx * height + hIdx].A = black.GetA();
				
				if (wIdx % ratio == MATCH && hIdx % ratio == MATCH && (Index = (wIdx / ratio) * (height / ratio) + (hIdx / ratio))< limit)
				{
					/*Index = (wIdx / ratio) * (height/ratio) + (hIdx / ratio);
					if ((Index  < limit))*/
					{
						target[wIdx * height + hIdx].R = source[Index].R;
						target[wIdx * height + hIdx].G = source[Index].G;
						target[wIdx * height + hIdx].B = source[Index].B;
						target[wIdx * height + hIdx].A = source[Index].A;

					//Index++;
					}
				}

			}

		}

		UInt64 t = 3;
		UInt64 t1 = 3;

		std::vector<UInt64> surrounding;

		black = Gdiplus::Color::Orange;
		INT64 Offset = ratio / 2;
		Color blk(black.GetR(),black.GetG(),black.GetB(),black.GetA());
		
		for (auto i = 1; i <= Offset - 1; i++)
			surrounding.push_back(i);

		UInt64 CurrentIndex = 0;
		
		UInt64 LeftPixel = ratio - 1;
		UInt64 UpperPixel = LeftPixel;
		bool result;
		UInt64 square = pow(surrounding.size(), 2);
		UInt64 sqrtlen = surrounding.size();


		for (register int64_t wIndex = 0; wIndex < width; wIndex++)
		{
			for (register int64_t hIndex = 0; hIndex < height; hIndex++)
			{
				if (wIndex % ratio != MATCH && hIndex % ratio != MATCH)
				{
					if ((hIndex + Offset < height) && (wIndex + Offset < width) && 
						!(wIndex-Offset< 0) && wIndex % Offset == 0 && hIndex % Offset == 0 )
					{ 
						
						Color c1 = target[(wIndex - Offset) * height + (hIndex + Offset)];
						Color c2 = target[(wIndex + Offset) * height + (hIndex + Offset)];
						Color c3 = target[(wIndex - Offset) * height + (hIndex - Offset)];
						Color c4 = target[(wIndex + Offset) * height + (hIndex - Offset)];
						Average(target[(wIndex - Offset) * height + (hIndex + Offset)], target[(wIndex + Offset) * height + (hIndex + Offset)], target[(wIndex - Offset) * height + (hIndex - Offset)], target[(wIndex + Offset) * height + (hIndex - Offset)], &target[wIndex * height + hIndex]);
						

						

						for (int64_t x = 0; x < surrounding.size(); x++)
						{
							for (int64_t y = 0; y < surrounding.size(); y++)
							{
								target[(wIndex - surrounding[x]) * height + (hIndex + surrounding[y])] = target[wIndex * height + hIndex];
								target[(wIndex + surrounding[x]) * height + (hIndex + surrounding[y])] = target[wIndex * height + hIndex];
								target[(wIndex - surrounding[x]) * height + (hIndex - surrounding[y])] = target[wIndex * height + hIndex];
								target[(wIndex + surrounding[x]) * height + (hIndex - surrounding[y])] = target[wIndex * height + hIndex];


								target[(wIndex) * height + (hIndex - surrounding[y])] = target[wIndex * height + hIndex];
								target[(wIndex) * height + (hIndex + surrounding[y])] = target[wIndex * height + hIndex];
								target[(wIndex - surrounding[x]) * height + (hIndex)] = target[wIndex * height + hIndex];
								target[(wIndex + surrounding[x]) * height + (hIndex)] = target[wIndex * height + hIndex];

							}
						}

						
					}
					else if ((hIndex + Offset < height) && (wIndex + Offset < width) && (wIndex - Offset < 0))
					{
						
						Average(target[(wIndex + Offset) * height + (hIndex)], target[(wIndex + Offset) * height + (hIndex - Offset)], &target[wIndex * height + hIndex]);
					}
					else if (hIndex + Offset > height && wIndex + Offset < width && wIndex - Offset >=0)
					{
						Average(target[(wIndex - Offset) * height + (hIndex - Offset)], target[(wIndex + Offset) * height + (hIndex - Offset)], &target[wIndex * height + hIndex]);
						
					}
					else if (hIndex + Offset < height && wIndex + Offset > width)
					{
						Average(target[(wIndex - Offset) * height + (hIndex - Offset)], target[(wIndex - Offset) * height + (hIndex + Offset)], &target[wIndex * height + hIndex]);
						
					}
				}
				else if (wIndex % ratio != MATCH && hIndex % ratio == MATCH || wIndex % ratio == MATCH && hIndex % ratio != MATCH)
				{
					
					if (wIndex % 2 != MATCH && wIndex!=0 && wIndex - LeftPixel - 1 >= 0 && (wIndex+ 1) % ratio== MATCH )
					{
					
							for (int64_t i = LeftPixel -1; i >= 0; i--)
							{
								

								if (wIndex - 1 >= 0 && hIndex + 1 < height && hIndex - 1 >= 0 && wIndex + 1 < width)
									Average(target[(wIndex - i) * height + hIndex + 1], 
											target[(wIndex - i) * height + hIndex - 1],
											target[(wIndex + 1) * height + hIndex],
											target[(wIndex - LeftPixel) * height + hIndex],
										
										&target[(wIndex - i) * height + hIndex]);
								
								else if (wIndex - 1 >= 0 && hIndex + 1 > height && hIndex - 1 >= 0 && wIndex + 1 < width)
									Average(target[(wIndex - i) * height + hIndex - 1], 
										target[(wIndex + 1) * height + hIndex],
											target[(wIndex - LeftPixel) * height + hIndex],
										
										&target[(wIndex - i) * height + hIndex]);
								
								else if (wIndex - 1 >= 0 && hIndex + 1 < height && hIndex - 1 < 0 && wIndex + 1 < width)
									Average(target[(wIndex - i) * height + hIndex + 1], 
										    target[(wIndex + 1) * height + hIndex],
											target[(wIndex - LeftPixel) * height + hIndex],
										
										&target[(wIndex - i) * height + hIndex]);

								
								
								
								
							}
					}//hIndex % 2 != MATCH && hIndex != 0 && hIndex - UpperPixel - 1 >= 0 && (hIndex + 1) % ratio == MATCH
					else if (hIndex - UpperPixel - 1 >= 0 && (hIndex + 1) % ratio == MATCH  && hIndex - UpperPixel - 1 >= 0)
					{
						for (int64_t i = UpperPixel - 1; i >= 0; i--)
						{

							
							if (hIndex - 1 >= 0 && wIndex + 1 < width && wIndex - 1 >= 0 && hIndex + 1 < height)
								Average(
									target[(wIndex-1) * height + hIndex - i],
									target[(wIndex) * height + (hIndex+1)],
									target[(wIndex) * height + (hIndex-UpperPixel)],

									&target[(wIndex ) * height + (hIndex - i)]);

							else if (hIndex - 1 >= 0 && wIndex + 1 > width&& wIndex - 1 >= 0 && hIndex + 1 < height)
								Average(
									target[(wIndex ) * height + (hIndex + 1)],
									target[(wIndex) * height + (hIndex - UpperPixel)],

									&target[(wIndex) * height + (hIndex - i)]);

							else if (hIndex - 1 >= 0 && wIndex + 1 < width && wIndex - 1 < 0 && hIndex + 1 < height)
								Average(
									target[(wIndex) * height + (hIndex+1)],
									target[(wIndex) * height + (hIndex-UpperPixel)],

									&target[(wIndex) * height + (hIndex-i)]);

						}
					}
					else if (wIndex == 0 && hIndex!=0)
					{
						target[(wIndex) * height + hIndex] = target[(wIndex) * height + hIndex-1];
					}
				
				}
			}
		}




	}
	

public:
	std::vector<Color>* originalPixels;
	std::vector<Color>* processedPixels;
	UInt64 originalWidth;
	UInt64 originalHeight;
	Gdiplus::Bitmap* bmp;
	unsigned int Ratio;
	
	Image(Gdiplus::Image* img, DWORD InputFormat, DWORD OutputFormat, unsigned int Ratio, INT TYPE)
	{

		originalPixels = new std::vector<Color>();
		processedPixels = new std::vector<Color>();
		this->Ratio = Ratio;
		

		originalWidth = img->GetWidth();
		originalHeight = img->GetHeight();
		originalPixels->reserve(originalWidth * originalHeight);



		processedPixels->reserve((originalWidth * originalHeight) * Ratio);
		_OutputFormat = OutputFormat;
		_InputFormat = InputFormat;
		_DefaultFormat = img->GetPixelFormat();


		bmp = (Gdiplus::Bitmap*)img;

	}
	Image(Gdiplus::Image *img,DWORD InputFormat,DWORD OutputFormat,unsigned int Ratio)
	{

		originalPixels = new std::vector<Color>();
		processedPixels = new std::vector<Color>();
		originalWidth = img->GetWidth();
		originalHeight = img->GetHeight();
		originalPixels->resize(originalWidth * originalHeight);

		_OutputFormat = OutputFormat;
		_InputFormat = InputFormat;
		_DefaultFormat = img->GetPixelFormat();

		this->Ratio = Ratio;
		bmp = (Gdiplus::Bitmap*)img;

	}

	const size_t& SourceLength()
	{
		return (size_t)(originalWidth*originalHeight);
	}

	const size_t& OutputLength()
	{
		return (size_t)((originalWidth *Ratio) * (originalHeight*Ratio));
	}
	void GetPixels()
	{
		GetPixelsInt(_InputFormat);
	}
	void InterpolateTiles()
	{
		InterpolationByTiles(originalPixels->data(), processedPixels->data(), originalHeight, originalWidth, Ratio);
	}
	void Interpolate()
	{
		Interpolation(originalPixels->data(), processedPixels->data(), originalWidth * Ratio, originalHeight * Ratio,Ratio);
	}

	void SaveFile(std::string path)
	{

		UInt64 index = path.find_last_of('.') + 1;
		std::string clsid = CLSIDCode::GetCLSID(path.substr(index, path.size() - index));

		if (clsid.empty())
		{
			std::string  error (path + " does not contain a valid file type supported by GDI+");
			throw std::exception(error.c_str());
		}

		Gdiplus::Bitmap* output = new Gdiplus::Bitmap(originalWidth*Ratio, originalHeight*Ratio, _OutputFormat);
		Image::SetPixels(output,originalWidth*Ratio,originalHeight*Ratio);
		CLSID clsidObj; 

		std::wstring   wclsid (clsid.begin(), clsid.end());
		std::wstring   wpath(path.begin(), path.end());

		CLSIDFromString(wclsid.c_str(), &clsidObj);
		Gdiplus::Status status = output->Save(wpath.c_str(), &clsidObj);

	}

};

void Image::Average(Color c1, Color c2, Color c3, Color c4, Color* res)
{

	(*res).A = (c1.A + c2.A + c3.A + c4.A) / 4;
	(*res).R = (c1.R + c2.R + c3.R + c4.R) / 4;
	(*res).G = (c1.G + c2.G + c3.G + c4.G) / 4;
	(*res).B = (c1.B + c2.B + c3.B + c4.B) / 4;
}

void Image::Average(Color c1, Color c2,Color c3, Color* res)
{
	(*res).A = (c1.A + c2.A + c3.A) / 3;
	(*res).R = (c1.R + c2.R + c3.R) / 3;
	(*res).G = (c1.G + c2.G + c3.G) / 3;
	(*res).B = (c1.B + c2.B + c3.B) / 3;
}
void Image::Average(Color c1, Color c2, Color* res)
{
	(*res).A = (c1.A + c2.A) / 2;
	(*res).R = (c1.R + c2.R) / 2;
	(*res).G = (c1.G + c2.G) / 2;
	(*res).B = (c1.B + c2.B) / 2;
}
#endif