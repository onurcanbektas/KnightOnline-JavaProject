package com.knightonline.game.network.packet.handlers;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

import com.knightonline.shared.network.KOServer;
import com.knightonline.shared.network.packet.Packet;
import com.knightonline.shared.network.packet.PacketHandlerBase;
import com.knightonline.shared.network.packet.PacketWriter;

/**
 * @author Mamaorha
 *
 */
@Component
public class OnlineUsersHandler extends PacketHandlerBase
{
	@Autowired
	protected PacketWriter packetWriter;

	@Autowired
	protected KOServer koServer;

	@Override
	public void handlePacket(Packet requestPacket)
	{
		Packet result = new Packet(requestPacket.getOpcode(), requestPacket.getMessageInfo());
		result.appendShort(Short.valueOf(String.valueOf(koServer.getLoggedInClients())));	
		
		packetWriter.sendPacket(result);
	}
}
