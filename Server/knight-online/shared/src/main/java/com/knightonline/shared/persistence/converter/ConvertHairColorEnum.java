package com.knightonline.shared.persistence.converter;

import javax.persistence.AttributeConverter;

import com.knightonline.shared.data.enums.HairColorEnum;

/**
 * @author Mamaorha
 *
 */
public class ConvertHairColorEnum implements AttributeConverter<HairColorEnum, Integer>
{
	@Override
	public Integer convertToDatabaseColumn(HairColorEnum hairColor)
	{
		return hairColor.getValue();
	}

	@Override
	public HairColorEnum convertToEntityAttribute(Integer hairColor)
	{
		return HairColorEnum.forValue(hairColor);
	}

}
