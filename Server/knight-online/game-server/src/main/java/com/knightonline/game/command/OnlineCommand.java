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
public class OnlineCommand extends BaseCommand
{
	@Autowired
	protected GameServer gameServer;
	
	@Override
	public void handleCommand(String command, String... args)
	{
		System.out.println(gameServer.getServer().getConnected());
	}

	@Override
	protected String getCommandName()
	{
		return "online?";
	}

}
