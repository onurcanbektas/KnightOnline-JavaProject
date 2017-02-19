package com.knightonline.shared.data.common;

import com.knightonline.shared.persistence.entities.CharacterData;
import com.knightonline.shared.utils.CommonActions;
import com.knightonline.shared.utils.KOApplicationContext;

/**
 * @author Mamaorha
 *
 */
public class ConnectedCharacterInfo
{
	private CharacterData characterData;

	public CharacterData getCharacterData()
	{
		return characterData;
	}

	public void setCharacterData(CharacterData characterData)
	{
		this.characterData = characterData;
	}
	
	public void save()
	{
		CommonActions commonActions = (CommonActions) KOApplicationContext.getInstance().getApplicationContext().getBean("commonActions");
		commonActions.saveCharacterData(characterData);
	}
}
