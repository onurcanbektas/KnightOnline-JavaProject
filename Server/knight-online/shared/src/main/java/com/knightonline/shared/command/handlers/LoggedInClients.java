package com.knightonline.shared.command.handlers;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

import com.knightonline.shared.network.KOServer;

/**
 * @author Mamaorha
 *
 */
@Component
public class LoggedInClients extends BaseCommand
{
	@Autowired
	protected KOServer koServer;
	
	@Override
	public void handleCommand(String... args)
	{
		System.out.println(koServer.getLoggedInClients());
	}

	@Override
	protected String getCommandName()
	{
		return "LoggedInClients?";
	}
	
	@Override
	public String getInfo()
	{
		return "return the number of connected clients that passed login";
	}
}
