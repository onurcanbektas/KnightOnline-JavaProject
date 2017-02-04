package com.knightonline.shared.persistence.converter;

import javax.persistence.AttributeConverter;

import com.knightonline.shared.data.enums.RaceEnum;

/**
 * @author Mamaorha
 *
 */
public class ConvertRaceEnum implements AttributeConverter<RaceEnum, Integer>
{
	@Override
	public Integer convertToDatabaseColumn(RaceEnum race)
	{
		return race.getValue();
	}

	@Override
	public RaceEnum convertToEntityAttribute(Integer race)
	{
		return RaceEnum.forValue(race);
	}

}
