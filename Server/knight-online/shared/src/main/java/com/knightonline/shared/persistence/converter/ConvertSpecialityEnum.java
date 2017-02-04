package com.knightonline.shared.persistence.converter;

import javax.persistence.AttributeConverter;

import com.knightonline.shared.data.enums.SpecialityEnum;

/**
 * @author Mamaorha
 *
 */
public class ConvertSpecialityEnum implements AttributeConverter<SpecialityEnum, Integer>
{
	@Override
	public Integer convertToDatabaseColumn(SpecialityEnum speciality)
	{
		return speciality.getValue();
	}

	@Override
	public SpecialityEnum convertToEntityAttribute(Integer speciality)
	{
		return SpecialityEnum.forValue(speciality);
	}

}
