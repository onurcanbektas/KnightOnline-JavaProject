package com.knightonline.game.network.packet.handlers;

import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

import com.knightonline.game.server.GameServer;
import com.knightonline.shared.data.enums.InventorySlotEnum;
import com.knightonline.shared.network.KOServer;
import com.knightonline.shared.network.packet.Packet;
import com.knightonline.shared.network.packet.handlers.LoggedInHandler;
import com.knightonline.shared.persistence.dao.ICharacterDataDAO;
import com.knightonline.shared.persistence.dao.ICharacterInventoryDAO;
import com.knightonline.shared.persistence.entities.CharacterData;
import com.knightonline.shared.persistence.entities.CharacterInventory;

/**
 * @author Mamaorha
 *
 */
@Component
public class CharacterInfoHandler extends LoggedInHandler
{
	@Autowired
	protected GameServer gameServer;

	@Autowired
	protected ICharacterDataDAO accountCharacterDAO;
	
	@Autowired
	protected ICharacterInventoryDAO characterInventoryDAO;
	
	@Override
	protected KOServer getKOServer()
	{
		return gameServer.getServer();
	}

	@Override
	protected void handlePacketImpl(Packet requestPacket)
	{
		List<CharacterData> characters = accountCharacterDAO.getCharacterDataByUsername(username);
		
		Packet result = new Packet(requestPacket.getOpcode(), requestPacket.getMessageInfo());
		result.appendInt8(characters.size());	
		
		for (CharacterData accountCharacter : characters)
		{
			result.appendInt8(accountCharacter.getSelectCharacterPosition().getValue());
			result.appendString(accountCharacter.getCharacterName());
			result.appendInt8(accountCharacter.getRace().getValue());
			result.appendShort(Short.valueOf(String.valueOf(accountCharacter.getSpeciality().getValue())));
			result.appendInt8(accountCharacter.getLevel());
			result.appendInt8(accountCharacter.getFace().getValue());
			result.appendInt8(accountCharacter.getHairColor().getValue());
			result.appendInt8(accountCharacter.getZone().getValue());
			
			Map<InventorySlotEnum, CharacterInventory> filterInventory = filterInventory(characterInventoryDAO.getCharacterInventory(accountCharacter.getCharacterName()));
			
			appendItem(result, filterInventory.get(InventorySlotEnum.HEAD));
			appendItem(result, filterInventory.get(InventorySlotEnum.CHEST));
			appendItem(result, filterInventory.get(InventorySlotEnum.SHOULDER));
			appendItem(result, filterInventory.get(InventorySlotEnum.LEG));
			appendItem(result, filterInventory.get(InventorySlotEnum.FOOT));
			appendItem(result, filterInventory.get(InventorySlotEnum.GLOVE));
			appendItem(result, filterInventory.get(InventorySlotEnum.RIGHT_HAND));
			appendItem(result, filterInventory.get(InventorySlotEnum.LEFT_HAND));
		}
		
		packetWriter.sendPacket(result);
	}
	
	private Map<InventorySlotEnum, CharacterInventory> filterInventory(List<CharacterInventory> totalInventory)
	{
		CharacterInventory empty = new CharacterInventory();
		empty.setItemId(0);
		empty.setItemDurability((short)0);
		
		Map<InventorySlotEnum, CharacterInventory> filtered = new HashMap<>();
		
		filtered.put(InventorySlotEnum.HEAD, empty);
		filtered.put(InventorySlotEnum.CHEST, empty);
		filtered.put(InventorySlotEnum.SHOULDER, empty);
		filtered.put(InventorySlotEnum.LEG, empty);
		filtered.put(InventorySlotEnum.FOOT, empty);
		filtered.put(InventorySlotEnum.GLOVE, empty);
		filtered.put(InventorySlotEnum.RIGHT_HAND, empty);
		filtered.put(InventorySlotEnum.LEFT_HAND, empty);
		
		for (CharacterInventory characterInventory : totalInventory)
		{
			if(filtered.containsKey(characterInventory.getSlot()))
			{
				filtered.put(characterInventory.getSlot(), characterInventory);
			}
		}
		
		return filtered;
	}

	private void appendItem(Packet result, CharacterInventory characterInventory)
	{
		result.appendLong(characterInventory.getItemId());
		result.appendShort(characterInventory.getItemDurability());
	}
}