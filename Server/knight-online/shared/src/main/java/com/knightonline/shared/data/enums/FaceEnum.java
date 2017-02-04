package com.knightonline.shared.data.enums;

import java.io.Serializable;
import java.util.HashMap;
import java.util.Map;

/**
 * @author Mamaorha
 *
 */
public enum FaceEnum implements Serializable
{
	FACE0(0), FACE1(1), FACE2(2), FACE3(3);
	
	private static Map<Integer, FaceEnum> namesMap = new HashMap<>();

	private Integer value;
	
	FaceEnum(Integer value)
	{
		this.value = value;
	}

	public Integer getValue()
	{
		return this.value;
	}
	
	public static FaceEnum forValue(Integer value)
	{
		FaceEnum faceEnum = namesMap.get(value);
		
		if(null == faceEnum)
		{
			faceEnum = FACE0;
		}
		
		return faceEnum;
	}
	
	static
	{
		final FaceEnum[] values = FaceEnum.values();
		
		for (FaceEnum temp : values)
		{
			namesMap.put(temp.getValue(), temp);
		}
	}
}
