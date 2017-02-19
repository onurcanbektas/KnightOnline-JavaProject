package com.knightonline.shared.data.enums;

import java.io.Serializable;
import java.util.HashMap;
import java.util.Map;

/**
 * @author Mamaorha
 *
 */
public enum ZoneTypeEnum implements Serializable
{
	NEUTRAL(0), ELMORAD(1), KARUS(2), FIGHT(3), WAR(4);

	private static Map<Integer, ZoneTypeEnum> namesMap = new HashMap<>();

	private Integer value;

	ZoneTypeEnum(Integer value)
	{
		this.value = value;
	}

	public Integer getValue()
	{
		return this.value;
	}

	public static ZoneTypeEnum forValue(Integer value)
	{
		return namesMap.get(value);
	}

	static
	{
		final ZoneTypeEnum[] values = ZoneTypeEnum.values();

		for (ZoneTypeEnum temp : values)
		{
			namesMap.put(temp.getValue(), temp);
		}
	}
}
