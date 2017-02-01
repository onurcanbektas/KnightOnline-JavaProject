#ifndef PREMIUM_H
#define PREMIUM_H

#include "Enum.h"
#include "MapManager.h"

class PremiumEnum : public Enum<short>
{
private:
	PremiumEnum(std::string name, short value) : Enum<short>(name, value) {};

	static PremiumEnum & createEnum(std::string name, short value)
	{
		if (values().get(value) == nullptr)
		{
			PremiumEnum * temp = new PremiumEnum(name, value);
			values().put(value, temp);
		}

		return *values().get(value);
	}

	static MapManager<short, PremiumEnum *> & values()
	{
		static MapManager<short, PremiumEnum *> INSTANCE;
		return INSTANCE;
	}

public:
	static PremiumEnum & forValue(short value)
	{
		PremiumEnum * temp = values().get(value);

		if (temp == nullptr)
		{
			throw "invalid value for PremiumEnum";
		}

		return *temp;
	}

	static PremiumEnum & NONE;
	static PremiumEnum & BRONZE;
	static PremiumEnum & SILVER;
	static PremiumEnum & GOLD;
	static PremiumEnum & PLATINUM;
};
#endif