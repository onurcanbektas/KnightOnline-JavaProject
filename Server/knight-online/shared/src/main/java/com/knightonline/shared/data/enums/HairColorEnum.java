package com.knightonline.shared.data.enums;

import java.io.Serializable;
import java.util.HashMap;
import java.util.Map;

/**
 * @author Mamaorha
 *
 */
public enum HairColorEnum implements Serializable
{
	HAIR0(0), HAIR1(1), HAIR2(2);
	
	private static Map<Integer, HairColorEnum> namesMap = new HashMap<>();

	private Integer value;
	
	HairColorEnum(Integer value)
	{
		this.value = value;
	}

	public Integer getValue()
	{
		return this.value;
	}
	
	public static HairColorEnum forValue(Integer value)
	{
		HairColorEnum hairColorEnum = namesMap.get(value);
		
		if(null == hairColorEnum)
		{
			hairColorEnum = HAIR0;
		}
		
		return hairColorEnum;
	}
	
	static
	{
		final HairColorEnum[] values = HairColorEnum.values();
		
		for (HairColorEnum temp : values)
		{
			namesMap.put(temp.getValue(), temp);
		}
	}
}
