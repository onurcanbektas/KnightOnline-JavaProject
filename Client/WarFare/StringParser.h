#ifndef STRING_PARSER_H
#define STRING_PARSER_H

#include<string>>

class StringParser
{
public:
	static std::string parse(const char * base, std::string arg);
	static std::string parse(const char * base, std::string arg, std::string arg2);
	static std::string parse(const char * base, std::string arg, std::string arg2, std::string arg3);
};
#endif