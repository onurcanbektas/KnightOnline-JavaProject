package com.knightonline.game.command;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

import com.knightonline.game.server.GameServer;
import com.knightonline.shared.command.handlers.BaseCommand;

/**
 * @author Mamaorha
 *
 */
@Component
public class KillAccountCommand extends BaseCommand
{
	@Autowired
	protected GameServer gameServer;
	
	@Override
	public void handleCommand(String command, String... args)
	{
		if(args.length < 1)
		{
			System.out.println("killAccount must have atleast 1 argument");
		}
		
		else
		{
			for (String username : args)
			{

				gameServer.getServer().killAccount(username);
			}
		}
	}

	@Override
	protected String getCommandName()
	{
		return "killAccount";
	}

}
