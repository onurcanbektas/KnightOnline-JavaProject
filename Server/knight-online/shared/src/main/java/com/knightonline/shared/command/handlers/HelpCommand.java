package com.knightonline.shared.command.handlers;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

import com.knightonline.shared.command.CommandListener;
import com.knightonline.shared.helper.CommonHelper;

/**
 * @author Mamaorha
 *
 */
@Component
public class HelpCommand extends BaseCommand
{
	@Autowired
	protected CommandListener commandListener;

	@Autowired
	protected CommonHelper commonHelper;
	
	@Override
	public void handleCommand(String... args)
	{
		if (null == args || args.length == 0)
		{
			StringBuilder str = new StringBuilder();

			for (String curr : commandListener.getCommands())
			{
				str.append(curr).append("\n");
			}

			System.out.println(str.toString());
		}
		
		else
		{
			String command = args[0];
			
			System.out.println(commonHelper.printSection(command));
			System.out.println(commandListener.getCommand(command).getInfo());
			System.out.println(commonHelper.printSection(null));
		}
	}

	@Override
	protected String getCommandName()
	{
		return "help";
	}
	
	@Override
	public String getInfo()
	{
		return "use this command to state all possible commands.\nthis command can be used to get information about exist command - for example: help online?";
	}
}
