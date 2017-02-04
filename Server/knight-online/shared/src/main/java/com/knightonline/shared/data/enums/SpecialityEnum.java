package com.knightonline.shared.data.enums;

import java.io.Serializable;
import java.util.HashMap;
import java.util.Map;

/**
 * @author Mamaorha
 *
 */
public enum SpecialityEnum implements Serializable
{
	KARUS_WARRIOR(101), KARUS_ROGUE(102), KARUS_WIZARD(103), KARUS_PRIEST(104),
	KARUS_BERSERKER(105), KARUS_GUARDIAN(106), KARUS_HUNTER(107), KARUS_PENETRATOR(108), KARUS_SORCERER(109), KARUS_NECROMANCER(110), KARUS_SHAMAN(111), KARUS_DARKPRIEST(112),
	
	ELMORAD_WARRIOR(201), ELMORAD_ROGUE(202), ELMORAD_WIZARD(203), ELMORAD_PRIEST(204),
	ELMORAD_BLADE(205), ELMORAD_PROTECTOR(206), ELMORAD_RANGER(207), ELMORAD_ASSASIN(208), ELMORAD_MAGE(209), ELMORAD_ENCHANTER(210), ELMORAD_CLERIC(211), ELMORAD_DRUID(212);
	
	private static Map<Integer, SpecialityEnum> namesMap = new HashMap<>();

	private Integer value;
	
	SpecialityEnum(Integer value)
	{
		this.value = value;
	}

	public Integer getValue()
	{
		return this.value;
	}
	
	public static SpecialityEnum forValue(Integer value)
	{
		return namesMap.get(value);
	}
	
	static
	{
		final SpecialityEnum[] values = SpecialityEnum.values();
		
		for (SpecialityEnum temp : values)
		{
			namesMap.put(temp.getValue(), temp);
		}
	}
}
