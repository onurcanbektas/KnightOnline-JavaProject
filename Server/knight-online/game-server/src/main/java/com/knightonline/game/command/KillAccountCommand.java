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
public class KillAccountCommand extends BaseCommand
{
	@Autowired
	protected KOServer koServer;
	
	@Override
	public void handleCommand(String... args)
	{
		if(args.length < 1)
		{
			System.out.println("killAccount must have atleast 1 argument");
		}
		
		else
		{
			for (String username : args)
			{

				koServer.killAccount(username);
			}
		}
	}

	@Override
	protected String getCommandName()
	{
		return "killAccount";
	}

	@Override
	public String getInfo()
	{
		return "close the socket between the given account";
	}
}
