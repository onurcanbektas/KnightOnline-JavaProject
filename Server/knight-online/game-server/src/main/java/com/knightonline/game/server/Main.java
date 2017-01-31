package com.knightonline.game.server;

import java.io.IOException;

import com.knightonline.shared.command.CommandListener;
import com.knightonline.shared.utils.KOApplicationContext;
import com.knightonline.shared.utils.PrintUtils;

public class Main
{
	public static void main(String[] args) throws IOException
	{
		final long start = System.currentTimeMillis();

		System.out.println(PrintUtils.printSection("Initializing Spring & Hibernate"));
		
		GameServer gameServer = (GameServer)KOApplicationContext.getInstance().getApplicationContext().getBean("gameServer");
		
		if(gameServer.startup())
		{
			System.out.println("Server started in " + ((System.currentTimeMillis() - start) / 1000) + " seconds successfully");
			
			CommandListener commandListener = (CommandListener)KOApplicationContext.getInstance().getApplicationContext().getBean("commandListener");
			commandListener.start();
		}
		
		else
		{	
			System.out.println("Press [enter] to close");
			System.in.read();
		}
	}
}
