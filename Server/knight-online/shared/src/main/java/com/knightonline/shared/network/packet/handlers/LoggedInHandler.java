package com.knightonline.shared.network.packet.handlers;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.transaction.annotation.Transactional;

import com.knightonline.shared.network.KOServer;
import com.knightonline.shared.network.packet.IPacketHandler;
import com.knightonline.shared.network.packet.Packet;
import com.knightonline.shared.network.packet.PacketWriter;

/**
 * @author Mamaorha
 *
 */

@Transactional
public abstract class LoggedInHandler implements IPacketHandler
{
	@Autowired
	protected PacketWriter packetWriter;
	
	protected String username;
	
	protected abstract KOServer getKOServer();
	
	@Override
	public void handlePacket(Packet requestPacket)
	{
		username = getKOServer().isConnectedAccount(requestPacket.getMessageInfo().getChannelId());
		
		if (null != username)
		{
			handlePacketImpl(requestPacket);
		}
		
		else
		{
			System.out.println("Client tried to send packet without log in");
			killConnection(requestPacket);			
		}
	}
	
	protected void killConnection(Packet requestPacket)
	{
		getKOServer().killConnection(requestPacket.getMessageInfo().getChannelId());
	}
	
	protected abstract void handlePacketImpl(Packet requestPacket);
}
