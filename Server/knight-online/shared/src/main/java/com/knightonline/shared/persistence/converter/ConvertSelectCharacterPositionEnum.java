package com.knightonline.shared.persistence.converter;

import javax.persistence.AttributeConverter;

import com.knightonline.shared.data.enums.SelectCharacterPositionEnum;

/**
 * @author Mamaorha
 *
 */
public class ConvertSelectCharacterPositionEnum implements AttributeConverter<SelectCharacterPositionEnum, Integer>
{
	@Override
	public Integer convertToDatabaseColumn(SelectCharacterPositionEnum selectCharacterPositionEnum)
	{
		return selectCharacterPositionEnum.getValue();
	}

	@Override
	public SelectCharacterPositionEnum convertToEntityAttribute(Integer selectCharacterPositionEnum)
	{
		return SelectCharacterPositionEnum.forValue(selectCharacterPositionEnum);
	}
}
