package com.knightonline.shared.command.handlers;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

import com.knightonline.shared.command.CommandListener;

/**
 * @author Mamaorha
 *
 */
@Component
public class HelpCommand extends BaseCommand
{
	@Autowired
	protected CommandListener commandListener;
	
	@Override
	public void handleCommand(String command, String... args)
	{
		StringBuilder str = new StringBuilder();
		
		for (String curr : commandListener.getCommands())
		{
			str.append(curr).append("\n");
		}
		
		System.out.println(str.toString());
	}

	@Override
	protected String getCommandName()
	{
		return "help";
	}
}
