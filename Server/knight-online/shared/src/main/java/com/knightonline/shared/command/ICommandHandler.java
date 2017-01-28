package com.knightonline.shared.command;

/**
 * @author Mamaorha
 *
 */
public interface ICommandHandler
{
	public void handleCommand(String command, String... args);
}
