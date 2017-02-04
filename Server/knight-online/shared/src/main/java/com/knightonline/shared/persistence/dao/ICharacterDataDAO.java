package com.knightonline.shared.persistence.dao;

import java.util.List;

import com.knightonline.shared.persistence.entities.CharacterData;

/**
 * @author Mamaorha
 *
 */
public interface ICharacterDataDAO extends IGenericDAO<CharacterData, Long>
{
	public List<CharacterData> getCharacterDataByUsername(String username);
	public CharacterData  getCharacterDataByCharacterName(String characterName);
}
