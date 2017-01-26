package com.knightonline.shared.data.enums;

import java.io.Serializable;
import java.util.HashMap;
import java.util.Map;
import java.util.Map.Entry;

/**
 * @author Mamaorha
 *
 */
public enum NationEnum implements Serializable
{
	NO_NATION(0), KARUS(1), ELMORAD(2);
	
	private int value;
	private static Map<Integer, NationEnum> namesMap = new HashMap<Integer, NationEnum>();

	NationEnum(int value)
	{
		this.value = value;
	}

	public int getValue()
	{
		return this.value;
	}

	static
	{
		final NationEnum[] values = NationEnum.values();
		
		for (NationEnum temp : values)
		{
			namesMap.put(temp.getValue(), temp);
		}
	}

	public static NationEnum forValue(int value)
	{
		return namesMap.get(value);
	}
	
	public int toValue()
	{
		for (Entry<Integer, NationEnum> entry : namesMap.entrySet())
		{
			if (entry.getValue() == this)
				return entry.getKey();
		}
		
		return NO_NATION.value; 
	}
}
