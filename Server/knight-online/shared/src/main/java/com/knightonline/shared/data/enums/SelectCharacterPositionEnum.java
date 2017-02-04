package com.knightonline.shared.data.enums;

import java.io.Serializable;
import java.util.HashMap;
import java.util.Map;

/**
 * @author Mamaorha
 *
 */
public enum SelectCharacterPositionEnum implements Serializable
{
	CENTER(0), LEFT(1), RIGHT(2);
	
	private static Map<Integer, SelectCharacterPositionEnum> namesMap = new HashMap<>();

	private Integer value;
	
	SelectCharacterPositionEnum(Integer value)
	{
		this.value = value;
	}

	public Integer getValue()
	{
		return this.value;
	}
	
	public static SelectCharacterPositionEnum forValue(Integer value)
	{
		return namesMap.get(value);
	}
	
	static
	{
		final SelectCharacterPositionEnum[] values = SelectCharacterPositionEnum.values();
		
		for (SelectCharacterPositionEnum temp : values)
		{
			namesMap.put(temp.getValue(), temp);
		}
	}
}
