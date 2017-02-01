package com.knightonline.shared.data.enums;

import java.io.Serializable;
import java.util.HashMap;
import java.util.Map;

/**
 * @author Mamaorha
 *
 */
public enum AuthorityEnum implements Serializable
{
	USER(1), MUTE(2), BAN(3), GM(4), ADMIN(5);  
	
	private Integer value;
	private static Map<Integer, AuthorityEnum> namesMap = new HashMap<>();

	AuthorityEnum(Integer value)
	{
		this.value = value;
	}

	public Integer getValue()
	{
		return this.value;
	}
	
	public static AuthorityEnum forValue(Integer value)
	{
		return namesMap.get(value);
	}
	
	static
	{
		final AuthorityEnum[] values = AuthorityEnum.values();
		
		for (AuthorityEnum temp : values)
		{
			namesMap.put(temp.getValue(), temp);
		}
	}
}
