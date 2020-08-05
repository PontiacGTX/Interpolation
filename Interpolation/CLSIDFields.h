#ifndef CLSID_FIELDS_H
#define CLSID_FIELDS_H
#include <map>

class CLSIDCode
{
public:
	
	static std::string GetCLSID(std::string type)
	{
		std::map<std::string, std::string> CLSIDm
		{
			{"bmp","{557cf400-1a04-11d3-9a73-0000f81ef32e}"},
			{"jpg","{557cf401-1a04-11d3-9a73-0000f81ef32e}" },
			{"gif","{557cf402-1a04-11d3-9a73-0000f81ef32e}" },
			{"tiff","{557cf405-1a04-11d3-9a73-0000f81ef32e}" },
			{"png", "{557cf406-1a04-11d3-9a73-0000f81ef32e}"}
		};

		return CLSIDm[type];
	}
};
#endif