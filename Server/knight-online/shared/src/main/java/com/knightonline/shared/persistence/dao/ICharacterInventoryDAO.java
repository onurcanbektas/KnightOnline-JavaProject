package com.knightonline.shared.persistence.dao;

import java.util.List;

import com.knightonline.shared.persistence.entities.CharacterInventory;

/**
 * @author Mamaorha
 *
 */
public interface ICharacterInventoryDAO extends IGenericDAO<CharacterInventory, Long>
{
	public List<CharacterInventory> getCharacterInventory(String characterName);
}