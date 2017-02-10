package com.knightonline.game.command;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

import com.knightonline.shared.command.handlers.BaseCommand;
import com.knightonline.shared.network.KOServer;

/**
 * @author Mamaorha
 *
 */
@Component
public class KillCommand extends BaseCommand
{
	@Autowired
	protected KOServer koServer;
	
	@Override
	public void handleCommand(String... args)
	{
		if(args.length < 1)
		{
			System.out.println("kill must have atleast 1 argument");
		}
		
		else
		{
			for (String characterName : args)
			{
				koServer.killCharacter(characterName);
			}
		}
	}

	@Override
	protected String getCommandName()
	{
		return "kill";
	}

	@Override
	public String getInfo()
	{
		return "close the socket between the given character";
	}
}
