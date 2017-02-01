#ifndef NATION_H
#define NATION_H

#include "Enum.h"
#include "MapManager.h"
class NationEnum : public Enum<short>
{
private:
	NationEnum(std::string name, short value) : Enum<short>(name, value) {};

	static NationEnum & createEnum(std::string name, short value)
	{
		if (values().get(value) == nullptr)
		{
			NationEnum * temp = new NationEnum(name, value);
			values().put(value, temp);
		}

		return *values().get(value);
	}

	static MapManager<short, NationEnum *> & values()
	{
		static MapManager<short, NationEnum *> INSTANCE;
		return INSTANCE;
	}

public:
	static NationEnum & forValue(short value)
	{
		NationEnum * temp = values().get(value);

		if (temp == nullptr)
		{
			throw "invalid value for NationEnum";
		}
		
		return *temp;
	}

	static NationEnum & NO_NATION;
	static NationEnum & KARUS;
	static NationEnum & ELMORAD;
};
#endif