package com.knightonline.shared.persistence.dao.impl;

import java.util.List;

import javax.ejb.TransactionAttribute;
import javax.ejb.TransactionAttributeType;

import org.springframework.stereotype.Repository;

import com.knightonline.shared.data.common.DynamicAttribute;
import com.knightonline.shared.data.constants.NamedQueriesConstants;
import com.knightonline.shared.data.constants.StringConstants;
import com.knightonline.shared.persistence.dao.ICharacterInventoryDAO;
import com.knightonline.shared.persistence.entities.CharacterInventory;

/**
 * @author Mamaorha
 *
 */
@Repository
@TransactionAttribute(value = TransactionAttributeType.REQUIRED)
public class CharacterInventoryHibernateDAO extends AbstractHibernateDAO<CharacterInventory, Long> implements ICharacterInventoryDAO
{
	@Override
	public List<CharacterInventory> getCharacterInventory(String characterName)
	{
		return executeNamedQuery(NamedQueriesConstants.GET_CHARACTER_INVENTORY, true, new DynamicAttribute(StringConstants.CHARACTER_NAME, characterName));
	}
	
}
