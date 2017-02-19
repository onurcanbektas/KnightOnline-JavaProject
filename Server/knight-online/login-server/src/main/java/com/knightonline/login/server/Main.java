package com.knightonline.login.server;

import java.io.IOException;

import com.knightonline.shared.command.CommandListener;
import com.knightonline.shared.helper.CommonHelper;
import com.knightonline.shared.utils.KOApplicationContext;

public class Main
{
	public static void main(String[] args) throws IOException
	{
		final long start = System.currentTimeMillis();

		CommonHelper helper = (CommonHelper)KOApplicationContext.getInstance().getApplicationContext().getBean("commonHelper");
		
		System.out.println(helper.printSection("Initializing Spring & Hibernate"));
		
		LoginServer loginServer = (LoginServer)KOApplicationContext.getInstance().getApplicationContext().getBean("loginServer");
		
		if(loginServer.startup())
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
