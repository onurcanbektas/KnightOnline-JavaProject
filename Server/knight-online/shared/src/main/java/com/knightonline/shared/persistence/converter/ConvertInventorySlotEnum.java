package com.knightonline.shared.persistence.converter;

import javax.persistence.AttributeConverter;

import com.knightonline.shared.data.enums.InventorySlotEnum;

/**
 * @author Mamaorha
 *
 */
public class ConvertInventorySlotEnum implements AttributeConverter<InventorySlotEnum, Integer>
{
	@Override
	public Integer convertToDatabaseColumn(InventorySlotEnum inventorySlot)
	{
		return inventorySlot.getValue();
	}

	@Override
	public InventorySlotEnum convertToEntityAttribute(Integer inventorySlot)
	{
		return InventorySlotEnum.forValue(inventorySlot);
	}

}
