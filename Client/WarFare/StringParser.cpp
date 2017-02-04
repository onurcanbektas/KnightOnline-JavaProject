#include "StringParser.h"

std::string StringParser::parse(const char * base, std::string arg)
{
	return parse(base, arg, "", "");
}

std::string StringParser::parse(const char * base, std::string arg, std::string arg2)
{
	return parse(base, arg, arg2, "");
}

std::string StringParser::parse(const char * base, std::string arg, std::string arg2, std::string arg3)
{
	char stemp[1000];
	snprintf(stemp, 1000, base, arg.c_str(), arg2.c_str(), arg3.c_str());

	std::string sd = stemp;

	return sd;
}