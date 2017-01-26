package com.knightonline.shared.utils;

/**
 * @author Mamaorha
 *
 */
public class PacketUtils
{
	public static byte[] shortToByteArray(short value)
	{
		byte[] ret = new byte[2];
		ret[0] = (byte) (value & 0xff);
		ret[1] = (byte) ((value >> 8) & 0xff);
		return ret;
	}
	
	public static short byteArrayToShort(byte[] data, int startPosition) 
	{
		short value = 0;
		
		for (int i = 0; i < data.length - startPosition; i++)
		{
			value += (data[i + startPosition] & 0xffL) << (8 * i);
		}
		
		return value;
	}
}
