package com.knightonline.shared.helper;

import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;

import org.springframework.stereotype.Component;

/**
 * @author Mamaorha
 *
 */
@Component
public class CommonHelper
{
	public String printSection(String sectionName)
	{
		StringBuilder sb = new StringBuilder();
		
		if(null != sectionName)
		{
			sb.append("[ " + sectionName + " ]");
		}
		
		while (sb.length() < 79 * 2)
		{
			sb.insert(0, "=");
			sb.append("=");
		}

		return sb.toString();
	}
	
	public String encryptMD5(String text)
	{
		try
		{
			MessageDigest md = MessageDigest.getInstance("MD5");
			md.update(text.getBytes());
		
			byte[] bytes = md.digest();
			
			StringBuilder sb = new StringBuilder();
			
			for (int i = 0; i < bytes.length; i++)
			{
				sb.append(Integer.toString((bytes[i] & 0xff) + 0x100, 16).substring(1));
			}
			
			return sb.toString();
		}
		
		catch (NoSuchAlgorithmException e)
		{
			e.printStackTrace();
		}
		
		return text;
	}
}
