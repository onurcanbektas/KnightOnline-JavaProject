package com.knightonline.shared.data.enums;

import java.io.Serializable;
import java.util.HashMap;
import java.util.Map;

/**
 * @author Mamaorha
 *
 */
public enum RaceEnum implements Serializable
{
	NONE(0),
	KARUS_ARK_TUAREK(1), KARUS_TUAREK(2), KARUS_WRINKLE_TUAREK(3), KARUS_PURI_TUAREK(4),
	ELMORAD_BABARIAN(11), ELMORAD_MAN(12), ELMORAD_WOMEN(13),
	NPC(100);
	
	private static Map<Integer, RaceEnum> namesMap = new HashMap<>();

	private Integer value;
	
	RaceEnum(Integer value)
	{
		this.value = value;
	}

	public Integer getValue()
	{
		return this.value;
	}
	
	public static RaceEnum forValue(Integer value)
	{
		return namesMap.get(value);
	}
	
	static
	{
		final RaceEnum[] values = RaceEnum.values();
		
		for (RaceEnum temp : values)
		{
			namesMap.put(temp.getValue(), temp);
		}
	}
}
