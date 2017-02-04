package com.knightonline.shared.data.enums;

import java.io.Serializable;
import java.util.HashMap;
import java.util.Map;

/**
 * @author Mamaorha
 *
 */
public enum InventorySlotEnum implements Serializable
{
	HEAD(1), CHEST(2), SHOULDER(3), LEG(4), FOOT(5), GLOVE(6), RIGHT_HAND(7), LEFT_HAND(8), RIGHT_EAR(9), LEFT_EAR(10), NECK(11), BELT(12), RIGHT_RING(13), LEFT_RING(14);
	private static Map<Integer, InventorySlotEnum> namesMap = new HashMap<>();

	private Integer value;
	
	InventorySlotEnum(Integer value)
	{
		this.value = value;
	}

	public Integer getValue()
	{
		return this.value;
	}
	
	public static InventorySlotEnum forValue(Integer value)
	{
		return namesMap.get(value);
	}
	
	static
	{
		final InventorySlotEnum[] values = InventorySlotEnum.values();
		
		for (InventorySlotEnum temp : values)
		{
			namesMap.put(temp.getValue(), temp);
		}
	}
}
