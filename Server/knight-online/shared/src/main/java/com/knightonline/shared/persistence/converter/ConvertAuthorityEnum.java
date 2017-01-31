package com.knightonline.shared.persistence.converter;

import javax.persistence.AttributeConverter;

import com.knightonline.shared.data.enums.AuthorityEnum;

/**
 * @author Mamaorha
 *
 */
public class ConvertAuthorityEnum implements AttributeConverter<AuthorityEnum, Integer>
{
	@Override
	public Integer convertToDatabaseColumn(AuthorityEnum nation)
	{
		return nation.getValue();
	}

	@Override
	public AuthorityEnum convertToEntityAttribute(Integer nation)
	{
		return AuthorityEnum.forValue(nation);
	}
}
