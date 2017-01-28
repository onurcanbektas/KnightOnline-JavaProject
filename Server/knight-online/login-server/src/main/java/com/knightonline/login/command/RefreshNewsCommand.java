package com.knightonline.login.command;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

import com.knightonline.login.network.packet.handlers.NewsHandler;
import com.knightonline.shared.command.handlers.BaseCommand;

/**
 * @author Mamaorha
 *
 */
@Component
public class RefreshNewsCommand extends BaseCommand
{
	@Autowired
	protected NewsHandler newsHandler;
	
	@Override
	public void handleCommand(String command, String... args)
	{
		newsHandler.refreshNews();
	}

	@Override
	protected String getCommandName()
	{
		return "refreshNews";
	}

}
