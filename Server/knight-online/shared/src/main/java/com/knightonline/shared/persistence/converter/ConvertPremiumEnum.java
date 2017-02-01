package com.knightonline.shared.persistence.converter;

import javax.persistence.AttributeConverter;

import com.knightonline.shared.data.enums.PremiumEnum;

/**
 * @author Mamaorha
 *
 */
public class ConvertPremiumEnum implements AttributeConverter<PremiumEnum, Integer>
{
	@Override
	public Integer convertToDatabaseColumn(PremiumEnum nation)
	{
		return nation.getValue();
	}

	@Override
	public PremiumEnum convertToEntityAttribute(Integer nation)
	{
		return PremiumEnum.forValue(nation);
	}

}
