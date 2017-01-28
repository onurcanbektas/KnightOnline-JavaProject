package com.knightonline.shared.network.common;

import com.knightonline.shared.network.packet.PacketHandler;

/**
 * @author Mamaorha
 *
 */
public class ServerConfiguration
{
	protected String ip;
	protected int port;
	protected int msgTimeout;
	protected int receiveBufferSize;
	protected HandlerTypeEnum handlerType;
	protected PacketHandler packetHandler;
	
	public String getIp()
	{
		return ip;
	}

	public void setIp(String ip)
	{
		this.ip = ip;
	}

	public int getPort()
	{
		return port;
	}

	public void setPort(int port)
	{
		this.port = port;
	}

	public int getMsgTimeout()
	{
		return msgTimeout;
	}

	public void setMsgTimeout(int msgTimeout)
	{
		this.msgTimeout = msgTimeout;
	}

	public int getReceiveBufferSize()
	{
		return receiveBufferSize;
	}

	public void setReceiveBufferSize(int receiveBufferSize)
	{
		this.receiveBufferSize = receiveBufferSize;
	}

	public HandlerTypeEnum getHandlerType()
	{
		return handlerType;
	}

	public void setHandlerType(HandlerTypeEnum handlerType)
	{
		this.handlerType = handlerType;
	}

	public PacketHandler getPacketHandler()
	{
		return packetHandler;
	}

	public void setPacketHandler(PacketHandler packetHandler)
	{
		this.packetHandler = packetHandler;
	}
}
