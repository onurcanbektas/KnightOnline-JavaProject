package com.knightonline.shared.network.common;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.charset.StandardCharsets;

/**
 * @author Mamaorha
 *
 */
public class Packet
{
	private short opcode;
	private byte[] data;
	private IResponseHandler server;
	private ByteBuffer byteBuffer;

	public Packet(short opcode, IResponseHandler server)
	{
		this.opcode = opcode;
		this.data = new byte[0];
		this.server = server;
	}

	public short getOpcode()
	{
		return opcode;
	}

	public void setOpcode(short opcode)
	{
		this.opcode = opcode;
	}

	public byte[] getData()
	{
		return data;
	}

	public void setData(byte[] data)
	{
		this.data = data;

		byteBuffer = ByteBuffer.allocate(data.length);
		byteBuffer.order(ByteOrder.LITTLE_ENDIAN);
		byteBuffer.put(data);
		byteBuffer.rewind();
	}

	public IResponseHandler getServer()
	{
		return server;
	}

	public void resetBuffer()
	{
		if (null != byteBuffer)
		{
			byteBuffer.rewind();
		}
	}

	public ByteBuffer getByteBuffer()
	{
		return byteBuffer;
	}

	public String readStringFromByteBuffer()
	{
		try
		{
			if (null != byteBuffer)
			{
				short length = byteBuffer.getShort();

				byte[] bytes = new byte[length];
				byteBuffer.get(bytes);

				return new String(bytes, StandardCharsets.UTF_8);
			}
		}
		catch (Exception e)
		{
		}
		
		return null;
	}
}
