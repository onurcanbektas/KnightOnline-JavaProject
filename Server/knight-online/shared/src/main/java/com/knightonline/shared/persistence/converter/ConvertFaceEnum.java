package com.knightonline.shared.persistence.converter;

import javax.persistence.AttributeConverter;

import com.knightonline.shared.data.enums.FaceEnum;

/**
 * @author Mamaorha
 *
 */
public class ConvertFaceEnum implements AttributeConverter<FaceEnum, Integer>
{
	@Override
	public Integer convertToDatabaseColumn(FaceEnum face)
	{
		return face.getValue();
	}

	@Override
	public FaceEnum convertToEntityAttribute(Integer face)
	{
		return FaceEnum.forValue(face);
	}

}
