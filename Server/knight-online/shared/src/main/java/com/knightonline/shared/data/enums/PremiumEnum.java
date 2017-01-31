package com.knightonline.shared.data.enums;

import java.io.Serializable;
import java.util.HashMap;
import java.util.Map;

/**
 * @author Mamaorha
 *
 */
public enum PremiumEnum implements Serializable
{
	NONE(0), BRONZE(1), SILVER(2), GOLD(3), PLATINUM(4);
	private static Map<Integer, PremiumEnum> namesMap = new HashMap<>();

	private Integer value;
	
	PremiumEnum(Integer value)
	{
		this.value = value;
	}

	public Integer getValue()
	{
		return this.value;
	}
	
	public static PremiumEnum forValue(Integer value)
	{
		return namesMap.get(value);
	}
	
	static
	{
		final PremiumEnum[] values = PremiumEnum.values();
		
		for (PremiumEnum temp : values)
		{
			namesMap.put(temp.getValue(), temp);
		}
	}
}
