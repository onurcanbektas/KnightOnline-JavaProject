package com.knightonline.shared.command;

import java.util.HashSet;
import java.util.Map;
import java.util.Scanner;
import java.util.Set;
import java.util.concurrent.ConcurrentHashMap;

import javax.annotation.PostConstruct;

import org.springframework.stereotype.Component;

/**
 * @author Mamaorha
 *
 */
@Component
public class CommandListener extends Thread
{
	private static final String COMMAND_HANDLER_THREAD = "command handler thread";
	private static final String UNHANDLED_COMMAND = "unhandled command - [%s]";
	private static final String COMMAND_IS_ALREADY_HANDLED = "command [%s] is already handled";
	
	private static Scanner scanner = new Scanner(System.in);
	private Map<String, ICommandHandler> commandHandlers;
	private Set<String> commands;
	
	@PostConstruct
	public void init()
	{
		this.commandHandlers = new ConcurrentHashMap<>();
		this.commands = new HashSet<>();
	}
	
	public void registerHandler(String command, ICommandHandler handler)
	{
		if (null != commandHandlers.get(command.toLowerCase()))
		{
			throw new InternalError(String.format(COMMAND_IS_ALREADY_HANDLED, command));
		}

		commandHandlers.put(command.toLowerCase(), handler);
		commands.add(command);
	}
	
	@Override
	public void run()
	{
		Thread.currentThread().setName(COMMAND_HANDLER_THREAD);

		try
		{
			while (!this.isInterrupted())
			{
				try
				{
					System.out.print("enter command(type help for the command list):");
					String commmandLine = scanner.nextLine();
					
					String command;
					String[] args = null;
					
					String[] data = commmandLine.split(" ");
					
					command = data[0].toLowerCase();
					
					if(command.isEmpty())
					{
						continue;
					}
					
					if(data.length > 1)
					{
						args = new String[data.length - 1];
						
						for (int i = 1; i < data.length; i++)
						{
							args[i - 1] = data[i];
						}
					}
					
					ICommandHandler commandHandler = commandHandlers.get(command);
					
					if(null == commandHandler)
					{
						System.out.println(String.format(UNHANDLED_COMMAND, data[0]));
					}
					
					else
					{
						commandHandler.handleCommand(args);
					}
				}
				
				catch (Throwable e)
				{
					e.printStackTrace();
				}
			}
		}
		
		catch (Exception e)
		{
			if (!this.isInterrupted())
			{
				e.printStackTrace();
			}
		}
	}
	
	public Set<String> getCommands()
	{
		return commands;
	}
	
	public ICommandHandler getCommand(String command)
	{
		return commandHandlers.get(command);
	}
}
