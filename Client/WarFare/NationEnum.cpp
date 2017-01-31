#include "NationEnum.h"

NationEnum & NationEnum::instance()
{
	static NationEnum INSTANCE;
	return INSTANCE;
}

NationEnum::NationEnum() {}

NationEnum::NationEnum(char * name, short value) : Enum(name, value)
{
	instance().values[value] = this;
}

NationEnum & NationEnum::forValue(short value)
{
	Enum * enumType = instance().forValueT(value);

	return *(NationEnum *)enumType;
}

const NationEnum & NationEnum::NO_NATION = NationEnum("NO_NATION", 0);
const NationEnum & NationEnum::KARUS = NationEnum("KARUS", 1);
const NationEnum & NationEnum::ELMORAD = NationEnum("ELMORAD", 2);