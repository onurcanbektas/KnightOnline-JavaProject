package com.knightonline.shared.persistence.converter;

import javax.persistence.AttributeConverter;

import com.knightonline.shared.data.enums.ZoneEnum;

/**
 * @author Mamaorha
 *
 */
public class ConvertZoneEnum implements AttributeConverter<ZoneEnum, Integer>
{
	@Override
	public Integer convertToDatabaseColumn(ZoneEnum zone)
	{
		return zone.getValue();
	}

	@Override
	public ZoneEnum convertToEntityAttribute(Integer zone)
	{
		return ZoneEnum.forValue(zone);
	}

}
