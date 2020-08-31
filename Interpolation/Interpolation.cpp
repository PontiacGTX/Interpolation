#define _CRT_SECURE_NO_DEPRECATE
#include <iostream>
#include "GDIPlusToken.h"
#include "Image.h"
#include "HFunctions.h"





void InterpolationCPU(std::string& inputPath, std::string& outputPath, UInt64 Ratio)
{

	GDIPlusToken token(inputPath);
	Image* imageObj = new Image(token.GetImage(), PixelFormat32bppARGB, PixelFormat32bppARGB, Ratio);
	imageObj->GetPixels();
	auto res = imageObj->originalWidth * imageObj->originalHeight;

	res = imageObj->OutputLength();
	StartTimer();
	imageObj->Interpolate();
	double seconds = StopTimer().count();
	std::cout << " elapsed time: " << std::fixed << std::setprecision(10) << seconds << "s \n";
	imageObj->SaveFile(outputPath);
}







int main(int argc, char** argv)
{


	if (argc > 1)
	{
		//InterpolationCL.exe -C:\Users\PontiacGTX\Desktop\image.jpg -d -fn1 -f4 -cpu
		//alternatively InterpolationCL.exe -C:\Users\PontiacGTX\Desktop\image.jpg -C:\Users\PontiacGTX\Desktop\image1.jpg -fn1 -f4 -cpu



		std::vector<std::string> arguments;

		arguments.assign(argv + 2, argv + argc);
		std::string inputPath(arguments[0].substr(1, arguments[0].size() - 1));

		if (FileExist(inputPath))
		{
			std::string outputPath;
			if (IsDefault((arguments[1])[1]))
			{
				WCHAR  path[MAX_PATH];
				GetModuleFileNameW(NULL, path, MAX_PATH);
				std::wstring current(path);
				outputPath = (arguments[0].substr(arguments[0].find_last_of('\\') + 1, arguments[0].size() - (arguments[0].find_last_of('\\') + 1)));
				current = current.substr(0, (current.find_last_of('\\') + 1));
				outputPath = std::string(current.begin(), current.end()) + outputPath;

			}
			else
			{
				outputPath = arguments[1].substr(1, arguments[1].size() - 1);
			}

			int function = std::stoi(arguments[2].substr(3, arguments[2].size() - 3));
			UInt64 ratio = (int)std::stoi(arguments[3].substr(2, arguments[3].size() - 2));
			std::string device(arguments[4].substr(1, arguments[4].size() - 1));


			if (function == 1)
			{
			  InterpolationCPU(inputPath, outputPath, ratio);

			}
		}

	}
	else
	{
		if (argc == 1)
			throw  std::runtime_error("Console Command Line Argument is empty");
		else
			throw  std::runtime_error("Input file Does not exist or Insuficient command arguments");
	}

	return 0;

}


