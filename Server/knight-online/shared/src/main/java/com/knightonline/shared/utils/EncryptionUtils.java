package com.knightonline.shared.utils;

import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;

/**
 * @author Mamaorha
 *
 */
public class EncryptionUtils
{
	public static String encryptMD5(String text)
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
