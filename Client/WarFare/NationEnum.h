#ifndef NATION_H
#define NATION_H

#include "Enum.h"

class NationEnum : public Enum<short>
{
private:
	NationEnum();
	NationEnum(char * name, short value);
	static NationEnum & instance();

public:
	static NationEnum & forValue(short value);
	static const NationEnum & NO_NATION;
	static const NationEnum & KARUS;
	static const NationEnum & ELMORAD;
};
#endif