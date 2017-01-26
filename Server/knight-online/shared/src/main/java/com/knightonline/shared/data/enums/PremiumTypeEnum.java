package com.knightonline.shared.data.enums;

import java.io.Serializable;
import java.util.HashMap;
import java.util.Map;
import java.util.Map.Entry;

/**
 * @author Mamaorha
 *
 */
public enum PremiumTypeEnum implements Serializable
{
	BRONZE("BRONZE"), SILVER("SILVER"), GOLD("GOLD"), PLATINUM("PLATINUM");
	
	private String value;
	private static Map<String, PremiumTypeEnum> namesMap = new HashMap<String, PremiumTypeEnum>();

	PremiumTypeEnum(String value)
	{
		this.value = value;
	}

	public String getValue()
	{
		return this.value;
	}

	static
	{
		final PremiumTypeEnum[] values = PremiumTypeEnum.values();
		
		for (PremiumTypeEnum temp : values)
		{
			namesMap.put(temp.getValue().toLowerCase(), temp);
		}
	}

	public static PremiumTypeEnum forValue(String value)
	{
		if (null == value)
		{
			return null;
		}

		return namesMap.get(value.toLowerCase());
	}
	
	public String toValue()
	{
		for (Entry<String, PremiumTypeEnum> entry : namesMap.entrySet())
		{
			if (entry.getValue() == this)
				return entry.getKey();
		}
		
		return null; 
	}
}
