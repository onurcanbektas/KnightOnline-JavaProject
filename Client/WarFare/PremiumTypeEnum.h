#include <iostream>
#include <string>
#include <map>

struct PremiumType
{
public:
	enum PremiumTypeEnum
	{
		NONE, BRONZE, SILVER, GOLD, PLATINUM
	};

	static std::string getName(PremiumTypeEnum premiumTypeEnum)
	{
		static std::map<PremiumTypeEnum, std::string> strings;

		if (strings.empty())
		{
			strings[PremiumType::NONE] = "NONE";
			strings[PremiumType::BRONZE] = "BRONZE";
			strings[PremiumType::SILVER] = "SILVER";
			strings[PremiumType::GOLD] = "GOLD";
			strings[PremiumType::PLATINUM] = "PLATINUM";
		}

		return strings[premiumTypeEnum];
	}

	static int value(PremiumTypeEnum premiumTypeEnum)
	{
		switch (premiumTypeEnum)
		{
		case PremiumType::NONE:
			return 0;
		case PremiumType::BRONZE:
			return 1;
		case PremiumType::SILVER:
			return 2;
		case PremiumType::GOLD:
			return 3;
		case PremiumType::PLATINUM:
			return 4;
		}
	}

	static PremiumTypeEnum forValue(int value)
	{
		switch (value)
		{
		case 1:
			return PremiumType::BRONZE;
		case 2:
			return PremiumType::SILVER;
		case 3:
			return PremiumType::GOLD;
		case 4:
			return PremiumType::PLATINUM;
		case 0:
		default:
			return PremiumType::NONE;
		}
	}
};