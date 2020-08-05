
#include <vector>
#include <iterator>
#include <sstream>

#define IsDivisibleBy64(x) ((x/64)==64)
#define IsDisibibleByLocalWorkSize(globalWorkSize,localWorkSize) (globalWorkSize[1] % localWorkSize[0] == 0 && globalWorkSize[0] % localWorkSize[0] == 0)
#define MiByteDiv 1024 / 1024
#define MByte 1024*1024
#define KiByte 1024
#define IsSizeMB(x) ((x/MiByteDiv)!=0)
#define IsSizeKB(x) ((x/KiByte)!=0)

#define BlueShift 0
#define GreenShift 8
#define RedShift 16
#define AlphaShift 24

#define GetRGBA(R,G,B,A) (((DWORD)(B) << BlueShift) | ((DWORD)(G) << GreenShift) | ((DWORD)(R) << RedShift) | ((DWORD)(A) << AlphaShift))
#define GetRGB(R,G,B) ((COLORREF)(((BYTE)(R) | ((WORD)((BYTE)(A)) << 8)) | (((DWORD)(BYTE)(B)) << 16)))


typedef double Fp64;
typedef long double LongFp64;
typedef float Fp32;

typedef unsigned long long ULongInt64;
typedef unsigned long UInt64;
typedef long long LongInt64;

typedef long Int64;
typedef unsigned int UInt32;
typedef int Int32;

typedef unsigned short UInt16;
typedef short Int16;

typedef unsigned char Byte;
typedef char Char;
typedef signed char SChar;



enum OCL_TY
{
	Histogram = 0,
	Interpolation = 1,
};