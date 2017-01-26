package com.knightonline.login.server;

import java.io.IOException;

import com.knightonline.shared.utils.KOApplicationContext;
import com.knightonline.shared.utils.PrintUtils;

public class Main
{
	public static void main(String[] args) throws IOException
	{
		final long start = System.currentTimeMillis();

		System.out.println(PrintUtils.printSection("Initializing Spring & Hibernate"));
		
		LoginServer loginServer = (LoginServer)KOApplicationContext.getInstance().getApplicationContext().getBean("loginServer");
		
		if(loginServer.startup())
		{
			System.out.println("Server started in " + ((System.currentTimeMillis() - start) / 1000) + " seconds successfully");
		}
		
		else
		{
			System.out.println("Press [enter] to close");
			System.in.read();
		}
	}
}