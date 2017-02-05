package com.knightonline.shared.persistence.entities;

import java.io.Serializable;

import javax.persistence.Basic;
import javax.persistence.Column;
import javax.persistence.Convert;
import javax.persistence.Id;
import javax.persistence.Table;

import org.hibernate.annotations.Cache;
import org.hibernate.annotations.CacheConcurrencyStrategy;
import org.hibernate.annotations.NamedQueries;
import org.hibernate.annotations.NamedQuery;

import com.knightonline.shared.data.constants.NamedQueriesConstants;
import com.knightonline.shared.data.enums.InventorySlotEnum;
import com.knightonline.shared.persistence.converter.ConvertInventorySlotEnum;

/**
 * @author Mamaorha
 *
 */
@javax.persistence.Entity()
@Table(name = "CHARACTER_INVENTORY")
@Cache(usage = CacheConcurrencyStrategy.READ_WRITE)
@NamedQueries({
	@NamedQuery(name = NamedQueriesConstants.GET_CHARACTER_INVENTORY, query = "from CharacterInventory where characterName = :characterName")
	})
public class CharacterInventory implements Serializable
{
	private static final long serialVersionUID = -684977594436091793L;

	protected String characterName;
	protected InventorySlotEnum slot;
	protected int itemId;
	protected short itemCount;
	protected short itemDurability;
	protected long itemSerial;
	
	@Id()
	@Column(name = "CHARACTER_NAME", unique = false, nullable = false, length = 45)
	public String getCharacterName()
	{
		return characterName;
	}

	public void setCharacterName(String characterName)
	{
		this.characterName = characterName;
	}

	@Id()
	@Column(name = "SLOT", unique = false, nullable = false)
	@Convert(converter = ConvertInventorySlotEnum.class)
	public InventorySlotEnum getSlot()
	{
		return slot;
	}

	public void setSlot(InventorySlotEnum slot)
	{
		this.slot = slot;
	}

	@Basic()
	@Column(name = "ITEM_ID", unique = false, nullable = false)
	public int getItemId()
	{
		return itemId;
	}

	public void setItemId(int itemId)
	{
		this.itemId = itemId;
	}

	@Basic()
	@Column(name = "ITEM_COUNT", unique = false, nullable = false)
	public short getItemCount()
	{
		return itemCount;
	}

	public void setItemCount(short itemCount)
	{
		this.itemCount = itemCount;
	}

	@Basic()
	@Column(name = "ITEM_DURABILITY", unique = false, nullable = false)
	public short getItemDurability()
	{
		return itemDurability;
	}

	public void setItemDurability(short itemDurability)
	{
		this.itemDurability = itemDurability;
	}

	@Basic()
	@Column(name = "ITEM_SERIAL", unique = false, nullable = false)
	public long getItemSerial()
	{
		return itemSerial;
	}

	public void setItemSerial(long itemSerial)
	{
		this.itemSerial = itemSerial;
	}
	
	@Override
	public int hashCode()
	{
		final int prime = 31;
		int result = 1;
		result = prime * result + ((characterName == null) ? 0 : characterName.hashCode());
		result = prime * result + itemCount;
		result = prime * result + itemDurability;
		long temp;
		temp = Double.doubleToLongBits(itemId);
		result = prime * result + (int) (temp ^ (temp >>> 32));
		result = prime * result + (int) (itemSerial ^ (itemSerial >>> 32));
		result = prime * result + ((slot == null) ? 0 : slot.hashCode());
		return result;
	}

	@Override
	public boolean equals(Object obj)
	{
		if (this == obj)
			return true;
		if (obj == null)
			return false;
		if (getClass() != obj.getClass())
			return false;
		CharacterInventory other = (CharacterInventory) obj;
		if (characterName == null)
		{
			if (other.characterName != null)
				return false;
		}
		else if (!characterName.equals(other.characterName))
			return false;
		if (itemCount != other.itemCount)
			return false;
		if (itemDurability != other.itemDurability)
			return false;
		if (Double.doubleToLongBits(itemId) != Double.doubleToLongBits(other.itemId))
			return false;
		if (itemSerial != other.itemSerial)
			return false;
		if (slot != other.slot)
			return false;
		return true;
	}

	@Override
	public String toString()
	{
		return "CharacterInventory [characterName=" + characterName + ", slot=" + slot + ", itemId=" + itemId + ", itemCount=" + itemCount + ", itemDurability=" + itemDurability + ", itemSerial=" + itemSerial + "]";
	}
}
