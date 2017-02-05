package com.knightonline.shared.network.packet;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.charset.StandardCharsets;

import com.knightonline.shared.network.common.MessageInfo;
import com.knightonline.shared.utils.PacketUtils;

/**
 * @author Mamaorha
 *
 */
public class Packet
{
	private short opcode;
	private byte[] data;
	private MessageInfo messageInfo;
	private ByteBuffer byteBuffer;

	public Packet(short opcode, MessageInfo messageInfo)
	{
		this.opcode = opcode;
		this.data = null;
		this.messageInfo = messageInfo;
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

	public MessageInfo getMessageInfo()
	{
		return messageInfo;
	}

	public String getString()
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

	public short getShort()
	{
		if (null != byteBuffer)
		{
			return byteBuffer.getShort();
		}
		
		return 0;
	}
	
	public byte getByte()
	{
		if (null != byteBuffer)
		{
			return byteBuffer.get();
		}
		
		return 0;
	}
	
	public void appendString(String data)
	{
		//add length
		appendShort((short) (data.length()));
		
		//add the bytes
		appendData(data.getBytes(StandardCharsets.UTF_8));
	}

	//server will read this with CAPISocket::Parse_GetByte
	public void appendInt8(int data)
	{
		appendData(new byte[] { (byte) data });
	}

	public void appendShort(short data)
	{
		appendData(PacketUtils.shortToByteArray(data));
	}

	public void appendLong(long data)
	{
		appendData(PacketUtils.longToByteArray(data));
	}
	
	public void appendDouble(double data)
	{
		appendData(PacketUtils.doubleToByteArray(data));
	}
	
	public void appendInt(int data)
	{
		appendData(PacketUtils.intToByteArray(data));
	}
	
	public void appendData(byte[] data)
	{
		if (null == this.data)
		{
			setData(data);
		}

		else
		{
			byte[] newData = new byte[this.data.length + data.length];
			System.arraycopy(this.data, 0, newData, 0, this.data.length);
			System.arraycopy(data, 0, newData, this.data.length, data.length);

			setData(newData);
		}
	}
}
