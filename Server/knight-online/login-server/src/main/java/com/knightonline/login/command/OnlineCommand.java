package com.knightonline.login.command;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

import com.knightonline.login.server.LoginServer;
import com.knightonline.shared.command.handlers.BaseCommand;

/**
 * @author Mamaorha
 *
 */
@Component
public class OnlineCommand extends BaseCommand
{
	@Autowired
	protected LoginServer loginServer;
	
	@Override
	public void handleCommand(String command, String... args)
	{
		System.out.println(loginServer.getServer().getConnected());
	}

	@Override
	protected String getCommandName()
	{
		return "online?";
	}

}
