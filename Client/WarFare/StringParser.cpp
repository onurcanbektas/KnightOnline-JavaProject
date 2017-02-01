#include "StringParser.h"

std::string StringParser::parse(const char * base, std::string arg)
{
	char stemp[1000];
	snprintf(stemp, 1000, base, arg.c_str());

	std::string sd = stemp;
	
	return sd;
}