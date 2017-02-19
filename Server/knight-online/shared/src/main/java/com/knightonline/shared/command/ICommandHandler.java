package com.knightonline.shared.command;

/**
 * @author Mamaorha
 *
 */
public interface ICommandHandler
{
	public void handleCommand(String... args);
	public String getInfo();
}
