package com.knightonline.game.server;

import java.io.IOException;

import com.knightonline.shared.command.CommandListener;
import com.knightonline.shared.helper.CommonHelper;
import com.knightonline.shared.utils.KOApplicationContext;
/**
 * @author Mamaorha
 *
 */
public class Main
{
	public static void main(String[] args) throws IOException
	{
		final long start = System.currentTimeMillis();

		CommonHelper commonHelper = (CommonHelper)KOApplicationContext.getInstance().getApplicationContext().getBean("commonHelper");
		
		System.out.println(commonHelper.printSection("Initializing Spring & Hibernate"));
		
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
