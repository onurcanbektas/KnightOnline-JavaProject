package com.knightonline.shared.persistence.dao.impl;

import java.util.List;

import javax.ejb.TransactionAttribute;
import javax.ejb.TransactionAttributeType;

import org.springframework.stereotype.Repository;

import com.knightonline.shared.data.common.DynamicAttribute;
import com.knightonline.shared.data.constants.NamedQueriesConstants;
import com.knightonline.shared.data.constants.StringConstants;
import com.knightonline.shared.persistence.dao.ICharacterDataDAO;
import com.knightonline.shared.persistence.entities.CharacterData;

/**
 * @author Mamaorha
 *
 */
@Repository
@TransactionAttribute(value = TransactionAttributeType.REQUIRED)
public class AccountCharacterHibernateDAO extends AbstractHibernateDAO<CharacterData, Long> implements ICharacterDataDAO
{
	@Override
	public List<CharacterData> getCharacterDataByUsername(String username)
	{
		return executeNamedQuery(NamedQueriesConstants.GET_CHARACTER_DATA_BY_USERNAME, true, new DynamicAttribute(StringConstants.USERNAME, username));
	}

	@Override
	public CharacterData getCharacterDataByCharacterName(String characterName)
	{
		return executeNamedQuerySingleResult(NamedQueriesConstants.GET_CHARACTER_DATA_BY_CHARACTER_NAME, true, new DynamicAttribute(StringConstants.CHARACTER_NAME, characterName));
	}
	
}
