package com.knightonline.shared.utils;

/**
 * @author Mamaorha
 *
 */
public class PrintUtils
{
	public static String printSection(String sectionName)
	{
		StringBuilder sb = new StringBuilder();
		sb.append("-[ " + sectionName + " ]");

		while (sb.length() < 79 * 2)
		{
			sb.insert(0, "=");
			sb.append("=");
		}

		return sb.toString();
	}
}
