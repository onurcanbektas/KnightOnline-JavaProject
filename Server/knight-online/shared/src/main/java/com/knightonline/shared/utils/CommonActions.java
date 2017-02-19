package com.knightonline.shared.utils;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

import com.knightonline.shared.exception.DAOException;
import com.knightonline.shared.persistence.dao.ICharacterDataDAO;
import com.knightonline.shared.persistence.entities.CharacterData;

/**
 * @author Mamaorha
 *
 */
@Component
public class CommonActions
{
	@Autowired
	protected ICharacterDataDAO characterDataDAO;
	
	public void saveCharacterData(CharacterData characterData)
	{
		try
		{
			characterDataDAO.persist(characterData);
		}
		catch (DAOException e)
		{
			e.printStackTrace();
		}
	}
}
