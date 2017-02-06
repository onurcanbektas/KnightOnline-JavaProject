package com.knightonline.shared.network.packet.handlers;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.transaction.annotation.Transactional;

import com.knightonline.shared.network.KOServer;
import com.knightonline.shared.network.packet.Packet;
import com.knightonline.shared.network.packet.PacketHandlerBase;
import com.knightonline.shared.network.packet.PacketWriter;

/**
 * @author Mamaorha
 *
 */

@Transactional
public abstract class LoggedInHandler extends PacketHandlerBase
{
	@Autowired
	protected PacketWriter packetWriter;
	
	@Autowired
	protected KOServer koServer;
	
	protected String username;
	
	@Override
	public void handlePacket(Packet requestPacket)
	{
		username = koServer.getUsernameByChannel(requestPacket.getMessageInfo().getChannelId());
		
		if (null != username)
		{
			handlePacketImpl(requestPacket);
		}
		
		else
		{
			System.out.println("Client tried to send packet without loggin in first");
			killConnection(requestPacket);			
		}
	}
	
	protected void killConnection(Packet requestPacket)
	{
		koServer.killConnection(requestPacket.getMessageInfo().getChannelId());
	}
	
	protected abstract void handlePacketImpl(Packet requestPacket);
}
