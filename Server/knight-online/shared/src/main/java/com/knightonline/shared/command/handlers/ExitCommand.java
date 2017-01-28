package com.knightonline.shared.command.handlers;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

import com.knightonline.shared.command.CommandListener;

/**
 * @author Mamaorha
 *
 */
@Component
public class ExitCommand extends BaseCommand
{
	@Autowired
	protected CommandListener commandListener;
	
	@Override
	public void handleCommand(String command, String... args)
	{
		System.exit(0);
	}

	@Override
	protected String getCommandName()
	{
		return "exit";
	}
}
