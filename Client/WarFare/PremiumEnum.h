#ifndef PREMIUM_H
#define PREMIUM_H

#include "Enum.h"

class PremiumEnum : public Enum<short>
{
private:
	PremiumEnum();
	PremiumEnum(char * name, short value);
	static PremiumEnum & instance();

public:
	static PremiumEnum & forValue(short value);
	static const PremiumEnum & NONE;
	static const PremiumEnum & BRONZE;
	static const PremiumEnum & SILVER;
	static const PremiumEnum & GOLD;
	static const PremiumEnum & PLATINUM;
};
#endif