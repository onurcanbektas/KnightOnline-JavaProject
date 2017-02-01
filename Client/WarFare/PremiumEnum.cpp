#include "PremiumEnum.h"

PremiumEnum & PremiumEnum::NONE = createEnum("NONE", 0);
PremiumEnum & PremiumEnum::BRONZE = createEnum("BRONZE", 1);
PremiumEnum & PremiumEnum::SILVER = createEnum("SILVER", 2);
PremiumEnum & PremiumEnum::GOLD = createEnum("GOLD", 3);
PremiumEnum & PremiumEnum::PLATINUM = createEnum("PLATINUM", 4);