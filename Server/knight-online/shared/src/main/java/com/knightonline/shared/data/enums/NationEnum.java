package com.knightonline.shared.data.enums;

import java.io.Serializable;
import java.util.HashMap;
import java.util.Map;

/**
 * @author Mamaorha
 *
 */
public enum NationEnum implements Serializable
{
	NO_NATION(0), KARUS(1), ELMORAD(2);
	
	private Integer value;
	private static Map<Integer, NationEnum> namesMap = new HashMap<>();

	NationEnum(Integer value)
	{
		this.value = value;
	}

	public Integer getValue()
	{
		return this.value;
	}
	
	public static NationEnum forValue(Integer value)
	{
		return namesMap.get(value);
	}
	
	static
	{
		final NationEnum[] values = NationEnum.values();
		
		for (NationEnum temp : values)
		{
			namesMap.put(temp.getValue(), temp);
		}
	}
}
