package com.knightonline.shared.utils;

import java.net.URL;

import org.springframework.context.ApplicationContext;
import org.springframework.context.support.FileSystemXmlApplicationContext;

/**
 * @author Mamaorha
 *
 */
public class KOApplicationContext
{
	private static KOApplicationContext executionApplicationContext = new KOApplicationContext();
	private ApplicationContext context = null;

	private KOApplicationContext()
	{
		init();
	}

	public void init()
	{
		if (null == context)
		{
			URL resource = getClass().getClassLoader().getResource("spring-config.xml");
			ApplicationContext context = new FileSystemXmlApplicationContext(resource.getPath());

			this.context = context;
		}
	}

	public ApplicationContext getApplicationContext()
	{
		return context;
	}

	public static KOApplicationContext getInstance()
	{
		return executionApplicationContext;
	}
}
