package com.knightonline.shared.persistence.converter;

import javax.persistence.AttributeConverter;

import com.knightonline.shared.data.enums.NationEnum;

/**
 * @author Mamaorha
 *
 */
public class ConvertNationEnum implements AttributeConverter<NationEnum, Integer>
{
	@Override
	public Integer convertToDatabaseColumn(NationEnum nation)
	{
		return nation.getValue();
	}

	@Override
	public NationEnum convertToEntityAttribute(Integer nation)
	{
		return NationEnum.forValue(nation);
	}

}
