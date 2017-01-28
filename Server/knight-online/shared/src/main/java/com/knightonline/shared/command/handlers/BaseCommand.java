package com.knightonline.shared.command.handlers;

import javax.annotation.PostConstruct;

import org.springframework.beans.factory.annotation.Autowired;

import com.knightonline.shared.command.CommandListener;
import com.knightonline.shared.command.ICommandHandler;

/**
 * @author Mamaorha
 *
 */
public abstract class BaseCommand implements ICommandHandler
{
	@Autowired
	protected CommandListener commandListener;
	
	@PostConstruct
	protected void registerCommand()
	{
		commandListener.registerHandler(getCommandName(), this);
	}
	
	protected abstract String getCommandName();
}
