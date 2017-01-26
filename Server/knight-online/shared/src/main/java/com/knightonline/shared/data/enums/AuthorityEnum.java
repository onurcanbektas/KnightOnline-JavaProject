package com.knightonline.shared.data.enums;

import java.io.Serializable;
import java.util.HashMap;
import java.util.Map;
import java.util.Map.Entry;

/**
 * @author Mamaorha
 *
 */
public enum AuthorityEnum implements Serializable
{
	ADMIN("ADMIN"), GM("GM"), USER("USER"), BANNED("BANNED"), MUTED("MUTED");
	
	private String value;
	private static Map<String, AuthorityEnum> namesMap = new HashMap<String, AuthorityEnum>();

	AuthorityEnum(String value)
	{
		this.value = value;
	}

	public String getValue()
	{
		return this.value;
	}

	static
	{
		final AuthorityEnum[] values = AuthorityEnum.values();
		
		for (AuthorityEnum temp : values)
		{
			namesMap.put(temp.getValue().toLowerCase(), temp);
		}
	}

	public static AuthorityEnum forValue(String value)
	{
		if (null == value)
		{
			return null;
		}

		return namesMap.get(value.toLowerCase());
	}
	
	public String toValue()
	{
		for (Entry<String, AuthorityEnum> entry : namesMap.entrySet())
		{
			if (entry.getValue() == this)
				return entry.getKey();
		}
		
		return null; 
	}
}