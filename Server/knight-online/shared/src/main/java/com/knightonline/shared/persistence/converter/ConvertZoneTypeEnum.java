package com.knightonline.shared.persistence.converter;

import javax.persistence.AttributeConverter;

import com.knightonline.shared.data.enums.ZoneTypeEnum;

/**
 * @author Mamaorha
 *
 */
public class ConvertZoneTypeEnum implements AttributeConverter<ZoneTypeEnum, Integer>
{
	@Override
	public Integer convertToDatabaseColumn(ZoneTypeEnum zoneType)
	{
		return zoneType.getValue();
	}

	@Override
	public ZoneTypeEnum convertToEntityAttribute(Integer zoneType)
	{
		return ZoneTypeEnum.forValue(zoneType);
	}

}
