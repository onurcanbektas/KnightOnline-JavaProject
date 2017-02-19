package com.knightonline.shared.utils;

import java.nio.ByteBuffer;

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

	public static byte[] intToByteArray(int value)
	{
		byte[] ret = new byte[4];
		ret[0] = (byte) (value & 0xff);
		ret[1] = (byte) ((value >> 8) & 0xff);
		ret[2] = (byte) ((value >> 16) & 0xff);
		ret[3] = (byte) ((value >> 24) & 0xff);
		return ret;
	}

	public static byte[] longToByteArray(long value)
	{
		ByteBuffer buffer = ByteBuffer.allocate(8);
		buffer.putLong(value);

		return buffer.array();
	}

	public static byte[] doubleToByteArray(double value)
	{
		ByteBuffer buffer = ByteBuffer.allocate(4);
		buffer.putDouble(value);

		return buffer.array();
	}
}
