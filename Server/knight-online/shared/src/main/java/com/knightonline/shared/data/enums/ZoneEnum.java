package com.knightonline.shared.data.enums;

import java.io.Serializable;
import java.util.HashMap;
import java.util.Map;

/**
 * @author Mamaorha
 *
 */
public enum ZoneEnum implements Serializable
{
	Moradon(1);
	
	private static Map<Integer, ZoneEnum> namesMap = new HashMap<>();

	private Integer value;
	
	ZoneEnum(Integer value)
	{
		this.value = value;
	}

	public Integer getValue()
	{
		return this.value;
	}
	
	public static ZoneEnum forValue(Integer value)
	{
		return namesMap.get(value);
	}
	
	static
	{
		final ZoneEnum[] values = ZoneEnum.values();
		
		for (ZoneEnum temp : values)
		{
			namesMap.put(temp.getValue(), temp);
		}
	}
}
