package com.knightonline.login.command;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

import com.knightonline.login.network.packet.handlers.ServerListHandler;
import com.knightonline.shared.command.handlers.BaseCommand;

/**
 * @author Mamaorha
 *
 */
@Component
public class RefreshServerListCommand extends BaseCommand
{
	@Autowired
	protected ServerListHandler serverListHandler;
	
	@Override
	public void handleCommand(String command, String... args)
	{
		serverListHandler.refreshServerList();
	}

	@Override
	protected String getCommandName()
	{
		return "refreshServerList";
	}

}
