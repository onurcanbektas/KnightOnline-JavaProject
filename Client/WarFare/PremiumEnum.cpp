#include "PremiumEnum.h"

PremiumEnum & PremiumEnum::instance()
{
	static PremiumEnum INSTANCE;
	return INSTANCE;
}

PremiumEnum::PremiumEnum() {}

PremiumEnum::PremiumEnum(char * name, short value) : Enum(name, value)
{
	instance().values[value] = this;
}

PremiumEnum & PremiumEnum::forValue(short value)
{
	Enum * enumType = instance().forValueT(value);

	return *(PremiumEnum *)enumType;
}

const PremiumEnum & PremiumEnum::NONE = PremiumEnum("NONE", 0);
const PremiumEnum & PremiumEnum::BRONZE = PremiumEnum("BRONZE", 1);
const PremiumEnum & PremiumEnum::SILVER = PremiumEnum("SILVER", 2);
const PremiumEnum & PremiumEnum::GOLD = PremiumEnum("GOLD", 3);
const PremiumEnum & PremiumEnum::PLATINUM = PremiumEnum("PLATINUM", 4);