package com.knightonline.game.command;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

import com.knightonline.game.server.GameServer;
import com.knightonline.shared.command.handlers.BaseCommand;
import com.knightonline.shared.persistence.dao.ICharacterDataDAO;
import com.knightonline.shared.persistence.entities.CharacterData;

/**
 * @author Mamaorha
 *
 */
@Component
public class KillCommand extends BaseCommand
{
	@Autowired
	protected GameServer gameServer;
	
	@Autowired
	protected  ICharacterDataDAO accountCharacterDAO;
	
	@Override
	public void handleCommand(String command, String... args)
	{
		if(args.length < 1)
		{
			System.out.println("kill must have atleast 1 argument");
		}
		
		else
		{
			for (String characterName : args)
			{
				CharacterData characterData = accountCharacterDAO.getCharacterDataByCharacterName(characterName);
				
				if(null != characterData)
				{
					gameServer.getServer().killAccount(characterData.getUsername());
				}
			}
		}
	}

	@Override
	protected String getCommandName()
	{
		return "kill";
	}

}
