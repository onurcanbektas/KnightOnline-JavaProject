#ifndef SELECT_CHARACTER_POSITION_H
#define SELECT_CHARACTER_POSITION_H

#include "Enum.h"
#include "MapManager.h"

class SelectCharacterPositionEnum : public Enum<short>
{
private:
	SelectCharacterPositionEnum(std::string name, short value) : Enum<short>(name, value) {};

	static SelectCharacterPositionEnum & createEnum(std::string name, short value)
	{
		if (values().get(value) == nullptr)
		{
			SelectCharacterPositionEnum * temp = new SelectCharacterPositionEnum(name, value);
			values().put(value, temp);
		}

		return *values().get(value);
	}

	static MapManager<short, SelectCharacterPositionEnum *> & values()
	{
		static MapManager<short, SelectCharacterPositionEnum *> INSTANCE;
		return INSTANCE;
	}

public:
	static SelectCharacterPositionEnum & forValue(short value)
	{
		SelectCharacterPositionEnum * temp = values().get(value);

		if (temp == nullptr)
		{
			throw "invalid value for SelectCharacterPositionEnum";
		}

		return *temp;
	}

	static SelectCharacterPositionEnum & CENTER;
	static SelectCharacterPositionEnum & LEFT;
	static SelectCharacterPositionEnum & RIGHT;
};
#endif