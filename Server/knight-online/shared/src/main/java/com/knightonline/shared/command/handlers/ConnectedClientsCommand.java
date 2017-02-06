package com.knightonline.shared.command.handlers;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

import com.knightonline.shared.network.KOServer;

/**
 * @author Mamaorha
 *
 */
@Component
public class ConnectedClientsCommand extends BaseCommand
{
	@Autowired
	protected KOServer koServer;
	
	@Override
	public void handleCommand(String... args)
	{
		System.out.println(koServer.getConnectedClients());
	}

	@Override
	protected String getCommandName()
	{
		return "ConnectedClients?";
	}
}
