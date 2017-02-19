package com.knightonline.shared.command.handlers;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

import com.knightonline.shared.command.CommandListener;
import com.knightonline.shared.network.KOServer;

/**
 * @author Mamaorha
 *
 */
@Component
public class ExitCommand extends BaseCommand
{
	@Autowired
	protected CommandListener commandListener;
	
	@Autowired
	protected KOServer koServer;
	
	@Override
	public void handleCommand(String... args)
	{
		koServer.shutdown();
		System.exit(0);
	}

	@Override
	protected String getCommandName()
	{
		return "exit";
	}
	
	@Override
	public String getInfo()
	{
		return "use this command to shut down the client properly";
	}
}
